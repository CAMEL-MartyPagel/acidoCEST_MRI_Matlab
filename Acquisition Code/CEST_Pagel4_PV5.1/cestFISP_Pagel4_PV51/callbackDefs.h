/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/callbackDefs.h,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: callbackDefs.h,v 1.12.2.1 2007/07/26 19:27:10 mawi Exp $
 *
 ****************************************************************/

/* Encoding */
relations PVM_EncodingHandler backbone;

/* inplane geometry */
relations PVM_InplaneGeometryHandler LocalInplaneGeoHandler;
/* slice geometry: */
relations PVM_SliceGeometryHandler  localHandleSliceGeometry;

relations PVM_DigHandler       backbone;
 
/* other parameters */
relations PVM_NucleiHandler    backbone; 
relations PVM_EffSWh           LocalSWhRels;
relations PVM_DeriveGains      backbone;
relations PVM_EchoTime         backbone;
relations PVM_NAverages        backbone;
relations PVM_RepetitionTime   repTimeRels;
relations PVM_ExcPulseAngle    ExcPulseAngleRelation;
relations PVM_ExcPulse         ExcPulseAngleRelation;
relations PVM_InversionTime    backbone; 
relations PVM_NRepetitions     backbone;
relations PVM_EchoTime1        LocalEchoTime1Relation;
relations PVM_TriggerHandler   backbone;
relations PVM_EchoPosition     MyEchoPosRel;
relations PVM_NMovieFrames     MovieFrames_Rel;
relations PVM_MovieOnOff       Local_MovieOnOffRels;

/*
 * Redirect relation for visu creation
 */
relations VisuDerivePars        deriveVisu;

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


