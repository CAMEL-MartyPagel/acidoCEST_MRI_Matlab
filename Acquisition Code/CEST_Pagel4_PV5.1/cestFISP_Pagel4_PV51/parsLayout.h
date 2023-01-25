/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/parsLayout.h,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsLayout.h,v 1.13.2.1 2007/03/28 09:31:39 sako Exp $
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/



/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

parclass
{
  FlipAngMz;
  NumPrep;
} 
attributes
{
  display_name "Starter Sequence";
} Starter_Sequence;

parclass
{
  PVM_NMovieFrames;
  Const_separation;
  Seg_time;
  Seg_Sep_time;
  Seg_Sep_Fra_time;  
} Movie;

parclass             /* ... kws */          
{
  ssfp_preparation;
  Starter_Sequence;
  Alpha2_Prep_time;
  DummyEchoes;
  Prescan_Delay;
  FISP_inversion_enable;
  PVM_InversionTime;
  Inv_Rep_time;   /* 09.04. kws */  
  PVM_MovieOnOff;
  Movie;
  PVM_TriggerModule;
  Trigger_Parameters;
  RFSpoilerOnOff;

} Preparation;

parclass
{
  ExcPulseEnum;
  ExcPulse;
  InvPulseEnum;
  InvPulse;    
} 
attributes
{
  display_name "RF Pulses";
} RF_pulses;

parclass
{
  PVM_EffSWh;
  PhaseAdvance;
  PVM_EchoPosition;
  PVM_MinFov;
  PVM_MinSliceThick;
  Spoiler_duration;
  DigitizerPars;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

parclass
{
  FitFunctionName;
  ProcessingMacro;
  ProcessingMacroName;
}attributes
{
  display_name "Post Processing";
} PostProcessing;

/* The following class is defined to assure that parameters of the 
 * scan editor are properly displayed. */
parclass
{
  PVM_EchoTime1;
  PVM_NEchoImages;
  PVM_NMovieFrames;
} ScanEditorInterface;

parclass
{
  PVM_ppgFlag1;
  PVM_ppgFlag2;
} PPGparameters;

parclass
{
  Method;
  ssfp;
  PresatDelay;                        	/* Added by Marty Pagel */ 
  NumSatPulses;           		/* Added by Marty Pagel */
  SatPower;             		/* added by Marty Pagel */
  SatPulseEnum;      			/* Added by Marty Pagel */
  SatPulse;          			/* Added by Marty Pagel */
  
  sat_range_number;			/* Added by Marty Pagel */
  sat_range_start;			/* Added by Marty Pagel */
  sat_range_end;			/* Added by Marty Pagel */
  sat_range_interval;                   /* Added by Marty Pagel */
  sat_range_NumSatFrq;			/* Added by Marty Pagel */

  NumSatFrqTotal;          		/* Added by Marty Pagel */
  SatFrqList;            		/* Added by Marty Pagel */
  DoHTYesNo;				/* Added by Marty Pagel */
  PVM_ExcPulseAngle;
  PVM_EchoTime;
  PVM_RepetitionTime;
  Nsegments;
  seg_mode; 
  Scan_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_DeriveGains;
  RF_pulses;
  Nuclei;
  Encoding;
  Sequence_Details; 
  StandardInplaneGeometry;
  StandardSliceGeometry;
  Preparation;
  PostProcessing;
  ScanEditorInterface;
  PPGparameters;
  Method_RecoOptions;
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

