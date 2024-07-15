/* 

    This LADSPA plugin provides a stereo input module

    (c) Fraser Stuart 2008

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


#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ladspa.h>
#include "libinv_common.h"
#include "inv_input.h"

/* The port numbers for the plugin: */

#define IINPUT_PHASE1 0
#define IINPUT_PHASE2 1
#define IINPUT_GAIN 2
#define IINPUT_PAN 3
#define IINPUT_WIDTH 4
#define IINPUT_NOCLIP 5
#define IINPUT_AUDIO_IN1  6
#define IINPUT_AUDIO_IN2  7
#define IINPUT_AUDIO_OUT1 8
#define IINPUT_AUDIO_OUT2 9


/* The structure used to hold port connection information and state */

typedef struct {

	unsigned long SampleRate; 
	/* Ports */
	LADSPA_Data * ControlPhase1;
	LADSPA_Data * ControlPhase2;         
	LADSPA_Data * ControlGain;
	LADSPA_Data * ControlPan;  
	LADSPA_Data * ControlWidth;  
	LADSPA_Data * ControlNoClip;  
	LADSPA_Data * AudioInputBuffer1;
	LADSPA_Data * AudioInputBuffer2; 
	LADSPA_Data * AudioOutputBuffer1;
	LADSPA_Data * AudioOutputBuffer2;

	/* stuff we need to remember to reduce cpu */ 
	LADSPA_Data LastPhase1;
	LADSPA_Data LastPhase2;     
	LADSPA_Data LastGain;
	LADSPA_Data LastPan;  
	LADSPA_Data LastWidth;  
	LADSPA_Data LastNoClip; 

	LADSPA_Data ConvertedPhase1;
	LADSPA_Data ConvertedPhase2; 
	LADSPA_Data ConvertedGain;
	LADSPA_Data ConvertedPan;  
	LADSPA_Data ConvertedWidth;  
	LADSPA_Data ConvertedNoClip; 

} Iinput;


/* Construct a new plugin instance. */
LADSPA_Handle instantiateIinput(const LADSPA_Descriptor * Descriptor,unsigned long SampleRate) {

	LADSPA_Handle Instance;
	Iinput * plug;

	if((Instance=malloc(sizeof(Iinput)))==NULL)
		return NULL;
	/* set some initial params */
	plug = (Iinput *)Instance;
	/* should check SampleRate > 0 */
	plug->SampleRate=SampleRate;
	return Instance;
}


/* Connect a port to a data location. */
void connectPortToIinput(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {

	Iinput * plug;

	plug = (Iinput *)Instance;
	switch (Port) {
		case IINPUT_PHASE1:
			plug->ControlPhase1 = DataLocation;
			break;
		case IINPUT_PHASE2:
			plug->ControlPhase2 = DataLocation;
			break;
		case IINPUT_GAIN:
			plug->ControlGain  = DataLocation;
			break;
		case IINPUT_PAN:
			plug->ControlPan = DataLocation;
			break;
		case IINPUT_WIDTH:
			plug->ControlWidth = DataLocation;
			break;
		case IINPUT_NOCLIP:
			plug->ControlNoClip = DataLocation;
			break;
		case IINPUT_AUDIO_IN1:
			plug->AudioInputBuffer1 = DataLocation;
			break;
		case IINPUT_AUDIO_IN2:
			plug->AudioInputBuffer2 = DataLocation;
			break;
		case IINPUT_AUDIO_OUT1:
			plug->AudioOutputBuffer1 = DataLocation;
			break;
		case IINPUT_AUDIO_OUT2:
			plug->AudioOutputBuffer2 = DataLocation;
			break;
	}
}

void activateIinput(LADSPA_Handle Instance) {
	Iinput * plug;	
	plug = (Iinput *)Instance;

	/* these values force the conversion to take place */      

	plug->LastPhase1 = 0;
	plug->LastPhase2 = 0;     
	plug->LastGain   = 0;
	plug->LastPan    = 0;  
	plug->LastWidth  = 0;  
	plug->LastNoClip = 0; 

	plug->ConvertedPhase1 = convertParam(IINPUT_PHASE1, plug->LastPhase1,  plug->SampleRate);
	plug->ConvertedPhase2 = convertParam(IINPUT_PHASE2, plug->LastPhase2,  plug->SampleRate);
	plug->ConvertedGain   = convertParam(IINPUT_GAIN,   plug->LastGain,    plug->SampleRate);
	plug->ConvertedPan    = convertParam(IINPUT_PAN,    plug->LastPan,     plug->SampleRate);
	plug->ConvertedWidth  = convertParam(IINPUT_WIDTH,  plug->LastWidth,   plug->SampleRate);
	plug->ConvertedNoClip = convertParam(IINPUT_NOCLIP, plug->LastNoClip,  plug->SampleRate);
}

void runStereoIinput(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data fPhase1,fPhase2,fGain,fPan,fLPan,fRPan,fWidth,fMono,fStereoL,fStereoR,fNoClip;
	LADSPA_Data fAudio1,fAudio2;
	Iinput * plug;
	unsigned long lSampleIndex;

	plug = (Iinput *)Instance;
	pParamFunc = &convertParam;

	checkParamChange(IINPUT_PHASE1, plug->ControlPhase1, &(plug->LastPhase1), &(plug->ConvertedPhase1), plug->SampleRate, pParamFunc);
	checkParamChange(IINPUT_PHASE2, plug->ControlPhase2, &(plug->LastPhase2), &(plug->ConvertedPhase2), plug->SampleRate, pParamFunc);
	checkParamChange(IINPUT_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(IINPUT_PAN,    plug->ControlPan,    &(plug->LastPan),    &(plug->ConvertedPan),    plug->SampleRate, pParamFunc);
	checkParamChange(IINPUT_WIDTH,  plug->ControlWidth,  &(plug->LastWidth),  &(plug->ConvertedWidth),  plug->SampleRate, pParamFunc);
	checkParamChange(IINPUT_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fPhase1    = plug->ConvertedPhase1;
	fPhase2    = plug->ConvertedPhase2;
	fGain      = plug->ConvertedGain;
	fPan       = plug->ConvertedPan;
	fWidth     = plug->ConvertedWidth;
	fNoClip    = plug->ConvertedNoClip;

	fLPan=1-fPan;
	fRPan=1+fPan;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioInput2 = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;
  
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {
  
		fAudio1 =  fPhase1 > 0 ? -(*(pfAudioInput1++)) : *(pfAudioInput1++) ;
		fAudio2 =  fPhase2 > 0 ? -(*(pfAudioInput2++)) : *(pfAudioInput2++) ;
		fAudio1 *= fGain;
		fAudio2 *= fGain;
		fAudio1 *= fLPan;
		fAudio2 *= fRPan;
	  
		if(fWidth<=0) {
			fMono = (fAudio1 + fAudio2) / 2;
			fAudio1 = (1+fWidth)*fAudio1 - fWidth*fMono;
			fAudio2 = (1+fWidth)*fAudio2 - fWidth*fMono;
		} else {
			fStereoL = (fAudio1 - fAudio2) / 2;
			fStereoR = (fAudio2 - fAudio1) / 2;
			fAudio1 = (1-fWidth)*fAudio1 + fWidth*fStereoL;
			fAudio2 = (1-fWidth)*fAudio2 + fWidth*fStereoR;
		}

		fAudio1 = fNoClip > 0 ? InoClip(fAudio1) : fAudio1;
		fAudio2 = fNoClip > 0 ? InoClip(fAudio2) : fAudio2;
		*(pfAudioOutput1++) = fAudio1;
		*(pfAudioOutput2++) = fAudio2;
	}
}

void cleanupIinput(LADSPA_Handle Instance) {
	free(Instance);
}

LADSPA_Descriptor * g_psStereoDescriptor = NULL;

/* _init() is called automatically when the plugin library is first
   loaded. */
void _init() {

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psStereoDescriptor 
	= (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

	if (g_psStereoDescriptor) {
		g_psStereoDescriptor->UniqueID          = 3301;
		g_psStereoDescriptor->Properties        = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psStereoDescriptor->Label             = strdup("invada_stereo_input_module_0_1");
		g_psStereoDescriptor->Name              = strdup(":: Invada :: Input Module");
		g_psStereoDescriptor->Maker             = strdup("Fraser At Invada Records dot Com");
		g_psStereoDescriptor->Copyright         = strdup("(c) Invada Records");      

		g_psStereoDescriptor->PortCount         = 10;

		piPortDescriptors                       = (LADSPA_PortDescriptor *)calloc(10, sizeof(LADSPA_PortDescriptor));
		g_psStereoDescriptor->PortDescriptors   = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[IINPUT_PHASE1]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_PHASE2]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_GAIN]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_PAN]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_WIDTH]         = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_NOCLIP]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IINPUT_AUDIO_IN1]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IINPUT_AUDIO_IN2]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IINPUT_AUDIO_OUT1]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IINPUT_AUDIO_OUT2]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

    
		pcPortNames                             = (char **)calloc(10, sizeof(char *));
		g_psStereoDescriptor->PortNames         = (const char **)pcPortNames;

		pcPortNames[IINPUT_PHASE1]              = strdup("Phase Reverse (Left)");
		pcPortNames[IINPUT_PHASE2]              = strdup("Phase Reverse (Right)");    
		pcPortNames[IINPUT_GAIN]                = strdup("Gain (dB)");
		pcPortNames[IINPUT_PAN]                 = strdup("Pan (L-R)");   
		pcPortNames[IINPUT_WIDTH]               = strdup("Width (M-S)"); 	
		pcPortNames[IINPUT_NOCLIP]              = strdup("Soft Clip");  
		pcPortNames[IINPUT_AUDIO_IN1]           = strdup("Input (Left)");
		pcPortNames[IINPUT_AUDIO_IN2]           = strdup("Input (Right)");
		pcPortNames[IINPUT_AUDIO_OUT1]          = strdup("Output (Left)");
		pcPortNames[IINPUT_AUDIO_OUT2]          = strdup("Output (Right)");

		psPortRangeHints                        = ((LADSPA_PortRangeHint *) calloc(10, sizeof(LADSPA_PortRangeHint)));
		g_psStereoDescriptor->PortRangeHints    = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[IINPUT_PHASE1].HintDescriptor       = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IINPUT_PHASE2].HintDescriptor       = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IINPUT_GAIN].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IINPUT_GAIN].LowerBound             = -24;
		psPortRangeHints[IINPUT_GAIN].UpperBound             = 24;
		psPortRangeHints[IINPUT_PAN].HintDescriptor          = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IINPUT_PAN].LowerBound              = -1.0;
		psPortRangeHints[IINPUT_PAN].UpperBound              =  1.0;
		psPortRangeHints[IINPUT_WIDTH].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IINPUT_WIDTH].LowerBound            = -1.0;
		psPortRangeHints[IINPUT_WIDTH].UpperBound            =  1.0;
		psPortRangeHints[IINPUT_NOCLIP].HintDescriptor       = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IINPUT_AUDIO_IN1].HintDescriptor    = 0;
		psPortRangeHints[IINPUT_AUDIO_IN2].HintDescriptor    = 0;
		psPortRangeHints[IINPUT_AUDIO_OUT1].HintDescriptor   = 0;
		psPortRangeHints[IINPUT_AUDIO_OUT2].HintDescriptor   = 0;
	
		g_psStereoDescriptor->instantiate         = instantiateIinput;
		g_psStereoDescriptor->connect_port        = connectPortToIinput;
		g_psStereoDescriptor->activate            = activateIinput;
		g_psStereoDescriptor->run                 = runStereoIinput;
		g_psStereoDescriptor->run_adding          = NULL;
		g_psStereoDescriptor->set_run_adding_gain = NULL;
		g_psStereoDescriptor->deactivate          = NULL;
		g_psStereoDescriptor->cleanup             = cleanupIinput;
	}
}

/*****************************************************************************/

void
deleteDescriptor(LADSPA_Descriptor * psDescriptor) {
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
	deleteDescriptor(g_psStereoDescriptor);
}


/* Return a descriptor of the requested plugin type. */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index) {
	switch (Index) {
		case 0:
			return g_psStereoDescriptor;
		default:
			return NULL;
	}
}


LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr) {
	LADSPA_Data result;
	switch(param)  {
		case IINPUT_PHASE1:
		case IINPUT_PHASE2:
			if(value<0.5)
				result= 0; 
			else
				result= 1;
			break;
		case IINPUT_GAIN:
			if(value<-24)
				result= pow(10, -24/20);
			else if (value < 24)
				result= pow(10, value/20);
			else
				result= pow(10, 1.2);
			break;
		case IINPUT_PAN:
		case IINPUT_WIDTH:
			if(value < -1)
				result= -1;
			else if (value < 1)
				result= value;
			else
				result= 1;
			break;
		case IINPUT_NOCLIP:
			if(value<0.5)
				result= 0; 
			else
				result= 1;
			break;
		default:
			result=0;
			break;
	}
	return result;
}


