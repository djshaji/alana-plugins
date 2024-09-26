/*
 * GNUitar
 * Distortion effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 * Port to LV2
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id$
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "log.h"

#include "biquad.c"
#include "distort2.h"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#define URI "http://shaji.in/plugins/gnuitar-distort2-np"

#define RC_FEEDBACK_R          4.7e3
#define RC_FEEDBACK_C          47e-9
#define RC_DRIVE_C             51e-12   /* farads */
#define DRIVE_STATIC           50e3     /* ohms */
#define DRIVE_LOG              500e3    /* ohms */
 
#define MAX_NEWTON_ITERATIONS   20      /* limits the time looking for convergence */
#define UPSAMPLING_RATE         4       /* you can't change this, see upsample[] */
 
/* the effect is defined in -1.0 .. 1.0 range */
#define DIST2_DOWNSCALE		(1.0 / MAX_SAMPLE)
#define DIST2_UPSCALE		(MAX_SAMPLE / 1.0)
/* when we have converged within one 16-bit sample, accept value */
#define EFFECT_PRECISION	(1.0 / 1)

/* Check if the compiler is Visual C or GCC */
#if defined(_MSC_VER)
#   pragma intrinsic (exp)
#   define expf(x) exp(x)
#endif

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Distort2 * distort2 = (Distort2 *) malloc (sizeof (Distort2));
    distort2->sample_rate = sample_rate;

    /* static shapers */
    set_rc_lowpass_biquad(sample_rate, 3200, &distort2->treble_highpass);
    set_rc_lowpass_biquad(sample_rate * UPSAMPLING_RATE, 
            1 / (2 * M_PI * RC_FEEDBACK_R * RC_FEEDBACK_C),
            &distort2->feedback_minus_loop);
    set_rc_highpass_biquad(sample_rate, 160, &distort2->output_bass_cut);
    
    return distort2 ;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
    Distort2 * params = (Distort2 *) instance ;
    switch (port) {
        case INPUT:
            params -> input = (float *) data ;
            break ;
        case OUTPUT:
            params -> output = (float *) data ;
            break ;
        case DRIVE:
            params -> drive = (float *) data ;
            break ;
        case TREBLE:
            params -> treble = (float *) data ;
            break ;
        case LEVEL:
            params -> clip = (float *) data ;
            break ;
    }
}

static void
activate(LV2_Handle instance) {
    Distort2 * distort2 = (Distort2 *) instance ;
    //~ *distort2->drive = 0.0;
    //~ *distort2->clip = 100.0;
    //~ *distort2->treble = 6.0;
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Distort2 * params = (Distort2 *) instance ;
    
    int_fast32_t        i,count,bailout;
    int_fast8_t	        curr_channel = 0;
    DSP_SAMPLE 	       *s;
    Distort2 *dp = params;
    float x,y,x1,x2,f,df,dx,e1,e2;
    DSP_SAMPLE upsample[UPSAMPLING_RATE];
    int buffer_size = n_samples - 1;
    float DRIVE = (float) DRIVE_STATIC + *dp->drive / 100.0f * (float) DRIVE_LOG;
    float mUt = (20.0f + 100.f - 70.f) * 1e-3f;
    /* correct Is with mUt to approximately keep drive the
     * same. Original parameters said Is is 10e-12 and mUt 30e-3.
     * If mUt grows, Is must shrink. 0.39 is experimental */
    float Is = 10e-12f * expf(0.39f/30e-3f - 0.39f/mUt);
    count = n_samples;
    
    
    //~ for (int i = 0 ; i < n_samples ; i ++)
        //~ params -> output [i] = params->input [i];

    s = params->input ;
    
    int ax = 0 ;
    /* no input, no output :-) to avoid extra calc. Optimized for noise gate,
     * when all input is zero.
     * This is the heuristics - since there is no the standard function
     * in the ANSI C library that reliably compares the memory region
     * with the given byte, we compare just a few excerpts from an array.
     * If everything is zero, we have a large chances that all array is zero. */
    //~ if(s[0]==0 && s[1]==0 && s[16]==0 && s[17]==0 &&
          //~ s[24]==0 && s[25]==0 && s[32]==0 && s[33]==0 &&
	  //~ s[buffer_size-1]==0) {
        //~ for (i = 0; i < MAX_CHANNELS; i += 1) {
            //~ dp->last[i] = 0;
        //~ }
        //~ return;
    //~ }

    set_rc_lowpass_biquad(params->sample_rate * UPSAMPLING_RATE, 720, &dp->rolloff);
    /*
     * process signal; x - input, in the range -1, 1
     */
    while (count) {
        /* "properly" interpolate previous input at positions 0 and 2 */
        fir_interpolate_2x(dp->interpolate_firmem[curr_channel],
                           *s, &upsample[2], &upsample[0]);
        /* estimate the rest, this should be good enough for our purposes. */
        upsample[1] = (upsample[0] + upsample[2]) / (DSP_SAMPLE) 2;
        /* looking into firmem is a gross violation of interface. This will
         * go away once I design fir_interpolate_4x. */
        upsample[3] = ((DSP_SAMPLE) 3 * upsample[2] + dp->interpolate_firmem[curr_channel][2]) / (DSP_SAMPLE) 4;

	/* Now the actual upsampled processing */
	for (i = 0; i < UPSAMPLING_RATE; i++)
	{
            /* scale down to -1 .. 1 range */
            x = upsample[i] * (float) DIST2_DOWNSCALE;
            
	    /* first compute the linear rc filter current output */
	    x2 = do_biquad(x, &dp->feedback_minus_loop, curr_channel);
            
            x1 = (x - x2) / (float) RC_FEEDBACK_R;
            
	    /* start searching from time previous point , improves speed */
	    y = dp->last[curr_channel];
            /* limit iterations if the algorithm fails to converge */
            bailout = MAX_NEWTON_ITERATIONS;
	    do {
		/* f(y) = 0 , y= ? */
                /* e^3 ~ 20 */
                e1 = expf((x - y) / mUt);
                e2 = 1.0f / e1;
		/* f=x1+(x-y)/DRIVE+Is*(exp((x-y)/mUt)-exp((y-x)/mUt));  optimized makes : */
		f = x1 + (x - y) / DRIVE + Is * (e1 - e2);
	
		/* df/dy */
		/*df=-1.0/DRIVE-Is/mUt*(exp((x-y)/mUt)+exp((y-x)/mUt)); optimized makes : */
		df = -1.0f / DRIVE - Is / mUt * (e1 + e2);
	
		/* This is the newton's algo, it searches a root of a function,
		 * f here, which must equal 0, using it's derivate. */
		dx = f/df;
		y -= dx;
	    }
	    while (fabs(dx) > (float) EFFECT_PRECISION && --bailout);
	    /* when dx gets very small, we found a solution. */
	    
            /* Ensure that the value gets reset if something goes wrong */
            if (isnan(y) || ! (y >= -2.0f && y <= 2.0f))
                y = 0.f;
            
	    dp->last[curr_channel] = y;
            /* static lowpass filtering -- this doubles as our decimation filter
             * the rolloff is at 720 Hz, but is only 6 dB/oct. */
            y = do_biquad(y, &dp->rolloff, curr_channel);
	}
        /* treble control + other final output filtering */
        y += (y - do_biquad(y, &dp->treble_highpass, curr_channel)) * *dp->treble / 3.0f;
        y = do_biquad(y, &dp->output_bass_cut, curr_channel);
	
        /* scale up from -1..1 range */
	*s = y * (float) DIST2_UPSCALE * (*dp->clip / 100.0f);
    params->output [ax++] = *s ;
	/*if(*s > MAX_SAMPLE)
	    *s=MAX_SAMPLE;
	else if(*s < -MAX_SAMPLE)
	    *s=-MAX_SAMPLE;*/

    //~ params->output [count] = params->input [count] ;
	s++;
	count--;

        curr_channel = (curr_channel + 1) % 1;
    }
}



static void
deactivate(LV2_Handle instance) {

}

static void
cleanup(LV2_Handle instance) {
	free(instance);
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

static const LV2_Descriptor descriptor = {
	URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index) {
	switch (index) {
        case 0:  
            return &descriptor;
        default: 
            return NULL;
	}
}
