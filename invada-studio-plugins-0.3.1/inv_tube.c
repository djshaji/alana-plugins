/* 

    This LADSPA plugin provides tube distortion

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
#include "inv_tube.h"

/* The port numbers for the plugin: */

#define ITUBE_DRIVE 0
#define ITUBE_DCOFFSET 1
#define ITUBE_PHASE 2
#define ITUBE_MIX 3
#define ITUBE_AUDIO_IN1  4
#define ITUBE_AUDIO_OUT1 5
#define ITUBE_AUDIO_IN2  6 /* not used in mono mode */
#define ITUBE_AUDIO_OUT2 7 /* not used in mono mode */


/* The structure used to hold port connection information and state
   (actually gain controls require no further state). */

typedef struct {
	unsigned long SampleRate; 
	/* Ports */

	LADSPA_Data * ControlDrive;         
	LADSPA_Data * ControlDcoffset;
	LADSPA_Data * ControlPhase;  
	LADSPA_Data * ControlMix; 
	LADSPA_Data * AudioInputBuffer1;
	LADSPA_Data * AudioOutputBuffer1;
	LADSPA_Data * AudioInputBuffer2; 
	LADSPA_Data * AudioOutputBuffer2;

	/* params with complex conversions get remembered */
	LADSPA_Data LastDrive;         
	LADSPA_Data LastDcoffset;
	LADSPA_Data LastPhase;         
	LADSPA_Data LastMix;

	LADSPA_Data ConvertedDrive;  
	LADSPA_Data ConvertedDcoffset;
	LADSPA_Data ConvertedPhase;  
	LADSPA_Data ConvertedMix;
 
} Itube;

/*****************************************************************************/

/* Construct a new plugin instance. */
LADSPA_Handle instantiateItube(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate) {
	LADSPA_Handle Instance;
	Itube * plug;
				 
	if((Instance=malloc(sizeof(Itube)))==NULL)
		return NULL;
				 
	/* set some initial params */
	plug = (Itube *)Instance;
	/* should check SampleRate > 0 */
	plug->SampleRate=SampleRate;
				 
	return Instance;
}

/*****************************************************************************/

/* Connect a port to a data location. */
void connectPortToItube(LADSPA_Handle Instance,unsigned long Port,LADSPA_Data * DataLocation) {

	Itube * plug;

	plug = (Itube *)Instance;
	switch (Port) {
		case ITUBE_DRIVE:
			plug->ControlDrive = DataLocation;
			break;
		case ITUBE_DCOFFSET:
			plug->ControlDcoffset  = DataLocation;
			break;
		case ITUBE_PHASE:
			plug->ControlPhase = DataLocation;
			break;
		case ITUBE_MIX:
			plug->ControlMix = DataLocation;
			break;
		case ITUBE_AUDIO_IN1:
			plug->AudioInputBuffer1 = DataLocation;
			break;
		case ITUBE_AUDIO_OUT1:
			plug->AudioOutputBuffer1 = DataLocation;
			break;
		case ITUBE_AUDIO_IN2:
			plug->AudioInputBuffer2 = DataLocation;
			break;
		case ITUBE_AUDIO_OUT2:
			plug->AudioOutputBuffer2 = DataLocation;
			break;
	}
}

void activateItube(LADSPA_Handle Instance) {
	Itube * plug;	
	plug = (Itube *)Instance;

	/* set some defaults */
	plug->LastDrive=0;         
	plug->LastDcoffset=0;
	plug->LastPhase=0;         
	plug->LastMix=75;

	plug->ConvertedDrive    = convertParam(ITUBE_DRIVE,    plug->LastDrive,     plug->SampleRate);
	plug->ConvertedDcoffset = convertParam(ITUBE_DCOFFSET, plug->LastDcoffset,  plug->SampleRate);
	plug->ConvertedPhase    = convertParam(ITUBE_PHASE,    plug->LastPhase,     plug->SampleRate);
	plug->ConvertedMix      = convertParam(ITUBE_MIX,      plug->LastMix,       plug->SampleRate);
}

void runMonoItube(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data fAudio1, fDrive, fDCOffset, fPhase, fMix, fDCOffsetADJ;
	Itube * plug;
	unsigned long lSampleIndex;
			   
	plug = (Itube *)Instance;
	pParamFunc = &convertParam;

	/* check for any params changes */
	checkParamChange(ITUBE_DRIVE,    plug->ControlDrive,    &(plug->LastDrive),    &(plug->ConvertedDrive),    plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_DCOFFSET, plug->ControlDcoffset, &(plug->LastDcoffset), &(plug->ConvertedDcoffset), plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_PHASE,    plug->ControlPhase,    &(plug->LastPhase),    &(plug->ConvertedPhase),    plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_MIX,      plug->ControlMix,      &(plug->LastMix),      &(plug->ConvertedMix),      plug->SampleRate, pParamFunc);

	fDrive    = plug->ConvertedDrive;
	fDCOffset = plug->ConvertedDcoffset;
	fPhase    = plug->ConvertedPhase;
	fMix      = plug->ConvertedMix;	

	fDCOffsetADJ = ITube(fDCOffset,fDrive);
			   
	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
  
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {
		fAudio1=*(pfAudioInput1++);
		*(pfAudioOutput1++) = fPhase <= 0 ? 
					(fAudio1*(1-fMix)) + (ITube(fAudio1 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix :
					(fAudio1*(1-fMix)) - (ITube(fAudio1 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix ;
	}
}

void runStereoItube(LADSPA_Handle Instance, unsigned long SampleCount) {
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL; 
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data fAudio1, fAudio2, fDrive, fDCOffset, fPhase, fMix, fDCOffsetADJ;
	Itube * plug;
	unsigned long lSampleIndex;
			   
	plug = (Itube *)Instance;
	pParamFunc = &convertParam;
			   
	/* check for any params changes */
	checkParamChange(ITUBE_DRIVE,    plug->ControlDrive,    &(plug->LastDrive),    &(plug->ConvertedDrive),    plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_DCOFFSET, plug->ControlDcoffset, &(plug->LastDcoffset), &(plug->ConvertedDcoffset), plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_PHASE,    plug->ControlPhase,    &(plug->LastPhase),    &(plug->ConvertedPhase),    plug->SampleRate, pParamFunc);
	checkParamChange(ITUBE_MIX,      plug->ControlMix,      &(plug->LastMix),      &(plug->ConvertedMix),      plug->SampleRate, pParamFunc);

	fDrive    = plug->ConvertedDrive;
	fDCOffset = plug->ConvertedDcoffset;
	fPhase    = plug->ConvertedPhase;
	fMix      = plug->ConvertedMix;	

	fDCOffsetADJ = ITube(fDCOffset,fDrive);
  
	pfAudioInput1 = plug->AudioInputBuffer1;
	pfAudioInput2 = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;
  
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) {
		fAudio1=*(pfAudioInput1++);
		*(pfAudioOutput1++) = fPhase <= 0 ? 
					(fAudio1*(1-fMix)) + (ITube(fAudio1 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix :
					(fAudio1*(1-fMix)) - (ITube(fAudio1 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix ;
		  
		fAudio2=*(pfAudioInput2++);
		*(pfAudioOutput2++) = fPhase <= 0 ? 
					(fAudio2*(1-fMix)) + (ITube(fAudio2 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix :
					(fAudio2*(1-fMix)) - (ITube(fAudio2 + fDCOffset,fDrive)-fDCOffsetADJ)*fMix ;
	}
}


void cleanupItube(LADSPA_Handle Instance) {
	free(Instance);
}

LADSPA_Descriptor * g_psMonoTubeDescriptor = NULL;
LADSPA_Descriptor * g_psStereoTubeDescriptor = NULL;

/* _init() is called automatically when the plugin library is first
   loaded. */
void _init() {
	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psMonoTubeDescriptor   = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psStereoTubeDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

	if (g_psMonoTubeDescriptor) {
		g_psMonoTubeDescriptor->UniqueID         = 3306;
		g_psMonoTubeDescriptor->Properties       = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psMonoTubeDescriptor->Label            = strdup("invada_mono_tube_module_0_1");
		g_psMonoTubeDescriptor->Name             = strdup(":: Invada :: Tube - Mono");
		g_psMonoTubeDescriptor->Maker            = strdup("Fraser At Invada Records dot Com");
		g_psMonoTubeDescriptor->Copyright        = strdup("(c) Invada Records");      

		g_psMonoTubeDescriptor->PortCount        = 6;

		piPortDescriptors                        = (LADSPA_PortDescriptor *)calloc(6, sizeof(LADSPA_PortDescriptor));
		g_psMonoTubeDescriptor->PortDescriptors  = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[ITUBE_DRIVE]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_DCOFFSET]        = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_PHASE]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_MIX]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_AUDIO_IN1]       = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ITUBE_AUDIO_OUT1]      = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                              = (char **)calloc(6, sizeof(char *));
		g_psMonoTubeDescriptor->PortNames        = (const char **)pcPortNames;

		pcPortNames[ITUBE_DRIVE]                 = strdup("Drive (dB)");    
		pcPortNames[ITUBE_DCOFFSET]              = strdup("DC Offset");  
		pcPortNames[ITUBE_PHASE]                 = strdup("Phase");  
		pcPortNames[ITUBE_MIX]                   = strdup("Wet/Dry Mix (%)");  
		pcPortNames[ITUBE_AUDIO_IN1]             = strdup("Input");
		pcPortNames[ITUBE_AUDIO_OUT1]            = strdup("Output");

		psPortRangeHints                         = ((LADSPA_PortRangeHint *) calloc(6, sizeof(LADSPA_PortRangeHint)));
		g_psMonoTubeDescriptor->PortRangeHints   = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[ITUBE_DRIVE].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[ITUBE_DRIVE].LowerBound             = 0;
		psPortRangeHints[ITUBE_DRIVE].UpperBound             = 18;
		psPortRangeHints[ITUBE_DCOFFSET].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ITUBE_DCOFFSET].LowerBound          = -1;
		psPortRangeHints[ITUBE_DCOFFSET].UpperBound          = 1;
		psPortRangeHints[ITUBE_PHASE].HintDescriptor         = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[ITUBE_MIX].HintDescriptor           = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_HIGH );
		psPortRangeHints[ITUBE_MIX].LowerBound               = 0;
		psPortRangeHints[ITUBE_MIX].UpperBound               = 100;
		psPortRangeHints[ITUBE_AUDIO_IN1].HintDescriptor     = 0;
		psPortRangeHints[ITUBE_AUDIO_OUT1].HintDescriptor    = 0;


		g_psMonoTubeDescriptor->instantiate         = instantiateItube;
		g_psMonoTubeDescriptor->connect_port        = connectPortToItube;
		g_psMonoTubeDescriptor->activate            = activateItube;
		g_psMonoTubeDescriptor->run                 = runMonoItube;
		g_psMonoTubeDescriptor->run_adding          = NULL;
		g_psMonoTubeDescriptor->set_run_adding_gain = NULL;
		g_psMonoTubeDescriptor->deactivate          = NULL;
		g_psMonoTubeDescriptor->cleanup             = cleanupItube;
	}
	if (g_psStereoTubeDescriptor) {
		g_psStereoTubeDescriptor->UniqueID         = 3307;
		g_psStereoTubeDescriptor->Properties       = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psStereoTubeDescriptor->Label            = strdup("invada_stereo_tube_module_0_1");
		g_psStereoTubeDescriptor->Name             = strdup(":: Invada :: Tube - Stereo");
		g_psStereoTubeDescriptor->Maker            = strdup("Fraser At Invada Records dot Com");
		g_psStereoTubeDescriptor->Copyright        = strdup("(c) Invada Records");      

		g_psStereoTubeDescriptor->PortCount        = 8;

		piPortDescriptors                          = (LADSPA_PortDescriptor *)calloc(8, sizeof(LADSPA_PortDescriptor));
		g_psStereoTubeDescriptor->PortDescriptors  = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[ITUBE_DRIVE]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_DCOFFSET]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_PHASE]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_MIX]               = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ITUBE_AUDIO_IN1]         = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ITUBE_AUDIO_OUT1]        = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ITUBE_AUDIO_IN2]         = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ITUBE_AUDIO_OUT2]        = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                                = (char **)calloc(8, sizeof(char *));
		g_psStereoTubeDescriptor->PortNames        = (const char **)pcPortNames;

		pcPortNames[ITUBE_DRIVE]                   = strdup("Drive (dB)");    
		pcPortNames[ITUBE_DCOFFSET]                = strdup("DC Offset");  
		pcPortNames[ITUBE_PHASE]                   = strdup("Phase");  
		pcPortNames[ITUBE_MIX]                     = strdup("Wet/Dry Mix (%)");   
		pcPortNames[ITUBE_AUDIO_IN1]               = strdup("Input (Left)");
		pcPortNames[ITUBE_AUDIO_OUT1]              = strdup("Output (Left)");
		pcPortNames[ITUBE_AUDIO_IN2]               = strdup("Input (Right)");
		pcPortNames[ITUBE_AUDIO_OUT2]              = strdup("Output (Right)");

		psPortRangeHints                           = ((LADSPA_PortRangeHint *) calloc(8, sizeof(LADSPA_PortRangeHint)));
		g_psStereoTubeDescriptor->PortRangeHints   = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[ITUBE_DRIVE].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[ITUBE_DRIVE].LowerBound             = 0;
		psPortRangeHints[ITUBE_DRIVE].UpperBound             = 18;
		psPortRangeHints[ITUBE_DCOFFSET].HintDescriptor      = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ITUBE_DCOFFSET].LowerBound          = -1;
		psPortRangeHints[ITUBE_DCOFFSET].UpperBound          = 1;
		psPortRangeHints[ITUBE_PHASE].HintDescriptor         = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[ITUBE_MIX].HintDescriptor           = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_HIGH );
		psPortRangeHints[ITUBE_MIX].LowerBound               = 0;
		psPortRangeHints[ITUBE_MIX].UpperBound               = 100;
		psPortRangeHints[ITUBE_AUDIO_IN1].HintDescriptor     = 0;
		psPortRangeHints[ITUBE_AUDIO_OUT1].HintDescriptor    = 0;
		psPortRangeHints[ITUBE_AUDIO_IN2].HintDescriptor     = 0;
		psPortRangeHints[ITUBE_AUDIO_OUT2].HintDescriptor    = 0;


		g_psStereoTubeDescriptor->instantiate         = instantiateItube;
		g_psStereoTubeDescriptor->connect_port        = connectPortToItube;
		g_psStereoTubeDescriptor->activate            = activateItube;
		g_psStereoTubeDescriptor->run                 = runStereoItube;
		g_psStereoTubeDescriptor->run_adding          = NULL;
		g_psStereoTubeDescriptor->set_run_adding_gain = NULL;
		g_psStereoTubeDescriptor->deactivate          = NULL;
		g_psStereoTubeDescriptor->cleanup             = cleanupItube;
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
	deleteDescriptor(g_psMonoTubeDescriptor);
	deleteDescriptor(g_psStereoTubeDescriptor);
}

/* Return a descriptor of the requested plugin type.  */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index) {
	switch (Index) {
		case 0:
			return g_psMonoTubeDescriptor;
		case 1:
			return g_psStereoTubeDescriptor;
		default:
			return NULL;
	}
}

LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr) {

	LADSPA_Data result;

	switch(param)
	{
		case ITUBE_DRIVE:
			if(value<0)
				result= 1;
			else if (value < 18)
				result = pow(10,value/20.0);
			else
				result= pow(10,0.9);
			break;
		case ITUBE_DCOFFSET:
			if(value<-1)
				result= -1;
			else if (value < 0)
				result = -pow(value,2);
			else if (value < 1)
				result = pow(value,2);
			else
				result= 1;
			break;
		case ITUBE_PHASE:
			if(value<0.5)
				result= 0; 
			else
				result= 1;
			break;
		case ITUBE_MIX:
			if(value<0)
				result= 0;
			else if (value < 100)
				result = value/100;
			else
				result= 1;
			break;
		default:
			result=0;
			break;
	}
	return result;
}

