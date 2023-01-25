/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/RecoRelations.c,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: RecoRelations.c,v 1.10.2.3 2007/07/02 16:08:15 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1



#include "method.h"

void SetRecoParam( void )
{
  int dim,i,size,ftSize[3];
#
  DB_MSG(("---> SetRecoParam"));

  /* default initialization of reco based on acqp pars allready set */
  
  ATB_InitDefaultReco();

  for(i=0; i<PTB_GetSpatDim(); i++)
    ftSize[i] = (int)(PVM_Matrix[i]*PVM_AntiAlias[i]);

  if(PVM_EncUseMultiRec == Yes || PVM_EncPftAccel1 > 1.0)
  {
    int k=0;

    /* select method specific reconstruction method */
    RECO_mode = USER_MODE;
    ParxRelsParRelations("RECO_mode",Yes);
    ATB_InitUserModeReco(ACQ_dim, PVM_EncMatrix, ftSize, 
			 PVM_EncSteps1, PVM_EncSteps2,
			 PVM_EncNReceivers, PVM_EncPpiAccel1, PVM_EncPpiRefLines1,
			 NI, ACQ_obj_order, ACQ_phase_factor, PVM_EchoPosition);

    /* set scaling values for phased array coils */
    for(k=0; k<PVM_EncNReceivers;k++)
      RecoScaleChan[k] = PVM_EncChanScaling[k];
  }
  
  /* set reco rotate according to phase offsets     */
 
  dim = PTB_GetSpatDim();

  /* set reco sizes and ft_mode (dim 2&3) */
  /* (dim 1 is kept as it was set by ATB_InitDefaultReco) */
  for(i=0; i<dim; i++)
  {
    size = (int)(PVM_Matrix[i]*PVM_AntiAlias[i]);
    RECO_ft_mode[i] = (size == PowerOfTwo(size)) ?  COMPLEX_FFT:COMPLEX_FT;
    RECO_ft_size[i] = size;
    RECO_size[i] = PVM_Matrix[i];
  }  
 
  ParxRelsParRelations("RECO_ft_mode",Yes);
  ParxRelsParRelations("RECO_ft_size",Yes);
  ParxRelsParRelations("RECO_size",Yes);

  
  /* set reco rotate */
  
  ATB_SetRecoRotate(PVM_EffPhase1Offset,
                    PVM_Fov[1]*PVM_AntiAlias[1],
                    NSLICES,    
                    PVM_NMovieFrames, /*PVM_NEchoImages*/
                    1) ;         /* phase1 direction*/
  
  if(dim==3)
  {
    ATB_SetRecoRotate(PVM_EffPhase2Offset,      
		      PVM_Fov[2]*PVM_AntiAlias[2],
		      NSLICES,    
		      PVM_NMovieFrames,           
		      2) ;         /* phase2 direction*/
  }
  
  
  /* set reco offset */
  
  ATB_SetRecoOffset(RECO_ft_size,
                    PVM_AntiAlias,
                    NI,             
                    dim);
  for(i=0;i<dim;i++)
    RECO_fov[i]= PVM_FovCm[i];
  
  ParxRelsParRelations("RECO_fov",Yes);
  
  ATB_SetRecoTransposition(PtrType3x3 ACQ_grad_matrix[0],
                           PVM_NSPacks,
			   NSLICES,
			   ACQ_ns_list_size,
                           ACQ_obj_order) ;
  
  
  DB_MSG(("Entering SetRecoParam"));
  
}


int PowerOfTwo(int x)
/* returns next power of two */
{

 return (1<<(int)ceil(log((double)x)/log(2.0)));
}
