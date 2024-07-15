/* Copyright 2003 tim goetze <tim@quitte.de> -- see the file 'COPYING.gz'. */

#ifndef _ONE_POLE_H_
#define _ONE_POLE_H_

namespace DSP {
	
class OnePoleLP
{
	public:
		d_sample a0, b1, y1;

		OnePoleLP (double d = 1.)
		{
			set (d);
			y1 = 0.;
		}

		inline void set (double d)
		{
			a0 = (d_sample) d;
			b1 = (d_sample) 1. - d;
		}

		inline d_sample process (d_sample x)
		{
			return y1 = a0 * x + b1 * y1;
		}
		
		inline void decay (double d)
		{
			a0 *= d;
			b1 = 1. - a0;
		}
};

class OnePoleHP
{
	public:
		d_sample a0, a1, b1, x1, y1;
		double fc;

		OnePoleHP (double d = 1.)
		{
			fc = 0;
			set (d);
			x1 = y1 = 0.;
		}

		void set_f (double f)
		{
			fc = f;
			set (exp (-2 * M_PI * fc));
		}

		inline void set (double d)
		{
			a0 = (d_sample) ((1. + d) / 2.);
			a1 = (d_sample) ((1. + d) / -2.);
			b1 = d;
		}

		inline d_sample process (d_sample x)
		{
			y1 = a0 * x + a1 * x1 + b1 * y1;
			x1 = x;
			return y1;
		}

		void reset()
		{
			x1 = y1 = 0;
		}
};

} /* namespace DSP */

#endif /* _ONE_POLE_H_ */
