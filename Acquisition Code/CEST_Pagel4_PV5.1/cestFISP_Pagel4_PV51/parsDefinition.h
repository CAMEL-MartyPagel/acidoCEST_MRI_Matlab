/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/parsDefinition.h,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsDefinition.h,v 1.9.2.1 2008/04/01 13:26:53 sako Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/


double parameter ReadGradRatio;
double parameter SliceGradRatio;
double parameter Phase3dInteg;
double parameter Phase2dInteg;
double parameter TotalPhase2Offset[];
double parameter PVM_Alpha2Delay;
double parameter PVM_Alpha2SliceRephGradient;
double parameter FID_delay;
double parameter ECHO_delay;
double parameter FlipAngMzAttenuation1;
double parameter FlipAngMzAttenuation2;
double parameter OneRepTime;

char parameter
{
  display_name "Fit Function Name:";
  relations backbone;
}FitFunctionName[32];

YesNo parameter
{
  display_name "Post Processing Macro :";
  relations backbone;
}ProcessingMacro;

YesNo parameter
{
  display_name "Do High Throughput";
  relations backbone;
}DoHTYesNo;


char parameter
{
  display_name "Post Processing Macro Name:";
  relations backbone;
}ProcessingMacroName[32];

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
} ExcPulseEnum;

PVM_RF_PULSE_TYPE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulseRelation;
} ExcPulse;

PV_PULSE_LIST parameter                /* 23.11. kws ... */
{
  display_name "Inversion Pulse Shape";
  relations    InvPulseEnumRelation;
} InvPulseEnum;

PVM_RF_PULSE_TYPE parameter
{
  display_name "Inversion Pulse";
  relations    InvPulseRelation;
} InvPulse;                            /* ... 23.11. kws */

SSFP_MODE parameter
{
  display_name "SSFP Mode";
  relations    backbone;
} ssfp;

int parameter
{
  display_name "Number of Frames";
  relations     backbone;
} Nframes;

int parameter
{
  display_name "Number of Segments";
  relations    backbone;
} Nsegments;

SEGMENTING_MODE parameter		/* kws ... */ 
{
  display_name "Segmenting Mode";
  relations    backbone;
} seg_mode;

SEPARATION_MODE parameter
{
  display_name "Separation Mode";
  relations    backbone;
} sep_mode;

double parameter              
{
  display_name "Frame Separation";
  units        "x Segment time";
  format       "%.2f";
  relations    backbone;
} Const_separation; 

double parameter             
{
  display_name "Logarithmic Separation";
  units        "x Segment time";
  format       "%.2f";
  relations    backbone;
} Log_separation; 

double parameter             
{
  display_name "RF Phase Advance";
  units        "degree";
  format       "%.2f";
  relations    backbone;
} PhaseAdvance; 

double parameter         
{
  display_name "One Frame without Separation";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_time; 

double parameter      
{
  display_name "One Frame with Separation";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_Sep_time; 

double parameter    
{
  display_name "All Frames (movie cycle)";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_Sep_Fra_time; 

double parameter    
{
  display_name "Time for Preparation";
  units        "x TR";
  format       "%.2f";
  relations    backbone;
} Alpha2_Prep_time;

double parameter    
{
  display_name "Prescan Delay";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Prescan_Delay;

double parameter    
{
  display_name "Spoiler Duration";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Spoiler_duration;

OnOff parameter
{
  display_name  "RF Spoiler";
  relations backbone;
}RFSpoilerOnOff;


double parameter   /* 09.04. kws */
{
  display_name "Scan Repetition Time";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Inv_Rep_time;

double parameter  
{
  display_name "Scan Repetition Time";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Scan_RepetitionTime;

double parameter   
{
  display_name "Total Scan Time";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Total_scan_time;		 

YesNo parameter   
{
  display_name "Inversion";
  relations    backbone;
} FISP_inversion_enable;		/* 26.11.kws */ 

int parameter
{
  display_name "Dummy Echoes";
  relations backbone;
} DummyEchoes;

int parameter
{
  relations backbone;
} SeparationEchoes;

double parameter
{
    display_name "Flip Angle for Mz";
    units "deg";
    format "%.2f";
    relations backbone;
} FlipAngMz;

int parameter
{
  display_name "Preparations for Mxy";
  relations NumPrepRel;
  format "%d";
}NumPrep;

double parameter
{
  display_name "Flip Angles";
  relations FlipAngArrRel;
  format "%f";
  units "deg";
}FlipAngArr[];

SSFPPreparation_MODE parameter
{
  display_name "SSFP Preparation Mode";
  relations SSFPPrepRel;
} ssfp_preparation;

/* PARAMETERS SPECIFIC TO CEST FISP*/

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



