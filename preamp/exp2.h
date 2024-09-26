#ifndef _EXP_2_H_
#define _EXP_2_H_

/* exp2 has been posted to music-dsp by andrew simper <andy@vellocet.com> 
 * a good pow (2, x) approximation for so few cycles.
 * 
 * thanks, andy!
 */

static inline float
exp2__ (float x)
{
	long * px = (long *) (&x); // store address of float as long pointer
	const float tx = (x - .5f) + (3 << 22); // temporary value for truncation
	const long  lx = *((long *) &tx) - 0x4b400000; // integer power of 2
	const float dx = x - (float) (lx); // float remainder of power of 2
	x = 1.f + dx * (0.6960656421638072f + // cubic apporoximation of 2^x
						dx * (0.224494337302845f +  // for x in the range [0, 1]
						dx * (0.07944023841053369f)));
	*px += (lx << 23); // add integer power of 2 to exponent

	return x;
}

#endif /* _EXP_2_H_ */
