 /****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/initMeth.c,v $
 *
 * Copyright (c) 1999-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: initMeth.c,v 1.19.2.2 2009/09/10 16:36:03 mawi Exp $
 *
 *
 ****************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.19.2.2 2009/09/10 16:36:03 mawi Exp $ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1




#include "method.h"


/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History: 
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{

  int dimRange[2] = { 2,3 };
  int lowMat[3] = { 32, 32, 8 };
  int upMat[3]  = { 1024, 512, 256 };

  DB_MSG(( "Entering fisp:initMeth()\n" ));

  PTB_VersionRequirement( Yes,20090101,"");

  UT_ResetErrorReporting();

  UT_ClearRequest();

  /* Initial NA (see code in parsRelations )                      Added by Marty Pagel */

  if(ParxRelsParHasValue("NumSatFrqTotal") == No)
    NumSatFrqTotal = 1;
    SatPower = 4;	

  if(ParxRelsParHasValue("SatFrqList") == No)     
    SatFrqList[0] = 0.0;         

  if(ParxRelsParHasValue("sat_range_number") == No) {
    sat_range_number = 1;
 
    PARX_change_dims("sat_range_start",1);
    PARX_change_dims("sat_range_end",1);
    PARX_change_dims("sat_range_interval",1);
    PARX_change_dims("sat_range_NumSatFrq",1);

    sat_range_start[0]=-10.0;
    sat_range_end[0]=10.0;
    sat_range_interval[0]=1.0;
    sat_range_NumSatFrq[0]=21;
  }

/*End of addition by Marty Pagel */

  STB_InitStandardInplaneGeoPars(2,dimRange,lowMat,upMat,No);
  STB_InitSliceGeoPars(0,0,0);

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  
  ParxRelsMakeNonEditable( "PVM_2dPhaseGradientTime,"
			   "PVM_3dPhaseGradientTime" );
  ParxRelsHideInFile( "PVM_2dPhaseGradientTime,"
		      "PVM_3dPhaseGradientTime" );


			  
  /* Initialisation of rf pulse parameters */

  /* 1: flip angle in the scan editor 
     (use for refoc. pulse angle in this method)*/
  if(ParxRelsParHasValue("PVM_ExcPulseAngle") == No)
      PVM_ExcPulseAngle = 180.0;
  ParxRelsShowInEditor("PVM_ExcPulseAngle");

  /* 2: pulses declared in parDefinitions.h 
     in this case - ExcPulse and RefPulse. */
  if(ParxRelsParHasValue("ExcPulse") == No)
     STB_InitRFPulse(&ExcPulse,
		     CFG_RFPulseDefaultShapename(LIB_EXCITATION),
		     1.0,  /* default duration in ms */
		     60.0);
  if(ParxRelsParHasValue("InvPulse") == No)  
     STB_InitRFPulse(&InvPulse,
                     CFG_RFPulseDefaultShapename(LIB_INVERSION),
                     1.0,  /* default duration in ms */
                     180.0);
  if(ParxRelsParHasValue("SatPulse") == No)                   /* Added on by Marty Pagel */
     STB_InitRFPulse(&SatPulse,
                     CFG_RFPulseDefaultShapename(LIB_EXCITATION),
                     1.0,  /* default duration in ms */
                     60.0);

  ExcPulseRange();
  InvPulseRange();                         
  SatPulseRange();                                            /* Added by Marty Pagel */


  /* 3: the corresponding pulse enums */

  STB_InitExcPulseEnum("ExcPulseEnum");
  STB_InitInvPulseEnum("InvPulseEnum");       
  STB_InitExcPulseEnum("SatPulseEnum");                       /* Added on by Marty Pagel */


  PTB_SetSpectrocopyDims( 0, 0 );

 /* Encoding */
  STB_InitEncoding();

  LocalSWhRange();
  STB_InitDigPars();

  ParxRelsMakeNonEditable("Seg_time");
  ParxRelsMakeNonEditable("Seg_Sep_time");
  ParxRelsMakeNonEditable("Seg_Sep_Fra_time");
  ParxRelsMakeNonEditable("Total_scan_time");
  ParxRelsMakeNonEditable("Alpha2_Prep_time");
  ParxRelsMakeNonEditable("Prescan_Delay");
   
  if(ParxRelsParHasValue("RFSpoilerOnOff") == 0)
    RFSpoilerOnOff = Off;
 
  if(ParxRelsParHasValue("PVM_ppgFlag1") == No)         
    PVM_ppgFlag1 = No;

  if(ParxRelsParHasValue("PVM_ppgFlag2") == No)         
    PVM_ppgFlag2 = No;

  if(ParxRelsParHasValue("ssfp") == No)
    ssfp = TRUE_FISP;

  if(ParxRelsParHasValue("seg_mode") == No) 
    seg_mode = sequential; 
  
  if(ParxRelsParHasValue("sep_mode") == No) 
    sep_mode = linear; 

  if(ParxRelsParHasValue("PVM_NRepetitions") == No) 
     PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("PVM_InversionTime") == No)  
     PVM_InversionTime = 0.0;

  if(ParxRelsParHasValue("FISP_inversion_enable") == No) 
     FISP_inversion_enable = No; 

  if(ParxRelsParHasValue("Nsegments") == No)
    Nsegments = 1;

  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
    PVM_NMovieFrames = 1;

  ParxRelsResetPar("Nframes");

  if(PVM_NMovieFrames > 1)
    PVM_MovieOnOff = On;

  if(ParxRelsParHasValue("PhaseAdvance") == No)
    PhaseAdvance = 180;

  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  if(ParxRelsParHasValue("Spoiler_duration") == No)
    Spoiler_duration = 5.0;

  if(ParxRelsParHasValue("FlipAngMz") == No)
    FlipAngMz = 30;


/*  if(ParxRelsParHasValue("PVM_EcgTriggerModuleOnOff") == No)
     PVM_EcgTriggerModuleOnOff = Off;
*/
  
  MyEchoPosRange();
  Local_MovieOnOffRange();

  if(ParxRelsParHasValue("DummyEchoes") == No)
     DummyEchoes = 0;

  InitFlipAngleArray();

  if(ParxRelsParHasValue("FitFunctionName") == No)
   sprintf(FitFunctionName,"No Function Defined");

  if(ParxRelsParHasValue("ProcessingMacro") == No)
    {
     ProcessingMacro = No;
     PARX_hide_pars(HIDE_FOR_EDIT,"ProcessingMacroName");
    } 
  STB_InitTriggerModule();

  /* Visibility of Scan Editor parameters */
  ParxRelsShowInEditor("PVM_EchoTime1,PVM_NEchoImages");
  ParxRelsHideClassInEditor("ScanEditorInterface");
  ParxRelsMakeEditable("PVM_EchoTime1"); 
  ParxRelsMakeNonEditable("PVM_NEchoImages");
  ParxRelsShowInFile("PVM_EchoTime1,PVM_EchoTime,PVM_NEchoImages");
  ParxRelsMakeEditable("PVM_NMovieFrames");
  

  ParxRelsHideClassInEditor("PPGparameters");
  ParxRelsShowInFile("PPGparameters");
 
  SSFPPrepRange();

  backbone(); 

  DB_MSG(( "Exiting fisp:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


