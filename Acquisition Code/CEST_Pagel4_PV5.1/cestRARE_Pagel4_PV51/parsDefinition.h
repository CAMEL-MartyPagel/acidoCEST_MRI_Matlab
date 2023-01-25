/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/parsDefinition.h,v $
 *
 * Copyright (c) 1999-2001
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.11.2.2 2008/04/01 12:58:18 sako Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/
double parameter ReadGradRatio;
double parameter SliceGradRatio;
double parameter Phase3dInteg;
double parameter Phase2dInteg;
double parameter minTE1;
double parameter minTE2;
double parameter EvolutionDuration;
double parameter OneRepTime;
double parameter
{
  display_name "Effective echo time";
  format "%.2f";
  units "ms";
  relations effTErels; 
} EffectiveTE[];


double parameter
{
  display_name "Slice Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} SliceSpoilerDuration;

double parameter
{
  display_name "Slice Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} SliceSpoilerStrength;

double parameter
{
  display_name "Repetition Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} RepetitionSpoilerDuration;

double parameter
{
  display_name "Repetition Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} RepetitionSpoilerStrength;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulseRelation;
}ExcPulse;

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulseEnumRelation;
}RefPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulseRelation;
}RefPulse;

double parameter
{
  display_name "Refocusing Flip Angle";
  format "%.1f";
  units "deg";
  relations RfcFlipAngleRel;
} RfcFlipAngle;

int parameter
{
  display_name "Number of Echoes";
} NEchoes;

double parameter
{
  display_name "Phase Encoding Start";
  format "%.2f";
  relations encodingRelations;
} PhaseEncodingStart;

int parameter 
{
  display_name "Number of Dummy Scans";
  relations dsRelations;
} NDummyScans;

double parameter
{
  display_name "Inter Slice Delay";
  relations backbone;
  units "ms";
  format "%.2f";
}SliceSegDur;

double parameter SliceSegDelay;
double parameter MinSliceSegDur;

double parameter SliceSegEndDelay;

/* PARAMETERS SPECIFIC TO CEST RARE*/

double parameter
{
  display_name "Presaturation Delay";                	/* Added by Marty Pagel */
  format "%.3f";
  units "ms";
  relations backbone;
} PresatDelay;

int parameter
{
  display_name "Number of Sat Pulses";                  /* Added by Marty Pagel */
  format "%d";
  relations backbone;
} NumSatPulses;

double parameter
{
  display_name "Sat Power";                		/* Added by Marty Pagel */
  format "%.3f";
  units "uT";
  relations backbone;
} SatPower;

PV_PULSE_LIST parameter                                 /* Added by Marty Pagel */
{
  display_name "Sat Pulse Shape";
  relations    SatPulseEnumRelation;
} SatPulseEnum;

PVM_RF_PULSE_TYPE parameter                             /* Added by Marty Pagel */
{
  display_name "Sat Pulse";
  relations    SatPulseRelation;
} SatPulse;

int parameter
{
  display_name "number of ranges";    			/* Added by Marty Pagel */
  format "%d";
  relations backbone;
} sat_range_number;

double parameter
{
  display_name "Begin Sat Freq range";    		/* Added by Marty Pagel */
  format "%.3f";
  relations backbone;
} sat_range_start[];

double parameter
{
  display_name "End Sat Freq range";    		/* Added by Marty Pagel */
  format "%.3f";
  relations backbone;
} sat_range_end[];

double parameter
{
  display_name "Sat Freq Interval";    			/* Added by Marty Pagel */
  format "%.3f";
  relations backbone;
} sat_range_interval[];

int parameter
{
  display_name "Number of Sat Freqs in range";    	/* Added by Marty Pagel */
  format "%d";
  relations backbone;
} sat_range_NumSatFrq[];

int parameter
{
  display_name "Total Number of Sat Freqs";             /* Added by Marty Pagel */
  format "%d";
  relations backbone;
} NumSatFrqTotal;

double parameter
{
  display_name "Sat Frq List";                		/* Added by Marty Pagel */
  format "%.3f";
  units "Hz";
  relations backbone;
} SatFrqList[];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/




