/*
 * GNUitar
 * Sustain effect
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
 */

#include <stdlib.h>
#include "sustain.h"
#include "log.h"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define URI "http://shaji.in/plugins/gnuitar-sustain-np"


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Sustain * psustain = (Sustain *) malloc (sizeof (Sustain));
    return psustain;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void * data)
{
    Sustain * params = (Sustain *) instance ;
    switch (port) {
		case INPUT:
			params -> input = (float *) data ;
			break ;
		case OUTPUT:
			params->output = (float *) data ;
			break ;
		case SUSTAIN:
			params->sust = (float *) data ;
			break ;
		case NOISE:
			params->noise = (float *) data ;
			break ;
		case GATE:
			params->threshold = (float *) data ;
			break ;
    }
}

static void
activate(LV2_Handle instance) {
    Sustain * psustain = (Sustain *) malloc (sizeof (Sustain));
    * psustain->noise = 40;
    * psustain->sust = 256;
    * psustain->threshold = 256;
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
    int             count;
    DSP_SAMPLE     *s;
    Sustain *ds = (Sustain *) instance;
    DSP_SAMPLE      volAccum,
                    tmp;
    float           CompW1;
    float           CompW2;
    float           gateFac;
    float           compFac;

    count = n_samples;
    s = ds -> input;

    * ds->sust = * ds->sust * 2.56;
    * ds->noise = * ds->noise * 2.56;
    * ds->threshold = * ds->threshold * 2.56;

    volAccum = ds->volaccum;
    CompW1 = * ds->sust / 100.0f;
    CompW2 = 1.0f - CompW1;

    while (count) {
	tmp = *s;
	/*
	 * update volAccum 
	 */
	tmp = (tmp < 0) ? -tmp : tmp;
	volAccum = (256 - *ds->noise) * volAccum + *ds->noise * tmp;
	volAccum /= 256;

	/*
	 * handle compression 
	 */
	compFac = 30000.0f / (float) volAccum;
	compFac = CompW1 * compFac + CompW2;
	/*
	 * handle gate 
	 */
	if (*ds->threshold <= 1.0f)
	    gateFac = 1.0f;
	else
	    gateFac = (volAccum > (*ds->threshold * 100)) ? 1.0f :
		((float) (volAccum) / (float) (*ds->threshold * 100));
	/*
	 * process signal... 
	 */
	tmp = ((float) (*s) * compFac * gateFac);
#ifdef CLIP_EVERYWHERE
        CLIP_SAMPLE(tmp)
#endif
	*s = tmp;
    * ds -> output = tmp ;
    ds -> output ++ ;
	s++;
	count--;
    }
    ds->volaccum = volAccum;

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

