/* 

    This LADSPA plugin provides an early reflection reverb from a mono source

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


    Some Notes:
    ===========
    This plugin is not your classic sort of reverb that makes long washy spaces.
    It simulates a room by calculating the early reflections that occur off the walls/roof etc.

    It's useful for:
    a) putting dry signals in a 'natural' space so they sit with sounds recorded with 'room' anbience in them.
    b) thickening strings, vocals etc without softening or washing out.
    c) as a singal preprocess for reverb that does not have early reflections.
    d) accurate stereo placement by setting the 'source' LR the same as any panning the dry signle has. This is 
    because the early reflections from the off-center single reinforce the location of the sound rather than 
    contradicting it as a normal reverb would do. 

    Parameter description:
    RoomLength,RoomWidth,RoomHeight - the dimensions (in meters) of the room
    SourceLR, SourceFB (FB=Front/Back) - where the sound source is in the room (always the back half)
    DestLR, DestFB (FB=Front/Back) - where the destination (or listener) is in the room (always the front half)
    HPF - roll off some bottom end
    Warmth - roll off top end (amount depends on reflection count)
    Diffusion - makes the relections less perfect to simulate objects in the room.

*/

#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include <ladspa.h>
#include "libinv_common.h"
#include "inv_erreverb.h"

/* The port numbers for the plugin: */

#define IERR_ROOMLENGTH 0
#define IERR_ROOMWIDTH 1
#define IERR_ROOMHEIGHT 2
#define IERR_SOURCELR 3
#define IERR_SOURCEFB 4
#define IERR_DESTLR 5
#define IERR_DESTFB 6
#define IERR_HPF 7
#define IERR_WARMTH 8
#define IERR_DIFFUSION 9
#define IERR_AUDIO_OUT1 10
#define IERR_AUDIO_OUT2 11 
#define IERR_AUDIO_IN1  12
#define IERR_AUDIO_IN2  13   /* not used in mono in mode */ 




/* The structure used to hold port connection information and state. */

typedef struct {
	unsigned long SampleRate;

	/* Ports */
	LADSPA_Data * ControlRoomLength;
	LADSPA_Data * ControlRoomWidth; 
	LADSPA_Data * ControlRoomHeight;
	LADSPA_Data * ControlSourceLR;
	LADSPA_Data * ControlSourceFB;
	LADSPA_Data * ControlDestLR; 
	LADSPA_Data * ControlDestFB;
	LADSPA_Data * ControlHPF;
	LADSPA_Data * ControlWarmth;
	LADSPA_Data * ControlDiffusion;

	LADSPA_Data * AudioOutputBuffer1;
	LADSPA_Data * AudioOutputBuffer2;
	LADSPA_Data * AudioInputBuffer1;
	LADSPA_Data * AudioInputBuffer2; 

	/* Stuff to remember to avoid recalculating the delays every run */
	LADSPA_Data LastRoomLength;
	LADSPA_Data LastRoomWidth; 
	LADSPA_Data LastRoomHeight;
	LADSPA_Data LastSourceLR;
	LADSPA_Data LastSourceFB;
	LADSPA_Data LastDestLR; 
	LADSPA_Data LastDestFB;
	LADSPA_Data LastHPF;
	LADSPA_Data LastWarmth; 
	LADSPA_Data LastDiffusion;

	LADSPA_Data ConvertedHPF; 
	LADSPA_Data ConvertedWarmth; 

	/* Delay Data */
	unsigned int er_size;
	struct ERunit * er;
	unsigned long SpaceSize;
	LADSPA_Data * SpaceL;
	LADSPA_Data * SpaceR;
	LADSPA_Data * SpaceLCur;
	LADSPA_Data * SpaceRCur;
	LADSPA_Data * SpaceLEnd;
	LADSPA_Data * SpaceREnd;

	LADSPA_Data AudioHPFLast;
	LADSPA_Data AudioIn1Last;
	LADSPA_Data AudioIn2Last;
	LADSPA_Data AudioIn3Last; 
	LADSPA_Data AudioIn4Last;

} IreverbER;

/*****************************************************************************/

/* Construct a new plugin instance. */
LADSPA_Handle instantiateIreverbER(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate) {

	LADSPA_Handle Instance;
	IreverbER * plug;
				 
	if((Instance=malloc(sizeof(IreverbER)))==NULL)
		return NULL;
				 
	/* set some initial params */
	plug = (IreverbER *)Instance;

	/* should check SampleRate > 0 */
	plug->SampleRate=SampleRate;
	plug->SpaceSize = REVERB_SPACE_SIZE * SampleRate;

	/* the delay space */
	if((plug->SpaceL  = (LADSPA_Data *)malloc(sizeof(LADSPA_Data) * plug->SpaceSize))==NULL)
    		return NULL;
	
	if((plug->SpaceR  = (LADSPA_Data *)malloc(sizeof(LADSPA_Data) * plug->SpaceSize))==NULL)
    		return NULL;
	
	/* the delays */
	if((plug->er  = (struct ERunit *)malloc(sizeof(struct ERunit) * MAX_ER))==NULL)
    		return NULL;	

	return Instance;
}

/*****************************************************************************/

/* Connect a port to a data location. */
void connectPortToIreverbER(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {

	IreverbER * plug;

	plug = (IreverbER *)Instance;
	switch (Port) {
		case IERR_ROOMLENGTH:
			plug->ControlRoomLength = DataLocation;
			break;
		case IERR_ROOMWIDTH:
			plug->ControlRoomWidth = DataLocation;
			break;
		case IERR_ROOMHEIGHT:
			plug->ControlRoomHeight = DataLocation;
			break;
		case IERR_SOURCELR:
			plug->ControlSourceLR = DataLocation;
			break;
		case IERR_SOURCEFB:
			plug->ControlSourceFB = DataLocation;
			break;
		case IERR_DESTLR:
			plug->ControlDestLR = DataLocation;
			break;
		case IERR_DESTFB:
			plug->ControlDestFB = DataLocation;
			break;
		case IERR_HPF:
			plug->ControlHPF = DataLocation;
			break;
		case IERR_WARMTH:
			plug->ControlWarmth = DataLocation;
			break;
		case IERR_DIFFUSION:
			plug->ControlDiffusion = DataLocation;
			break;
		case IERR_AUDIO_OUT1:
			plug->AudioOutputBuffer1 = DataLocation;
			break;
		case IERR_AUDIO_OUT2:
			plug->AudioOutputBuffer2 = DataLocation;
			break;
		case IERR_AUDIO_IN1:
			plug->AudioInputBuffer1 = DataLocation;
			break;
		case IERR_AUDIO_IN2:
			plug->AudioInputBuffer2 = DataLocation;
			break;
	}
}

void activateIreverbER(LADSPA_Handle Instance) {

	unsigned long i;
	LADSPA_Data * p;
	LADSPA_Data * q;
	IreverbER * plug;	
	plug = (IreverbER *)Instance;

	//set ourselves at the beginning of space
	plug->SpaceLCur=plug->SpaceL;
	plug->SpaceRCur=plug->SpaceR;

	// clear space	
	p=plug->SpaceL;
	q=plug->SpaceR;
	for(i=0; i < plug->SpaceSize; i++) {
		*(p++)=0;
		*(q++)=0;
	}
	plug->SpaceLEnd=--p;
	plug->SpaceREnd=--q;
  
	//set defaults
	plug->LastRoomLength = 26.5;
	plug->LastRoomWidth  = 26.5; 
	plug->LastRoomHeight = 9.75;
	plug->LastSourceLR   = 0.5;
	plug->LastSourceFB   = 0.775;
	plug->LastDestLR     = 0.5; 
	plug->LastDestFB     = 0.225;
	plug->LastHPF        = 0.001;
	plug->LastWarmth     = 0.5;
	plug->LastDiffusion  = 0.5;

	plug->AudioHPFLast=0;
	plug->AudioIn1Last=0;
	plug->AudioIn2Last=0;
	plug->AudioIn3Last=0; 
	plug->AudioIn4Last=0;

	plug->ConvertedHPF    = convertParam(IERR_HPF,    plug->LastHPF,    plug->SampleRate);  
	plug->ConvertedWarmth = convertParam(IERR_WARMTH, plug->LastWarmth, plug->SampleRate);  
	calculateIreverbER(Instance);
}


void runIMonoreverbER(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data AudioIn,AudioHPF,AudioIn1,AudioIn2,AudioIn3,AudioIn4,AudioProc;
	LADSPA_Data HPFsamples,WarmthSamples;

	IreverbER * plug;
	struct ERunit * er;

	unsigned long lSampleIndex;
	unsigned int i;
	unsigned int er_size;
	unsigned long SpaceSize;

	plug = (IreverbER *)Instance;
	pParamFunc = &convertParam;

	LADSPA_Data * SpaceLStr;
	LADSPA_Data * SpaceRStr;
	LADSPA_Data * SpaceLCur;
	LADSPA_Data * SpaceRCur;
	LADSPA_Data * SpaceLEnd;
	LADSPA_Data * SpaceREnd;

	/* see if the room has changed and recalculate the reflection details if needed */
	if(*(plug->ControlRoomLength) != plug->LastRoomLength || 
	   *(plug->ControlRoomWidth)  != plug->LastRoomWidth  ||
	   *(plug->ControlRoomHeight) != plug->LastRoomHeight ||
	   *(plug->ControlSourceLR)   != plug->LastSourceLR   ||
	   *(plug->ControlSourceFB)   != plug->LastSourceFB   ||
	   *(plug->ControlDestLR)     != plug->LastDestLR     ||
	   *(plug->ControlDestFB)     != plug->LastDestFB     ||
	   *(plug->ControlDiffusion)  != plug->LastDiffusion  ) {
		  plug->LastRoomLength = *(plug->ControlRoomLength);
		  plug->LastRoomWidth  = *(plug->ControlRoomWidth);
		  plug->LastRoomHeight = *(plug->ControlRoomHeight);
		  plug->LastSourceLR   = *(plug->ControlSourceLR);
		  plug->LastSourceFB   = *(plug->ControlSourceFB);
		  plug->LastDestLR     = *(plug->ControlDestLR);
		  plug->LastDestFB     = *(plug->ControlDestFB);
		  plug->LastDiffusion  = *(plug->ControlDiffusion);
		  
		  calculateIreverbER(Instance);
	}

	/* check if any other params have changed */
	checkParamChange(IERR_WARMTH, plug->ControlWarmth, &(plug->LastWarmth), &(plug->ConvertedWarmth), plug->SampleRate, pParamFunc);
	checkParamChange(IERR_HPF,    plug->ControlHPF,    &(plug->LastHPF),    &(plug->ConvertedHPF),    plug->SampleRate, pParamFunc);

	WarmthSamples   = plug->ConvertedWarmth;
	HPFsamples   = plug->ConvertedHPF;
	
	er_size   =plug->er_size;
	SpaceSize =plug->SpaceSize;

	SpaceLStr =plug->SpaceL;
	SpaceRStr =plug->SpaceR;
	SpaceLCur =plug->SpaceLCur;
	SpaceRCur =plug->SpaceRCur;
	SpaceLEnd =plug->SpaceLEnd;
	SpaceREnd =plug->SpaceREnd;

	AudioHPF=plug->AudioHPFLast;
	AudioIn1=plug->AudioIn1Last;
	AudioIn2=plug->AudioIn2Last;
	AudioIn3=plug->AudioIn3Last;
	AudioIn4=plug->AudioIn4Last;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;

	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {

		AudioIn=*(pfAudioInput1++);
		// apply HPF as bottom end in reverbs sounds crap
		AudioHPF = ((HPFsamples-1) * AudioHPF + AudioIn) / HPFsamples;  
		AudioIn = AudioIn - AudioHPF;

		// apply simple LPF filter repeatedly to audio to simluate frequency loss with each reflection
		AudioIn1=((WarmthSamples-1) * AudioIn1 + AudioIn) / WarmthSamples; 
		AudioIn2=((WarmthSamples-1) * AudioIn2 + AudioIn1) / WarmthSamples; 
		AudioIn3=((WarmthSamples-1) * AudioIn3 + AudioIn2) / WarmthSamples; 
		AudioIn4=((WarmthSamples-1) * AudioIn4 + AudioIn3) / WarmthSamples; 
		  
		er = plug->er;
		  
		// now calculate the reflections
		for(i=0;i<er_size;i++) {
			// pick the right version of the audio as per reflection count
			switch(er->Reflections) {
				case 0:
					AudioProc=AudioIn;
					break;
				case 1:
					AudioProc=AudioIn1;
					break;
				case 2:
					AudioProc=AudioIn2;
					break;
				case 3:
					AudioProc=AudioIn3;
					break;
				case 4:
				default:
					AudioProc=AudioIn4;
					break;
			}
			// add the reflection into the delay space
			if(SpaceLCur+er->Delay > SpaceLEnd)
				*(SpaceLCur+er->Delay-SpaceSize)+=AudioProc*er->GainL*(1-er->DelayOffset);
			else
				*(SpaceLCur+er->Delay)+=AudioProc*er->GainL*(1-er->DelayOffset);

			if(SpaceLCur+er->Delay+1 > SpaceLEnd)
				*(SpaceLCur+er->Delay-SpaceSize+1)+=AudioProc*er->GainL*er->DelayOffset;
			else
				*(SpaceLCur+er->Delay+1)+=AudioProc*er->GainL*er->DelayOffset;

			if(SpaceRCur+er->Delay > SpaceREnd)
				*(SpaceRCur+er->Delay-SpaceSize)+=AudioProc*er->GainR*(1-er->DelayOffset);
			else
				*(SpaceRCur+er->Delay)+=AudioProc*er->GainR*(1-er->DelayOffset);

			if(SpaceRCur+er->Delay+1 > SpaceREnd)
				*(SpaceRCur+er->Delay-SpaceSize+1)+=AudioProc*er->GainR*er->DelayOffset;
			else
				*(SpaceRCur+er->Delay+1)+=AudioProc*er->GainR*er->DelayOffset;
 
			er++;
		}
		// read the audio out of the delay space
		*(pfAudioOutput1++) = *(SpaceLCur);
		*(pfAudioOutput2++) = *(SpaceRCur);
		// zero the spot we just read
		*(SpaceLCur)=0;
		*(SpaceRCur)=0;
		// advance the pointer to the next spot
		SpaceLCur = SpaceLCur < SpaceLEnd ? SpaceLCur + 1 : SpaceLStr;
		SpaceRCur = SpaceRCur < SpaceREnd ? SpaceRCur + 1 : SpaceRStr;
	
	}
	// remember for next run
	plug->SpaceLCur=SpaceLCur;
	plug->SpaceRCur=SpaceRCur;
	plug->AudioHPFLast=(fabs(AudioHPF)<1.0e-10)  ? 0.f : AudioHPF; 
	plug->AudioIn1Last=(fabs(AudioIn1)<1.0e-10)  ? 0.f : AudioIn1; 
	plug->AudioIn2Last=(fabs(AudioIn2)<1.0e-10)  ? 0.f : AudioIn2; 
	plug->AudioIn3Last=(fabs(AudioIn3)<1.0e-10)  ? 0.f : AudioIn3; 
	plug->AudioIn4Last=(fabs(AudioIn4)<1.0e-10)  ? 0.f : AudioIn4; 
}

void runISumreverbER(LADSPA_Handle Instance, unsigned long SampleCount) {

	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data AudioIn,AudioHPF,AudioIn1,AudioIn2,AudioIn3,AudioIn4,AudioProc;
	LADSPA_Data HPFsamples,WarmthSamples;

	IreverbER * plug;
	struct ERunit * er;

	unsigned long lSampleIndex;
	unsigned int i;
	unsigned int er_size;
	unsigned long SpaceSize;

	plug = (IreverbER *)Instance;
	pParamFunc = &convertParam;

	LADSPA_Data * SpaceLStr;
	LADSPA_Data * SpaceRStr;
	LADSPA_Data * SpaceLCur;
	LADSPA_Data * SpaceRCur;
	LADSPA_Data * SpaceLEnd;
	LADSPA_Data * SpaceREnd;
	
	/* see if the room has changed and recalculate the refeclection details if needed */
	if(*(plug->ControlRoomLength) != plug->LastRoomLength || 
	   *(plug->ControlRoomWidth)  != plug->LastRoomWidth  ||
	   *(plug->ControlRoomHeight) != plug->LastRoomHeight ||
	   *(plug->ControlSourceLR)   != plug->LastSourceLR   ||
	   *(plug->ControlSourceFB)   != plug->LastSourceFB   ||
	   *(plug->ControlDestLR)     != plug->LastDestLR     ||
	   *(plug->ControlDestFB)     != plug->LastDestFB     ||
	   *(plug->ControlDiffusion)  != plug->LastDiffusion  ) {
		  plug->LastRoomLength = *(plug->ControlRoomLength);
		  plug->LastRoomWidth  = *(plug->ControlRoomWidth);
		  plug->LastRoomHeight = *(plug->ControlRoomHeight);
		  plug->LastSourceLR   = *(plug->ControlSourceLR);
		  plug->LastSourceFB   = *(plug->ControlSourceFB);
		  plug->LastDestLR     = *(plug->ControlDestLR);
		  plug->LastDestFB     = *(plug->ControlDestFB);
		  plug->LastDiffusion  = *(plug->ControlDiffusion);
		  
		  calculateIreverbER(Instance);
	}

	/* check if any other params have changed */
	checkParamChange(IERR_WARMTH, plug->ControlWarmth, &(plug->LastWarmth), &(plug->ConvertedWarmth), plug->SampleRate, pParamFunc);
	checkParamChange(IERR_HPF,    plug->ControlHPF,    &(plug->LastHPF),    &(plug->ConvertedHPF),    plug->SampleRate, pParamFunc);

	WarmthSamples = plug->ConvertedWarmth;
	HPFsamples    = plug->ConvertedHPF;
	
	er_size   =plug->er_size;
	SpaceSize =plug->SpaceSize;

	SpaceLStr =plug->SpaceL;
	SpaceRStr =plug->SpaceR;
	SpaceLCur =plug->SpaceLCur;
	SpaceRCur =plug->SpaceRCur;
	SpaceLEnd =plug->SpaceLEnd;
	SpaceREnd =plug->SpaceREnd;

	AudioHPF=plug->AudioHPFLast;
	AudioIn1=plug->AudioIn1Last;
	AudioIn2=plug->AudioIn2Last;
	AudioIn3=plug->AudioIn3Last;
	AudioIn4=plug->AudioIn4Last;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioInput2 = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;

	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {

		AudioIn=( *(pfAudioInput1++) + *(pfAudioInput2++) )/2;

		// apply HPF as bottom end in reverbs sounds crap
		AudioHPF = ((HPFsamples-1) * AudioHPF + AudioIn) / HPFsamples;  
		AudioIn = AudioIn - AudioHPF;

		// apply simple filter repeatedly to audio to simluate frequency loss with each reflection
		AudioIn1=((WarmthSamples-1) * AudioIn1 + AudioIn) / WarmthSamples; 
		AudioIn2=((WarmthSamples-1) * AudioIn2 + AudioIn1) / WarmthSamples; 
		AudioIn3=((WarmthSamples-1) * AudioIn3 + AudioIn2) / WarmthSamples; 
		AudioIn4=((WarmthSamples-1) * AudioIn4 + AudioIn3) / WarmthSamples; 
		  
		er = plug->er;
		  
		// now calculate the reflections
		for(i=0;i<er_size;i++) {
			// pick the right version of the audio as per reflection count
			switch(er->Reflections) {
				case 0:
					AudioProc=AudioIn;
					break;
				case 1:
					AudioProc=AudioIn1;
					break;
				case 2:
					AudioProc=AudioIn2;
					break;
				case 3:
					AudioProc=AudioIn3;
					break;
				case 4:
				default:
					AudioProc=AudioIn4;
					break;
			}
			// add the reflection into the delay space
			if(SpaceLCur+er->Delay > SpaceLEnd)
				*(SpaceLCur+er->Delay-SpaceSize)+=AudioProc*er->GainL*(1-er->DelayOffset);
			else
				*(SpaceLCur+er->Delay)+=AudioProc*er->GainL*(1-er->DelayOffset);

			if(SpaceLCur+er->Delay+1 > SpaceLEnd)
				*(SpaceLCur+er->Delay-SpaceSize+1)+=AudioProc*er->GainL*er->DelayOffset;
			else
				*(SpaceLCur+er->Delay+1)+=AudioProc*er->GainL*er->DelayOffset;

			if(SpaceRCur+er->Delay > SpaceREnd)
				*(SpaceRCur+er->Delay-SpaceSize)+=AudioProc*er->GainR*(1-er->DelayOffset);
			else
				*(SpaceRCur+er->Delay)+=AudioProc*er->GainR*(1-er->DelayOffset);

			if(SpaceRCur+er->Delay+1 > SpaceREnd)
				*(SpaceRCur+er->Delay-SpaceSize+1)+=AudioProc*er->GainR*er->DelayOffset;
			else
				*(SpaceRCur+er->Delay+1)+=AudioProc*er->GainR*er->DelayOffset;
 
			er++;
		}
		// read the audio out of the delay space
		*(pfAudioOutput1++) = *(SpaceLCur);
		*(pfAudioOutput2++) = *(SpaceRCur);
		// zero the spot we just read
		*(SpaceLCur)=0;
		*(SpaceRCur)=0;
		// advance the pointer to the next spot
		SpaceLCur = SpaceLCur < SpaceLEnd ? SpaceLCur + 1 : SpaceLStr;
		SpaceRCur = SpaceRCur < SpaceREnd ? SpaceRCur + 1 : SpaceRStr;
	
	}
	// remember for next run
	plug->SpaceLCur=SpaceLCur;
	plug->SpaceRCur=SpaceRCur;
	plug->AudioHPFLast=(fabs(AudioHPF)<1.0e-10)  ? 0.f : AudioHPF; 
	plug->AudioIn1Last=(fabs(AudioIn1)<1.0e-10)  ? 0.f : AudioIn1; 
	plug->AudioIn2Last=(fabs(AudioIn2)<1.0e-10)  ? 0.f : AudioIn2; 
	plug->AudioIn3Last=(fabs(AudioIn3)<1.0e-10)  ? 0.f : AudioIn3; 
	plug->AudioIn4Last=(fabs(AudioIn4)<1.0e-10)  ? 0.f : AudioIn4; 
}

/* Throw away a reverb. */
void cleanupIreverbER(LADSPA_Handle Instance) {

	IreverbER * plug;
	plug = (IreverbER *)Instance;

	free(plug->er);
	free(plug->SpaceL);
	free(plug->SpaceR);
	free(Instance);
}

LADSPA_Descriptor * g_psMonoReverbDescriptor = NULL;
LADSPA_Descriptor * g_psSumReverbDescriptor = NULL;

/* _init() is called automatically when the plugin library is first loaded. */
void _init() {

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psMonoReverbDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psSumReverbDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

	if (g_psMonoReverbDescriptor) {
		g_psMonoReverbDescriptor->UniqueID        = 3311;
		g_psMonoReverbDescriptor->Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psMonoReverbDescriptor->Label           = strdup("invada_mono_reverbER_module_0_1");
		g_psMonoReverbDescriptor->Name            = strdup(":: Invada :: ER Reverb - Mono In");
		g_psMonoReverbDescriptor->Maker           = strdup("Fraser At Invada Records dot Com");
		g_psMonoReverbDescriptor->Copyright       = strdup("(c) Invada Records");      

		g_psMonoReverbDescriptor->PortCount       = 13;

		piPortDescriptors                           = (LADSPA_PortDescriptor *)calloc(13, sizeof(LADSPA_PortDescriptor));
		g_psMonoReverbDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;


		piPortDescriptors[IERR_ROOMLENGTH] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_ROOMWIDTH]  = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_ROOMHEIGHT] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	  
		piPortDescriptors[IERR_SOURCELR]   = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_SOURCEFB]   = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_DESTLR]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	 
		piPortDescriptors[IERR_DESTFB]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_HPF]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_WARMTH]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_DIFFUSION]  = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	 
		piPortDescriptors[IERR_AUDIO_OUT1] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IERR_AUDIO_OUT2] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IERR_AUDIO_IN1]  = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;

    
		pcPortNames                         = (char **)calloc(13, sizeof(char *));
		g_psMonoReverbDescriptor->PortNames = (const char **)pcPortNames;

		pcPortNames[IERR_ROOMLENGTH]          = strdup("Room Length");
		pcPortNames[IERR_ROOMWIDTH]           = strdup("Room Width");
		pcPortNames[IERR_ROOMHEIGHT]          = strdup("Room Height");  
		pcPortNames[IERR_SOURCELR]            = strdup("Source (L/R)");
		pcPortNames[IERR_SOURCEFB]            = strdup("Source (F/B)");
		pcPortNames[IERR_DESTLR]              = strdup("Listener (L/R)");
		pcPortNames[IERR_DESTFB]              = strdup("Listener (F/B)");
		pcPortNames[IERR_HPF]                 = strdup("HPF (Hz)");
		pcPortNames[IERR_WARMTH]              = strdup("Warmth");
		pcPortNames[IERR_DIFFUSION]           = strdup("Diffusion");
		pcPortNames[IERR_AUDIO_OUT1]          = strdup("Output (Left)");
		pcPortNames[IERR_AUDIO_OUT2]          = strdup("Output (Right)");
		pcPortNames[IERR_AUDIO_IN1]           = strdup("Input");

		psPortRangeHints                         = ((LADSPA_PortRangeHint *) calloc(13, sizeof(LADSPA_PortRangeHint)));
		g_psMonoReverbDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;


		psPortRangeHints[IERR_ROOMLENGTH].HintDescriptor     = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMLENGTH].LowerBound         = 3;
		psPortRangeHints[IERR_ROOMLENGTH].UpperBound         = 100;
		psPortRangeHints[IERR_ROOMWIDTH].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMWIDTH].LowerBound          = 3;
		psPortRangeHints[IERR_ROOMWIDTH].UpperBound          = 100;
		psPortRangeHints[IERR_ROOMHEIGHT].HintDescriptor     = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMHEIGHT].LowerBound         = 3;
		psPortRangeHints[IERR_ROOMHEIGHT].UpperBound         = 30;
		psPortRangeHints[IERR_SOURCELR].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_SOURCELR].LowerBound           = 0.01;
		psPortRangeHints[IERR_SOURCELR].UpperBound           = 0.99;
		psPortRangeHints[IERR_SOURCEFB].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_SOURCEFB].LowerBound           = 0.55;
		psPortRangeHints[IERR_SOURCEFB].UpperBound           = 1;
		psPortRangeHints[IERR_DESTLR].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DESTLR].LowerBound             = 0.01;
		psPortRangeHints[IERR_DESTLR].UpperBound             = 0.99;
		psPortRangeHints[IERR_DESTFB].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DESTFB].LowerBound             = 0;
		psPortRangeHints[IERR_DESTFB].UpperBound             = 0.45;
		psPortRangeHints[IERR_HPF].HintDescriptor            = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[IERR_HPF].LowerBound                = 0.001;
		psPortRangeHints[IERR_HPF].UpperBound                = 0.05;
		psPortRangeHints[IERR_WARMTH].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_WARMTH].LowerBound             = 0;
		psPortRangeHints[IERR_WARMTH].UpperBound             = 1;
		psPortRangeHints[IERR_DIFFUSION].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DIFFUSION].LowerBound          = 0;
		psPortRangeHints[IERR_DIFFUSION].UpperBound          = 1;
		psPortRangeHints[IERR_AUDIO_OUT1].HintDescriptor     = 0;
		psPortRangeHints[IERR_AUDIO_OUT2].HintDescriptor     = 0;
		psPortRangeHints[IERR_AUDIO_IN1].HintDescriptor      = 0;

        
		g_psMonoReverbDescriptor->instantiate         = instantiateIreverbER;
		g_psMonoReverbDescriptor->connect_port        = connectPortToIreverbER;
		g_psMonoReverbDescriptor->activate            = activateIreverbER;
		g_psMonoReverbDescriptor->run                 = runIMonoreverbER;
		g_psMonoReverbDescriptor->run_adding          = NULL;
		g_psMonoReverbDescriptor->set_run_adding_gain = NULL;
		g_psMonoReverbDescriptor->deactivate          = NULL;
		g_psMonoReverbDescriptor->cleanup             = cleanupIreverbER;
	}
	if (g_psSumReverbDescriptor) {
		g_psSumReverbDescriptor->UniqueID        = 3312;
		g_psSumReverbDescriptor->Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psSumReverbDescriptor->Label           = strdup("invada_sum_reverbER_module_0_1");
		g_psSumReverbDescriptor->Name            = strdup(":: Invada :: ER Reverb - Sum L+R In");
		g_psSumReverbDescriptor->Maker           = strdup("Fraser At Invada Records dot Com");
		g_psSumReverbDescriptor->Copyright       = strdup("(c) Invada Records");      

		g_psSumReverbDescriptor->PortCount       = 14;

		piPortDescriptors                           = (LADSPA_PortDescriptor *)calloc(14, sizeof(LADSPA_PortDescriptor));
		g_psSumReverbDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;


		piPortDescriptors[IERR_ROOMLENGTH] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_ROOMWIDTH]  = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_ROOMHEIGHT] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	  
		piPortDescriptors[IERR_SOURCELR]   = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_SOURCEFB]   = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_DESTLR]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	 
		piPortDescriptors[IERR_DESTFB]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_HPF]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_WARMTH]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IERR_DIFFUSION]  = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;	 
		piPortDescriptors[IERR_AUDIO_OUT1] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IERR_AUDIO_OUT2] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IERR_AUDIO_IN1]  = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IERR_AUDIO_IN2]  = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
    
		pcPortNames                         = (char **)calloc(14, sizeof(char *));
		g_psSumReverbDescriptor->PortNames = (const char **)pcPortNames;

		pcPortNames[IERR_ROOMLENGTH]          = strdup("Room Length");
		pcPortNames[IERR_ROOMWIDTH]           = strdup("Room Width");
		pcPortNames[IERR_ROOMHEIGHT]          = strdup("Room Height");  
		pcPortNames[IERR_SOURCELR]            = strdup("Source (L/R)");
		pcPortNames[IERR_SOURCEFB]            = strdup("Source (F/B)");
		pcPortNames[IERR_DESTLR]              = strdup("Listener (L/R)");
		pcPortNames[IERR_DESTFB]              = strdup("Listener (F/B)");
		pcPortNames[IERR_HPF]                 = strdup("HPF (Hz)");
		pcPortNames[IERR_WARMTH]              = strdup("Warmth");
		pcPortNames[IERR_DIFFUSION]           = strdup("Diffusion");
		pcPortNames[IERR_AUDIO_OUT1]          = strdup("Output (Left)");
		pcPortNames[IERR_AUDIO_OUT2]          = strdup("Output (Right)");
		pcPortNames[IERR_AUDIO_IN1]           = strdup("Input (Left)");
		pcPortNames[IERR_AUDIO_IN2]           = strdup("Input (Right)");

		psPortRangeHints                         = ((LADSPA_PortRangeHint *) calloc(14, sizeof(LADSPA_PortRangeHint)));
		g_psSumReverbDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;


		psPortRangeHints[IERR_ROOMLENGTH].HintDescriptor     = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMLENGTH].LowerBound         = 3;
		psPortRangeHints[IERR_ROOMLENGTH].UpperBound         = 100;
		psPortRangeHints[IERR_ROOMWIDTH].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMWIDTH].LowerBound          = 3;
		psPortRangeHints[IERR_ROOMWIDTH].UpperBound          = 100;
		psPortRangeHints[IERR_ROOMHEIGHT].HintDescriptor     = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[IERR_ROOMHEIGHT].LowerBound         = 3;
		psPortRangeHints[IERR_ROOMHEIGHT].UpperBound         = 30;
		psPortRangeHints[IERR_SOURCELR].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_SOURCELR].LowerBound           = 0.01;
		psPortRangeHints[IERR_SOURCELR].UpperBound           = 0.99;
		psPortRangeHints[IERR_SOURCEFB].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_SOURCEFB].LowerBound           = 0.55;
		psPortRangeHints[IERR_SOURCEFB].UpperBound           = 1;
		psPortRangeHints[IERR_DESTLR].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DESTLR].LowerBound             = 0.01;
		psPortRangeHints[IERR_DESTLR].UpperBound             = 0.99;
		psPortRangeHints[IERR_DESTFB].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DESTFB].LowerBound             = 0;
		psPortRangeHints[IERR_DESTFB].UpperBound             = 0.45;
		psPortRangeHints[IERR_HPF].HintDescriptor            = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[IERR_HPF].LowerBound                = 0.001;
		psPortRangeHints[IERR_HPF].UpperBound                = 0.05;
		psPortRangeHints[IERR_WARMTH].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_WARMTH].LowerBound             = 0;
		psPortRangeHints[IERR_WARMTH].UpperBound             = 1;
		psPortRangeHints[IERR_DIFFUSION].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IERR_DIFFUSION].LowerBound          = 0;
		psPortRangeHints[IERR_DIFFUSION].UpperBound          = 1;
		psPortRangeHints[IERR_AUDIO_OUT1].HintDescriptor     = 0;
		psPortRangeHints[IERR_AUDIO_OUT2].HintDescriptor     = 0;
		psPortRangeHints[IERR_AUDIO_IN1].HintDescriptor      = 0;
		psPortRangeHints[IERR_AUDIO_IN2].HintDescriptor      = 0;

        
		g_psSumReverbDescriptor->instantiate         = instantiateIreverbER;
		g_psSumReverbDescriptor->connect_port        = connectPortToIreverbER;
		g_psSumReverbDescriptor->activate            = activateIreverbER;
		g_psSumReverbDescriptor->run                 = runISumreverbER;
		g_psSumReverbDescriptor->run_adding          = NULL;
		g_psSumReverbDescriptor->set_run_adding_gain = NULL;
		g_psSumReverbDescriptor->deactivate          = NULL;
		g_psSumReverbDescriptor->cleanup             = cleanupIreverbER;
	}
}

void deleteDescriptor(LADSPA_Descriptor * psDescriptor) {
	unsigned long lIndex;
	if (psDescriptor) {
		free((char *)psDescriptor->Label);
		free((char *)psDescriptor->Name);
		free((char *)psDescriptor->Maker);
		free((char *)psDescriptor->Copyright);
		free((LADSPA_PortDescriptor *)psDescriptor->PortDescriptors);
		for (lIndex = 0; lIndex < psDescriptor->PortCount; lIndex++)
			free((char *)(psDescriptor->PortNames[lIndex]));
		free((char **)psDescriptor->PortNames);
		free((LADSPA_PortRangeHint *)psDescriptor->PortRangeHints);
		free(psDescriptor);
	}
} 

/* _fini() is called automatically when the library is unloaded. */
void _fini() {
	deleteDescriptor(g_psMonoReverbDescriptor);
	deleteDescriptor(g_psSumReverbDescriptor);
}

/* Return a descriptor of the requested plugin type.  */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index) {

	switch (Index) {
		case 0:
			return g_psMonoReverbDescriptor;
		case 1:
			return g_psSumReverbDescriptor;
		default:
			return NULL;
	}
}

void calculateIreverbER(LADSPA_Handle Instance) {
	IreverbER * plug;	
	plug = (IreverbER *)Instance;

	LADSPA_Data SourceToLeft,SourceToRight,SourceToRear,SourceToFront;
	LADSPA_Data DestToLeft,DestToRight,DestToRear,DestToFront;
	LADSPA_Data RoofHeight,FloorDepth;
	LADSPA_Data DirectLength,DirectWidth,DirectHeight,DirectDistanceSQRD,DirectDistance;
	LADSPA_Data ERLength,ERWidth,ERHeight,MaxGain;
	LADSPA_Data convertedWidth,convertedLength,convertedHeight,convertedSourceLR,convertedSourceFB,convertedDestLR,convertedDestFB,convertedDiffusion;

	struct ERunit *er, *er2;;
	unsigned int TotalNum,Num,i;

	if (plug->LastRoomWidth < 3.0)
		convertedWidth = 3.0;
	else if (plug->LastRoomWidth <= 100.0)
		convertedWidth = plug->LastRoomWidth;
	else
		convertedWidth = 100.0;

	if (plug->LastRoomLength < 3.0)
		convertedLength = 3.0;
	else if (plug->LastRoomLength <= 100.0)
		convertedLength = plug->LastRoomLength;
	else
		convertedLength = 100.0;

	if (plug->LastRoomHeight < 3.0)
		convertedHeight = 3.0;
	else if (plug->LastRoomHeight <= 30.0)
		convertedHeight = plug->LastRoomHeight;
	else
		convertedHeight = 30.0;

	if (plug->LastSourceLR < -0.99)
		convertedSourceLR = -0.99;
	else if (plug->LastSourceLR <= 0.99)
		convertedSourceLR = plug->LastSourceLR;
	else
		convertedSourceLR = 0.99;

	if (plug->LastSourceFB < 0.51)
		convertedSourceFB = 0.51;
	else if (plug->LastSourceFB <= 0.99)
		convertedSourceFB = plug->LastSourceFB;
	else
		convertedSourceFB = 0.99;

	if (plug->LastDestLR < -0.99)
		convertedDestLR = -0.99;
	else if (plug->LastDestLR <= 0.99)
		convertedDestLR = plug->LastDestLR;
	else
		convertedDestLR = 0.99;

	if (plug->LastDestFB < 0.01)
		convertedDestFB = 0.01;
	else if (plug->LastDestFB <= 0.49)
		convertedDestFB = plug->LastDestFB;
	else
		convertedDestFB = 0.49;

	if (plug->LastDiffusion < 0.0)
		convertedDiffusion = 0.0;
	else if (plug->LastDiffusion <= 1.0)
		convertedDiffusion = plug->LastDiffusion;
	else
		convertedDiffusion = 1.0;

	SourceToLeft = convertedSourceLR     * convertedWidth;
	SourceToRight= (1-convertedSourceLR) * convertedWidth;
	SourceToFront= convertedSourceFB     * convertedLength;
	SourceToRear = (1>convertedSourceFB) * convertedLength;

	DestToLeft = convertedDestLR     * convertedWidth;
	DestToRight= (1-convertedDestLR) * convertedWidth;
	DestToFront= convertedDestFB     * convertedLength;
	DestToRear = (1-convertedDestFB) * convertedLength;

	RoofHeight = convertedHeight - OBJECT_HEIGHT;
	FloorDepth = OBJECT_HEIGHT;

	DirectLength = SourceToFront-DestToFront;
	DirectWidth = SourceToLeft-DestToLeft;
	DirectHeight =0; // both the source and the lisenter are at the same height
	DirectDistanceSQRD = pow(DirectLength,2)+pow(DirectWidth,2) < 1 ? 1 : pow(DirectLength,2)+pow(DirectWidth,2);
	DirectDistance = sqrt(DirectDistanceSQRD) < 1 ? 1 : sqrt(DirectDistanceSQRD);

	er=plug->er;
	Num=0;
	MaxGain=0.000000000001; /* this is used to scale up the reflections so that the loudest one has a gain of 1 (0db) */

	/* seed the random sequence with a version of the diffusion */
	srand48(314159265);
  
	// reflections from the left wall
	// 0: S->Left->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, -1, 1, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 1: S->BackWall->Left->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 2: S->Right->Left->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToRight + convertedWidth + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 3: S->BackWall->Right->Left->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = -(SourceToRight + convertedWidth + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;  

	// 4: S->Left->Rigtht->Left->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + (2 * convertedWidth) + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 5: S->BackWall->Left->Right->Left->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = -(SourceToLeft + (2 * convertedWidth) + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 4, DirectDistance,  plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;  

	// reflections from the right wall
	// 6: S->Right->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 1, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 7: S->BackWall->Right->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 8: S->Left->Right->D
	ERLength       = DirectLength;
	ERWidth        = SourceToLeft + convertedWidth + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 9: S->BackWall->Left->Right->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = SourceToLeft + convertedWidth + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 10: S->Right->Left->Right->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + (2 * convertedWidth) + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 11: S->BackWall->Right->Left->Right->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = SourceToRight + (2 * convertedWidth) + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 4, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// reflections from the rear wall
	// 12: S->BackWall->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = DirectWidth;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 1, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 13: S->NearWall->BackWall->D
	ERLength       = SourceToFront + convertedLength + DestToRear;
	ERWidth        = DirectWidth;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 14: S->Left->NearWall->BackWall->D
	ERLength       = SourceToFront + convertedLength + DestToRear;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 15: S->Right->NearWall->BackWall->D
	ERLength       = SourceToFront + convertedLength + DestToRear;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = DirectHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// reflections from the roof
	// 16: S->Roof->Left->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = 2*RoofHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 17: S->Roof->Right->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = 2*RoofHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 1, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 18: S->BackWall->Roof->Left->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = 2*RoofHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 19: S->BackWall->Roof->Right->D
	ERLength       = SourceToRear + DestToRear;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = 2*RoofHeight;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// reflections from the floor 
	// 20: S->Floor->Left->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 21: S->Floor->Right->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, 1, 2, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// reflections from roof and floor
	// 22: S->Roof->Left->Floor->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + DestToLeft);
	ERHeight       = 2*RoofHeight + 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 23: S->Roof->Right->Floor->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + DestToRight;
	ERHeight       = 2*RoofHeight + 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 3, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;


	// 24: S->Roof->Left->Floor->Right->Roof->D
	ERLength       = DirectLength;
	ERWidth        = -(SourceToLeft + DirectWidth + DestToLeft);
	ERHeight       = 4*RoofHeight + 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength,  ERHeight, -1, 5, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;

	// 25: S->Roof->Right->Floor->Left->Roof->D
	ERLength       = DirectLength;
	ERWidth        = SourceToRight + DirectWidth + DestToRight;
	ERHeight       = 4*RoofHeight + 2*FloorDepth;
	calculateSingleIreverbER(er, ERWidth, ERLength, ERHeight, -1, 5, DirectDistance, plug->SampleRate);
	if(er->AbsGain > MaxGain)
		MaxGain=er->AbsGain;
	er++;
	Num++;


	er2=er;
	er=plug->er;
	TotalNum=Num;
	for(i=0;i<Num;i++) {
		//create a new reflection based on the diffusion
		if(convertedDiffusion > 0 && 4*er->AbsGain/MaxGain > 1-convertedDiffusion) {
			er2->Active=1;
			er2->rand=er->rand;
			er2->DelayActual=er->DelayActual*(1.085+(er->rand*convertedDiffusion/7));
			er2->Delay = (unsigned long)er2->DelayActual;
			er2->DelayOffset = er2->DelayActual - (float)er2->Delay;
			er2->Reflections=er->Reflections;
			er2->AbsGain=er->AbsGain*convertedDiffusion*0.6/MaxGain;
			er2->GainL=er->GainL*convertedDiffusion*0.6/MaxGain;
			er2->GainR=er->GainR*convertedDiffusion*0.6/MaxGain;
			TotalNum++;
			er2++;
		}

		//scale up reflection and calculate sample delay
		er->DelayActual=er->DelayActual*(1.01+(er->rand*convertedDiffusion/14));
		er->Delay = (unsigned long)er->DelayActual;
		er->DelayOffset = er->DelayActual - (float)er->Delay;
		er->AbsGain=er->AbsGain/MaxGain;
		er->GainL=er->GainL/MaxGain;
		er->GainR=er->GainR/MaxGain;
		er++;
	}

	plug->er_size = TotalNum; 

}

void calculateSingleIreverbER(struct ERunit * er, LADSPA_Data Width, LADSPA_Data Length, LADSPA_Data Height, int Phase, unsigned int Reflections, LADSPA_Data DDist, unsigned long sr) {


	LADSPA_Data ERAngle,ERDistanceSQRD,ERDistance,ERRelDelayActual,ERRelGain,ERRelGainL,ERRelGainR;

	ERAngle           = atan(Width/Length);
	ERDistanceSQRD    = pow(Length,2) + pow(Width,2)+ pow(Height,2);
	ERDistance        = sqrt(ERDistanceSQRD);
	ERRelDelayActual  = ((ERDistance-DDist) * (float)sr /SPEED_OF_SOUND);
	ERRelGain         = Phase / ERDistanceSQRD;
	ERRelGainL        = (ERRelGain * (1 - (ERAngle/PI_ON_2)))/2;
	ERRelGainR        = (ERRelGain * (1 + (ERAngle/PI_ON_2)))/2;

	er->Active=1;
	er->rand=drand48();
	er->DelayActual=ERRelDelayActual;
	er->Reflections=Reflections;
	er->AbsGain=fabs(ERRelGain);
	er->GainL=ERRelGainL;
	er->GainR=ERRelGainR;
}

LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr) {

	LADSPA_Data temp;
	LADSPA_Data result;

	switch(param)
	{
		case IERR_HPF:
			temp = value / (LADSPA_Data)sr;
			if (temp < 0.001)
				result = 500;
			else if (temp <= 0.05)
				result = 1/(2*temp);
			else
				result=10;
			break;
		case IERR_WARMTH:
			if(value<0)
				result= 1;
			else if (value < 1)
				result = pow(2,value*2);
			else
				result= 4;
			break;
			
			break;
		default:
			result=0;
			break;
	}
	return result;
}

