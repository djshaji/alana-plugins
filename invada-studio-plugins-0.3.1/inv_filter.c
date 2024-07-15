/* 

    This LADSPA plugin provides mono and stereo filters

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

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ladspa.h>
#include "libinv_common.h"
#include "inv_filter.h"

/* The port numbers for the plugin: */

#define IFILTER_FREQ 0
#define IFILTER_GAIN 1
#define IFILTER_NOCLIP 2
#define IFILTER_AUDIO_IN1  3
#define IFILTER_AUDIO_OUT1 4
#define IFILTER_AUDIO_IN2  5 /* not used in mono mode */
#define IFILTER_AUDIO_OUT2 6 /* not used in mono mode */

/* The structure used to hold port connection information and state. */

typedef struct {
	unsigned long SampleRate; 

	/* Ports */
	LADSPA_Data * ControlFreq;         
	LADSPA_Data * ControlGain;
	LADSPA_Data * ControlNoClip;  
	LADSPA_Data * AudioInputBuffer1;
	LADSPA_Data * AudioOutputBuffer1;
	LADSPA_Data * AudioInputBuffer2; 
	LADSPA_Data * AudioOutputBuffer2;

	/* stuff we need to remember to reduce cpu */
	LADSPA_Data LastFreq;         
	LADSPA_Data LastGain;
	LADSPA_Data LastNoClip;

	LADSPA_Data ConvertedFreq;         
	LADSPA_Data ConvertedGain;
	LADSPA_Data ConvertedNoClip;

	/* stuff we need to remember between calls */
	LADSPA_Data Audio1Last; 
	LADSPA_Data Audio2Last;

} Ifilter;

/* Construct a new plugin instance. */
LADSPA_Handle instantiateIfilter(const LADSPA_Descriptor * Descriptor,unsigned long SampleRate) {
	LADSPA_Handle Instance;
	Ifilter * plug;
				 
	if((Instance=malloc(sizeof(Ifilter)))==NULL)
	return NULL;
				 
	/* set some initial params */
	plug = (Ifilter *)Instance;
	/* should check SampleRate > 0 */
	plug->SampleRate=SampleRate;
	 
	return Instance;
	}

/* Connect a port to a data location. */
void connectPortToIfilter(LADSPA_Handle Instance,unsigned long Port, LADSPA_Data * DataLocation) {
	Ifilter * plug;

	plug = (Ifilter *)Instance;
	switch (Port) {
		case IFILTER_FREQ:
			plug->ControlFreq = DataLocation;
			break;
		case IFILTER_GAIN:
			plug->ControlGain  = DataLocation;
			break;
		case IFILTER_NOCLIP:
			plug->ControlNoClip = DataLocation;
			break;
		case IFILTER_AUDIO_IN1:
			plug->AudioInputBuffer1 = DataLocation;
			break;
		case IFILTER_AUDIO_OUT1:
			plug->AudioOutputBuffer1 = DataLocation;
			break;
		case IFILTER_AUDIO_IN2:
			plug->AudioInputBuffer2 = DataLocation;
			break;
		case IFILTER_AUDIO_OUT2:
			plug->AudioOutputBuffer2 = DataLocation;
			break;
	}
}

void activateIfilter(LADSPA_Handle Instance) {
	Ifilter * plug;	
	plug = (Ifilter *)Instance;

	plug->Audio1Last = 0;
	plug->Audio2Last = 0;

	/* defaults */
	plug->LastFreq = 0.015811388;   // middle on a logarithmic scale      
	plug->LastGain = 0;
	plug->LastNoClip = 0;

	plug->ConvertedFreq   = convertParam(IFILTER_FREQ,   plug->LastFreq,    plug->SampleRate);
	plug->ConvertedGain   = convertParam(IFILTER_GAIN,   plug->LastGain,    plug->SampleRate);
	plug->ConvertedNoClip = convertParam(IFILTER_NOCLIP, plug->LastNoClip,  plug->SampleRate);
}

void runMonoLPFIfilter(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data fSamples,fGain,fNoClip;
	LADSPA_Data fAudio1,fAudio1Sum;
	Ifilter * plug;
	unsigned long lSampleIndex;

	plug = (Ifilter *)Instance;
	pParamFunc = &convertParam;

	checkParamChange(IFILTER_FREQ,   plug->ControlFreq,   &(plug->LastFreq),   &(plug->ConvertedFreq),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fSamples = plug->ConvertedFreq;
	fGain    = plug->ConvertedGain;
	fNoClip  = plug->ConvertedNoClip;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioOutput1 = plug->AudioOutputBuffer1;

	fAudio1Sum = plug->Audio1Last;
  
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) 
	{
		fAudio1Sum = ((fSamples-1) * fAudio1Sum + *(pfAudioInput1++)) / fSamples;  
		fAudio1 = fAudio1Sum*fGain; 
		*(pfAudioOutput1++)=fNoClip > 0 ? InoClip(fAudio1) : fAudio1;  
	}
  
	plug->Audio1Last = (fabs(fAudio1Sum)<1.0e-10)  ? 0.f : fAudio1Sum;  // and store values for next loop
}

void runMonoHPFIfilter(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data fSamples,fGain,fNoClip;
	LADSPA_Data fAudio1,fAudio1Sum;
	Ifilter * plug;
	unsigned long lSampleIndex;

	plug = (Ifilter *)Instance;
	pParamFunc = &convertParam;

	checkParamChange(IFILTER_FREQ,   plug->ControlFreq,   &(plug->LastFreq),   &(plug->ConvertedFreq),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fSamples = plug->ConvertedFreq;
	fGain    = plug->ConvertedGain;
	fNoClip  = plug->ConvertedNoClip;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioOutput1 = plug->AudioOutputBuffer1;

	fAudio1Sum = plug->Audio1Last;

	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) 
	{
		fAudio1 = *(pfAudioInput1++);
		fAudio1Sum = ((fSamples-1) * fAudio1Sum + fAudio1) / fSamples;  
		fAudio1 = (fAudio1 - fAudio1Sum)*fGain;
		*(pfAudioOutput1++)=fNoClip > 0 ? InoClip(fAudio1) : fAudio1;  
	}

	plug->Audio1Last = (fabs(fAudio1Sum)<1.0e-10)  ? 0.f : fAudio1Sum;  // and store values for next loop
}

void runStereoLPFIfilter(LADSPA_Handle Instance,unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data fSamples,fGain,fNoClip;
	LADSPA_Data fAudio1,fAudio2,fAudio1Sum,fAudio2Sum;
	Ifilter * plug;
	unsigned long lSampleIndex;

	plug = (Ifilter *)Instance;
	pParamFunc = &convertParam;

	checkParamChange(IFILTER_FREQ,   plug->ControlFreq,   &(plug->LastFreq),   &(plug->ConvertedFreq),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fSamples = plug->ConvertedFreq;
	fGain    = plug->ConvertedGain;
	fNoClip  = plug->ConvertedNoClip;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioInput2 = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;

	fAudio1Sum = plug->Audio1Last;
	fAudio2Sum = plug->Audio2Last;

	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {
		fAudio1Sum = ((fSamples-1) * fAudio1Sum + *(pfAudioInput1++)) / fSamples;  
		fAudio2Sum = ((fSamples-1) * fAudio2Sum + *(pfAudioInput2++)) / fSamples;
		  
		fAudio1 = fAudio1Sum*fGain; 
		fAudio2 = fAudio2Sum*fGain; 
		  
		*(pfAudioOutput1++)=fNoClip > 0 ? InoClip(fAudio1) : fAudio1;  
		*(pfAudioOutput2++)=fNoClip > 0 ? InoClip(fAudio2) : fAudio2;	
	}
  
	plug->Audio1Last = (fabs(fAudio1Sum)<1.0e-10)  ? 0.f : fAudio1Sum;  // and store values for next loop
	plug->Audio2Last = (fabs(fAudio2Sum)<1.0e-10)  ? 0.f : fAudio2Sum;  // and store values for next loop
}

void runStereoHPFIfilter(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data fSamples,fGain,fNoClip;
	LADSPA_Data fAudio1,fAudio2,fAudio1Sum,fAudio2Sum;
	Ifilter * plug;
	unsigned long lSampleIndex;

	plug = (Ifilter *)Instance;
	pParamFunc = &convertParam;

	checkParamChange(IFILTER_FREQ,   plug->ControlFreq,   &(plug->LastFreq),   &(plug->ConvertedFreq),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(IFILTER_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fSamples = plug->ConvertedFreq;
	fGain    = plug->ConvertedGain;
	fNoClip  = plug->ConvertedNoClip;

	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioInput2 = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;

	fAudio1Sum = plug->Audio1Last;
	fAudio2Sum = plug->Audio2Last;
  
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {
		fAudio1 = *(pfAudioInput1++);
		fAudio2 = *(pfAudioInput2++);
		  
		fAudio1Sum = ((fSamples-1) * fAudio1Sum + fAudio1) / fSamples;  
		fAudio2Sum = ((fSamples-1) * fAudio2Sum + fAudio2) / fSamples;
		  
		fAudio1 = (fAudio1 - fAudio1Sum)*fGain;
		fAudio2 = (fAudio2 - fAudio2Sum)*fGain;
		
		*(pfAudioOutput1++)=fNoClip > 0 ? InoClip(fAudio1) : fAudio1;
		*(pfAudioOutput2++)=fNoClip > 0 ? InoClip(fAudio2) : fAudio2;	  
	}

	plug->Audio1Last = (fabs(fAudio1Sum)<1.0e-10)  ? 0.f : fAudio1Sum;  // and store values for next loop
	plug->Audio2Last = (fabs(fAudio2Sum)<1.0e-10)  ? 0.f : fAudio2Sum;  // and store values for next loop
}


/* Throw away. */
void cleanupIfilter(LADSPA_Handle Instance) {
	free(Instance);
}
 
LADSPA_Descriptor * g_psMonoLPFDescriptor = NULL;
LADSPA_Descriptor * g_psMonoHPFDescriptor = NULL;
LADSPA_Descriptor * g_psStereoLPFDescriptor = NULL;
LADSPA_Descriptor * g_psStereoHPFDescriptor = NULL;


/* _init() is called automatically when the plugin library is first loaded. */
void _init() {

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psMonoLPFDescriptor   = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psMonoHPFDescriptor   = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psStereoLPFDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psStereoHPFDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

	if (g_psMonoLPFDescriptor) {
		g_psMonoLPFDescriptor->UniqueID       = 3302;
		g_psMonoLPFDescriptor->Properties     = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psMonoLPFDescriptor->Label          = strdup("invada_lp_mono_filter_module_0_1");
		g_psMonoLPFDescriptor->Name           = strdup(":: Invada :: Filter - Low Pass Mono");
		g_psMonoLPFDescriptor->Maker          = strdup("Fraser At Invada Records dot Com");
		g_psMonoLPFDescriptor->Copyright      = strdup("(c) Invada Records"); 
     
		g_psMonoLPFDescriptor->PortCount       = 5;

		piPortDescriptors                      = (LADSPA_PortDescriptor *)calloc(5, sizeof(LADSPA_PortDescriptor));
		g_psMonoLPFDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[IFILTER_FREQ]       = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_GAIN]       = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_NOCLIP]     = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_AUDIO_IN1]  = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT1] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                           = (char **)calloc(5, sizeof(char *));
		g_psMonoLPFDescriptor->PortNames      = (const char **)pcPortNames;

		pcPortNames[IFILTER_FREQ]             = strdup("Frequency (Hz)");    
		pcPortNames[IFILTER_GAIN]             = strdup("Gain (dB)");  
		pcPortNames[IFILTER_NOCLIP]           = strdup("Soft Clip");  
		pcPortNames[IFILTER_AUDIO_IN1]        = strdup("Input");
		pcPortNames[IFILTER_AUDIO_OUT1]       = strdup("Output");

		psPortRangeHints                      = ((LADSPA_PortRangeHint *) calloc(5, sizeof(LADSPA_PortRangeHint)));
		g_psMonoLPFDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[IFILTER_FREQ].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IFILTER_FREQ].LowerBound            = 0.0005;
		psPortRangeHints[IFILTER_FREQ].UpperBound            = 0.5;
		psPortRangeHints[IFILTER_GAIN].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IFILTER_GAIN].LowerBound            = 0;
		psPortRangeHints[IFILTER_GAIN].UpperBound            = 12;
		psPortRangeHints[IFILTER_NOCLIP].HintDescriptor      = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IFILTER_AUDIO_IN1].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT1].HintDescriptor  = 0;

		g_psMonoLPFDescriptor->instantiate         = instantiateIfilter;
		g_psMonoLPFDescriptor->connect_port        = connectPortToIfilter;
		g_psMonoLPFDescriptor->activate            = activateIfilter;
		g_psMonoLPFDescriptor->run                 = runMonoLPFIfilter;
		g_psMonoLPFDescriptor->run_adding          = NULL;
		g_psMonoLPFDescriptor->set_run_adding_gain = NULL;
		g_psMonoLPFDescriptor->deactivate          = NULL;
		g_psMonoLPFDescriptor->cleanup             = cleanupIfilter;
	}
	if (g_psMonoHPFDescriptor) {
		g_psMonoHPFDescriptor->UniqueID        = 3303;
		g_psMonoHPFDescriptor->Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psMonoHPFDescriptor->Label           = strdup("invada_hp_mono_filter_module_0_1");
		g_psMonoHPFDescriptor->Name            = strdup(":: Invada :: Filter - High Pass Mono");
		g_psMonoHPFDescriptor->Maker           = strdup("Fraser At Invada Records dot Com");
		g_psMonoHPFDescriptor->Copyright       = strdup("(c) Invada Records");      

		g_psMonoHPFDescriptor->PortCount       = 5;

		piPortDescriptors                      = (LADSPA_PortDescriptor *)calloc(5, sizeof(LADSPA_PortDescriptor));
		g_psMonoHPFDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[IFILTER_FREQ]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_GAIN]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_NOCLIP]      = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_AUDIO_IN1]   = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT1]  = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                            = (char **)calloc(5, sizeof(char *));
		g_psMonoHPFDescriptor->PortNames       = (const char **)pcPortNames;

		pcPortNames[IFILTER_FREQ]              = strdup("Frequency (Hz)");    
		pcPortNames[IFILTER_GAIN]              = strdup("Gain (dB)");  
		pcPortNames[IFILTER_NOCLIP]            = strdup("Soft Clip");  
		pcPortNames[IFILTER_AUDIO_IN1]         = strdup("Input");
		pcPortNames[IFILTER_AUDIO_OUT1]        = strdup("Output");

		psPortRangeHints                       = ((LADSPA_PortRangeHint *) calloc(5, sizeof(LADSPA_PortRangeHint)));
		g_psMonoHPFDescriptor->PortRangeHints  = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[IFILTER_FREQ].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IFILTER_FREQ].LowerBound            = 0.0005;
		psPortRangeHints[IFILTER_FREQ].UpperBound            = 0.5;
		psPortRangeHints[IFILTER_GAIN].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IFILTER_GAIN].LowerBound            = 0;
		psPortRangeHints[IFILTER_GAIN].UpperBound            = 12;
		psPortRangeHints[IFILTER_NOCLIP].HintDescriptor      = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IFILTER_AUDIO_IN1].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT1].HintDescriptor  = 0;


		g_psMonoHPFDescriptor->instantiate         = instantiateIfilter;
		g_psMonoHPFDescriptor->connect_port        = connectPortToIfilter;
		g_psMonoHPFDescriptor->activate            = activateIfilter;
		g_psMonoHPFDescriptor->run                 = runMonoHPFIfilter;
		g_psMonoHPFDescriptor->run_adding          = NULL;
		g_psMonoHPFDescriptor->set_run_adding_gain = NULL;
		g_psMonoHPFDescriptor->deactivate          = NULL;
		g_psMonoHPFDescriptor->cleanup             = cleanupIfilter;
	}
	if (g_psStereoLPFDescriptor) {
		g_psStereoLPFDescriptor->UniqueID        = 3304;
		g_psStereoLPFDescriptor->Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psStereoLPFDescriptor->Label           = strdup("invada_lp_stereo_filter_module_0_1");
		g_psStereoLPFDescriptor->Name            = strdup(":: Invada :: Filter - Low Pass Stereo");
		g_psStereoLPFDescriptor->Maker           = strdup("Fraser At Invada Records dot Com");
		g_psStereoLPFDescriptor->Copyright       = strdup("(c) Invada Records");      

		g_psStereoLPFDescriptor->PortCount       = 7;

		piPortDescriptors                        = (LADSPA_PortDescriptor *)calloc(7, sizeof(LADSPA_PortDescriptor));
		g_psStereoLPFDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[IFILTER_FREQ]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_GAIN]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_NOCLIP]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_AUDIO_IN1]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT1]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_IN2]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT2]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                              = (char **)calloc(7, sizeof(char *));
		g_psStereoLPFDescriptor->PortNames       = (const char **)pcPortNames;

		pcPortNames[IFILTER_FREQ]                = strdup("Frequency (Hz)");    
		pcPortNames[IFILTER_GAIN]                = strdup("Gain (dB)");  
		pcPortNames[IFILTER_NOCLIP]              = strdup("Soft Clip");  
		pcPortNames[IFILTER_AUDIO_IN1]           = strdup("Input (Left)");
		pcPortNames[IFILTER_AUDIO_OUT1]          = strdup("Output (Left)");
		pcPortNames[IFILTER_AUDIO_IN2]           = strdup("Input (Right)");
		pcPortNames[IFILTER_AUDIO_OUT2]          = strdup("Output (Right)");

		psPortRangeHints                         = ((LADSPA_PortRangeHint *) calloc(7, sizeof(LADSPA_PortRangeHint)));
		g_psStereoLPFDescriptor->PortRangeHints  = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[IFILTER_FREQ].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IFILTER_FREQ].LowerBound            = 0.0005;
		psPortRangeHints[IFILTER_FREQ].UpperBound            = 0.5;
		psPortRangeHints[IFILTER_GAIN].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IFILTER_GAIN].LowerBound            = 0;
		psPortRangeHints[IFILTER_GAIN].UpperBound            = 12;
		psPortRangeHints[IFILTER_NOCLIP].HintDescriptor      = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IFILTER_AUDIO_IN1].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT1].HintDescriptor  = 0;
		psPortRangeHints[IFILTER_AUDIO_IN2].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT2].HintDescriptor  = 0;

		g_psStereoLPFDescriptor->instantiate         = instantiateIfilter;
		g_psStereoLPFDescriptor->connect_port        = connectPortToIfilter;
		g_psStereoLPFDescriptor->activate            = activateIfilter;
		g_psStereoLPFDescriptor->run                 = runStereoLPFIfilter;
		g_psStereoLPFDescriptor->run_adding          = NULL;
		g_psStereoLPFDescriptor->set_run_adding_gain = NULL;
		g_psStereoLPFDescriptor->deactivate          = NULL;
		g_psStereoLPFDescriptor->cleanup             = cleanupIfilter;
	}
	if (g_psStereoHPFDescriptor) {
		g_psStereoHPFDescriptor->UniqueID        = 3305;
		g_psStereoHPFDescriptor->Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psStereoHPFDescriptor->Label           = strdup("invada_hp_stereo_filter_module_0_1");
		g_psStereoHPFDescriptor->Name            = strdup(":: Invada :: Filter - High Pass Stereo");
		g_psStereoHPFDescriptor->Maker           = strdup("Fraser At Invada Records dot Com");
		g_psStereoHPFDescriptor->Copyright       = strdup("(c) Invada Records");      

		g_psStereoHPFDescriptor->PortCount       = 7;

		piPortDescriptors                        = (LADSPA_PortDescriptor *)calloc(7, sizeof(LADSPA_PortDescriptor));
		g_psStereoHPFDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[IFILTER_FREQ]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_GAIN]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_NOCLIP]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[IFILTER_AUDIO_IN1]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT1]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_IN2]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[IFILTER_AUDIO_OUT2]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                              = (char **)calloc(7, sizeof(char *));
		g_psStereoHPFDescriptor->PortNames          = (const char **)pcPortNames;

		pcPortNames[IFILTER_FREQ]                = strdup("Frequency (Hz)");    
		pcPortNames[IFILTER_GAIN]                = strdup("Gain (dB)");  
		pcPortNames[IFILTER_NOCLIP]              = strdup("Soft Clip");  
		pcPortNames[IFILTER_AUDIO_IN1]           = strdup("Input (Left)");
		pcPortNames[IFILTER_AUDIO_OUT1]          = strdup("Output (Left)");
		pcPortNames[IFILTER_AUDIO_IN2]           = strdup("Input (Right)");
		pcPortNames[IFILTER_AUDIO_OUT2]          = strdup("Output (Right)");

		psPortRangeHints                         = ((LADSPA_PortRangeHint *) calloc(7, sizeof(LADSPA_PortRangeHint)));
		g_psStereoHPFDescriptor->PortRangeHints  = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[IFILTER_FREQ].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_SAMPLE_RATE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[IFILTER_FREQ].LowerBound            = 0.0005;
		psPortRangeHints[IFILTER_FREQ].UpperBound            = 0.5;
		psPortRangeHints[IFILTER_GAIN].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[IFILTER_GAIN].LowerBound            = 0;
		psPortRangeHints[IFILTER_GAIN].UpperBound            = 12;
		psPortRangeHints[IFILTER_NOCLIP].HintDescriptor      = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[IFILTER_AUDIO_IN1].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT1].HintDescriptor  = 0;
		psPortRangeHints[IFILTER_AUDIO_IN2].HintDescriptor   = 0;
		psPortRangeHints[IFILTER_AUDIO_OUT2].HintDescriptor  = 0;


		g_psStereoHPFDescriptor->instantiate         = instantiateIfilter;
		g_psStereoHPFDescriptor->connect_port        = connectPortToIfilter;
		g_psStereoHPFDescriptor->activate            = activateIfilter;
		g_psStereoHPFDescriptor->run                 = runStereoHPFIfilter;
		g_psStereoHPFDescriptor->run_adding          = NULL;
		g_psStereoHPFDescriptor->set_run_adding_gain = NULL;
		g_psStereoHPFDescriptor->deactivate          = NULL;
		g_psStereoHPFDescriptor->cleanup             = cleanupIfilter;
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
	deleteDescriptor(g_psMonoLPFDescriptor);
	deleteDescriptor(g_psMonoHPFDescriptor);
	deleteDescriptor(g_psStereoLPFDescriptor);
	deleteDescriptor(g_psStereoHPFDescriptor);
}

/* Return a descriptor of the requested plugin type. */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index) {
	switch (Index) {
		case 0:
			return g_psMonoLPFDescriptor;
		case 1:
			return g_psMonoHPFDescriptor;
		case 2:
			return g_psStereoLPFDescriptor;
		case 3:
			return g_psStereoHPFDescriptor;
		default:
			return NULL;
	}
}


LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr) {

	LADSPA_Data temp;
	LADSPA_Data result;

	switch(param)
	{
		case IFILTER_FREQ:
			temp = value / (LADSPA_Data)sr;
			if (temp <  0.0005)
				result = 1000;
			else if (temp <= 0.5)
				result = 1/(2*temp);
			else
				result=1;
			break;
		case IFILTER_GAIN:
			if(value<0)
				result= 1;
			else if (value < 12)
				result = pow(10,value/20);
			else
				result= pow(10,0.6);
			break;
		case IFILTER_NOCLIP:
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

