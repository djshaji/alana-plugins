/* 

    (c) Fraser Stuart 2009

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

/* 2.0 * atan(1.0) */
#define PI_ON_2 1.570796327

/* -1.0 / log10(sin(0.2 * atan(1.0))) */
#define ITUBE_MAGIC 1.241206735

/*2^31-2 */
#define TWO31_MINUS2 2147483646

/* param change detect function */
void checkParamChange(unsigned long param, LADSPA_Data * control, LADSPA_Data * last, LADSPA_Data * converted, unsigned long sr, LADSPA_Data (*ConvertFunction)(unsigned long, LADSPA_Data, unsigned long));

/* soft clipping function */
LADSPA_Data InoClip(LADSPA_Data in);

/* distortion function */
LADSPA_Data ITube(LADSPA_Data in, LADSPA_Data Drive);
