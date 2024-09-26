/*
 *                           0BSD 
 * 
 *                    BSD Zero Clause License
 * 
 *  Copyright (c) 2019 Hermann Meyer
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#pragma once

#ifndef XWAVEVIEW_H_
#define XWAVEVIEW_H_

#include "xputty.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    float* wave;
    int size;
    char pad[4];
} WaveView_t;


/**
 * @brief add_waveview        - add a waveview to a Widget_t
 * @param *parent             - pointer to the Widget_t request the waveview
 * @param *label              - Label to show on the waveview
 * @param x,y,width,height    - the position/geometry to create the waveview
 * @return Widget_t*          - pointer to the Widget_t waveview struct
 */

Widget_t* add_waveview(Widget_t *parent, const char * label,
                int x, int y, int width, int height);

void update_waveview(Widget_t *waveview, float* wave, int size);

void waveview_mem_free(void *w_, void* user_data);

#ifdef __cplusplus
}
#endif

#endif //XWAVEVIEW_H_
