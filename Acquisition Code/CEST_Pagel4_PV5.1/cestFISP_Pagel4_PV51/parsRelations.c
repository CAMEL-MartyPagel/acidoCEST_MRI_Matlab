 /****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/parsRelations.c,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsRelations.c,v 1.31.2.12 2009/11/04 09:03:27 sako Exp $
 *
 *
 ****************************************************************/

static const char resid[] = "$Id: parsRelations.c,v 1.31.2.12 2009/11/04 09:03:27 sako Exp $ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"

double Local_MinFov[3];  /* min fov calculated in backbone (depending on TE)
                            is stored here and used by inplaneGeoHandler
                            to constrain fov */

double minInversionTime;

void backbone( void )
{
  YesNo refAttIsAviable;
  double grad_lim, referenceAttenuation = 0.0, minSliceThick;
  int dim;
  double amplifierenable;

  DB_MSG(("---> fisp:backbone"));

  amplifierenable = CFG_AmplifierEnable();
 
  STB_UpdateNuclei(Yes);


  sep_mode = linear; 

  /* handle RF Spoiling */


switch(ssfp)
    {
      default:
      case TRUE_FISP:
        ParxRelsMakeNonEditable("PVM_EchoPosition");
	RFSpoilerOnOff = Off;
        ParxRelsHideInEditor("RFSpoilerOnOff");
        ParxRelsHideInFile("RFSpoilerOnOff");
	break;
      case ECHO:
	RFSpoilerOnOff = Off;
        ParxRelsHideInEditor("RFSpoilerOnOff");
        ParxRelsHideInFile("RFSpoilerOnOff");
        ParxRelsMakeEditable("PVM_EchoPosition");
	break;
      case FID:
        ParxRelsShowInEditor("RFSpoilerOnOff");
        ParxRelsShowInFile("RFSpoilerOnOff");
        ParxRelsMakeEditable("PVM_EchoPosition");
	break;
    }



  /* handle RF pulses */

  if(PVM_DeriveGains == Yes)
    refAttIsAviable =
      STB_GetRefAtt(1,PVM_Nucleus1,&referenceAttenuation); 
  else
    refAttIsAviable = No;

  /* CEST presaturation */

  STB_UpdateRFPulse("SatPulse",                      	/*Added by Marty Pagel */
                    &SatPulse,                       	/*Added by Marty Pagel */
                    refAttIsAviable,                    /*Added by Marty Pagel */
                    referenceAttenuation);              /*Added by Marty Pagel */
  STB_UpdateExcPulseEnum("SatPulseEnum",             	/*Added by Marty Pagel */
                         &SatPulseEnum,              	/*Added by Marty Pagel */
                         SatPulse.Filename,          	/*Added by Marty Pagel */
                         SatPulse.Classification);   	/*Added by Marty Pagel */


  STB_UpdateRFPulse("ExcPulse",
		    &ExcPulse,
		    refAttIsAviable,
		    referenceAttenuation);
  STB_UpdateExcPulseEnum("ExcPulseEnum",
			 &ExcPulseEnum,
			 ExcPulse.Filename,
			 ExcPulse.Classification);

  PVM_ExcPulseAngle = ExcPulse.FlipAngle;


  STB_UpdateRFPulse("InvPulse",
		    &InvPulse,
		    refAttIsAviable,
		    referenceAttenuation);
  STB_UpdateInvPulseEnum("InvPulseEnum",
			 &InvPulseEnum,          /* 23.11. kws */
			 InvPulse.Filename,
			 InvPulse.Classification);

  PVM_InvPulseAngle = InvPulse.FlipAngle;
 
  PVM_ExSlicePulseLength   = ExcPulse.Length;
  PVM_ExSliceBandWidth     = ExcPulse.Bandwidth;
  PVM_ExSliceRephaseFactor = ExcPulse.RephaseFactor;

  /* Preparation for Mz  */
  FlipAngMz = MAX_OF(2.0,FlipAngMz);
  FlipAngMz = MIN_OF(85.0, FlipAngMz);
  FlipAngMzAttenuation1 = ExcPulse.Attenuation
      + 20.0*log10( ExcPulse.FlipAngle / FlipAngMz );

  FlipAngMzAttenuation2 = ExcPulse.Attenuation
      + 20.0*log10( ExcPulse.FlipAngle / (90.0 - FlipAngMz ));
 

  /* next lines added by Marty Pagel */
  SatPulse.FlipAngle = SatPulse.Length*SatPower*42.58*360/1000;
  /* where flip angle in degrees = (360 degrees / 1 cycle) ((1 cycle/sec)/Hz) (42.58 Hz / uT) (SatPower in uT) (PulseLength in sec) */
  /* Note that dividing by 1000 accounts for the SatPuilse.Length being listed in msec but sec is used in the calculation */
  /* end of lines added */

  /***  UpdateFlipAngleArray(&ExcPulse);  ****/
  UpdateFlipAngleArray(&ExcPulse);
  UpdateFlipAngleArray(&SatPulse);              /*Added by Marty Pagel */

 /* number of segments */
  Nsegments = MAX_OF(Nsegments,1);
  Nsegments = MIN_OF(Nsegments,1024);
  SegmRels();

  /* once the image size is decided (after first update of inpl geo), we
     can update the Encoding parameters, to get the acquisition size (PVM_EncMatrix)*/
  
  PVM_RareFactor = 1;
  STB_UpdateEncoding(PTB_GetSpatDim(),  /* total dimensions */
		     PVM_Matrix,        /* image size */ 
		     PVM_AntiAlias,     /* a-alias */
		     &PVM_RareFactor,   /* segment size */
                     seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
		     Yes,               /* ppi in 2nd dim allowed */
		     Yes,               /* ppi ref lines in 2nd dim allowed */
		     Yes);              /* partial ft in 2nd dim allowed */ 

  PVM_RareFactor = PVM_EncMatrix[1]/Nsegments;

  STB_UpdateEncoding(PTB_GetSpatDim(),  /* total dimensions */
		     PVM_Matrix,        /* image size */ 
		     PVM_AntiAlias,     /* a-alias */
		     &PVM_RareFactor,   /* segment size */
                     seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
		     Yes,               /* ppi in 2nd dim allowed */
		     Yes,               /* ppi ref lines in 2nd dim allowed */
		     Yes);              /* partial ft in 2nd dim allowed */ 

  Nsegments = PVM_EncMatrix[1]/PVM_RareFactor;
  

  /** bandwidth and acq duration */
  			  
  STB_UpdateDigPars( &PVM_EffSWh,
		     PVM_EncMatrix[0],
		     PVM_AntiAlias,
		     &PVM_AcquisitionTime);


  /** begin Update Geometry **/
  /* 1: in-plane geometry */
  
  PVM_LimReadDephaseGradient = 99;
  PVM_LimExSliceGradient = 50;
  PVM_LimReadGradient = 50.0;


  Local_MinFov[0] = 100.0*PVM_EffSWh/(PVM_LimReadGradient*PVM_GradCalConst);
  
  /* in this method, TE is adjusted to FOV[1,2] therefore no
     constraints (or just very rough ones):;  */
  Local_MinFov[1] = Local_MinFov[2] = 5.0;  
  
  /** minimum Slice Thickness  **/
  minSliceThick = 100.0*PVM_ExSliceBandWidth/
                      (PVM_LimExSliceGradient*PVM_GradCalConst);

  dim = PTB_GetSpatDim();

  /*
   * Constrain minimum fov in 3D phase direction and 
   * minimum slice thickness in case of 3D measurements
   */

  if(dim == 3)
  {
    double min;

    min=MAX_OF(Local_MinFov[2],minSliceThick);
    Local_MinFov[2] = minSliceThick = min;
    
  }


  STB_StandardInplaneGeoParHandler(Local_MinFov,2.0);

  /* 2: slice geometry  */
 
  if(dim == 3) /* 1 sl/package, slthck=fov[2] */
  {
    PVM_SliceThick = PVM_Fov[2];
    STB_UpdateSliceGeoPars(0,0,1,minSliceThick);
  }
  else         /* no constraints */
  {
    STB_UpdateSliceGeoPars(0,0,0,minSliceThick); 
  }
  
  /* end Update Geometry */

 
  /* RF phase advance against special ghosts */
  PhaseAdvance = MAX_OF(PhaseAdvance,0.0);
  PhaseAdvance = MIN_OF(PhaseAdvance,360.0);


  /* ------------------------------------ */
  

  LocalGradientStrengthRels(); /* calculates PVM_ReadGradient,
                                             PVM_ExSliceGradient*/
  LocalFrequencyOffsetRels();

  /* PVM_NRepetitions = 1; */                     /* kws */
  PVM_NRepetitions = MAX_OF(PVM_NRepetitions, 1);
  PVM_NRepetitions = MIN_OF(PVM_NRepetitions, 10000);

  Local_UpdateMovie();
  PVM_NEchoImages = PVM_NMovieFrames; /* 25.2.kws */
  if (ParxRelsParHasValue("PVM_NAverages") == 0 || PVM_NAverages<1)
     PVM_NAverages = 1;

  localEchoTimeRels(); /* this sets PVM_echoTime to what PVM_Fov allows,
                          and calculates atoms (refoc del, amp etc ) */

  /* Size of ranges */
  PARX_change_dims("sat_range_start",sat_range_number);    	/* Added by Marty Pagel */
  PARX_change_dims("sat_range_end",sat_range_number);    	/* Added by Marty Pagel */
  PARX_change_dims("sat_range_interval",sat_range_number);    	/* Added by Marty Pagel */
  PARX_change_dims("sat_range_NumSatFrq",sat_range_number);    	/* Added by Marty Pagel */

  NumSatFrqTotal=0;						/* Added by Marty Pagel */
  for(int i=0;i<sat_range_number;i++)				/* Added by Marty Pagel */
  {
	if (sat_range_interval[i] > 0) 				/* Added by Marty Pagel */
        {
		int NumSatFrq=(int)(((sat_range_end[i]-sat_range_start[i])/sat_range_interval[i])+1.1);
       		sat_range_NumSatFrq[i]=NumSatFrq;		/* Added by Marty Pagel */
        	NumSatFrqTotal+=NumSatFrq;			/* Added by Marty Pagel */
	}
  }

  PARX_change_dims("SatFrqList",NumSatFrqTotal);    		/* Added by Marty Pagel */

  int my_counter=0;						/* Added by Marty Pagel */
  for(int i=0;i<sat_range_number;i++)				/* Added by Marty Pagel */
  {
        double cest_step_size=(sat_range_interval[i])*CONFIG_basic_frequency;	/* Added by Marty Pagel */
        if (sat_range_interval[i] > 0)						/* Added by Marty Pagel */
	{
        	int NumSatFrq=(int)(((sat_range_end[i]-sat_range_start[i])/sat_range_interval[i])+1.1);
        	for(int j=0;j<NumSatFrq;j++)			/* Added by Marty Pagel */
        	{
                	SatFrqList[my_counter]=sat_range_start[i]*CONFIG_basic_frequency+j*cest_step_size;
                	my_counter++;				/* Added by Marty Pagel */
        	}						/* Added by Marty Pagel */
	}							/* Added by Marty Pagel */
  }								/* Added by Marty Pagel */
 
  STB_UpdateTriggerModule();


  repetitionTimeRels(); /* spoiler must be set before */

  /** Set Echo Parameters for Scan Editor (taken from echoTimeRels) **/
  PVM_EchoTime1 = PVM_EchoTime;
  
  DummyEchoes = MAX_OF(0,DummyEchoes);

  /* calculates timing info values */
     localTimeInfoRels();

  /* For Alpha2-Preparation */           /* sako */
  PVM_Alpha2Delay = PVM_RepetitionTime/2 - PVM_ExSlicePulseLength*PVM_ExSliceRephaseFactor/100.0 - 0.01 - (2*PVM_RiseTime);
  PVM_Alpha2SliceRephGradient = - PVM_ExSliceGradient * (PVM_ExSlicePulseLength + 0.02 + PVM_RiseTime) / PVM_Alpha2Delay;

  grad_lim =  MAX_OF(-100, PVM_ExSliceGradient - 100);
  if( PVM_Alpha2SliceRephGradient < grad_lim)
    {
      PVM_Alpha2SliceRephGradient = grad_lim;
      PVM_Alpha2Delay =  PVM_ExSliceGradient * (PVM_ExSlicePulseLength + 0.02 + PVM_RiseTime) / (-grad_lim);
    }

  Alpha2_Prep_time = (PVM_ExSlicePulseLength*PVM_ExSliceRephaseFactor/100.0 + 0.01 + PVM_Alpha2Delay + 2*PVM_RiseTime)/PVM_RepetitionTime;

  /* Spoiler  */    
  Spoiler_duration = MIN_OF(MAX_OF(PVM_RiseTime, Spoiler_duration),20.0); 


  /* calculates timing info values */
  localTimeInfoRels();

  /* set fit function parameter */
  if((PVM_NMovieFrames > 1)||(PVM_NRepetitions > 1))
  {
    ParxRelsShowClassInEditor("PostProcessing");
    if(ProcessingMacro == Yes)
      ParxRelsShowInEditor("ProcessingMacroName");
    if(ProcessingMacro == No)
      ParxRelsHideInEditor("ProcessingMacroName");
  }
  else
  {
    ParxRelsHideClassInEditor("PostProcessing");
  }
  
  
  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam(); 


  DB_MSG(("<--- fisp:backbone"));
}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/


void localEchoTimeRels(void)
/* ------------------------------------------------------------------
   calculates PVM_MinEchoTime, constrains PVM_EchoTime
   and finds refoc. and ph_enc amplitudes. 
   Calculates Local_MinFov[1,2] for this PVM_EchoTime.
   ---------------------------------------------------------------- */ 
{
   double ref_del, sl_ref_amp, rd_ref_amp, half_pulse, ramp, ramp_int,
     ph1_amp, ph2_amp=0.0, ref_del_min, kHz_per_mm, half_acq,echo_del,
          lim2dgrad, lim3dgrad,lim_read_ref, lim_slice_ref;
   int ch1=0, ch2=0, ch3=0, ch4=0;
   
   if(ssfp == TRUE_FISP)
   {
     PVM_EchoPosition = 50.0;
   }


   half_pulse = PVM_ExSlicePulseLength*PVM_ExSliceRephaseFactor/100.0
                + 0.01;
   echo_del   = PVM_AcquisitionTime*PVM_EchoPosition/100.0 +0.02;
   half_acq   = PVM_AcquisitionTime/2 +0.02; 
   ramp       = PVM_RiseTime;
   ramp_int   = 50.0; 
   ref_del    = PVM_EchoTime - half_pulse - echo_del;

   /* the above line is for single ramps; for double ramps take:
   ref_del    = PVM_EchoTime - half_pulse - half_acq - 2*ramp;
   DEOSC >= aqq+PVM_DigEndDelOpt
   -> ref_del_min >=  PVM_DigEndDelOpt-0.01
   */
   ref_del_min= MAX_OF(2*ramp,PVM_DigEndDelOpt-0.01);
   kHz_per_mm = PVM_GradCalConst / 1000.0;
   lim2dgrad = 57;
   lim3dgrad = 57;
   lim_read_ref = MIN_OF(57,(100-PVM_ReadGradient));
   lim_slice_ref = MIN_OF(57,(100-PVM_ExSliceGradient));
   
 
   /* read and slice refocusing */
   ch1 = set_refoc(&ref_del, &rd_ref_amp, echo_del, PVM_ReadGradient,
	           ramp, ramp_int, lim_read_ref, ref_del_min);
   ch2 = set_refoc(&ref_del, &sl_ref_amp, half_pulse, PVM_ExSliceGradient,
	           ramp, ramp_int, lim_slice_ref, ref_del_min);
   ch3 = set_pulse(&ref_del, &ph1_amp, PVM_SpatResol[1],ramp, 
                   lim2dgrad, kHz_per_mm);
   if(PTB_GetSpatDim() == 3)
   {
     lim3dgrad = 57 - fabs(sl_ref_amp);
     ch4 = set_pulse(&ref_del, &ph2_amp, PVM_SpatResol[2],ramp, 
                     lim3dgrad, kHz_per_mm);	
   }
                                              
   if(ch1 || ch2 || ch3 || ch4)
   { /* ref_del has increased: slice and read refocusing must be calculated
        again and TE must be updated. Current ref_del will be accepted. */

     (void)set_refoc(&ref_del, &rd_ref_amp, echo_del, PVM_ReadGradient,
	             ramp, ramp_int, lim_read_ref, ref_del_min);
     (void)set_refoc(&ref_del, &sl_ref_amp, half_pulse, PVM_ExSliceGradient,
	             ramp, ramp_int, lim_slice_ref, ref_del_min);
     ch3 = set_pulse(&ref_del, &ph1_amp, PVM_SpatResol[1],ramp, 
                   lim2dgrad, kHz_per_mm);
     if(PTB_GetSpatDim() == 3)
     {
       lim3dgrad = 57 - fabs(sl_ref_amp);
       ch4 = set_pulse(&ref_del, &ph2_amp, PVM_SpatResol[2],ramp, 
                     lim3dgrad, kHz_per_mm);	     
     }
 
     /* PVM_EchoTime = ref_del + half_pulse + half_acq; */
     /* the above line is for single ramps; for double ramps take:
     PVM_EchoTime = ref_del + half_pulse + half_acq + 2*ramp;
     */
                      
   }
    
   PVM_EchoTime = ref_del + half_pulse + echo_del;
  
 
   /* the following assignments are needed to pass values to
      SetBaseLevelPars() */
   PVM_ReadDephaseTime = ref_del;
   PVM_ExSliceRephaseTime = ref_del;
   PVM_2dPhaseGradientTime = ref_del;
   PVM_3dPhaseGradientTime = ref_del;
   PVM_ExSliceRephaseGradient = sl_ref_amp;
   PVM_2dPhaseGradient = ph1_amp;
   PVM_3dPhaseGradient = ph2_amp;
   PVM_ReadDephaseGradient = rd_ref_amp;
  
          
}



void repetitionTimeRels( void )
/* -----------------------------------------------------------
   calculate PVM_MinRepetitionTime and constrain PVM_RepetitionTime
   ---------------------------------------------------------------*/ 
{
  double min_d0;

  DB_MSG(("--> repetitionTimeRels"));

  min_d0 = 0.01; 

  /** Calculates delays for FID and ECHO mode **/

   if(ssfp==FID)
   {
     FID_delay = 0.02;
     ECHO_delay = PVM_RepetitionTime-PVM_ExSlicePulseLength-0.02-2*PVM_2dPhaseGradientTime-PVM_AcquisitionTime-0.02;
   }
  
   
   if(RFSpoilerOnOff == On)
   {
    ECHO_delay = MIN_OF(MAX_OF(0.42,ECHO_delay),10.02);  
   }

   if(ssfp==ECHO)
   {
    FID_delay = PVM_RepetitionTime-PVM_ExSlicePulseLength-0.02-2*PVM_2dPhaseGradientTime-PVM_AcquisitionTime-0.02;
    ECHO_delay = 0.02;
   }

   FID_delay = MIN_OF(MAX_OF(0.02,FID_delay),10.02); 
   ECHO_delay = MIN_OF(MAX_OF(0.02,ECHO_delay),10.02);
 
   if(ssfp==TRUE_FISP)
   {
     FID_delay=0.02;
     ECHO_delay=0.02;
   }
   PVM_MinRepetitionTime = PVM_ExSlicePulseLength+0.02+2*PVM_2dPhaseGradientTime+PVM_AcquisitionTime+0.04;
   PVM_RepetitionTime = PVM_MinRepetitionTime + FID_delay + ECHO_delay - 0.040;
   PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);
  
  DB_MSG(("<-- repetitionTimeRels"));
} /* end of repetitionTimeRels() */



void localTimeInfoRels(void)                                     /* kws */
/* ----------------------------------------------------------------------
   calculates Seg_time, Seg_Sep_time, Seg_Sep_Fra_time, Total_scan_time 
   ---------------------------------------------------------------------- */ 
{
  int dim, nSlices;
  double frameloop;
  double amplifierenable;

  DB_MSG(("--> localTimeInfoRels"));

  SSFPPrepTimeRel();

  amplifierenable = CFG_AmplifierEnable();

  Seg_time = PVM_RepetitionTime*PVM_EncMatrix[1]/Nsegments;

  Seg_Sep_time = Seg_time * (1 + Const_separation);


  if(sep_mode == linear)
  {
    ParxRelsHideInEditor("Log_separation");
    ParxRelsShowInEditor("Const_separation");

    SeparationEchoes = (int)(Const_separation * PVM_EncMatrix[1]/Nsegments);
    Const_separation = SeparationEchoes/(PVM_EncMatrix[1]/Nsegments);
    Seg_Sep_time = Seg_time * (1 + Const_separation);
    if(SeparationEchoes>0)
      PVM_ppgFlag1 = Yes;
    else
      PVM_ppgFlag1 = No;
 
  }
  else /* log separation not implemented */
  {
    SeparationEchoes = 0;
    Const_separation = 0.0;
    PVM_ppgFlag1 = No;
  }
  


  Seg_Sep_Fra_time = Seg_Sep_time * PVM_NMovieFrames;

  /* Inversion Delay */
 
  minInversionTime = InvPulse.Length/2 
    + Prescan_Delay                               /* approach to ss  */
    + (PVM_EncCentralStep1 -1)*PVM_RepetitionTime /* del. to k=0     */
    + 0.01;                                       /* min d16         */


  if(FISP_inversion_enable == Yes)
  {
     ParxRelsShowInEditor("PVM_InversionTime");
     ParxRelsShowInEditor("InvPulseEnum");
     ParxRelsShowInEditor("InvPulse");
     PVM_InversionTime = MAX_OF(PVM_InversionTime, minInversionTime);
     PVM_InversionTime = MIN_OF(PVM_InversionTime,100000);
     Inv_Rep_time = MAX_OF((PVM_InversionTime+amplifierenable+InvPulse.Length/2+0.02+PVM_RiseTime+Seg_Sep_Fra_time-(PVM_EncCentralStep1 -1)*PVM_RepetitionTime),Inv_Rep_time);
     Inv_Rep_time = MIN_OF(100000,Inv_Rep_time);
     ParxRelsMakeEditable("Inv_Rep_time");
     Scan_RepetitionTime=Inv_Rep_time;
     ParxRelsMakeNonEditable("Scan_RepetitionTime");
  }
  else
  {
     ParxRelsHideInEditor("PVM_InversionTime");
     ParxRelsHideInEditor("InvPulseEnum");
     ParxRelsHideInEditor("InvPulse");
     PVM_InversionTime = 0.0;
     Inv_Rep_time = MAX_OF((Prescan_Delay+Seg_Sep_Fra_time+0.02),Inv_Rep_time);
     Inv_Rep_time = MIN_OF(100000,Inv_Rep_time);
     ParxRelsMakeNonEditable("Inv_Rep_time");
     Scan_RepetitionTime = MAX_OF((Prescan_Delay+Seg_Sep_Fra_time+0.02),Scan_RepetitionTime); 
     Scan_RepetitionTime = MIN_OF(100000,Scan_RepetitionTime);
     Inv_Rep_time=Scan_RepetitionTime;
     ParxRelsMakeEditable("Scan_RepetitionTime");
  }


 /*  Scan_RepetitionTime = Inv_Rep_time */
  frameloop = Inv_Rep_time;

  /** Calculate Total Scan Time and Set for Scan Editor **/ 
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  dim = PTB_GetSpatDim();
  if(dim == 3)
    Total_scan_time = frameloop * PVM_NAverages * Nsegments * 
      nSlices * NumSatFrqTotal * PVM_NRepetitions * PVM_EncMatrix[2]; /*Added by Marty Pagel */
  else
    Total_scan_time = frameloop * PVM_NAverages * Nsegments * 
      nSlices * NumSatFrqTotal * PVM_NRepetitions;                    /*Added by Marty Pagel */

  /* time for one repetition */
  OneRepTime = Total_scan_time/(PVM_NRepetitions*1000.0);

  UT_ScanTimeStr(PVM_ScanTimeStr,Total_scan_time); 
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- localTimeInfoRels"));
}



void LocalGradientStrengthRels( void )
/* ---------------------------------------------------------
   Calculates PVM_ReadGradient and PVM_ExSliceGradient
   --------------------------------------------------------*/
{

  /* simplified version - only readout amp and slice amp; rest from TE*/
  PVM_ReadGradient = MRT_ReadGrad( PVM_EffSWh,PVM_Fov[0],PVM_GradCalConst );
  PVM_ExSliceGradient = MRT_SliceGrad( PVM_ExSliceBandWidth,
				       PVM_SliceThick,
				       PVM_GradCalConst );     
}


void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;
 
  DB_MSG(("-->LocalFrequencyOffsetRels\n"));
 
  spatDim = PTB_GetSpatDim();
 
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);
 
  /*
   * Calculate offset in read direction
   */
  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          PVM_ReadGradient,
                          PVM_GradCalConst,
                          PVM_ReadOffsetHz );
 
  /*
   * Calculate slice offset
   */
 
  MRT_FrequencyOffsetList(nslices,
                          PVM_EffSliceOffset,
                          PVM_ExSliceGradient,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz );
 
  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }
 
  DB_MSG(("<--LocalFrequencyOffsetRels\n"));
 
}



void LocalInplaneGeoHandler(void)
/* -----------------------------------------------------------------
   redirected relation of PVM_InplaneGeometryHandler
   ----------------------------------------------------------------*/
{

  DB_MSG(("-->LocalInplaneGeoHandler\n"));

  /*  constrain fov to the minima calculated in the last
      call of backbone: */
     
  STB_StandardInplaneGeoParHandler(Local_MinFov,2.0);
  
  if( PVM_ErrorDetected == Yes ){
    UT_ReportError("LocalInplaneGeoHandler: In function call!");
    return;
  }

  backbone();
  DB_MSG(("<--LocalInplaneGeoHandler\n"));
}


/*===========================================================
 * The update of slice geometry is done in backbone.
 * This function connects the Fov in 3rd spatial dimension
 * to the slice thickness.
 *==========================================================*/


void localHandleSliceGeometry(void)
{
  DB_MSG(("-->localHandleSliceGeometry\n"));

  if(PTB_GetSpatDim()==3)
  {

    STB_IsotropicRange();

    switch(PVM_Isotropic)
    {
      case Isotropic_Fov:
      case Isotropic_All:
	PVM_Fov[0] = 
	PVM_Fov[1] = 
	PVM_Fov[2] = PVM_SliceThick;
	break;
      default:
	PVM_Fov[2] = PVM_SliceThick;
	break;
    }
  }

  backbone();


  DB_MSG(("<--localHandleSliceGeometry\n"));
  return;
}


/*--------------------------------------------------------------
  ExcPulseAngleRelation
  Redirected relation of PVM_ExcPulseAngle
  -------------------------------------------------------------*/
void ExcPulseAngleRelation(void)
{
  DB_MSG(("-->ExcPulseAngleRelation\n"));
  ExcPulse.FlipAngle = PVM_ExcPulseAngle;
  ExcPulseRange();
  backbone();
  DB_MSG(("<--ExcPulseAngleRelation\n"));
}


/* --------------------------------------------------------------
   ExcPulseEnumRelation
   Relation of ExcPulseEnum (a dynamic enmueration parameter which
   allows to select one of the existing library exc. pulses)
   Sets the name and the clasification  of the pulse perameter ExcPulse 
   according to the selected enum value.
   --------------------------------------------------------------*/
void ExcPulseEnumRelation(void)
{
  YesNo status;
  DB_MSG(("-->ExcPulsesEnumRelation\n"));
  
  /* set the name and clasification of ExcPulse: */
  status = STB_UpdateExcPulseName("ExcPulseEnum",
				  &ExcPulseEnum,
				  ExcPulse.Filename,
				  &ExcPulse.Classification);

  /* exc pulse changed - check it */
  ExcPulseRange();

  /* call the method relations */
  backbone();

  DB_MSG(("<--ExcPulseEnumRelation status = %s\n",
	  status == Yes? "Yes":"No"));
}

/* Added by Marty Pagel -------------------------
   SatPulseEnumRelation
   Relation of SatPulseEnum (a dynamic enmueration parameter which
   allows to select one of the existing library exc. pulses)
   Sets the name and the clasification  of the pulse perameter SatPulse
   according to the selected enum value.
   --------------------------------------------------------------*/
void SatPulseEnumRelation(void)                                      	/* Added by Marty Pagel */
{
  YesNo status;                                                         /* Added by Marty Pagel */
  DB_MSG(("-->SatPulsesEnumRelation\n"));                            	/* Added by Marty Pagel */

  /* set the name and clasification of SatPulse: */                  	/* Added by Marty Pagel */
  status = STB_UpdateExcPulseName("SatPulseEnum",                    	/* Added by Marty Pagel */
                                  &SatPulseEnum,                     	/* Added by Marty Pagel */
                                  SatPulse.Filename,                 	/* Added by Marty Pagel */
                                  &SatPulse.Classification);         	/* Added by Marty Pagel */

  /* exc pulse changed - check it */                                    /* Added by Marty Pagel */
  SatPulseRange();                                                   	/* Added by Marty Pagel */

  /* call the method relations */                                       /* Added by Marty Pagel */
  backbone();                                                           /* Added by Marty Pagel */

  DB_MSG(("<--SatPulseEnumRelation status = %s\n",                   	/* Added by Marty Pagel */
          status == Yes? "Yes":"No"));                                  /* Added by Marty Pagel */
}                                                                       /* Added by Marty Pagel */

/* -----------------------------------------------------------
   Relation of ExcPulse 
 
   All pulses of type PVM_RF_PULSE_TYPE must have relations like this.
   However, if you clone this funtion for a different pulse parameter
   remember to replace the param name in the call to UT_SetRequest!

   IMPORTANT: this function should not be invoked in the backbone!
   -----------------------------------------------------------*/
void ExcPulseRelation(void)
{
  DB_MSG(("-->ExcPulseRelation\n"));

  /* Tell the request handling system that the parameter
     ExcPulse has been edited */
  UT_SetRequest("ExcPulse");

  /* Check the values of ExcPulse */
  ExcPulseRange();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */
 
  backbone();

  DB_MSG(("-->ExcPulseRelation\n"));
}

/* Added by Marty Pagel ------------   RELATION OF SAT Pulse*/
void SatPulseRelation(void)                          /* Added by Marty Pagel */
{
  DB_MSG(("-->SatPulseRelation\n"));                 /* Added by Marty Pagel */
  UT_SetRequest("SatPulse");                         /* Added by Marty Pagel */
  SatPulseRange();                                   /* Added by Marty Pagel */
  backbone();                                        /* Added by Marty Pagel */
  DB_MSG(("-->SatPulseRelation\n"));                 /* Added by Marty Pagel */
}

void ExcPulseRange(void)
{
  DB_MSG(("-->ExcPulseRange\n"));
  
  /* allowed clasification */

  switch(ExcPulse.Classification)
  {
  default:
    ExcPulse.Classification = LIB_EXCITATION;
    break;
  case LIB_EXCITATION:
  case PVM_EXCITATION:
  case USER_PULSE:
    break;
  }

  /* allowed angle for this pulse */
 
  ExcPulse.FlipAngle = MIN_OF(90.0,ExcPulse.FlipAngle);


  /* general verifiation of all pulse attributes  */

  STB_CheckRFPulse(&ExcPulse);

  DB_MSG(("<--ExcPulseRange\n"));

}

void SatPulseRange(void)                     	/* Added by Marty Pagel*/
{                                               /* Added by Marty Pagel*/

  DB_MSG(("-->SatPulseRange\n"));            	/* Added by Marty Pagel*/

  switch(SatPulse.Classification)            	/* Added by Marty Pagel*/
  {                                             /* Added by Marty Pagel*/
  default:                                      /* Added by Marty Pagel*/
    SatPulse.Classification = LIB_EXCITATION;  	/* Added by Marty Pagel*/
    break;                                      /* Added by Marty Pagel*/
  case LIB_EXCITATION:                          /* Added by Marty Pagel*/
  case PVM_EXCITATION:                          /* Added by Marty Pagel*/
  case USER_PULSE:                              /* Added by Marty Pagel*/
    break;                                      /* Added by Marty Pagel*/
  }                                             /* Added by Marty Pagel*/

  STB_CheckRFPulse(&SatPulse);               	/* Added by Marty Pagel*/

  DB_MSG(("<--SatPulseRange\n"));            	/* Added by Marty Pagel*/
}                                               /* Added by Marty Pagel*/



/* -----------------------------------------------------------
   Relation of InvPulse 
 
   All pulses of type PVM_RF_PULSE_TYPE must have relations like this.
   However, if you clone this function for a different pulse parameter
   remember to replace the param name in the call to UT_SetRequest!

   IMPORTANT: this function should not be invoked in the backbone!
   -----------------------------------------------------------*/
void InvPulseRelation(void)
{
  DB_MSG(("-->InvPulseRelation\n"));

  /* Tell the request handling system that the parameter
     ExcPulse has been edited */
  UT_SetRequest("InvPulse");

  /* Check the values of InvPulse */
  InvPulseRange();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */
 
  backbone();

  DB_MSG(("-->InvPulseRelation\n"));
}

void InvPulseRange(void)
{
  DB_MSG(("-->InvPulseRange\n"));
  
  /* allowed classification */

  switch(InvPulse.Classification)
  {
  default:
    InvPulse.Classification = LIB_INVERSION;
    break;
  case LIB_INVERSION:
  case PVM_INVERSION:
  case USER_PULSE:
    break;
  }

  /* allowed angle for this pulse */
 
  InvPulse.FlipAngle = MIN_OF(180.0,InvPulse.FlipAngle);
  InvPulse.FlipAngle = MAX_OF(180.0,InvPulse.FlipAngle);


  /* general verifiation of all pulse attributes  */

  STB_CheckRFPulse(&InvPulse);

  DB_MSG(("<--InvPulseRange\n"));
}

/* --------------------------------------------------------------
   InvPulseEnumRelation; 21.12.01 kws 
   Relation of InvPulseEnum (a dynamic enmueration parameter which
   allows to select one of the existing library inv. pulses)
   Sets the name and the clasification  of the pulse perameter InvPulse 
   according to the selected enum value.
   --------------------------------------------------------------*/
void InvPulseEnumRelation(void)
{
  YesNo status;
  DB_MSG(("-->InvPulsesEnumRelation\n"));
  
  /* set the name and clasification of InvPulse: */
  status = STB_UpdateInvPulseName("InvPulseEnum",
				  &InvPulseEnum,
				  InvPulse.Filename,
				  &InvPulse.Classification);

  /* inv pulse changed - check it */
  InvPulseRange();

  /* call the method relations */
  backbone();

  DB_MSG(("<--InvPulseEnumRelation status = %s\n",
	  status == Yes? "Yes":"No"));
}



void repTimeRels(void)
/* ---------------------------------------------------------------
   redirected relations of PVM_RepetitionTime 
   --------------------------------------------------------------*/
{
  /* PVM_EchoTime = PVM_RepetitionTime/2.0 - PVM_AcquisitionTime*(50.0-PVM_EchoPosition)/100.0; */
  
  if(ssfp==TRUE_FISP)
  {
    PVM_EchoTime = PVM_RepetitionTime/2.0;
  }
  backbone();
}


/*----------------------------------------------------------------------
  calculate possible copyrar factor and n. of echoes
 ---------------------------------------------------------------------*/
void SegmRels(void)
{   
 
 
  DB_MSG(("Entering SegmRels\n"));

  Const_separation = MAX_OF(Const_separation, 0);  /* kws */
  Const_separation = MIN_OF(Const_separation, 10000);

  DB_MSG(("Exiting SegmRels\n"));

} 

void LocalSWhRange(void)
{
  DB_MSG(("-->LocalSWhRange"));

  if(!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh = 100000.0;
  }
  else
  {
    PVM_EffSWh = MIN_OF(MAX_OF(2000.0,PVM_EffSWh),1000000);
  }

  DB_MSG(("<--LocalSWhRange"));
}

void LocalSWhRels(void)
{
  DB_MSG(("-->LocalSWhRel"));

  UT_SetRequest("PVM_EffSWh");
  LocalSWhRange();
  backbone();

  DB_MSG(("<--LocalSWhRel"));
  return;
}

void LocalEchoTime1Relation(void)
{
  DB_MSG(("-->LocalEchoTime1Relation\n"));
 
  PVM_EchoTime = PVM_EchoTime1;
  backbone();
 
  DB_MSG(("<--LocalEchoTime1Relation\n"));
}


void MyEchoPosRange(void)
{
  DB_MSG(("-->MyEchoPosRange"));
  if(!ParxRelsParHasValue("PVM_EchoPosition"))
  {
    PVM_EchoPosition=50.0;
  }
  else
  {
    double dval;
    
    dval = PVM_EchoPosition;
    PVM_EchoPosition = MIN_OF(MAX_OF(dval,0),50);
  }


  DB_MSG(("<--MyEchoPosRange"));
}


void MyEchoPosRel(void)
{
  DB_MSG(("-->MyEchoPosRel"));
  MyEchoPosRange();
  backbone();
  
  DB_MSG(("<--MyEchoPosRel"));
}

void MovieFrames_Rel(void)
{
 PVM_NMovieFrames = MAX_OF(PVM_NMovieFrames,1);
 PVM_NMovieFrames = MIN_OF(PVM_NMovieFrames,100);
 backbone();
}



void FlipAngArrRange(void)
{
  int dim,i;
  double *dval;

  if(!ParxRelsParHasValue("FlipAngArr") || PARX_get_dim("FlipAngArr",1) < 1)
  {
    PARX_change_dims("FlipAngArr",1);
  }
  else
  {
    dim = (int)PARX_get_dim("FlipAngArr",1);
    dval = FlipAngArr;
    for(i=0;i<dim;i++)
    {
      dval[i] = MAX_OF(0,dval[i]);
    }
  }

}

void FlipAngArrRel(void)
{
  FlipAngArrRange();
  backbone();

}


void InitFlipAngleArray(void)
{
  NumPrepRange();
  FlipAngArrRange();
  ParxRelsMakeNonEditable("PVM_ppgPowerList1"); 


}

void UpdateFlipAngleArray(PVM_RF_PULSE_TYPE *pulse)
{

  int i, size;
  double alphaRef,refatt;

  const char *const hideandstore = "FlipAngArr,"
                                   "PVM_ppgPowerList1";

  NumPrepRange();

  FlipAngArrRange();

  PVM_ppgPowerList1Size = NumPrep;

  if(NumPrep != (int) PARX_get_dim("PVM_ppgPowerList1",1))
  {
    ParxRelsParRelations("PVM_ppgPowerList1Size",Yes);
  }

  ParxRelsShowInFile(hideandstore);
  ParxRelsHideInEditor(hideandstore);

  if(NumPrep > 0)
  {
    size = (int) PARX_get_dim("FlipAngArr",1);

    if(NumPrep != size)
    {
      PARX_change_dims("FlipAngArr",NumPrep);
      for(i=size;i<NumPrep;i++)
      {
	FlipAngArr[i] = FlipAngArr[i-1];
      }
    }
  }

  size = NumPrep;


  alphaRef = ExcPulse.FlipAngle;
   
  refatt = pulse->Attenuation;

  for(i=0;i<size;i++)
  {
    FlipAngArr[i]        = alphaRef*(2*i+1)/(2*size+1);
    PVM_ppgPowerList1[i] = refatt + 20*log10(alphaRef/FlipAngArr[i]); 
  }


}

void NumPrepRange(void)
{
  DB_MSG(("-->NumPrepRange"));
  if (!ParxRelsParHasValue("NumPrep"))
  {
    NumPrep = 8;
  }
  else
  {
    NumPrep = MAX_OF(0,NumPrep);
  }
  DB_MSG(("<--NumPrepRange: %d", NumPrep));

}

void NumPrepRel(void)
{
  NumPrepRange();
  backbone();
}

void SSFPPrepRange(void)
{
  DB_MSG(("-->SSFPPrepRange"));

  if(!ParxRelsParHasValue("ssfp_preparation"))
  {
    ssfp_preparation = Alpha_2;
  }

  switch(ssfp_preparation)
  {
  default:
    ssfp_preparation = Alpha_2;
  case Alpha_2:
    ParxRelsShowInEditor("Alpha2_Prep_time");
    ParxRelsHideClassInEditor("Starter_Sequence");
    ParxRelsShowInEditor("DummyEchoes");
    break;
  case No_Preparation:
    ParxRelsHideClassInEditor("Starter_Sequence");
    ParxRelsShowInEditor("DummyEchoes");
    ParxRelsHideInEditor("Alpha2_Prep_time");
    break;

  case Starter_Sequence:
    ParxRelsShowClassInEditor("Starter_Sequence");
    ParxRelsHideInEditor("DummyEchoes");
    ParxRelsHideInEditor("Alpha2_Prep_time");
    DummyEchoes=0;
     break;
  }

  DB_MSG(("<--SSFPPrepRange"));
}

void SSFPPrepRel(void)
{
  DB_MSG(("-->SSFPPrepRel"));

  SSFPPrepRange();
  backbone();

  DB_MSG(("<--SSFPPrepRel"));
}

void SSFPPrepTimeRel(void)
{
  DB_MSG(("-->SSFPPrepTimeRel"));

  switch(ssfp_preparation)
  {
  default:
  case Alpha_2:
    Prescan_Delay = Spoiler_duration + Alpha2_Prep_time * PVM_RepetitionTime + DummyEchoes*PVM_RepetitionTime;
    break;
  case Starter_Sequence:
    Prescan_Delay = Spoiler_duration + PVM_RepetitionTime + 4.0 + NumPrep*PVM_RepetitionTime + 2.0*PVM_RiseTime;
    break;
  case No_Preparation:
    Prescan_Delay = Spoiler_duration + DummyEchoes*PVM_RepetitionTime;
    break;
  }

  DB_MSG(("<--SSFPPrepTimeRel"));
}

void Local_MovieOnOffRange(void)
{

  /* Range check of PVM_MovieOnOff */
  if(ParxRelsParHasValue("PVM_MovieOnOff") == No)
  { 
    PVM_MovieOnOff = Off;
  }
  
}

void Local_MovieOnOffRels(void)
{
  Local_MovieOnOffRange();
  backbone();
}

void Local_UpdateMovie(void)
{
  if(PVM_MovieOnOff == Off)
  {
    PVM_NMovieFrames = 1;
    ParxRelsHideClassInEditor("Movie");
  }
  else
  {
    ParxRelsShowClassInEditor("Movie");
  }
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


