/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/config/proto.head,v $
 *
 * Copyright (c) 1995
 * BRUKER ANALYTISCHE MESSTECHNIK GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $State: Exp $
 *
 *******************************************************************
 */

#ifndef _P_
#	if defined(HAS_PROTO) || defined(__STDC__) || defined(__cplusplus)
#		define _P_(s) s
#	else
#		define _P_(s) ()
#	endif
#endif

/* /home/mpagel/src/PV5.1/RARE/initMeth.c */
void initMeth _P_((void));
/* /home/mpagel/src/PV5.1/RARE/loadMeth.c */
void loadMeth _P_((const char *));
/* /home/mpagel/src/PV5.1/RARE/parsRelations.c */
void backbone _P_((void));
void Local_NAveragesRange _P_((void));  /* added by Marty Pagel */
void dsRange _P_((void));  /* added by Marty Pagel */
void RefPulseRange _P_((void));  /* added by Marty Pagel */
void ConstrainGradientLimits _P_((YesNo));  /* added by Marty Pagel */
void EffSWhRange _P_((void));  /* added by Marty Pagel */
void LocalGeometryMinimaRels _P_((double *, double *));  /* added by Marty Pagel */
void echoTimeRels _P_((void));  /* added by Marty Pagel */
void SliceSegDurRels _P_((void)); /* added by Marty Pagel */
void localEchoTimeRels _P_((void));
void repetitionTimeRels _P_((void));
void localTimeInfoRels _P_((void));
void LocalGradientStrengthRels _P_((void));
void LocalFrequencyOffsetRels _P_((void));
void LocalInplaneGeoHandler _P_((void));
void localHandleSliceGeometry _P_((void));
void ExcPulseAngleRelation _P_((void));
void ExcPulseEnumRelation _P_((void));
void SatPulseEnumRelation _P_((void)); /*edited by Marty Pagel */
void ExcPulseRelation _P_((void));
void SatPulseRelation _P_((void));   /*edited by Marty Pagel */
void ExcPulseRange _P_((void));
void SatPulseRange _P_((void));  /*edited by Marty Pagel */
void InvPulseRelation _P_((void));
void InvPulseRange _P_((void));
void InvPulseEnumRelation _P_((void));
void repTimeRels _P_((void));
void SegmRels _P_((void));
void LocalSWhRange _P_((void));
void LocalSWhRels _P_((void));
void LocalEchoTime1Relation _P_((void));
void MyEchoPosRange _P_((void));
void MyEchoPosRel _P_((void));
void MovieFrames_Rel _P_((void));
void FlipAngArrRange _P_((void));
void FlipAngArrRel _P_((void));
void InitFlipAngleArray _P_((void));
void UpdateFlipAngleArray _P_((PVM_RF_PULSE_TYPE *));
void NumPrepRange _P_((void));
void NumPrepRel _P_((void));
void SSFPPrepRange _P_((void));
void SSFPPrepRel _P_((void));
void SSFPPrepTimeRel _P_((void));
void Local_MovieOnOffRange _P_((void));
void Local_MovieOnOffRels _P_((void));
void Local_UpdateMovie _P_((void));
/* /home/mpagel/src/PV5.1/RARE/BaseLevelRelations.c */
void SetBaseLevelParam _P_((void));
void SetBasicParameters _P_((void));
void SetFrequencyParameters _P_((void));
void SetGradientParameters _P_((void));
void SetInfoParameters _P_((void));
void SetMachineParameters _P_((void));
void SetPpgParameters _P_((void));
/* /home/mpagel/src/PV5.1/RARE/RecoRelations.c */
void SetRecoParam _P_((void));
int PowerOfTwo _P_((int));
/* /home/mpagel/src/PV5.1/RARE/gradTools.c */
int set_refoc _P_((double *, double *, double, double, double, double, double, double));
int set_pulse _P_((double *, double *, double, double, double, double));
/* /home/mpagel/src/PV5.1/RARE/deriveVisu.c */
void deriveVisu _P_((void));
