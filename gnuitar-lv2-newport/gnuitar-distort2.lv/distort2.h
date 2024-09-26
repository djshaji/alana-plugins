#ifndef __DISTORT2_H
#define __DISTORT2_H

#include <string.h>

#define MAX_SAMPLE 1 //(32767 << 8)
#define MAX_CHANNELS 1

typedef struct distort2_params {
    float       * drive, * clip, * treble;
    float 	last[MAX_CHANNELS];
    int sample_rate ;

    DSP_SAMPLE  interpolate_firmem[MAX_CHANNELS][8];
    Biquad_t    feedback_minus_loop, output_bass_cut,
                rolloff, treble_highpass;
                
    float * input, * output ;
} Distort2;

typedef enum {
    INPUT,
    OUTPUT,
    DRIVE,
    LEVEL,
    TREBLE
} PortIndex ;


#endif __DISTORT2_H
