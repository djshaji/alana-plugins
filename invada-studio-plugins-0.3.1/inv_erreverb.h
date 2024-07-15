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

#define REVERB_SPACE_SIZE 2   /* size in seconds */
#define MAX_ER            60  /* maximun number of early reflections to calculate */
#define SPEED_OF_SOUND    330 /* speed of sound in air in meters/second */
#define OBJECT_HEIGHT     1.5 /* the height of the sound source and the listener */

struct ERunit {
	int Active;
	LADSPA_Data rand;
	LADSPA_Data DelayActual;
	LADSPA_Data DelayOffset;
	unsigned long Delay;
	unsigned int Reflections;
	LADSPA_Data AbsGain;
	LADSPA_Data GainL;
	LADSPA_Data GainR;
};

/* works out the reflection details */
void calculateIreverbER(LADSPA_Handle Instance);

/* works out a single reflection */
void calculateSingleIreverbER(struct ERunit * er, LADSPA_Data Width, LADSPA_Data Length, LADSPA_Data Height, int Phase, unsigned int reflections, LADSPA_Data DDist, unsigned long sr);

/* control conversion function */
LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr);

