/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/FISP/loadMeth.c,v $
 *
 * Copyright (c) 1999
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: loadMeth.c,v 1.3 2006/11/07 13:05:07 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: loadMeth.c,v 1.3 2006/11/07 13:05:07 sako Exp $ (C) 1999 BRUKER MEDICAL";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


#include "method.h"


/*:=MPB=:=======================================================*
 *
 * Global Function: loadMeth
 *
 * Description: This procedure is automatically called in
 *	response to a method file for this method being read.
 *
 * Error History: 
 *
 * Interface:							*/

void loadMeth
(
  const char *	className
)
     
/*:=MPE=:=======================================================*/
{
  DB_MSG(( "Entering fisp:loadMeth( %s )", className ));


  if ( (0 != className) && ( 0 == strcmp( className, "MethodClass" ))) 
  {
    
    /* DB_MSG(( "...responding to loadMeth call - I know this class" )); */
    
    /* backboneCBRels(); */
    
    /* converting the old protocol parameter Nframes to the new parameter */
    if (ParxRelsParHasValue("Nframes"))
    {
      PVM_NMovieFrames = Nframes;
    }
    initMeth();
    
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }
  
  DB_MSG(( "Exiting fisp:loadMeth( %s )", className ));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


