/* Copyright 2003 tim goetze <tim@quitte.de> -- see the file 'COPYING.gz'. */

#ifndef _BI_QUAD_H_
#define _BI_QUAD_H_

namespace DSP {

class BiQuad
{
	public:
		/* coefficients */
		d_sample * a, * b;

		/* history */
		int h;
		d_sample x[2], y[2];

		BiQuad (d_sample * _a, d_sample * _b)
		{
			a = _a;
			b = _b;
			
			reset();
		}

		void reset()
		{
			h = 0;

			x[0] = x[1] = 
			y[0] = y[1] = 0.;
		}

		inline d_sample process (d_sample s)
		{
			register int z = h;

			register d_sample r = s * a[0];
			
			r += a[1] * x[z];
			r += b[1] * y[z];

			z ^= 1;
			r += a[2] * x[z];
			r += b[2] * y[z];

			y[z] = r;
			x[z] = s;
			
			h = z;

			return r;
		}

		inline d_sample process_0_1()
		{
			register int z = h;

			register d_sample r = 0;
			
			r += a[1] * x[z];
			r += b[1] * y[z];

			z ^= 1;
			r += a[2] * x[z];
			r += b[2] * y[z];

			y[z] = r;
			x[z] = 0; 
			
			h = z;

			return r;
		}

		inline d_sample process_0_2()
		{
			register int z = h;

			register d_sample r = 0;
			
			r += b[1] * y[z];

			z ^= 1;
			r += a[2] * x[z];
			r += b[2] * y[z];

			y[z] = r;
			x[z] = 0;
			
			h = z;

			return r;
		}

		inline d_sample process_0_3()
		{
			register int z = h;

			register d_sample r = 0;
			
			r += b[1] * y[z];

			z ^= 1;
			r += b[2] * y[z];

			y[z] = r;
			x[z] = 0;
			
			h = z;

			return r;
		}
};

} /* namespace DSP */

#endif /* _BI_QUAD_H_ */
