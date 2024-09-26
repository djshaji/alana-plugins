/*
 * GNUitar
 * Autowah effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 */


#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "autowah.h"
#include "log.h"

#include "backbuf.c"
#include "biquad.c"

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define URI "http://shaji.in/plugins/gnuitar-autowah-np"

/*  reminder:
    this is how we convert
    power to db
    use this in stuff
*/

static double
power2db(double power)
{
    return log(power) / log(10) * 10;
}

/*  todo: implement this somewhere */
float midi_get_continuous_control(int type) {
    return 1.0f;
}



/* tabularised sin() */
// extern float sin_lookup_table[SIN_LOOKUP_SIZE+1];

static inline float
sin_lookup(float * sin_lookup_table, float pos) {
    return sin_lookup_table[(int) (pos * (float) SIN_LOOKUP_SIZE)];
}


static void
init_sin_lookup_table(float * sin_lookup_table) {
    int i = 0;
    for (i = 0; i < SIN_LOOKUP_SIZE + 1; i += 1)
        sin_lookup_table[i] = sin(2 * M_PI * i / SIN_LOOKUP_SIZE);
}

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    sample_rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
    Autowah * autowah = (Autowah *) malloc (sizeof (Autowah)), * ap;
    ap = autowah;
    autowah->db = (data_block_t *) malloc (sizeof (data_block_t));
    autowah->sample_rate = sample_rate;

    ap->history = new_Backbuf(MAX_SAMPLE_RATE * AUTOWAH_HISTORY_LENGTH / 1000);
    
    init_sin_lookup_table (ap->sin_lookup_table) ;
    return autowah;
}


static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	Autowah * params = (Autowah *) instance ;
	switch (port) {
		case INPUT:
			params -> input = (float *) data ;
			break ;
		case OUTPUT:
			params->output = (float *) data ;
			break ;
		case FREQ_LOW:
			params->freq_low = (float *) data ;
			break ;
		case FREQ_HIGH:
			params->freq_high = (float *) data ;
			break ;
		case DRY_WET:
			params->drywet = (float *) data ;
			break ;
		case RES:
			params->res = (float *) data ;
			break;
		case SYNC:
			params->sync = (float *) data ;
			break;
		case METHOD:
			params->method = (float *) data ;
			break;
        case SWEEP_TIME:
            params->sweep_time = (float *) data ;
            break ;
	}
}


static void
activate(LV2_Handle instance) {
    Autowah * ap = (Autowah *) instance ;

    * ap->method = 0; /* low-pass resonant filter */
    * ap->freq_low = 280;
    * ap->freq_high = 900;
    * ap->drywet = 100;
    * ap->sync = 0;
    * ap->sweep_time = 500;
    * ap->res = 85;
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Autowah *ap = (Autowah *) instance;
    int             i, curr_channel = 0, delay_time;
    float           freq, g, g2;

    data_block_t * db = ap -> db ;
    db->data = ap -> input ;
    db->data_swap = ap -> output ;
    db->len = n_samples;
    db->channels = 1 ;
    int sample_rate = ap->sample_rate;

    memcpy(db->data_swap, db->data, db->len * sizeof(DSP_SAMPLE));

    if (* ap->sync == 1) { /* continuous sweep */
        /* recover from noncontinuous sweep */
        if (ap->dir == 0)
            ap->dir = 1.0;
        
        if (ap->f > 1.0f && ap->dir > 0)
            ap->dir = -1;
        if (ap->f < 0.0f && ap->dir < 0)
            ap->dir = 1;
    }
    if (* ap->sync == 0) { /* envelope detect */
        /* Firstly, quiesce wah if we have reached the end of sweep */
        if (ap->f < 0.0f) {
            ap->f = 0.0;
            ap->dir = 0;
        }
        delay_time = sample_rate * AUTOWAH_HISTORY_LENGTH / 1000;
        
        /* Estimate signal higher frequency content's power. When user picks
         * the string strongly it's the high frequency content that increases
         * most. */

        /* XXX we should probably treat all channels separately.
         * We just skip all channels but the first presently. */
        for (i = 0; i < db->len; i++) { if (curr_channel == 0) {
            ap->delayed_accum_power += pow(ap->history->get(ap->history, delay_time), 2);
            ap->fresh_accum_power += pow(db->data[i], 2);
            
            ap->delayed_accum_delta +=
                pow(ap->history->get(ap->history, delay_time) -
                    ap->history->get(ap->history, delay_time - 1), 2);
            
            ap->fresh_accum_delta +=
                    pow(db->data[i] - ap->history->get(ap->history, 0), 2);

            ap->history->add(ap->history, db->data[i]);
            
            ap->accum_n += 1;
            if (ap->accum_n > 8192) {
                ap->fresh_accum_power   /= 2; 
                ap->fresh_accum_delta   /= 2; 
                ap->delayed_accum_power /= 2; 
                ap->delayed_accum_delta /= 2;
                ap->accum_n             /= 2;
            }
        } curr_channel = (curr_channel + 1) % db->channels; }

        /* I skip some scale factors here that would cancel out */
        if ((power2db(ap->fresh_accum_delta) > power2db(ap->delayed_accum_delta)
                                              + AUTOWAH_DISCANT_TRIGGER) ||
            (power2db(ap->fresh_accum_power) > power2db(ap->delayed_accum_power)
                                              + AUTOWAH_BASS_TRIGGER)) {
            ap->f = 1.0f;
            ap->dir = -1.0;
        }
    }
    if (* ap->sync == 2) { /* midi control, from Roland FC-200 or somesuch */
        ap->f = midi_get_continuous_control(-1);
        ap->dir = 0;
        ap->freq_vibrato = 0;
    }

    /* in order to have audibly linear sweep, we must map
     * [0..1] -> [freq_low, freq_high] linearly in log2, which requires
     * f(x) = a * 2 ^ (b * x)
     *
     * we know that f(0) = freq_low, and f(1) = freq_high. It follows that:
     * a = freq_low, and b = log2(freq_high / freq_low)
     */

    ap->smoothed_f = (ap->f + ap->smoothed_f) / 2.f;
    freq = * ap->freq_low * pow(2, log(* ap->freq_high / * ap->freq_low)/log(2) * ap->smoothed_f + 0.2 * sin_lookup(ap->sin_lookup_table, ap->freq_vibrato));
    ap->f += ap->dir * 1000.0f / * ap->sweep_time * db->len / (sample_rate * db->channels) * 2;

    ap->freq_vibrato += 1000.0f / * ap->sweep_time * db->len / (sample_rate * db->channels) / 2.0f;
    if (ap->freq_vibrato >= 1.0f)
        ap->freq_vibrato -= 1.0f;
    
    switch ((int) ap->method) {
      case 0:
        /* lowpass resonant filter -- we must avoid setting value 0 to
         * resonance. We also drop level by 6 dB to leave some room for it. */
        set_lpf_biquad(sample_rate, freq, 1.1 + *ap->res / 100.0, &ap->lpf);
        for (i = 0; i < db->len; i += 1) {
            db->data[i] = do_biquad(db->data[i], &ap->lpf, curr_channel) / 2;
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;
        
      case 1: 
        set_bpf_biquad(sample_rate, freq, 1.1 + *ap->res / 100.0, &ap->bpf);
        for (i = 0; i < db->len; i += 1) {
            db->data[i] = do_biquad(db->data[i], &ap->bpf, curr_channel);
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;

      case 2:
      case 3:
        /* Moog ladder filter according to Antti Huovilainen. */

/* I, C, V = electrical parameters
 * f = center frequency
 * r = resonance amount 0 .. 1
 *
 * ya(n) = ya(n-1) + I / (C * f) * (tanh( (x(n) - 4 * r * yd(n-1)) / (2 * V) ) - Wa(n-1))
 * yb(n) = yb(n-1) + I / (C * f) * (Wa(n) - Wb(n-1))
 * yc(n) = yc(n-1) + I / (C * f) * (Wb(n) - Wc(n-1))
 * yd(n) = yd(n-1) + I / (C * f) * (Wc(n) - Wd(n-1))
 *
 * Wx = tanh(Yx(n) / (2 * Vt)) */

        g = 1.f - expf((float) (-2.0 * M_PI) * freq / sample_rate);
        g2 = g;
        /* TB-303 style: the first phase is one octave higher than rest */
        if (* ap->method == 3)
            g2 = 1.f - expf((float) (-2.0 * M_PI) * 2 * freq / sample_rate);
        for (i = 0; i < db->len; i += 1) {
#define PARAM_V (MAX_SAMPLE * 1.0) /* the sound gets dirtier if the factor gets small */
            ap->ya[curr_channel] += (float) PARAM_V * g2 *
                (tanhf( (db->data[i] - 4.f * *ap->res/100.0f * ap->yd[curr_channel]) / (float) PARAM_V )
                 - tanhf( ap->ya[curr_channel] / (float) PARAM_V));
            ap->yb[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->ya[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yb[curr_channel] / (float) PARAM_V ));
            ap->yc[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->yb[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yc[curr_channel] / (float) PARAM_V ));
            ap->yd[curr_channel] += (float) PARAM_V * g *
                (tanhf( ap->yc[curr_channel] / (float) PARAM_V )
                 - tanhf( ap->yd[curr_channel] / (float) PARAM_V ));

            /* the wah causes a gain loss of 12 dB which, but due to
             * resonance we may clip; to compromise I'll adjust 6 dB back. */
            db->data[i] = ap->yd[curr_channel] * 2.f;
            curr_channel = (curr_channel + 1) % db->channels;
        }
        break;

      default:
        break;
    }
    
    /* mix with dry sound */
    for (i = 0; i < db->len; i++)
        ap->output [i] = (ap->input [i] * *ap->drywet + ap->output [i] *(100.f- *ap->drywet))/100.0f;

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

