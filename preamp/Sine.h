/* Copyright 2001-3 tim goetze <tim@quitte.de> -- see 'COPYING.gz'. */

#ifndef _DSP_SINE_H_
#define _DSP_SINE_H_

namespace DSP {
	
class Sine
{
	protected:
		int z;
		d_float y[2];
		d_float b;

	public:
		Sine()
		{ 
			z = 0;
			y[0] = y[1] = 0;
			b = 0;
		}

		void set_f (double f, double fs, double phase = 0.)
		{
			double w = (f * M_PI) / fs;
			
			b = 2 * cos (w);
			y[0] = sin (phase - w);
			y[1] = sin (phase - w * 2);
			z = 0;
		}

		/* advance and return 1 sample */
		inline double get()
		{
			register double s = b * y[z]; 
			z ^= 1;
			s -= y[z];
			return y[z] = s;
		}

		double get_phase()
		{
			double x0 = y[z], x1 = get();
			double phi = asin (x0);
			
			/* slope is falling, we're into the 2nd half. */
			if (x1 < x0)
				return M_PI - phi;

			return phi;
		}
};

} /* namespace DSP */

#endif /* _DSP_SINE_H_ */
