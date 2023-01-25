;****************************************************************
;
; Copyright (c) 1998-2003
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; All Rights Reserved
;
; TriggerPerPhaseStep.mod - Trigger Module
;
;****************************************************************
;
;      PVM_TrigD0               --> PVM_EcgTriggerModuleTime = ACQ_trigger_delay
;
if (PVM_TriggerMode == per_PhaseStep)
{
if (ACQ_trigger_enable == 1)
{
  10u   ECG_STAMP_LOW
if (CONFIG_instrument_type == Avance_III)
{
  10u   trignl1
  10u   GRAD_SYNC
}
else
{
  10u   trigpl1
}
  10u   ECG_STAMP_HIGH
  TrigD0
}
}
