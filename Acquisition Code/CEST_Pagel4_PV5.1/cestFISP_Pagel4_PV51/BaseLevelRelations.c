/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/BaseLevelRelations.c,v $
 * Then modified by Marty Pagel
 *
 * Copyright (c) 1999-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: BaseLevelRelations.c,v 1.33.2.6 2010/03/31 09:28:26 dgem Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: BaseLevelRelations.c,v 1.33.2.6 2010/03/31 09:28:26 dgem Exp $ (C) 1995-2009 Bruker BioSpin MRI GmbH ";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


#include "method.h"

extern double minInversionTime; /* defined in parsRelations.c */

void SetBaseLevelParam( void )
{
 
  DB_MSG(("---> SetBaseLevelParam()"));


  SetBasicParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: Error in function call!");
    return;
  }
  
 SetFrequencyParameters();

 if( PVM_ErrorDetected == Yes )
 {
   UT_ReportError("SetBaseLevelParam: In function call!");
   return;
 }
 
 SetPpgParameters();

 if( PVM_ErrorDetected == Yes )
 {
   UT_ReportError("SetBaseLevelParam: In function call!");
   return;
 }

 SetGradientParameters();
 
 if( PVM_ErrorDetected == Yes )
 {
   UT_ReportError("SetBaseLevelParam: In function call!");
   return;
 }

 SetInfoParameters();

 if( PVM_ErrorDetected == Yes )
 {
   UT_ReportError("SetBaseLevelParam: In function call!");
   return;
 }

 SetMachineParameters();

 if( PVM_ErrorDetected == Yes )
 {
   UT_ReportError("SetBaseLevelParam: In function call!");
   return;
 }


  /* ------------------------------------------------------------------------
     Sets parameters needed for multi-receiver acq. Overrides some previously
     set parameters such as NUCn Must be called at the end of
     SetBaseLevel.
     ----------------------------------------------------------------------- */

/* Next section modified by Marty Pagel */
 if(Yes==ATB_SetMultiRec())
 {
   ATB_SetPulprog("cestFISP_Pagel4.4ch");
 }


 ATB_SetTriggerBaseLevel();

 DB_MSG(("<--- SetBaseLevelParam"));


}


void SetBasicParameters( void )
{
  int spatDim, specDim;
  int nSlices;
  int dim ;                    /* 09.04. kws */

  DB_MSG(("Entering SetBasicParameters()"));
    
  /* ACQ_dim */

  spatDim = PTB_GetSpatDim();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }

  specDim = PTB_GetSpecDim();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  ACQ_dim = spatDim + specDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  
  ATB_SetAcqDimDesc( specDim, spatDim, NULL );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* ACQ_size */
  /* With the Encoding group, this call
     ATB_SetAcqSize( Spatial, spatDim, PVM_Matrix, PVM_AntiAlias, No );
     is replaced by: */
  ATB_SetAcqSize( Spatial, spatDim, PVM_EncMatrix, NULL, No ); 
 
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NSLICES */
  
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  ATB_SetNSlices( nSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NR - counts repetitions */ /* 25.2.  kws */

  ATB_SetNR( PVM_NRepetitions );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }

  ATB_SetNR( NumSatFrqTotal * PVM_NRepetitions );          /* Added by Marty Pagel */
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }

  /* NI - counts moovie frames and slices */ /* 25.2.  kws*/

  ATB_SetNI( PVM_NMovieFrames * nSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }


  /* NA */

  ATB_SetNA( 1 ); /* 25.2. kws */
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }


  /* NAE counts averages of experiments */

  ATB_SetNAE( PVM_NAverages ); /* 25.2. kws */
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  


  /* ACQ_ns */

  ACQ_ns_list_size = PVM_NEchoImages;
  
  dim = PARX_get_dim("ACQ_ns_list",1);
  if( dim != 1 )
  {
    PARX_change_dims( "ACQ_ns_list",1 );
  }
  
  NS = 1;
  ACQ_ns = NS;
  ACQ_ns_list[0] = ACQ_ns;

  ParxRelsParRelations("ACQ_ns",Yes);


  /* NECHOES */

  NECHOES = PVM_NMovieFrames;        /* 25.2. kws */


  /* ACQ_obj_order */

  PARX_change_dims("ACQ_obj_order",NI);

  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NMovieFrames, 1 );
   

  /* DS */

  DS =0 ;
  ACQ_DS_enabled = Yes;

    
  ATB_DisableAcqUserFilter();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  ATB_SetAcqScanSize( One_scan );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }


  DB_MSG(("Exiting SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
  int nslices, i, *sort, ssfpsize;
  double d1,d9, freq1, freq2, cyc, cyc2, cyc_frac, cyc_frac2;
  double *O2_list, *O3_list;

  DB_MSG(("Entering SetFrequencyParameters()"));


  ATB_SetNuc1(PVM_Nucleus1);
   
  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");

  ATB_SetNucleus(PVM_Nucleus1);

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetDigPars();
  

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }


  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode",Yes);

  ACQ_O2_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O2_mode",Yes);

  ACQ_O3_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O3_mode",Yes);

  O1 = 0.0;
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );
  
  
  /* delay used for phase advance in ppg in a TR */
  d1 = (PVM_2dPhaseGradientTime - PVM_RiseTime - 0.015) / 1000.0; 
  /* delay used for phase advance in ppg after alpha/2 */
  d9 = (D[9]-1e-5); 
  
 
  if(RFSpoilerOnOff != On)
  {
    O2_list = (double *)(malloc(nslices*sizeof(double)));
    O3_list = (double *)(malloc(nslices*sizeof(double)));
    sort =    (int *)(malloc(nslices*sizeof(int)));
    
    for( i=0; i<nslices; i++ )
    {
      freq1 = ACQ_O1_list[i];
      /* RF phase rotation between alpha pulses, in cycles */
      cyc = freq1*1e-3*PVM_RepetitionTime;
      cyc_frac = cyc - floor(cyc); 
      
      /* RF phase rotation between alpha/2 and alpha, in cycles */
      cyc2 = freq1*1e-3*PVM_RepetitionTime*Alpha2_Prep_time;
      cyc_frac2 = cyc2 - floor(cyc2); 
      
      freq2 = freq1 + (PhaseAdvance/360.0 - cyc_frac) / d1; /*30.01.*/
      O2_list[i] = freq2;
      
      
      freq2 = freq1 + (PhaseAdvance/360.0 - cyc_frac2) / d9; 
      O3_list[i] = freq2;
      sort[i] =i;
    }
    ATB_SetAcqO2List( nslices, sort, O2_list );
    ATB_SetAcqO3List( nslices, sort, O3_list );
    free(O2_list);
    free(O3_list);
    free(sort);
  }
  else
  {
    /* ssfpsize = L[6]+(L[4]+L[1])*L[3]; */

    ssfpsize = (int)(DummyEchoes + (PVM_Matrix[1]*PVM_AntiAlias[1]/Nsegments + SeparationEchoes)*PVM_NMovieFrames); 
    ACQ_O2_list_size = ssfpsize;
    ParxRelsParRelations("ACQ_O2_list_size", Yes);
    MRT_RFSpoilFreqList(117,ssfpsize,ACQ_O2_list,d1*1000);

    ACQ_O3_list_size = 1;
    ParxRelsParRelations("ACQ_O3_list_size", Yes);
    ACQ_O3_list[0] = 0.0;
  }
  

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);
  
  
  
  ATB_SetRouting();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  

  
  DB_MSG(("Exiting SetFrequencyParameters()"));
}

void SetGradientParameters( void )
{
  int spatDim, dim;

  /* int  j, k;
  double mi, phase_si, phase_di, phase_val; */ 

  DB_MSG(("Entering SetGradientParameters()"));
  
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetGradientParameters: In function call!");
      return;
    }


 spatDim = PTB_GetSpatDim();
  
  dim = PARX_get_dim("ACQ_phase_encoding_mode", 1 );
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );
  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2] = -1; /* set, but no used */
      ACQ_spatial_size_2 = PVM_EncMatrix[2];
      ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncValues2");
      /* no break */
    case 2:
      ACQ_phase_encoding_mode[1] = User_Defined_Encoding;;
      ACQ_phase_enc_start[1] = -1.0; /* set, but no used */
      ACQ_spatial_size_1 = PVM_EncMatrix[1];
      ParxRelsCopyPar("ACQ_spatial_phase_1","PVM_EncValues1");
      /* no break */
    default:
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0] = -1;
  }



  ATB_SetAcqPhaseFactor(PVM_RareFactor );  
    
  ACQ_rare_factor = PVM_RareFactor;       

  
   
  ATB_SetAcqGradMatrix( PVM_NSPacks, PVM_SPackArrNSlices,
			PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_ObjOrderList );
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;


  ACQ_grad_str_X = 0.0;
  ACQ_grad_str_Y = 0.0;
  ACQ_grad_str_Z = 0.0;


  strcpy(GRDPROG, "");
 
  ATB_SetAcqTrims(12,
	    PVM_ExSliceGradient,        /* 0 */
	    (ssfp==ECHO? -1.0:1.0) * PVM_ExSliceRephaseGradient, /* 1 */
	    (ssfp==ECHO? -1.0:1.0) * PVM_ReadDephaseGradient,    /* 2 */
	    PVM_2dPhaseGradient,        /* 3 */
	    -PVM_3dPhaseGradient,        /* 4 */
	    PVM_ReadGradient,           /* 5 */
	    0.0,                        /* 6 */
	    -PVM_2dPhaseGradient,       /* 7 */
	    PVM_3dPhaseGradient,       /* 8 */
	    (ssfp==FID? -1.0:1.0) * PVM_ReadDephaseGradient,     /* 9  */
	    (ssfp==FID? -1.0:1.0) * PVM_ExSliceRephaseGradient, /* 10 */
	    PVM_Alpha2SliceRephGradient);                    /* 11 */
     

    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetGradientParameters: In function call!");
      return;
    }

  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  int slices, i, spatDim, nrep;

  DB_MSG(("Entering SetInfoParameters()"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  spatDim = PTB_GetSpatDim();
  nrep = PVM_NRepetitions;

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }
  
  ATB_SetAcqMethod();
  
  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }
  
  ACQ_flip_angle = PVM_ExcPulseAngle;

  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - PVM_ExSlicePulseLength;

  /* calculation of ACQ_time_points */
  PARX_change_dims("ACQ_time_points",nrep);
  ACQ_time_points[0] = 0;
  for(i=1; i<nrep; i++)
    ACQ_time_points[i] = OneRepTime * i; 

  if(FISP_inversion_enable == Yes)
  {
    PARX_change_dims("ACQ_inversion_time",PVM_NMovieFrames);
    ACQ_inversion_time[0] = PVM_InversionTime;
    for( i=1; i < PVM_NMovieFrames; i++ )
    {
      ACQ_inversion_time[i] = PVM_InversionTime + i * Seg_Sep_time;
    }
  }
  else
    ParxRelsResetPar("ACQ_inversion_time");
  
  strcpy(ACQ_fit_function_name,FitFunctionName);
  if( ParxRelsVisibleForEdit("ProcessingMacro") == 1)
  {
  GO_use_macro = ProcessingMacro;
  ParxRelsParRelations("GO_use_macro", Yes);
  if(ProcessingMacro == Yes)
  strcpy(GO_macro,ProcessingMacroName);
  } 
  else
  {
  GO_use_macro = No;
  ParxRelsParRelations("GO_use_macro", Yes);
  }

  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }

  ParxRelsCopyPar("ACQ_slice_offset","PVM_SliceOffset");
  ParxRelsCopyPar("ACQ_read_offset","PVM_ReadOffset");
  ParxRelsCopyPar("ACQ_phase1_offset","PVM_Phase1Offset");

  if( spatDim == 3 )
  {
    ParxRelsCopyPar("ACQ_phase2_offset","PVM_Phase2Offset");  
  }
  
  ParxRelsShowInFile("ACQ_slice_offset,ACQ_read_offset,"
		     "ACQ_phase1_offset,ACQ_phase2_offset");

  ACQ_read_ext = (int)PVM_AntiAlias[0];

  PARX_change_dims("ACQ_slice_sepn", slices==1 ? 1 : slices-1);

  if( slices == 1 )
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for( i=1; i<slices;i++ )
    {
      ACQ_slice_sepn[i-1]=PVM_SliceOffset[i]-PVM_SliceOffset[i-1];
    }
  }

  ATB_SetAcqSliceSepn( PVM_SPackArrSliceDistance,
                       PVM_NSPacks );
 

  
  ATB_SetAcqPatientPosition();
  
  ATB_SetAcqExpType( Imaging );
  
  ACQ_n_t1_points = 1;
  
  if( ParxRelsParHasValue("ACQ_transmitter_coil") == No )
  {
    ACQ_transmitter_coil[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_contrast_agent") == No )
  {
      ACQ_contrast_agent[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_contrast") == No )
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }

  ParxRelsParRelations("ACQ_contrast_agent",Yes);

  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  ACQ_flipback = No;

  /* frames come inside the slice loop - they need to be declared
     as echo images: */
  ACQ_n_echo_images = PVM_NMovieFrames;
  if(!((PVM_MovieOnOff==Off)&&(FISP_inversion_enable == No))) 
  {
    PARX_change_dims("ACQ_echo_descr", PVM_NMovieFrames, 20);
    for(i=0; i<PVM_NMovieFrames; i++)
    {
      if(FISP_inversion_enable == No)
      {
        snprintf(ACQ_echo_descr[i],20, "frame %d",i+1 );
      }
      else
      {
        int inversiontime,j;
        inversiontime = (int)floor(ACQ_inversion_time[i]);
        j=i+1;
        snprintf(ACQ_echo_descr[i],20, "frame %d TI=%dms",j , inversiontime);
      }
    }
  }

  ACQ_n_movie_frames = NumSatFrqTotal;    /* Added by Marty Pagel start of edits */
  PARX_change_dims("ACQ_movie_descr",NumSatFrqTotal,20);

  for(i=0;i<NumSatFrqTotal;i++)
  {
    sprintf(ACQ_movie_descr[i],"VCEST=%8.1fHz",SatFrqList[i]);  /* Added VCEST (change from VTR) by Marty Pagel end of edits */
  }
/*  ACQ_n_movie_frames = 1;*/

  DB_MSG(("Exiting SetInfoParameters()"));
  
}

void SetMachineParameters( void )
{
  DB_MSG(("Entering SetMachineParameters()"));

  
  if( ParxRelsParHasValue("ACQ_word_size") == No )
  {
    ACQ_word_size = _32_BIT;
  }

  /* DIGMOD AQ_mod DSPFIRM set in frequency parameter above */


  DEOSC = (PVM_AcquisitionTime + PVM_2dPhaseGradientTime+0.01 - PVM_RiseTime)*1000.0;
  ACQ_scan_shift = -1;
  ParxRelsParRelations("ACQ_scan_shift",Yes);

  DE = DE < 6.0 ? 6.0: DE;


  PAPS = QP;

  ACQ_BF_enable = Yes;

  DB_MSG(("Exiting SetMachineParameters"));
}

void SetPpgParameters( void )
{
  /* double l1;  */
  int dim;

  DB_MSG(("Entering SetPpgParameters()"));

  if( ParxRelsParHasValue("ACQ_trigger_enable") == No )
  {
    ACQ_trigger_enable = No;
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_reference") == No )
  {
    ACQ_trigger_reference[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_delay") == No )
  {
    ACQ_trigger_delay = 0;
  }
  
  ParxRelsParRelations("ACQ_trigger_reference",Yes);


  ACQ_vd_list_size=1;
  PARX_change_dims("ACQ_vd_list",1);
  ACQ_vd_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vd_list",Yes);

  ACQ_vp_list_size=1;
  PARX_change_dims("ACQ_vp_list",1);
  ACQ_vp_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vp_list",Yes);


  ATB_SetPulprog("cestFISP_Pagel4.ppg");               /* Modified by Marty Pagel */

  D[0]  = 1e-6;  /* not used */
  D[1]  = (PVM_2dPhaseGradientTime - PVM_RiseTime) / 1000.0;
  D[2]  = (PVM_ReadDephaseTime - PVM_2dPhaseGradientTime)/1000.0;
  D[3]  = 1e-6;  /* not used */
  D[4]  = PVM_RiseTime / 1000.0;
  D[5]  = PresatDelay / 1000.0;                        /* Modified by Marty Pagel */
  if(FISP_inversion_enable == Yes)
  {
    D[6]  = (Inv_Rep_time-PVM_InversionTime-CFG_AmplifierEnable()-InvPulse.Length/2-0.02-PVM_RiseTime-Seg_Sep_Fra_time+(PVM_EncCentralStep1 -1)*PVM_RepetitionTime) / 1000.0;
  }
  else
  {
    D[6]  = (Inv_Rep_time-Prescan_Delay-Seg_Sep_Fra_time-0.02) / 1000.0; 
  } 
  D[7]  = 1e-3;  /* Trigger delay fix, 25.2.2002 */
  D[8]  = CFG_AmplifierEnable()/1000.0;
  D[9]  = PVM_Alpha2Delay/1000.0;
  D[10] = Spoiler_duration/1000.0;
  D[16] = (PVM_InversionTime - minInversionTime + 0.01)/1000.0;
  D[20] = FID_delay/1000.0;
  D[21] = ECHO_delay/1000.0;  
  

 /* set shaped pulses */

  sprintf(TPQQ[0].name,ExcPulse.Filename);
  sprintf(TPQQ[1].name,ExcPulse.Filename);
  sprintf(TPQQ[3].name,ExcPulse.Filename);
  sprintf(TPQQ[4].name,InvPulse.Filename);
  sprintf(TPQQ[5].name,ExcPulse.Filename);
  sprintf(TPQQ[6].name,ExcPulse.Filename); 

  sprintf(DPQQ[11].name,SatPulse.Filename);   /* Added by Marty Pagel */

  if(PVM_DeriveGains==Yes)
  {
    TPQQ[0].power  = ExcPulse.Attenuation;        /* alpha   */
    TPQQ[3].power  = ExcPulse.Attenuation + 6.0;  /* alpha/2 */
    TPQQ[4].power  = InvPulse.Attenuation;        /* inversion */
    TPQQ[5].power  = FlipAngMzAttenuation1;
    TPQQ[6].power  = FlipAngMzAttenuation2;
    DPQQ[11].power  = SatPulse.Attenuation;  /* Added by Marty Pagel */

  }
  TPQQ[0].offset = 0.0;
  TPQQ[1].offset = 0.0;
  TPQQ[2].offset = 0.0;
  TPQQ[3].offset = 0.0;
  TPQQ[4].offset = 0.0; 
  TPQQ[5].offset = 0.0;
  TPQQ[6].offset = 0.0; 

  DPQQ[11].offset= 0.0;                         /* Added by Marty Pagel */
  DPQQ[12].offset= 0.0;                         /* Added by Marty Pagel */

  ParxRelsParRelations("TPQQ",Yes);
  ParxRelsParRelations("DPQQ",Yes);             /* Added by Marty Pagel */

  /* set duration of pulse, in this method P[0] is used          */
  P[0] = ExcPulse.Length * 1000;
  P[4] = InvPulse.Length * 1000; 
  P[27] = SatPulse.Length * 1000;		/* Added by Marty Pagel */

  ParxRelsParRelations("P",Yes);



  /* loop counters */


  L[1] = SeparationEchoes;

  
  
  L[2] = Nsegments;
  L[3] = PVM_NMovieFrames; /* 25.2. kws */                    
  L[4] = PVM_EncMatrix[1]/Nsegments;  
  
  dim = PTB_GetSpatDim();
  if(dim == 3)
    L[5] = ACQ_size[2]; 
  L[6] = DummyEchoes;
  L[7] = NumPrep;
   
  /* inversion flag */

  PVM_ppgFlag2 = No;
  if(FISP_inversion_enable == Yes)
    PVM_ppgFlag2 = Yes;
  


  if(RFSpoilerOnOff == On)
  {
     PVM_ppgMode1 = 0;
  }
  else
  {
     PVM_ppgMode1 = 1;
  }


  
  DB_MSG(("Exiting SetPpgParameters"));
}



