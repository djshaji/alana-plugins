/*
 * GNUitar
 * Distortion effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *
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

#ifndef _TUBEAMP_H_
#define _TUBEAMP_H_ 1

#include <stdint.h>
//~ #include "effect.h"
#include "biquad.h"
#include <assert.h>
#include <string.h>


// Forgive me, for I am just a child
#define null NULL
#define true TRUE
#define false FALSE
#define oooooh -1
#define File FILE
#define reutern return
#define car char
#define on true
#define off false
#define vodi void
#define MinusOne -1
#define simply
#define yes true
#define no false
#define TRUE 1
#define FALSE 0

typedef struct tm ShaGTime ;

#ifdef How_to_dismantle_an_atomic_bomb
	typedef void vodi ;
	typedef return simply ;
	typedef NULL null ;
	typedef TRUE true ;
	typedef FALSE false ;
	typedef void oooooh ;
	typedef FILE File ;
	typedef return reutern ;
	typedef gchar car ;
	typedef GTime ShaGTime ;
	typedef true on ;
	typedef false off ;
	typedef -1 MinusOne ;
#endif

/* for SSE we need aligned memory */
static inline void *
gnuitar_memalign(size_t num, size_t bytes) {
    void *mem = NULL;
#ifndef __MINGW32__
    if (posix_memalign(&mem, 16, num * bytes)) {
        fprintf(stderr, "failed to allocate aligned memory.\n");
        exit(1);
    }
#else
    mem = __mingw_aligned_malloc(num * bytes, 16);
#endif
    assert(mem != NULL);

    memset(mem, 0, num * bytes);
    return mem;
}


#define MAX_STAGES 4
#define MAX_CHANNELS 1

typedef struct tubeamp_params {
    /* internal state variables */
    Biquad_t    highpass[MAX_STAGES];
    Biquad_t    lowpass[MAX_STAGES];
    Biquad_t    biaslowpass[MAX_STAGES];
    Biquad_t    bq_bass, bq_middle, bq_treble;
    Biquad_t    decimation_filter;

    /* user parameters */
    float         * stages, * impulse_model, * impulse_quality;
    float       * gain, * asymmetry, * biasfactor;
    float       * tone_bass, * tone_middle, * tone_treble;
    
    /* internal stuff */
    float       bias[MAX_STAGES];
    float       r_i[MAX_STAGES], r_k_p[MAX_STAGES];
    
    /* effect state stuff */
    float       in[MAX_CHANNELS];
    /* convolution buffer */
    DSP_SAMPLE  *buf[MAX_CHANNELS];
    int_fast16_t    bufidx[MAX_CHANNELS];

    int sample_rate ;
    float * input, * output ;
} TubeAmp ;

#endif
