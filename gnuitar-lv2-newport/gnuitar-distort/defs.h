#ifndef _DEFS_H
#define _DEFS_H


typedef float DSP_SAMPLE ;
#define MAX_CHANNELS 1
#define MAX_FILTERS 10

typedef struct {
    DSP_SAMPLE * __restrict__ data;
    DSP_SAMPLE * __restrict__ data_swap;
    int    len;
    int     channels;
    int sample_rate ;
} data_block_t;

#endif