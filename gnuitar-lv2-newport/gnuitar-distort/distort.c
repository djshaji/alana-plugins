/*
 * distort.c
 * 
 * GNUitar
 * Distortion effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 * * 
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
 * 
 */


#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "distort.h"
#include "log.h"
#include "rcfilter.c"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define URI "http://shaji.in/plugins/gnuitar-distort-np"


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
	Distort * ap = (Distort *) malloc (sizeof (Distort));
	ap->db = (data_block_t *) malloc (sizeof (data_block_t));
	
    ap->noisegate = 3000;

    RC_setup(10, 1.5, &(ap->fd));

    RC_setup(10, 1, &(ap->noise));
    RC_set_freq(ap->noisegate, &(ap->noise));
	ap -> sample_rate = sample_rate ;
	ap -> db -> sample_rate = sample_rate ; 
	ap->fd.sample_rate = sample_rate;
	ap->noise.sample_rate = sample_rate;
	
	return ap;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	Distort * params = (Distort *) instance ;
	switch (port) {
		case INPUT:
			params -> input = (float *) data ;
			break ;
		case OUTPUT:
			params->output = (float *) data ;
			break ;
		case DRIVE:
			params->drive = (float *) data ;
			break ;
		case LEVEL:
			params->level = (float *) data ;
			break ;
		case SATURATION:
			params->sat = (float *) data ;
			break ;
		case LOWPASS_:
			params->lowpass = (float *) data ;
			break;
	}
}

static void
activate(LV2_Handle instance) {
	Distort * ap = (Distort *) instance;
    * ap->sat = 10000;
    * ap->level = 20;
    * ap->drive = 555;
    * ap->lowpass = 350;
    RC_set_freq(*ap->lowpass, &(ap->fd));

}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
    int             count,
                    currchannel = 0;
    DSP_SAMPLE     *s;
    Distort *dp = (Distort *) instance;
	* dp->drive = * dp->drive * 10 ;
	* dp->level = * dp->level * 2.56 ;
	* dp->sat = * dp -> sat  * 300 ;
   float	    t;
   int append = 0;
   
   for (int i = 0 ; i < n_samples ; i ++)
		dp-> output [i] = dp -> input [i];
	

	data_block_t * db = dp -> db ;
	db->data = dp->output ;
	db->len = n_samples;
	db->channels = 1 ;

    /*
     * sat clips derivative by limiting difference between samples. Use lastval 
     * member to store last sample for seamlessness between chunks. 
     */
    count = db->len;
    s = db->data;

    RC_highpass(db, &(dp->fd));

    while (count) {
	/*
	 * apply drive  
	 */
	t=*s;
	t *= *dp->drive;
	t /= 16;

	/*
	 * apply sat 
	 */
	if ((t - dp->lastval[currchannel]) > ((int) dp->sat << 8))
	    t = dp->lastval[currchannel] + ((int) dp->sat << 8);
	else if ((dp->lastval[currchannel] - t) > ((int) dp->sat << 8))
	    t = dp->lastval[currchannel] - ((int) dp->sat << 8);

	dp->lastval[currchannel] = t;
        currchannel = (currchannel + 1) % db->channels;
        
	t *= *dp->level;
	t /= 256;
#ifdef CLIP_EVERYWHERE
        CLIP_SAMPLE(t)
#endif
	if(isnan(t))
	    t=0;
	*s=t;

	//~ dp->output [append] = t ;//dp->input [append ++];
	//~ dp->output++ ;

	s++;
	count--;
    }

    LC_filter(db, 0, * dp->lowpass, &(dp->noise));

}


static void
deactivate(LV2_Handle instance) {

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

