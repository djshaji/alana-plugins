/* 
    This LADSPA plugin provides a mono and stereo compressor.

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
#include "inv_compressor.h"

/* The port numbers for the plugin: */

#define ICOMP_RMS 0
#define ICOMP_ATTACK 1
#define ICOMP_RELEASE 2
#define ICOMP_THRESH 3
#define ICOMP_RATIO 4
#define ICOMP_GAIN 5
#define ICOMP_NOCLIP 6
#define ICOMP_METER 7
#define ICOMP_AUDIO_INPUT1  8
#define ICOMP_AUDIO_OUTPUT1 9
#define ICOMP_AUDIO_INPUT2  10  /* not used in mono mode */
#define ICOMP_AUDIO_OUTPUT2 11  /* not used in mono mode */


/* The structure used to hold port connection information and state */

typedef struct {
	unsigned long SampleRate; 

	/* Ports */
	LADSPA_Data * ControlRms;         
	LADSPA_Data * ControlAttack;
	LADSPA_Data * ControlRelease;  
	LADSPA_Data * ControlThresh; 
	LADSPA_Data * ControlRatio;
	LADSPA_Data * ControlGain;
	LADSPA_Data * ControlNoClip;
	LADSPA_Data * ControlMeter;
	LADSPA_Data * AudioInputBuffer1;
	LADSPA_Data * AudioOutputBuffer1;
	LADSPA_Data * AudioInputBuffer2; 
	LADSPA_Data * AudioOutputBuffer2;

	/* these params are used to remember the control values and the converted (internal) value to save a bit of cpu converting them every run */
	LADSPA_Data LastRms;         
	LADSPA_Data LastAttack;
	LADSPA_Data LastRelease;  
	LADSPA_Data LastThresh; 
	LADSPA_Data LastRatio;
	LADSPA_Data LastGain;
	LADSPA_Data LastNoClip;

	LADSPA_Data ConvertedRms;         
	LADSPA_Data ConvertedAttack;
	LADSPA_Data ConvertedRelease;  
	LADSPA_Data ConvertedThresh; 
	LADSPA_Data ConvertedRatio;
	LADSPA_Data ConvertedGain;
	LADSPA_Data ConvertedNoClip;

	/* this stuff needs to be remembered between run calls */
	LADSPA_Data Envelope; 
	LADSPA_Data Rms;

} Icomp;

/* Construct a new plugin instance. */
LADSPA_Handle instantiateIcomp(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate) 
{
	LADSPA_Handle Instance;
	Icomp * plug;
				 
	if((Instance=malloc(sizeof(Icomp)))==NULL)
		return NULL;
				 
	/* set some initial params */
	plug = (Icomp *)Instance;
	/* should check SampleRate > 0 */
	plug->SampleRate=SampleRate;

	return Instance;
}

/* Connect a port to a data location. */
void connectPortToIcomp(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) 
{
	Icomp * plug;
	plug = (Icomp *)Instance;
	switch (Port) {
		case ICOMP_RMS:
			plug->ControlRms = DataLocation;
			break;
		case ICOMP_ATTACK:
			plug->ControlAttack  = DataLocation;
			break;
		case ICOMP_RELEASE:
			plug->ControlRelease = DataLocation;
			break;
		case ICOMP_THRESH:
			plug->ControlThresh = DataLocation;
			break;
		case ICOMP_RATIO:
			plug->ControlRatio = DataLocation;
			break;
		case ICOMP_GAIN:
			plug->ControlGain = DataLocation;
			break;
		case ICOMP_NOCLIP:
			plug->ControlNoClip = DataLocation;
			break;
		case ICOMP_METER:
			plug->ControlMeter = DataLocation;
			break;
		case ICOMP_AUDIO_INPUT1:
			plug->AudioInputBuffer1 = DataLocation;
			break;
		case ICOMP_AUDIO_OUTPUT1:
			plug->AudioOutputBuffer1 = DataLocation;
			break;
		case ICOMP_AUDIO_INPUT2:
			plug->AudioInputBuffer2 = DataLocation;
			break;
		case ICOMP_AUDIO_OUTPUT2:
			plug->AudioOutputBuffer2 = DataLocation;
			break;
	}
}

void activateIcomp(LADSPA_Handle Instance)
{
	Icomp * plug;	
	plug = (Icomp *)Instance;

	plug->Envelope=0;
	plug->Rms=0;

	/* default values */
	plug->LastRms    =0.5;         
	plug->LastAttack =0.01;
	plug->LastRelease=1;  
	plug->LastThresh =0; 
	plug->LastRatio  =1;
	plug->LastGain   =0;
	plug->LastNoClip =1;

	plug->ConvertedRms    =convertParam(ICOMP_RMS,     plug->LastRms,     plug->SampleRate);
	plug->ConvertedAttack =convertParam(ICOMP_ATTACK,  plug->LastAttack,  plug->SampleRate);
	plug->ConvertedRelease=convertParam(ICOMP_RELEASE, plug->LastRelease, plug->SampleRate);
	plug->ConvertedThresh =convertParam(ICOMP_THRESH,  plug->LastThresh,  plug->SampleRate);
	plug->ConvertedRatio  =convertParam(ICOMP_RATIO,   plug->LastRatio,   plug->SampleRate);
	plug->ConvertedGain   =convertParam(ICOMP_GAIN,    plug->LastGain,    plug->SampleRate);
	plug->ConvertedNoClip =convertParam(ICOMP_NOCLIP,  plug->LastNoClip,  plug->SampleRate);
}

void runMonoIcomp(LADSPA_Handle Instance, unsigned long SampleCount) 
{ 
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data fAudio1,fEnvelope,fRms,fRmsSize;
	LADSPA_Data fAttack,fRelease,fThresh,fRatio,fGain,fCompGain,fNoClip;
	Icomp * plug;
	unsigned long lSampleIndex;
			   
	plug = (Icomp *)Instance;
	pParamFunc = &convertParam;

	/* see if any params have changed */
	checkParamChange(ICOMP_RMS,    plug->ControlRms,    &(plug->LastRms),    &(plug->ConvertedRms),    plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_ATTACK, plug->ControlAttack, &(plug->LastAttack), &(plug->ConvertedAttack), plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_RELEASE,plug->ControlRelease,&(plug->LastRelease),&(plug->ConvertedRelease),plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_THRESH, plug->ControlThresh, &(plug->LastThresh), &(plug->ConvertedThresh), plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_RATIO,  plug->ControlRatio,  &(plug->LastRatio),  &(plug->ConvertedRatio),  plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fRmsSize  = plug->ConvertedRms;
	fAttack   = plug->ConvertedAttack;
	fRelease  = plug->ConvertedRelease;
	fThresh   = plug->ConvertedThresh;
	fRatio    = plug->ConvertedRatio;
	fGain     = plug->ConvertedGain;
	fNoClip   = plug->ConvertedNoClip;

	fEnvelope = plug->Envelope;   
	fRms      = plug->Rms;
	fCompGain = 1; // this is set before it is used unless we are given no samples in which case it doesn't matter

	pfAudioInput1  = plug->AudioInputBuffer1;
	pfAudioOutput1 = plug->AudioOutputBuffer1;

	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) 
	{
		fAudio1=*(pfAudioInput1++);
		// work out the rms
		fRms = sqrt(( (fRmsSize-1)*fRms*fRms + fAudio1*fAudio1 ) / fRmsSize); 
		// work out the envelope
		fEnvelope += (fRms > fEnvelope) ? fAttack * (fRms - fEnvelope) : fRelease * (fRms - fEnvelope);
		// work out the gain	  
		fCompGain = (fEnvelope > fThresh) ? (pow((fEnvelope/fThresh), ((1.0/fRatio)-1.0) )) : 1;

		*(pfAudioOutput1++) = fNoClip > 0 ? InoClip(fAudio1*fCompGain * fGain ) : fAudio1*fCompGain * fGain ;
	}
	// remember for next time round
	plug->Envelope = (fabs(fEnvelope)<1.0e-10)  ? 0.f : fEnvelope; 
	plug->Rms = (fabs(fRms)<1.0e-10)  ? 0.f : fRms; 

	// update the meter. 0.015848932=-36dB (the max gain reduction this compressor can do)
	*(plug->ControlMeter)=(fCompGain > 0.015848932) ? 20*log10(fCompGain) : -36.0;
}

void runStereoIcomp(LADSPA_Handle Instance,unsigned long SampleCount) 
{
	LADSPA_Data (*pParamFunc)(unsigned long, LADSPA_Data, unsigned long) = NULL;
	LADSPA_Data * pfAudioInput1;
	LADSPA_Data * pfAudioInput2;
	LADSPA_Data * pfAudioOutput1;
	LADSPA_Data * pfAudioOutput2;
	LADSPA_Data fAudio1,fAudio2,fMaxAudio,fEnvelope,fRms,fRmsSize;
	LADSPA_Data fAttack,fRelease,fThresh,fRatio,fGain,fCompGain,fNoClip;
	Icomp * plug;
	unsigned long lSampleIndex;
			   
	plug = (Icomp *)Instance;
	pParamFunc = &convertParam;
			   
	/* see if any params have changed */
	checkParamChange(ICOMP_RMS,    plug->ControlRms,    &(plug->LastRms),    &(plug->ConvertedRms),    plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_ATTACK, plug->ControlAttack, &(plug->LastAttack), &(plug->ConvertedAttack), plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_RELEASE,plug->ControlRelease,&(plug->LastRelease),&(plug->ConvertedRelease),plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_THRESH, plug->ControlThresh, &(plug->LastThresh), &(plug->ConvertedThresh), plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_RATIO,  plug->ControlRatio,  &(plug->LastRatio),  &(plug->ConvertedRatio),  plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_GAIN,   plug->ControlGain,   &(plug->LastGain),   &(plug->ConvertedGain),   plug->SampleRate, pParamFunc);
	checkParamChange(ICOMP_NOCLIP, plug->ControlNoClip, &(plug->LastNoClip), &(plug->ConvertedNoClip), plug->SampleRate, pParamFunc);

	fRmsSize  = plug->ConvertedRms;
	fAttack   = plug->ConvertedAttack;
	fRelease  = plug->ConvertedRelease;
	fThresh   = plug->ConvertedThresh;
	fRatio    = plug->ConvertedRatio;
	fGain     = plug->ConvertedGain;
	fNoClip   = plug->ConvertedNoClip;

	fEnvelope = plug->Envelope;   
	fRms      = plug->Rms;
	fCompGain = 1; // this is set before it is used

	pfAudioInput1  = plug->AudioInputBuffer1;
	pfAudioInput2  = plug->AudioInputBuffer2;
	pfAudioOutput1 = plug->AudioOutputBuffer1;
	pfAudioOutput2 = plug->AudioOutputBuffer2;
	
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++) 
	{
		fAudio1=*(pfAudioInput1++);
		fAudio2=*(pfAudioInput2++);

		// work out the rms
		fMaxAudio = fabs(fAudio1) > fabs(fAudio2) ? fAudio1 : fAudio2;
		fRms = sqrt(( (fRmsSize-1)*fRms*fRms + fMaxAudio*fMaxAudio ) / fRmsSize); 
		// work out the envelope
		fEnvelope += (fRms > fEnvelope) ? fAttack * (fRms - fEnvelope) : fRelease * (fRms - fEnvelope);
		// work out the gain	  
		fCompGain = (fEnvelope > fThresh) ? (pow((fEnvelope/fThresh), ((1.0/fRatio)-1.0))) : 1;

		*(pfAudioOutput1++) = fNoClip > 0 ? InoClip(fAudio1*fCompGain*fGain) : fAudio1*fCompGain*fGain ;
		*(pfAudioOutput2++) = fNoClip > 0 ? InoClip(fAudio2*fCompGain*fGain) : fAudio2*fCompGain*fGain ;
	}
	// remember for next time round
	plug->Envelope = (fabs(fEnvelope)<1.0e-10)  ? 0.f : fEnvelope; 
	plug->Rms = (fabs(fRms)<1.0e-10)  ? 0.f : fRms; 

	// update the meter
	*(plug->ControlMeter)=(fCompGain > 0.015848932) ? 20*log10(fCompGain) : -36.0;
}

void cleanupIcomp(LADSPA_Handle Instance) {
	free(Instance);
}
 
LADSPA_Descriptor * g_psMonoCompDescriptor = NULL;
LADSPA_Descriptor * g_psStereoCompDescriptor = NULL;

void _init() {

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psMonoCompDescriptor   = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	g_psStereoCompDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	
	if (g_psMonoCompDescriptor) {

		g_psMonoCompDescriptor->UniqueID          = 3308;
		g_psMonoCompDescriptor->Properties        = LADSPA_PROPERTY_HARD_RT_CAPABLE;
		g_psMonoCompDescriptor->Label             = strdup("invada_mono_compressor_module_0_1");
		g_psMonoCompDescriptor->Name              = strdup(":: Invada :: Compressor - Mono");
		g_psMonoCompDescriptor->Maker             = strdup("Fraser At Invada Records dot Com");
		g_psMonoCompDescriptor->Copyright         = strdup("(c) Invada Records");      
		g_psMonoCompDescriptor->PortCount         = 10;

		piPortDescriptors                         = (LADSPA_PortDescriptor *)calloc(10, sizeof(LADSPA_PortDescriptor));
		g_psMonoCompDescriptor->PortDescriptors   = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[ICOMP_RMS]              = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_ATTACK]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_RELEASE]          = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_THRESH]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_RATIO]            = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_GAIN]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_NOCLIP]           = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_METER]            = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_AUDIO_INPUT1]     = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ICOMP_AUDIO_OUTPUT1]    = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                               = (char **)calloc(10, sizeof(char *));
		g_psMonoCompDescriptor->PortNames         = (const char **)pcPortNames;

		pcPortNames[ICOMP_RMS]                    = strdup("Tight / Sloppy");    
		pcPortNames[ICOMP_ATTACK]                 = strdup("Attack (ms)");  
		pcPortNames[ICOMP_RELEASE]                = strdup("Release (ms)");  
		pcPortNames[ICOMP_THRESH]                 = strdup("Threshold (dB)");  
		pcPortNames[ICOMP_RATIO]                  = strdup("Ratio");  
		pcPortNames[ICOMP_GAIN]                   = strdup("Gain (dB)");  
		pcPortNames[ICOMP_NOCLIP]                 = strdup("Soft Clip");  
		pcPortNames[ICOMP_METER]                  = strdup("Gain Reduction");  
		pcPortNames[ICOMP_AUDIO_INPUT1]           = strdup("Input");
		pcPortNames[ICOMP_AUDIO_OUTPUT1]          = strdup("Output");

		psPortRangeHints                          = ((LADSPA_PortRangeHint *) calloc(10, sizeof(LADSPA_PortRangeHint)));
		g_psMonoCompDescriptor->PortRangeHints    = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[ICOMP_RMS].HintDescriptor           = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[ICOMP_RMS].LowerBound               = 0;
		psPortRangeHints[ICOMP_RMS].UpperBound               = 1;

		psPortRangeHints[ICOMP_ATTACK].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_MINIMUM );
		psPortRangeHints[ICOMP_ATTACK].LowerBound            = 0.01;
		psPortRangeHints[ICOMP_ATTACK].UpperBound            = 750;

		psPortRangeHints[ICOMP_RELEASE].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_MINIMUM );
		psPortRangeHints[ICOMP_RELEASE].LowerBound           = 1;
		psPortRangeHints[ICOMP_RELEASE].UpperBound           = 5000;

		psPortRangeHints[ICOMP_THRESH].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ICOMP_THRESH].LowerBound            = -36;
		psPortRangeHints[ICOMP_THRESH].UpperBound            = 0;

		psPortRangeHints[ICOMP_RATIO].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_1 );
		psPortRangeHints[ICOMP_RATIO].LowerBound             = 1;
		psPortRangeHints[ICOMP_RATIO].UpperBound             = 20;

		psPortRangeHints[ICOMP_GAIN].HintDescriptor          = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ICOMP_GAIN].LowerBound              = -6;
		psPortRangeHints[ICOMP_GAIN].UpperBound              = 36;

		psPortRangeHints[ICOMP_NOCLIP].HintDescriptor        = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_1);

		psPortRangeHints[ICOMP_METER].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE);
		psPortRangeHints[ICOMP_METER].LowerBound             = -36;
		psPortRangeHints[ICOMP_METER].UpperBound             = 0;

		psPortRangeHints[ICOMP_AUDIO_INPUT1].HintDescriptor  = 0;
		psPortRangeHints[ICOMP_AUDIO_OUTPUT1].HintDescriptor = 0;


		g_psMonoCompDescriptor->instantiate         = instantiateIcomp;
		g_psMonoCompDescriptor->connect_port        = connectPortToIcomp;
		g_psMonoCompDescriptor->activate            = activateIcomp;
		g_psMonoCompDescriptor->run                 = runMonoIcomp;
		g_psMonoCompDescriptor->run_adding          = NULL;
		g_psMonoCompDescriptor->set_run_adding_gain = NULL;
		g_psMonoCompDescriptor->deactivate          = NULL;
		g_psMonoCompDescriptor->cleanup             = cleanupIcomp;
	}
	if (g_psStereoCompDescriptor) {
		g_psStereoCompDescriptor->UniqueID          = 3309;
		g_psStereoCompDescriptor->Properties        = LADSPA_PROPERTY_HARD_RT_CAPABLE;

		g_psStereoCompDescriptor->Label             = strdup("invada_stereo_compressor_module_0_1");
		g_psStereoCompDescriptor->Name              = strdup(":: Invada :: Compressor - Stereo");
		g_psStereoCompDescriptor->Maker             = strdup("Fraser At Invada Records dot Com");
		g_psStereoCompDescriptor->Copyright         = strdup("(c) Invada Records");      

		g_psStereoCompDescriptor->PortCount         = 12;

		piPortDescriptors                           = (LADSPA_PortDescriptor *)calloc(12, sizeof(LADSPA_PortDescriptor));
		g_psStereoCompDescriptor->PortDescriptors   = (const LADSPA_PortDescriptor *)piPortDescriptors;

		piPortDescriptors[ICOMP_RMS]                = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_ATTACK]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_RELEASE]            = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_THRESH]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_RATIO]              = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_GAIN]               = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_NOCLIP]             = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_METER]              = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[ICOMP_AUDIO_INPUT1]       = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ICOMP_AUDIO_OUTPUT1]      = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ICOMP_AUDIO_INPUT2]       = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[ICOMP_AUDIO_OUTPUT2]      = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;

		pcPortNames                                 = (char **)calloc(12, sizeof(char *));
		g_psStereoCompDescriptor->PortNames         = (const char **)pcPortNames;

		pcPortNames[ICOMP_RMS]                      = strdup("Tight / Sloppy");    
		pcPortNames[ICOMP_ATTACK]                   = strdup("Attack (ms)");  
		pcPortNames[ICOMP_RELEASE]                  = strdup("Release (ms)");  
		pcPortNames[ICOMP_THRESH]                   = strdup("Threshold (dB)");  
		pcPortNames[ICOMP_RATIO]                    = strdup("Ratio");  
		pcPortNames[ICOMP_GAIN]                     = strdup("Gain (dB)");  
		pcPortNames[ICOMP_NOCLIP]                   = strdup("Soft Clip"); 
		pcPortNames[ICOMP_METER]                    = strdup("Gain Reduction");   
		pcPortNames[ICOMP_AUDIO_INPUT1]             = strdup("Input (Left)");
		pcPortNames[ICOMP_AUDIO_OUTPUT1]            = strdup("Output (Left)");
		pcPortNames[ICOMP_AUDIO_INPUT2]             = strdup("Input (Right)");
		pcPortNames[ICOMP_AUDIO_OUTPUT2]            = strdup("Output (Right)");

		psPortRangeHints                            = ((LADSPA_PortRangeHint *) calloc(12, sizeof(LADSPA_PortRangeHint)));
		g_psStereoCompDescriptor->PortRangeHints    = (const LADSPA_PortRangeHint *)psPortRangeHints;

		psPortRangeHints[ICOMP_RMS].HintDescriptor           = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_LOW);
		psPortRangeHints[ICOMP_RMS].LowerBound               = 0;
		psPortRangeHints[ICOMP_RMS].UpperBound               = 1;

		psPortRangeHints[ICOMP_ATTACK].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_MINIMUM );
		psPortRangeHints[ICOMP_ATTACK].LowerBound            = 0.01;
		psPortRangeHints[ICOMP_ATTACK].UpperBound            = 300;

		psPortRangeHints[ICOMP_RELEASE].HintDescriptor       = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_MINIMUM );
		psPortRangeHints[ICOMP_RELEASE].LowerBound           = 1;
		psPortRangeHints[ICOMP_RELEASE].UpperBound           = 2000;

		psPortRangeHints[ICOMP_THRESH].HintDescriptor        = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ICOMP_THRESH].LowerBound            = -36;
		psPortRangeHints[ICOMP_THRESH].UpperBound            = 0;

		psPortRangeHints[ICOMP_RATIO].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_LOGARITHMIC | 
									LADSPA_HINT_DEFAULT_1 );
		psPortRangeHints[ICOMP_RATIO].LowerBound             = 1;
		psPortRangeHints[ICOMP_RATIO].UpperBound             = 20;

		psPortRangeHints[ICOMP_GAIN].HintDescriptor          = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE | 
									LADSPA_HINT_DEFAULT_0 );
		psPortRangeHints[ICOMP_GAIN].LowerBound              = -6;
		psPortRangeHints[ICOMP_GAIN].UpperBound              = 36;

		psPortRangeHints[ICOMP_NOCLIP].HintDescriptor        = (LADSPA_HINT_TOGGLED | 
									LADSPA_HINT_DEFAULT_1);

		psPortRangeHints[ICOMP_METER].HintDescriptor         = (LADSPA_HINT_BOUNDED_BELOW | 
									LADSPA_HINT_BOUNDED_ABOVE);
		psPortRangeHints[ICOMP_METER].LowerBound	     = -36;
		psPortRangeHints[ICOMP_METER].UpperBound             = 0;

		psPortRangeHints[ICOMP_AUDIO_INPUT1].HintDescriptor  = 0;
		psPortRangeHints[ICOMP_AUDIO_OUTPUT1].HintDescriptor = 0;
		psPortRangeHints[ICOMP_AUDIO_INPUT2].HintDescriptor  = 0;
		psPortRangeHints[ICOMP_AUDIO_OUTPUT2].HintDescriptor = 0;


		g_psStereoCompDescriptor->instantiate         = instantiateIcomp;
		g_psStereoCompDescriptor->connect_port        = connectPortToIcomp;
		g_psStereoCompDescriptor->activate            = activateIcomp ;
		g_psStereoCompDescriptor->run                 = runStereoIcomp;
		g_psStereoCompDescriptor->run_adding          = NULL;
		g_psStereoCompDescriptor->set_run_adding_gain = NULL;
		g_psStereoCompDescriptor->deactivate          = NULL;
		g_psStereoCompDescriptor->cleanup             = cleanupIcomp;
	}
}

void deleteDescriptor(LADSPA_Descriptor * psDescriptor) 
{
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
void _fini() 
{
	deleteDescriptor(g_psMonoCompDescriptor);
	deleteDescriptor(g_psStereoCompDescriptor);
}

/* Return a descriptor of the requested plugin type. */
const LADSPA_Descriptor * 
ladspa_descriptor(unsigned long Index) {
	switch (Index) {
		case 0:
			return g_psMonoCompDescriptor;
		case 1:
			return g_psStereoCompDescriptor;
		default:
			return NULL;
	}
}

LADSPA_Data convertParam(unsigned long param, LADSPA_Data value, unsigned long sr) {
/* some conversion formulae are shared so the bounds are the min/max across all ports */
	LADSPA_Data result;
	switch(param)  {
		case ICOMP_RMS:
			if(value<0) 
				result= 1;
			else if (value < 1)
				result= (pow(value,3) * (LADSPA_Data)sr/20)+1;
			else
				result= ((LADSPA_Data)sr/20)+1;
			break;
		case ICOMP_ATTACK:
		case ICOMP_RELEASE:
			if(value<0.01)
				result= 1 - pow(10, -301.0301 / ((LADSPA_Data)sr * 0.01)); 
			else if (value <5000)
				result= 1 - pow(10, -301.0301 / ((LADSPA_Data)sr * value)); 
			else
				result= 1 - pow(10, -301.0301 / ((LADSPA_Data)sr * 5000)); 
			break;
		case ICOMP_THRESH:
		case ICOMP_RATIO:
		case ICOMP_GAIN:
			if(value<-36)
				result= pow(10, -1.8);
			else if (value < 36)
				result= pow(10, value/20);
			else
				result= pow(10, 1.8);
			break;
		case ICOMP_NOCLIP:
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

