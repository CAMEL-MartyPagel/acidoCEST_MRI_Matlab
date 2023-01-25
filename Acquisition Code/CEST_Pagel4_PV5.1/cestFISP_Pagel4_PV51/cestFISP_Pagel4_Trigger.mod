; cestFISP_Pagel4_Trigger.mod
; 
; This Trigger mod file from Bruker was modified by Marty Pagel
; and colleagues (especially Ed Randtke and Kyle Jones) to create a
; CEST FISP MRI acquisition method.  Please contact Marty at
; mdpagel@mdanderson.org or martypagel@gmail.com for questions or sugestions.
;
; This pulse program is provided for free to the MRI research community.
; Although this pulse program has worked on all Bruker MRI scanners
; with ParaVision 5.1 that have been tested so far,
; there is no guarantee that this pulse program
; will work on all Bruker MRI scanners.
; Also, while we are collegial researchers, we may not be able to provide
; assistance due to limited time and many other priorities.
;



;--- Start of cestFISP Modification by Marty Pagel ---


d5 gatepulse 1
10u o4list:f1

mtc, 10u
        p27:sp27        ;sp27 is DPQQ[11]
        lo to mtc times NumSatPulses

if (ACQ_trigger_enable == 1)
{
  5u   ECG_STAMP_LOW
  if (CONFIG_instrument_type == Avance_III)
  {
    startgate, 5u
      p27:sp27
      30u
      if trigpl1 goto endgate
      goto startgate

    endgate, 5u   trignl1
    	     5u   GRAD_SYNC
  }
  else
  {
    startgate, 5u
      p27:sp27
      30u
      if trignl1 goto endgate
      goto startgate

    endgate, 5u   trigpl1
  }
  5u   ECG_STAMP_HIGH
  TrigD0
}


;--- End of cestFISP Modification by Marty Pagel ---

