/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/parsTypes.h,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsTypes.h,v 1.4 2006/02/03 15:15:39 sako Exp $
 *
 *
 ****************************************************************/

#define MIN_D0 0.01

/****************************************************************/
/*	TYPEDEF's						*/
/****************************************************************/

typedef enum
{
  TRUE_FISP,
  FID,
  ECHO
} SSFP_MODE;

typedef enum   /* kws */
{
  sequential,
  interleaved 
} SEGMENTING_MODE;


typedef enum   /* kws */
{
  linear,
  logarithmic
} SEPARATION_MODE;

typedef enum
{
  Alpha_2,
  Starter_Sequence,
  No_Preparation
} SSFPPreparation_MODE;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/















