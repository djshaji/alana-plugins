/*
 * GNUitar
 * Autowah effect: definitions
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

#ifndef _AUTOWAH_H_
#define _AUTOWAH_H_ 1

#include "defs.h"
#include "backbuf.h"
#include "biquad.h"

typedef enum {
    INPUT,
    OUTPUT,
    FREQ_LOW,
    FREQ_HIGH,
    DRY_WET,
    RES,
    SYNC,
    METHOD,
    SWEEP_TIME
} PortIndex;

typedef struct autowah_params {
    Backbuf_t       *history;

    /* keep some widgets so we can toggle their enabled state */
    // GtkWidget       *w_control;
    // GtkWidget       *w_sweep;
    // GtkWidget       *w_resonance;
    
    float       *    sweep_time;
    float          * freq_low;
    float          * freq_high;
    float          * res;
    float 	    * drywet;
    float           *  sync;
    float           *  method;

    float           fresh_accum_delta;
    float           fresh_accum_power;
    float           delayed_accum_delta;
    float           delayed_accum_power;
    int             accum_n;
    
    float           f, smoothed_f, freq_vibrato;
    int             dir;
    float           ya[MAX_CHANNELS];
    float           yb[MAX_CHANNELS];
    float           yc[MAX_CHANNELS];
    float           yd[MAX_CHANNELS];

    Biquad_t        lpf;
    Biquad_t        bpf;
    float * input, * output;
    data_block_t * db ;
    int sample_rate ;
    float sin_lookup_table[SIN_LOOKUP_SIZE + 1];

} Autowah;

/* these thresholds are used to trigger the sweep. The system accumulates
 * time-weighted average of square difference between samples ("delta") and
 * the energy per sample ("power"). If either suddenly increases, the
 * sweep triggers. Data is collected AUTOWAH_HISTORY_LENGTH ms apart. */

#define AUTOWAH_HISTORY_LENGTH  100  /* ms */
#define AUTOWAH_DISCANT_TRIGGER 0.60 /* dB */
#define AUTOWAH_BASS_TRIGGER    0.65 /* dB */

#endif
