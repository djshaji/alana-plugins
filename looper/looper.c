/*
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// ha ha
#ifdef __ANDROID__
    #include <android/log.h>
    #define MODULE_NAME "LOOPER"
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MODULE_NAME, __VA_ARGS__)
#else
    #define LOGD(...) printf(__VA_ARGS__)
#endif

#define URI "http://shaji.in/plugins/looper"

/** Define a macro for converting a gain in dB to a coefficient. */
#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)
#define MAX_BUFFER 1024 * 1024

typedef enum {
	TOGGLE_RECORD   = 0,
	TOGGLE_PLAY = 1,
	TOGGLE_FILE = 100,
	GAIN = 3,
	BUFFER_SIZE = 4,
    START = 5,
    END = 6,
	INPUT  = 7,
	OUTPUT = 8,
    FILE_LOAD_SIZE = 99
} PortIndex;

typedef struct {
	// Port buffers
	float * gain;
    float * toggle_play ;
    float * toggle_rec ;
    float * file_buffer ;
	const float * input;
	float * output;
	int * buffer_size_control ;
	//~ float  buffer [MAX_BUFFER+1];
    float * buffer ;
    float * start ;
    float * end ;
    int counter ;
    int buffer_size ;
    int file_load_size ;
    int recorded ;
    int bypass ;
} Looper;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
	Looper * looper = (Looper*)malloc(sizeof(Looper));
    //~ * looper -> start = 0 ;
    //~ * looper -> end = 0 ;
    //~ looper -> counter = 0 ;
    //~ //looper -> buffer = malloc (* looper -> buffer_size_control * 16) ;
    //~ looper -> buffer_size = MAX_BUFFER ;
    //~ looper -> buffer [0] = 1 ;
    //~ for (int i = 0 ; i < MAX_BUFFER; i ++)
        //~ looper -> buffer [i] = -1 ;
        
	return (LV2_Handle)looper;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	Looper* looper = (Looper*)instance;
	float * d ;
    //~ LOGD ("control: %d\tvalue: %f", port, * (float *) data); 

	switch ((PortIndex)port) {
	case GAIN:
		looper->gain = ( float*)data;
		break;
	case INPUT:
		looper->input = (const float*)data;
		break;
	case OUTPUT:
		looper->output = (float*)data;
		break;
	case TOGGLE_PLAY:
	    looper -> toggle_play = (float *) data ;
	    break ;
	case TOGGLE_RECORD:
	    looper -> toggle_rec = (float *) data ;
	    break ;
    case FILE_LOAD_SIZE:
        looper -> file_load_size = * (int *) data ;
        break ;
	case BUFFER_SIZE:
        break ;
        looper -> bypass = 1 ;
	    looper -> buffer_size_control = (int *) data ;
        //~ if (*looper -> buffer_size_control < 128)
            //~ *looper -> buffer_size_control = 128 ;
        * looper -> buffer_size_control = * looper -> buffer_size_control * 1024 ;
        LOGD ("set buffer size to %d", * looper -> buffer_size_control) ;
        looper -> buffer_size = * looper -> buffer_size_control ;
        looper -> buffer = malloc (sizeof (int) * (looper -> buffer_size + 1)) ;
        for (int i = 0 ; i < looper -> buffer_size; i ++)
            looper -> buffer [i] = 0 ;

        looper -> bypass = 0 ;
	    break ;
	case START:
	    looper -> start = (float *) data ;
	    break ;
	case END:
	    looper -> end = (float *) data ;
	    break ;
	case TOGGLE_FILE:
	    d = (float *) data ;
	    LOGD ("buffer size: %d", looper -> buffer_size);
	    for (int i = 0 ; i < looper -> file_load_size; i ++) {
            looper -> buffer [i] = d [i] ;
            looper -> recorded = i ;
            //~ LOGD ("COPY SAMPLE %d", i);
            if (i >= looper -> buffer_size)
                break ;
	    }
	    
	    LOGD ("copied %d samples", looper -> recorded);
	    if (looper -> recorded < looper -> buffer_size) {
		for (int i = looper -> recorded ; i < looper -> buffer_size ; i ++) {
		    looper -> buffer [i] = 0;
		}
	    }
	    
	    looper -> counter = looper -> recorded ;
	    break ;
	default:
	    break ;
	}
}

static void
activate(LV2_Handle instance) {
    Looper * looper = (Looper *) instance ;
    //~ * looper -> start = 0 ;
    //~ * looper -> end = 0 ;
    looper -> counter = 0 ;
    looper -> recorded = 0 ;
    looper -> bypass = 0 ;

    looper -> buffer = malloc (sizeof (int) * (MAX_BUFFER + 1)) ;
    looper -> buffer_size = MAX_BUFFER ;
    for (int i = 0 ; i < looper -> buffer_size; i ++)
        looper -> buffer [i] = 0 ;
    //~ looper -> buffer_size = * looper -> buffer_size_control  ;
    //~ LOGD ("allocating buffer size: %f", * looper -> buffer_size_control);
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
	Looper * looper = (Looper *)instance;

    if (looper -> bypass)
        return ;

    // warning: The following is severely looked down upon
    /*
    if (*looper -> buffer_size_control != looper -> buffer_size ) {
        *looper -> buffer_size_control = * looper->buffer_size_control * 1024 ;
        LOGD ("WARNING Reallocating buffer to %d wtf dude do you even audio engineer", *looper -> buffer_size_control);
        looper -> buffer_size = * looper -> buffer_size_control ;
        free (looper -> buffer);
        looper -> buffer = malloc (sizeof (int) * (looper -> buffer_size + 1)) ;
        for (int i = 0 ; i < looper -> buffer_size; i ++)
            looper -> buffer [i] = 0 ;
    }
    */

	const float        gain   = *(looper->gain);
	const float* const input  = looper->input;
	float* const       output = looper->output;
    const float coef = DB_CO(* looper -> gain);
    float start = 0, end = 0 ;
    
    //~ LOGD ("gain %f\tstart %f\tend %f", gain, *looper -> start, *looper -> end);

    if (* looper -> toggle_rec > 0) {
        for (uint32_t pos = 0; pos < n_samples; pos++) {
            output [pos] = input [pos];
            looper -> buffer [looper -> counter] = input [pos] ;
            looper -> counter ++ ;
            //~ LOGD ("[rec] %d\t%d\t%d\n", looper -> counter, pos, looper->buffer_size) ;
            if (looper -> counter > looper -> buffer_size) {
                // * looper -> toggle_rec = 0 ;
                looper -> counter = 0 ;
                //~ break ;
            } else {
                looper -> recorded = looper -> counter ;
            }
        }
    } else if (* looper -> toggle_play > 0) {
        for (uint32_t pos = 0; pos < n_samples; pos++) {
            start = (* looper -> start / 100) * looper -> recorded ;
            if (looper -> counter < start) {
                looper -> counter = start ;
            } else if (looper -> counter > (* looper -> end / 100) * looper -> recorded) {
                looper -> counter = 0;
            }
            //~ LOGD ("[play] %d\t%d\n", looper -> counter, pos) ;
            if (looper -> counter < looper -> recorded) {
                output[pos] = (looper -> buffer [looper -> counter] * gain ) + input [pos] ;
                //printf ("%d\t%d\n", pos, looper -> counter);
                looper -> counter ++ ;
            } else {
                looper -> counter = 0 ;
            }

            if (looper -> counter > looper -> buffer_size) {
                looper -> counter = 0 ;
            }
        }
    } else {
        for (uint32_t pos = 0; pos < n_samples; pos++) {
            output [pos] = input [pos];
        }

        //~ if (looper -> counter > 0) {
            //~ for (int i = looper -> counter ; i < looper -> buffer_size ; i ++) {
                //~ looper -> buffer [i] = -1 ;
            //~ }
        looper -> counter = 0 ;
    }
}

static void
deactivate(LV2_Handle instance) {
    Looper * looper = (Looper *) instance ;
    looper -> counter = 0 ;
    looper -> buffer_size = 0 ;
    //free (looper -> buffer) ;

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
