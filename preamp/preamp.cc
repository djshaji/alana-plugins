/*
	preamp.cc
	
	Copyright 2003 Tim Goetze <tim@quitte.de>
	
	http://quitte.de/dsp/

	loosely 12ax7-based valve preamp model, up to 4x oversampling with
	IIR filters.

	Nov  2 2003: more attenuation at higher gain, cosmetics
	Oct 31 2003: cosmetics

*/
/*
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA or point your web browser to http://www.gnu.org.
*/

#include <malloc.h>
#include <string.h>
#include <math.h>

#include <ladspa.h>

typedef LADSPA_Data d_sample;
typedef double d_float;

#include "OnePole.h"
#include "BiQuad.h"
#include "Sine.h"
#include "exp2.h"

/* antialias lo-pass coefficients */
d_sample A[] = {
	0.6597035633861333403515914142190013080835, 
	-0.5153697960101932817877923298510722815990, 
	0.4780344140343041892116104918386554345489
};

d_sample B[] = {
	0.0, 
	1.6747364125388575217812103801406919956207, 
	-0.7469419725275743271453166016726754605770
};

/* flavours for sample store functions run() and run_adding() */
typedef void (*sample_func_t) (d_sample *, int, d_sample, d_sample);

/* /////////////////////////////////////////////////////////////////////// */

class Preamp
{
	public:
		d_sample gain, fc;

		double fs;

		/* polynomial and clipping parameters */
		double b, c, d;
		struct {
			double threshold, value;
		} clip[2];

		/* input is hi-passed first, processed, then hi-passed again */
		DSP::OnePoleHP hp;
		DSP::OnePoleHP dc_blocker;
		
		/* antialias filters */
		DSP::BiQuad pre, post;

		/* the 'real thing' constantly moves a dc offset about */
		DSP::Sine lfo;

		/* LADSPA */
		d_sample adding_gain;
		struct {
			d_sample * in, * out, * gain, * fc;
		} ports;

	public:
		Preamp (double _fs);

		template <sample_func_t, int> void process (int frames);

		inline d_sample transfer (d_sample);
		inline d_sample transfer_clip (d_sample);

		void set_clips();
		double get_root (double sign);
};

/* /////////////////////////////////////////////////////////////////////// */

Preamp::Preamp (double _fs)
: pre (A, B), post (A, B)
{
	gain = 1;

	fc = 440; /* Hz */
	fs = _fs;

	/* transfer polynomial parameters ('a' would be dc but there's none) */
	b = -0.79618574210627535;
	c = -0.21108555430962023;
	d = 0.38944033523200522;

	set_clips();

	hp.set_f (fc / fs);
	dc_blocker.set_f (10. / fs);
}

double
Preamp::get_root (double sign)
{
	return (-2*c + sign * sqrt ((2*c) * (2*c) - 4 * (3*d * b))) / (2 * 3 * d);
}

void
Preamp::set_clips()
{
	/* find 0 crossings in the derived, this is where we'll clip */
	double x0 = get_root (-1);
	double x1 = get_root (+1);

	clip[0].value = transfer (x0);
	clip[1].value = transfer (x1);

	clip[0].threshold = x0;
	clip[1].threshold = x1;
}

/* non-clipping valve transfer */
inline d_sample
Preamp::transfer (d_sample x)
{
	return x * (b + x * (c + x * d));
}

/* clipping transfer */
inline d_sample
Preamp::transfer_clip (d_sample x)
{
	if (x <= clip[0].threshold)
		return clip[0].value;
	
	if (x >= clip[1].threshold)
		return clip[1].value;
	
	return x * (b + x * (c + x * d));
}

/* these go as template parameter F below */
inline void
store_func (d_sample * s, int i, d_sample x, d_sample gain)
{
	s[i] = x;
}

inline void
adding_func (d_sample * s, int i, d_sample x, d_sample gain)
{
	s[i] += gain * x;
}

/* seems about .3 % quicker on a k6-III */
#define BIG_CACHE

template <sample_func_t F, int N> void
Preamp::process (int frames)
{
	d_sample * s = ports.in;
	d_sample * d = ports.out;

	if (gain != *ports.gain)
		gain = *ports.gain;

	if (fc != *ports.fc)
	{
		fc = *ports.fc;
		hp.set_f (fc / fs);
	}
	
	double g = gain < 1 ? gain + .00001 : exp2__ (gain - 1);
	double ig = .063 / g;

	g *= 4.; /* correction for first valve transfer at -12 dB gain */
		
	/* 'ig' also corrects for the oversampling filter gain */
	if (ig < .003) ig = .003; 

	for (int i = 0; i < frames; ++i)
	{
		register d_sample a = hp.process (s[i]);
		
		a = g * transfer (a * .25);
		
		a += lfo.get() * .01;
		a = ig * post.process (transfer_clip (pre.process (a)));

		/* these ifs eliminated by the compiler (gcc) because N is known
		 * at compile time. */
#ifdef BIG_CACHE
		if (N >= 2)
			post.process (transfer_clip (pre.process_0_1()));
		if (N >= 3)
			post.process (transfer_clip (pre.process_0_2()));
		if (N >= 4)
			post.process (transfer_clip (pre.process_0_3()));
#else
		if (N >= 2)
			post.process (transfer_clip (pre.process(0)));
		if (N >= 3)
			post.process (transfer_clip (pre.process(0)));
		if (N >= 4)
			post.process (transfer_clip (pre.process(0)));
#endif
		
		a = dc_blocker.process (a);

		/* store the sample */
		F (d, i, a, adding_gain);
	}
}

/* /////////////////////////////////////////////////////////////////////// */

static LADSPA_Handle
instantiate (const struct _LADSPA_Descriptor * desc, unsigned long fs)
{
	return new Preamp (fs);
}

static void
connect_port (LADSPA_Handle h, unsigned long n, LADSPA_Data * to)
{
	Preamp * p = (Preamp *) h;

	if (n == 0)
		p->ports.in = to;
	else if (n == 1)
		p->ports.gain = to;
	else if (n == 2)
		p->ports.fc = to;
	else if (n == 3)
		p->ports.out = to;
}

static void
activate (LADSPA_Handle h)
{
	Preamp * p = (Preamp *) h;

	/* zero filter histories */
	p->hp.reset();
	p->pre.reset();
	p->post.reset();
	p->dc_blocker.reset();
}

static void
run (LADSPA_Handle h, unsigned long frames)
{
	Preamp * p = (Preamp *) h;

	if (p->fs >= 150000)
		p->process<store_func, 1> ((int) frames);
	else if (p->fs >= 75000)
		p->process<store_func, 2> ((int) frames);
	else 
		p->process<store_func, 4> ((int) frames);
}

static void
run_adding (LADSPA_Handle h, unsigned long frames)
{
	Preamp * p = (Preamp *) h;

	if (p->fs >= 150000)
		p->process<adding_func, 1> ((int) frames);
	else if (p->fs >= 75000)
		p->process<adding_func, 2> ((int) frames);
	else 
		p->process<adding_func, 4> ((int) frames);
}

static void
set_run_adding_gain (LADSPA_Handle h, LADSPA_Data adding_gain)
{
	((Preamp *) h)->adding_gain = adding_gain;
}

static void
deactivate (LADSPA_Handle h)
{
	;
}

static void
cleanup (LADSPA_Handle h)
{
	delete (Preamp *) h;
}

/* /////////////////////////////////////////////////////////////////////// */

static const LADSPA_PortDescriptor 
port_descriptors[] = 
{
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO,
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL,
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL,
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
};

static const char *
port_names[] =
{
	"in",
	"gain",
	"fc",
	"out",
};

static const LADSPA_PortRangeHint
port_range_hints[] =
{
	/* in */
	{
		LADSPA_HINT_BOUNDED_BELOW | 
		LADSPA_HINT_BOUNDED_ABOVE | 
		LADSPA_HINT_DEFAULT_MIDDLE,
		-1.0, 1.0
	},
	/* gain */
	{
		LADSPA_HINT_BOUNDED_BELOW | 
		LADSPA_HINT_BOUNDED_ABOVE | 
		LADSPA_HINT_DEFAULT_1,
		0.0, 10.0
	},
	/* fc, correct host implementations will set 440 Hz */
	{
		LADSPA_HINT_BOUNDED_BELOW | 
		LADSPA_HINT_BOUNDED_ABOVE |
		LADSPA_HINT_LOGARITHMIC |
		LADSPA_HINT_DEFAULT_440,
		20.0, 4000.0
	},
	/* out */
	{
		LADSPA_HINT_BOUNDED_BELOW | 
		LADSPA_HINT_BOUNDED_ABOVE | 
		LADSPA_HINT_DEFAULT_MIDDLE,
		-1.0, 1.0
	},
};

static const LADSPA_Descriptor
descriptor =
{
	UniqueID:	1765,
	"preamp", /* Label */
	LADSPA_PROPERTY_HARD_RT_CAPABLE, /* Properties */
	"valve preamplifier model", /* Name */
	"Tim Goetze <tim@quitte.de>", /* Maker */
	"GPL, 2003 <tim@quitte.de>", /* Copyright */
	
	4, /* PortCount */
	port_descriptors, /* PortDescriptors */
	port_names, /* PortNames */
	port_range_hints, /* PortRangeHints */
	
	0, /* ImplementationData */

	/* methods */
	instantiate, /* instantiate */
	connect_port, /* connect_port */
	activate, /* activate */
	run, /* run */
	run_adding, /* run_adding */
	set_run_adding_gain, /* set_run_adding_gain */
	deactivate, /* deactivate */
	cleanup, /* cleanup */
};

/* /////////////////////////////////////////////////////////////////////// */

const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index) 
{
	return index ? 0 : &descriptor;
}

