/* src/game/app/class_registry_options.inc -- the Splice option tables of the crt0_splicewrap TU: the option records
 * g_aeopid (.data 0x2AC880..0x2BAAE8), one option array per class and the per-class index g_mpcidarysplopt
 * (.rodata 0x262380..0x27A108), and HandleBasicSpliceEvent's event parameter types (0x27A108..0x27A250).
 *
 * Not compiled on its own: class_registry.cpp includes it in its data part, after the functions and before the class
 * descriptor table D_002BAAE8 (definition order is section order). Values from the ELF, written once by a converter
 * (docs/guides/tu-merging.md, "The crt0_splicewrap tables"); this file is the source now and is edited like any other.
 *
 * Layout (spliceobj.cpp's PeopidFind, brx.cpp's LoadOptionsFromBrx, sly2/eopid.h):
 *  - g_aeopid D_002AC880[1449]: one EOPID per option binding (sly2/eopid.h: otyp, grfeopid, then four 8-byte words:
 *    get +0x8, set +0x10, file/argument count +0x18, object +0x20). A word is g++ 2.95's pointer to member function
 *    {short delta; short index; pfn or short delta2} (index -1: non-virtual, the word after is the function; else
 *    the vtable index and delta2), a Splice thunk (a function pointer, then 0), or a number (byte offset, count).
 *    EOPID's unions cannot be brace-initialized past their first member, so the table uses a row of the same
 *    layout with plain words (EOPIDROW, as D_002BAAE8 uses CLASSROW) and one macro per word kind below.
 *  - D_<addr>: the option array of one class, {option id, record} sorted by option id (PeopidFind's binary search),
 *    the class's own options merged with every base class's; 199 arrays in CID order.
 *  - g_mpcidarysplopt D_00279AD0[199]: {count, array} per CID.
 * The arrays are global (extern), not static: g++ emits file-static data at the end of the TU, which would put the
 * arrays after D_00279AD0. Option tables are named constants of the first part of .rodata (".rodata.early",
 * tu-merging.md "Phase 5"). The record comment gives the option id(s) and the root-most class listing the record
 * (+n: n more classes list it). */

/* The functions the records point to, by their current symbols: an asm label per name, so the declarations neither
 * meet the prototypes of the same functions elsewhere in this file nor need them (several are class members or
 * wrappers declared under C linkage in their own namespace). The inline accessors of the headers (cm.h, lo.h, so.h,
 * xfm.h, step.h, stepplayer.h, guard.h) and the emitter.h function templates are not declared here: the records name
 * them by their C++ names (EOPID_PMFI, EOPID_PMFT below), and taking their addresses is what makes g++ emit their
 * out-of-line copies (.gnu.linkonce.t.<symbol>) in this object, the only one that needs them (tu-merging.md "Inline
 * functions and linkonce copies"). */
namespace EOPIDFN {
void AddButtonPushObject() __asm__("AddButtonPushObject");
void AddDetectorTarget() __asm__("AddDetectorTarget");
void AddSensorNoTriggerClass() __asm__("AddSensorNoTriggerClass");
void AddSensorNoTriggerObject() __asm__("AddSensorNoTriggerObject");
void AddVolbtnPushObject() __asm__("AddVolbtnPushObject");
void CidBasic() __asm__("CidBasic");
void FAsegaRetracted() __asm__("FAsegaRetracted");
void FIsCurrentJob() __asm__("FIsCurrentJob");
void FIsCurrentTask() __asm__("FIsCurrentTask");
void FIsPoActive() __asm__("FIsPoActive");
void FIsPoCur() __asm__("FIsPoCur");
void FJobComplete() __asm__("FJobComplete");
void FPausedEmitter__FP7EMITTER() __asm__("FPausedEmitter__FP7EMITTER");
void FReconTargetDone() __asm__("FReconTargetDone");
void FRelayFiring() __asm__("FRelayFiring");
void FStepplayerInJtmode() __asm__("FStepplayerInJtmode");
void FSwHandsOff() __asm__("FSwHandsOff");
void FTaskComplete() __asm__("FTaskComplete");
void GetCmRotDeg() __asm__("GetCmRotDeg");
void GetPoCtlState() __asm__("GetPoCtlState");
void GetSmaGoal() __asm__("GetSmaGoal");
void GetSmaNext() __asm__("GetSmaNext");
void GetSoDvGravity() __asm__("GetSoDvGravity");
void GetSoIceable() __asm__("GetSoIceable");
void GetSoIgnoreLocked() __asm__("GetSoIgnoreLocked");
void GetTnFocusX() __asm__("GetTnFocusX");
void GetWrWavelength() __asm__("GetWrWavelength");
void IdBootyCur() __asm__("IdBootyCur");
void MpStepplayerCur() __asm__("MpStepplayerCur");
void OidFromTaskState() __asm__("OidFromTaskState");
void OidProxyLo() __asm__("OidProxyLo");
void PdetectorGetStepguard() __asm__("PdetectorGetStepguard");
void PemitterEnsureEmitter__FP7EMITTER4ENSK() __asm__("PemitterEnsureEmitter__FP7EMITTER4ENSK");
void PpoCur() __asm__("PpoCur");
void PpoHero() __asm__("PpoHero");
void PtnfnFromTn() __asm__("PtnfnFromTn");
void PwreGetWrBend__FP2WR4ENSK() __asm__("PwreGetWrBend__FP2WR4ENSK");
void PwreGetWrCircle__FP2WR4ENSK() __asm__("PwreGetWrCircle__FP2WR4ENSK");
void RefThunkAsegAPPLY() __asm__("RefThunkAsegAPPLY");
void RefThunkAsegENSURE() __asm__("RefThunkAsegENSURE");
void RefThunkAsegFIND_LABEL() __asm__("RefThunkAsegFIND_LABEL");
void RefThunkAsegaRETRACT() __asm__("RefThunkAsegaRETRACT");
void RefThunkAsegaSEEK() __asm__("RefThunkAsegaSEEK");
void RefThunkBasicCUSTOM_BOOL_PROPERTY() __asm__("RefThunkBasicCUSTOM_BOOL_PROPERTY");
void RefThunkBasicCUSTOM_CLQ_PROPERTY() __asm__("RefThunkBasicCUSTOM_CLQ_PROPERTY");
void RefThunkBasicCUSTOM_FLOAT_PROPERTY() __asm__("RefThunkBasicCUSTOM_FLOAT_PROPERTY");
void RefThunkBasicCUSTOM_INT_PROPERTY() __asm__("RefThunkBasicCUSTOM_INT_PROPERTY");
void RefThunkBasicCUSTOM_LM_PROPERTY() __asm__("RefThunkBasicCUSTOM_LM_PROPERTY");
void RefThunkBasicCUSTOM_OID_PROPERTY() __asm__("RefThunkBasicCUSTOM_OID_PROPERTY");
void RefThunkBasicCUSTOM_VECTOR_PROPERTY() __asm__("RefThunkBasicCUSTOM_VECTOR_PROPERTY");
void RefThunkBasicDERIVED_FROMQ() __asm__("RefThunkBasicDERIVED_FROMQ");
void RefThunkBombDETONATE() __asm__("RefThunkBombDETONATE");
void RefThunkBombPRIME() __asm__("RefThunkBombPRIME");
void RefThunkButtonSET_SM_GOAL() __asm__("RefThunkButtonSET_SM_GOAL");
void RefThunkButtonTRIGGER() __asm__("RefThunkButtonTRIGGER");
void RefThunkButtonUNTRIGGER() __asm__("RefThunkButtonUNTRIGGER");
void RefThunkCmCUT() __asm__("RefThunkCmCUT");
void RefThunkCmJOLT() __asm__("RefThunkCmJOLT");
void RefThunkDetector_GetDetectorTargetStateById() __asm__("RefThunkDetector_GetDetectorTargetStateById");
void RefThunkDetector_SetDetectorTargetStateById() __asm__("RefThunkDetector_SetDetectorTargetStateById");
void RefThunkEmitterPAUSE() __asm__("RefThunkEmitterPAUSE");
void RefThunkEmitterPAUSE_FOREVER() __asm__("RefThunkEmitterPAUSE_FOREVER");
void RefThunkEmitterUNPAUSE() __asm__("RefThunkEmitterUNPAUSE");
void RefThunkExitTRIGGER() __asm__("RefThunkExitTRIGGER");
void RefThunkExplEXPLODE() __asm__("RefThunkExplEXPLODE");
void RefThunkExplEXPLODE_OVR() __asm__("RefThunkExplEXPLODE_OVR");
void RefThunkHacklockSET_SM_GOAL() __asm__("RefThunkHacklockSET_SM_GOAL");
void RefThunkHacklockTRIGGER() __asm__("RefThunkHacklockTRIGGER");
void RefThunkJobSET_SM_GOAL() __asm__("RefThunkJobSET_SM_GOAL");
void RefThunkLoADD() __asm__("RefThunkLoADD");
void RefThunkLoMATCHES_NAMEQ() __asm__("RefThunkLoMATCHES_NAMEQ");
void RefThunkLoREMOVEFn() __asm__("RefThunkLoREMOVEFn");
void RefThunkLoREMOVEFnUser() __asm__("RefThunkLoREMOVEFnUser");
void RefThunkRelayCLOCK() __asm__("RefThunkRelayCLOCK");
void RefThunkRelayCONTEXT() __asm__("RefThunkRelayCONTEXT");
void RefThunkRelayCUT() __asm__("RefThunkRelayCUT");
void RefThunkRelayEXPLODE() __asm__("RefThunkRelayEXPLODE");
void RefThunkRelayFOCUS() __asm__("RefThunkRelayFOCUS");
void RefThunkRelayJOLT() __asm__("RefThunkRelayJOLT");
void RefThunkRelayLABEL() __asm__("RefThunkRelayLABEL");
void RefThunkRelayPAUSE() __asm__("RefThunkRelayPAUSE");
void RefThunkRelaySEEK() __asm__("RefThunkRelaySEEK");
void RefThunkRelaySET_SM_GOAL() __asm__("RefThunkRelaySET_SM_GOAL");
void RefThunkRelayTRIGGER() __asm__("RefThunkRelayTRIGGER");
void RefThunkRelayTUNNEL() __asm__("RefThunkRelayTUNNEL");
void RefThunkRelayUNTRIGGER() __asm__("RefThunkRelayUNTRIGGER");
void RefThunkSmAPPLY() __asm__("RefThunkSmAPPLY");
void RefThunkSmaRETRACT() __asm__("RefThunkSmaRETRACT");
void RefThunkSmaSEEK() __asm__("RefThunkSmaSEEK");
void RefThunkSoFADE_IN() __asm__("RefThunkSoFADE_IN");
void RefThunkSoFADE_OUT() __asm__("RefThunkSoFADE_OUT");
void RefThunkSoPOINT_IN_BSP_QUICK() __asm__("RefThunkSoPOINT_IN_BSP_QUICK");
void RefThunkSo_HideSoGlobsById() __asm__("RefThunkSo_HideSoGlobsById");
void RefThunkSo_HideSoPartsById() __asm__("RefThunkSo_HideSoPartsById");
void RefThunkSo_ShowSoGlobsById() __asm__("RefThunkSo_ShowSoGlobsById");
void RefThunkSo_ShowSoPartsById() __asm__("RefThunkSo_ShowSoPartsById");
void RefThunkStepguard_GetDetectorTargetStateById() __asm__("RefThunkStepguard_GetDetectorTargetStateById");
void RefThunkStepguard_SetDetectorTargetStateById() __asm__("RefThunkStepguard_SetDetectorTargetStateById");
void RefThunkSwHANDS_OFF() __asm__("RefThunkSwHANDS_OFF");
void RefThunkSwHANDS_ON() __asm__("RefThunkSwHANDS_ON");
void RefThunkTaskSET_SM_GOAL() __asm__("RefThunkTaskSET_SM_GOAL");
void RefThunkVaultTRIGGER() __asm__("RefThunkVaultTRIGGER");
void RefThunkVolCHECK_POINT() __asm__("RefThunkVolCHECK_POINT");
void RefThunkVolbtnSET_SM_GOAL() __asm__("RefThunkVolbtnSET_SM_GOAL");
void RefThunkVolbtnTRIGGER() __asm__("RefThunkVolbtnTRIGGER");
void RefThunkVolbtnUNTRIGGER() __asm__("RefThunkVolbtnUNTRIGGER");
void RefThunkWarpSET_SM_GOAL() __asm__("RefThunkWarpSET_SM_GOAL");
void RefThunkWrADD_BEND_NOISE() __asm__("RefThunkWrADD_BEND_NOISE");
void RefThunkWrADD_BEND_WARP() __asm__("RefThunkWrADD_BEND_WARP");
void RefThunkWrADD_CIRCLE_WARP() __asm__("RefThunkWrADD_CIRCLE_WARP");
void RefThunkWrADD_SWIVEL_NOISE() __asm__("RefThunkWrADD_SWIVEL_NOISE");
void RefThunk_ActivateTask() __asm__("RefThunk_ActivateTask");
void RefThunk_AddBarrierClass() __asm__("RefThunk_AddBarrierClass");
void RefThunk_AddBarrierObject() __asm__("RefThunk_AddBarrierObject");
void RefThunk_AddBootyEntry() __asm__("RefThunk_AddBootyEntry");
void RefThunk_AddBootyEntryOid() __asm__("RefThunk_AddBootyEntryOid");
void RefThunk_AddButtonAseg() __asm__("RefThunk_AddButtonAseg");
void RefThunk_AddButtonOffAseg() __asm__("RefThunk_AddButtonOffAseg");
void RefThunk_AddDancerMove() __asm__("RefThunk_AddDancerMove");
void RefThunk_AddExitRsmg() __asm__("RefThunk_AddExitRsmg");
void RefThunk_AddHackshipOidPair() __asm__("RefThunk_AddHackshipOidPair");
void RefThunk_AddJobCompleteEvent() __asm__("RefThunk_AddJobCompleteEvent");
void RefThunk_AddLasenBeam() __asm__("RefThunk_AddLasenBeam");
void RefThunk_AddLootEntry() __asm__("RefThunk_AddLootEntry");
void RefThunk_AddLootEntryOid() __asm__("RefThunk_AddLootEntryOid");
void RefThunk_AddRelayAddObject() __asm__("RefThunk_AddRelayAddObject");
void RefThunk_AddRelayBlockBegin() __asm__("RefThunk_AddRelayBlockBegin");
void RefThunk_AddRelayBlockEnd() __asm__("RefThunk_AddRelayBlockEnd");
void RefThunk_AddRelayButtonPrompt() __asm__("RefThunk_AddRelayButtonPrompt");
void RefThunk_AddRelayCamBlendFrom() __asm__("RefThunk_AddRelayCamBlendFrom");
void RefThunk_AddRelayCamBlendTo() __asm__("RefThunk_AddRelayCamBlendTo");
void RefThunk_AddRelayClearHintBox() __asm__("RefThunk_AddRelayClearHintBox");
void RefThunk_AddRelayClearPartFlag() __asm__("RefThunk_AddRelayClearPartFlag");
void RefThunk_AddRelayClearSubtitle() __asm__("RefThunk_AddRelayClearSubtitle");
void RefThunk_AddRelayCutCamera() __asm__("RefThunk_AddRelayCutCamera");
void RefThunk_AddRelayEndJtMode() __asm__("RefThunk_AddRelayEndJtMode");
void RefThunk_AddRelayEyes() __asm__("RefThunk_AddRelayEyes");
void RefThunk_AddRelayFireRelay() __asm__("RefThunk_AddRelayFireRelay");
void RefThunk_AddRelayFollowPath() __asm__("RefThunk_AddRelayFollowPath");
void RefThunk_AddRelayGotoLabelIf() __asm__("RefThunk_AddRelayGotoLabelIf");
void RefThunk_AddRelayHandsOff() __asm__("RefThunk_AddRelayHandsOff");
void RefThunk_AddRelayHeroState() __asm__("RefThunk_AddRelayHeroState");
void RefThunk_AddRelayHidePart() __asm__("RefThunk_AddRelayHidePart");
void RefThunk_AddRelayHintBox() __asm__("RefThunk_AddRelayHintBox");
void RefThunk_AddRelayLaunchTo() __asm__("RefThunk_AddRelayLaunchTo");
void RefThunk_AddRelayLine() __asm__("RefThunk_AddRelayLine");
void RefThunk_AddRelayPipeTo() __asm__("RefThunk_AddRelayPipeTo");
void RefThunk_AddRelayPlayAseg() __asm__("RefThunk_AddRelayPlayAseg");
void RefThunk_AddRelayPlayAsegDefault() __asm__("RefThunk_AddRelayPlayAsegDefault");
void RefThunk_AddRelayRaiseAlarm() __asm__("RefThunk_AddRelayRaiseAlarm");
void RefThunk_AddRelayRemoveObject() __asm__("RefThunk_AddRelayRemoveObject");
void RefThunk_AddRelayResetStepGoal() __asm__("RefThunk_AddRelayResetStepGoal");
void RefThunk_AddRelayRespawn() __asm__("RefThunk_AddRelayRespawn");
void RefThunk_AddRelayRumble() __asm__("RefThunk_AddRelayRumble");
void RefThunk_AddRelayRunOff() __asm__("RefThunk_AddRelayRunOff");
void RefThunk_AddRelayRunOn() __asm__("RefThunk_AddRelayRunOn");
void RefThunk_AddRelaySetActor() __asm__("RefThunk_AddRelaySetActor");
void RefThunk_AddRelaySetCtlState() __asm__("RefThunk_AddRelaySetCtlState");
void RefThunk_AddRelaySetPartFlag() __asm__("RefThunk_AddRelaySetPartFlag");
void RefThunk_AddRelaySetSsgGoal() __asm__("RefThunk_AddRelaySetSsgGoal");
void RefThunk_AddRelaySetTaskState() __asm__("RefThunk_AddRelaySetTaskState");
void RefThunk_AddRelayShowPart() __asm__("RefThunk_AddRelayShowPart");
void RefThunk_AddRelaySignal() __asm__("RefThunk_AddRelaySignal");
void RefThunk_AddRelaySound() __asm__("RefThunk_AddRelaySound");
void RefThunk_AddRelayStartJtMode() __asm__("RefThunk_AddRelayStartJtMode");
void RefThunk_AddRelayStartMusic() __asm__("RefThunk_AddRelayStartMusic");
void RefThunk_AddRelayStepCmd1D() __asm__("RefThunk_AddRelayStepCmd1D");
void RefThunk_AddRelayStepCmd1E() __asm__("RefThunk_AddRelayStepCmd1E");
void RefThunk_AddRelayStepCmd1F() __asm__("RefThunk_AddRelayStepCmd1F");
void RefThunk_AddRelayStepCmd20() __asm__("RefThunk_AddRelayStepCmd20");
void RefThunk_AddRelayStepCmd21() __asm__("RefThunk_AddRelayStepCmd21");
void RefThunk_AddRelayStepCmd22() __asm__("RefThunk_AddRelayStepCmd22");
void RefThunk_AddRelayStepCmd29() __asm__("RefThunk_AddRelayStepCmd29");
void RefThunk_AddRelayStopBlock() __asm__("RefThunk_AddRelayStopBlock");
void RefThunk_AddRelayStopLine() __asm__("RefThunk_AddRelayStopLine");
void RefThunk_AddRelayStopMusic() __asm__("RefThunk_AddRelayStopMusic");
void RefThunk_AddRelayStopRelay() __asm__("RefThunk_AddRelayStopRelay");
void RefThunk_AddRelayStopSound() __asm__("RefThunk_AddRelayStopSound");
void RefThunk_AddRelayTweenAimObj() __asm__("RefThunk_AddRelayTweenAimObj");
void RefThunk_AddRelayTweenPosObj() __asm__("RefThunk_AddRelayTweenPosObj");
void RefThunk_AddRelayTweenStart() __asm__("RefThunk_AddRelayTweenStart");
void RefThunk_AddRelayTweenTarget() __asm__("RefThunk_AddRelayTweenTarget");
void RefThunk_AddRelayTweenTime() __asm__("RefThunk_AddRelayTweenTime");
void RefThunk_AddRelayWaitAlarm() __asm__("RefThunk_AddRelayWaitAlarm");
void RefThunk_AddRelayWaitBlocks() __asm__("RefThunk_AddRelayWaitBlocks");
void RefThunk_AddRelayWaitTimer() __asm__("RefThunk_AddRelayWaitTimer");
void RefThunk_AddRelayWalkTo() __asm__("RefThunk_AddRelayWalkTo");
void RefThunk_AddRelayWidgetHide() __asm__("RefThunk_AddRelayWidgetHide");
void RefThunk_AddRelayWidgetShow() __asm__("RefThunk_AddRelayWidgetShow");
void RefThunk_AddSafehouseEntry() __asm__("RefThunk_AddSafehouseEntry");
void RefThunk_AddVolbtnAseg() __asm__("RefThunk_AddVolbtnAseg");
void RefThunk_AddVolbtnOffAseg() __asm__("RefThunk_AddVolbtnOffAseg");
void RefThunk_AddWarpOidPair() __asm__("RefThunk_AddWarpOidPair");
void RefThunk_CancelNotice() __asm__("RefThunk_CancelNotice");
void RefThunk_ClearRelayAlarms() __asm__("RefThunk_ClearRelayAlarms");
void RefThunk_DtSmaToState() __asm__("RefThunk_DtSmaToState");
void RefThunk_EmitSwNoise() __asm__("RefThunk_EmitSwNoise");
void RefThunk_EnsureProxyCopy() __asm__("RefThunk_EnsureProxyCopy");
void RefThunk_EnsureRelayAlarm() __asm__("RefThunk_EnsureRelayAlarm");
void RefThunk_FCanAcquireProxy() __asm__("RefThunk_FCanAcquireProxy");
void RefThunk_FInvulnerablePo() __asm__("RefThunk_FInvulnerablePo");
void RefThunk_FRelayHasAlarm() __asm__("RefThunk_FRelayHasAlarm");
void RefThunk_FStepguardHasSgf() __asm__("RefThunk_FStepguardHasSgf");
void RefThunk_FireMissile() __asm__("RefThunk_FireMissile");
void RefThunk_ForceStepplayerLeanTarget() __asm__("RefThunk_ForceStepplayerLeanTarget");
void RefThunk_ForceTaskState() __asm__("RefThunk_ForceTaskState");
void RefThunk_GrfzpdFromSoGrfzpk() __asm__("RefThunk_GrfzpdFromSoGrfzpk");
void RefThunk_HideWidget() __asm__("RefThunk_HideWidget");
void RefThunk_IGadgetSlotFromId() __asm__("RefThunk_IGadgetSlotFromId");
void RefThunk_KCalculateDetectorTarget() __asm__("RefThunk_KCalculateDetectorTarget");
void RefThunk_LaunchSoFromLauncher() __asm__("RefThunk_LaunchSoFromLauncher");
void RefThunk_LockProxyPlo() __asm__("RefThunk_LockProxyPlo");
void RefThunk_POCTL_FButtonDown() __asm__("RefThunk_POCTL_FButtonDown");
void RefThunk_POCTL_FConsumePressed() __asm__("RefThunk_POCTL_FConsumePressed");
void RefThunk_PlayWidgetAnim() __asm__("RefThunk_PlayWidgetAnim");
void RefThunk_PloAcquireProxy() __asm__("RefThunk_PloAcquireProxy");
void RefThunk_PostNotice() __asm__("RefThunk_PostNotice");
void RefThunk_PpoClosestSw() __asm__("RefThunk_PpoClosestSw");
void RefThunk_PsaaFindSoOid() __asm__("RefThunk_PsaaFindSoOid");
void RefThunk_PsmaApplySmEx() __asm__("RefThunk_PsmaApplySmEx");
void RefThunk_PushMusic() __asm__("RefThunk_PushMusic");
void RefThunk_RemoveRelayAlarm() __asm__("RefThunk_RemoveRelayAlarm");
void RefThunk_RequestTaskState() __asm__("RefThunk_RequestTaskState");
void RefThunk_ResetJobTasks() __asm__("RefThunk_ResetJobTasks");
void RefThunk_ResetRelay() __asm__("RefThunk_ResetRelay");
void RefThunk_ResetRhythmScore() __asm__("RefThunk_ResetRhythmScore");
void RefThunk_ResumeSnd() __asm__("RefThunk_ResumeSnd");
void RefThunk_SetAsegaPriority() __asm__("RefThunk_SetAsegaPriority");
void RefThunk_SetLoParent() __asm__("RefThunk_SetLoParent");
void RefThunk_SetPoChoices() __asm__("RefThunk_SetPoChoices");
void RefThunk_SetScrollerMasterSpeeds() __asm__("RefThunk_SetScrollerMasterSpeeds");
void RefThunk_SetSmPendingApply() __asm__("RefThunk_SetSmPendingApply");
void RefThunk_SetSmaGoalIfReachable() __asm__("RefThunk_SetSmaGoalIfReachable");
void RefThunk_SetSndRange() __asm__("RefThunk_SetSndRange");
void RefThunk_SetSndmgrChannelVolume() __asm__("RefThunk_SetSndmgrChannelVolume");
void RefThunk_SetSndmgrMasterVolume() __asm__("RefThunk_SetSndmgrMasterVolume");
void RefThunk_SetStepplayerGadget() __asm__("RefThunk_SetStepplayerGadget");
void RefThunk_SetWidgetAnimPhase() __asm__("RefThunk_SetWidgetAnimPhase");
void RefThunk_ShowWidget() __asm__("RefThunk_ShowWidget");
void RefThunk_ShowWidgetArgsFromSplice() __asm__("RefThunk_ShowWidgetArgsFromSplice");
void RefThunk_StartDialog() __asm__("RefThunk_StartDialog");
void RefThunk_StartMurrayLiftGrabspot() __asm__("RefThunk_StartMurrayLiftGrabspot");
void RefThunk_StartStepplayerFollow() __asm__("RefThunk_StartStepplayerFollow");
void RefThunk_StopDialog() __asm__("RefThunk_StopDialog");
void RefThunk_StopSnd() __asm__("RefThunk_StopSnd");
void RefThunk_StopWidgetAnim() __asm__("RefThunk_StopWidgetAnim");
void RefThunk_TMusicCur() __asm__("RefThunk_TMusicCur");
void RefThunk_UnlockProxyPlo() __asm__("RefThunk_UnlockProxyPlo");
void RemoveDetectorTarget() __asm__("RemoveDetectorTarget");
void SelectBootyEntry() __asm__("SelectBootyEntry");
void SetAsegMasterSpeed() __asm__("SetAsegMasterSpeed");
void SetAsegaHandsOff() __asm__("SetAsegaHandsOff");
void SetAsegaMasterSpeed() __asm__("SetAsegaMasterSpeed");
void SetAsegaSpeed__FP5ASEGAf() __asm__("SetAsegaSpeed__FP5ASEGAf");
void SetButtonButtons() __asm__("SetButtonButtons");
void SetCmBaseFovTarget() __asm__("SetCmBaseFovTarget");
void SetCmBaseNearClip() __asm__("SetCmBaseNearClip");
void SetCmFarClip__FP2CMf() __asm__("SetCmFarClip__FP2CMf");
void SetCmFarFog__FP2CMf() __asm__("SetCmFarFog__FP2CMf");
void SetCmFov__FP2CMf() __asm__("SetCmFov__FP2CMf");
void SetCmMrdRatio__FP2CMf() __asm__("SetCmMrdRatio__FP2CMf");
void SetCmNearFog__FP2CMf() __asm__("SetCmNearFog__FP2CMf");
void SetCmPos() __asm__("SetCmPos");
void SetCmRgbaFog__FP2CMP4RGBA() __asm__("SetCmRgbaFog__FP2CMP4RGBA");
void SetCmRotDeg() __asm__("SetCmRotDeg");
void SetCmUFogMax__FP2CMf() __asm__("SetCmUFogMax__FP2CMf");
void SetEmitterAutoPause__FP7EMITTERi() __asm__("SetEmitterAutoPause__FP7EMITTERi");
void SetEmitterParticleCount__FP7EMITTERi() __asm__("SetEmitterParticleCount__FP7EMITTERi");
void SetExitExits() __asm__("SetExitExits");
void SetEyesClosed() __asm__("SetEyesClosed");
void SetLightKind() __asm__("SetLightKind");
void SetLoBusyk() __asm__("SetLoBusyk");
void SetPoTnCam() __asm__("SetPoTnCam");
void SetRhythmMgrState() __asm__("SetRhythmMgrState");
void SetRobottankState() __asm__("SetRobottankState");
void SetShapeChainMode() __asm__("SetShapeChainMode");
void SetSmaGoal() __asm__("SetSmaGoal");
void SetSmaMask() __asm__("SetSmaMask");
void SetSmaSvt() __asm__("SetSmaSvt");
void SetSndPan() __asm__("SetSndPan");
void SetSndPitch() __asm__("SetSndPitch");
void SetSndSourceLo() __asm__("SetSndSourceLo");
void SetSndSourcePos() __asm__("SetSndSourcePos");
void SetSoAlpha() __asm__("SetSoAlpha");
void SetSoCnstrForce() __asm__("SetSoCnstrForce");
void SetSoCnstrTorque() __asm__("SetSoCnstrTorque");
void SetSoDvGravity() __asm__("SetSoDvGravity");
void SetSoFadeRate() __asm__("SetSoFadeRate");
void SetSoIceable__FP2SOi() __asm__("SetSoIceable__FP2SOi");
void SetSoIgnoreLocked__FP2SOi() __asm__("SetSoIgnoreLocked__FP2SOi");
void SetStepSpinRatio__FP4STEPf() __asm__("SetStepSpinRatio__FP4STEPf");
void SetStepguardHpLevel() __asm__("SetStepguardHpLevel");
void SetStepplayerHp() __asm__("SetStepplayerHp");
void SetStepplayerMp() __asm__("SetStepplayerMp");
void SetSwIllum() __asm__("SetSwIllum");
void SetSwIllumShadow() __asm__("SetSwIllumShadow");
void SetTnFocusRatio() __asm__("SetTnFocusRatio");
void SetTnTnsOverride() __asm__("SetTnTnsOverride");
void SetVolbtnButtons() __asm__("SetVolbtnButtons");
void SetWrWavelength__FP2WRf() __asm__("SetWrWavelength__FP2WRf");
void func_00100760() __asm__("func_00100760");
void func_001007E8() __asm__("func_001007E8");
void func_00100860() __asm__("func_00100860");
void func_00100A00() __asm__("func_00100A00");
void func_00100C08() __asm__("func_00100C08");
void func_00100F38() __asm__("func_00100F38");
void func_00100FE0() __asm__("func_00100FE0");
void func_00101218() __asm__("func_00101218");
void func_001012C0() __asm__("func_001012C0");
void func_001013E8() __asm__("func_001013E8");
void func_00101468() __asm__("func_00101468");
void func_001014E8() __asm__("func_001014E8");
void func_00101588() __asm__("func_00101588");
void func_00101608() __asm__("func_00101608");
void func_00101768() __asm__("func_00101768");
void func_00101848() __asm__("func_00101848");
void func_001019D0() __asm__("func_001019D0");
void func_00101D98() __asm__("func_00101D98");
void func_00101E18() __asm__("func_00101E18");
void func_00102150() __asm__("func_00102150");
void func_001021D0() __asm__("func_001021D0");
void func_00102250() __asm__("func_00102250");
void func_001022C8() __asm__("func_001022C8");
void func_00102340() __asm__("func_00102340");
void func_001023B8() __asm__("func_001023B8");
void func_001024B0() __asm__("func_001024B0");
void func_001027E0() __asm__("func_001027E0");
void func_00102858() __asm__("func_00102858");
void func_00102900() __asm__("func_00102900");
void func_00102988() __asm__("func_00102988");
void func_00102A10() __asm__("func_00102A10");
void func_00102A88() __asm__("func_00102A88");
void func_00102B60() __asm__("func_00102B60");
void func_00102C38() __asm__("func_00102C38");
void func_00102D30() __asm__("func_00102D30");
void func_00102E28() __asm__("func_00102E28");
void func_00102F38() __asm__("func_00102F38");
void func_00103048() __asm__("func_00103048");
void func_00103100() __asm__("func_00103100");
void func_001031B8() __asm__("func_001031B8");
void func_00103270() __asm__("func_00103270");
void func_00103328() __asm__("func_00103328");
void func_001033E0() __asm__("func_001033E0");
void func_00103498() __asm__("func_00103498");
void func_00103520() __asm__("func_00103520");
void func_001035A8() __asm__("func_001035A8");
void func_00103630() __asm__("func_00103630");
void func_001036A8() __asm__("func_001036A8");
void func_00103730() __asm__("func_00103730");
void func_001037A8() __asm__("func_001037A8");
void func_00103830() __asm__("func_00103830");
void func_001038B8() __asm__("func_001038B8");
void func_00103930() __asm__("func_00103930");
void func_001039B8() __asm__("func_001039B8");
void func_00103A40() __asm__("func_00103A40");
void func_00103B38() __asm__("func_00103B38");
void func_00103C18() __asm__("func_00103C18");
void func_00103CA0() __asm__("func_00103CA0");
void func_00103D28() __asm__("func_00103D28");
void func_00103DB0() __asm__("func_00103DB0");
void func_00103E38() __asm__("func_00103E38");
void func_00103EC0() __asm__("func_00103EC0");
void func_00103F40() __asm__("func_00103F40");
void func_00103FE8() __asm__("func_00103FE8");
void func_00104350() __asm__("func_00104350");
void func_00104408() __asm__("func_00104408");
void func_001044B0() __asm__("func_001044B0");
void func_00104558() __asm__("func_00104558");
void func_00104638() __asm__("func_00104638");
void func_001047A0() __asm__("func_001047A0");
void func_00104D70() __asm__("func_00104D70");
void func_00104DF8() __asm__("func_00104DF8");
void func_00105158() __asm__("func_00105158");
void func_00105508() __asm__("func_00105508");
void func_00105AF0() __asm__("func_00105AF0");
void func_00105CD0() __asm__("func_00105CD0");
void func_00105D58() __asm__("func_00105D58");
void func_00105DE0() __asm__("func_00105DE0");
void func_00105E70() __asm__("func_00105E70");
void func_00105F00() __asm__("func_00105F00");
void func_00105F88() __asm__("func_00105F88");
void func_00106010() __asm__("func_00106010");
void func_00106098() __asm__("func_00106098");
void func_00106150() __asm__("func_00106150");
void func_00106208() __asm__("func_00106208");
void func_00106290() __asm__("func_00106290");
void func_00106318() __asm__("func_00106318");
void func_001063A0() __asm__("func_001063A0");
void func_00106448() __asm__("func_00106448");
void func_001064C0() __asm__("func_001064C0");
void func_00106538() __asm__("func_00106538");
void func_001065E0() __asm__("func_001065E0");
void func_00106710() __asm__("func_00106710");
void func_00107B18() __asm__("func_00107B18");
void func_00108140() __asm__("func_00108140");
void func_00108230() __asm__("func_00108230");
void func_001092D0() __asm__("func_001092D0");
void func_00109868() __asm__("func_00109868");
void func_00109AB8() __asm__("func_00109AB8");
void func_00109B70() __asm__("func_00109B70");
void func_00109C28() __asm__("func_00109C28");
void func_00109CB0() __asm__("func_00109CB0");
void func_00109FB8() __asm__("func_00109FB8");
void func_0010A0A8() __asm__("func_0010A0A8");
void func_0010A130() __asm__("func_0010A130");
void func_0010A1B8() __asm__("func_0010A1B8");
void func_0010A4E0() __asm__("func_0010A4E0");
void func_0010A568() __asm__("func_0010A568");
void func_0010A620() __asm__("func_0010A620");
void func_0010A6D8() __asm__("func_0010A6D8");
void func_0010A790() __asm__("func_0010A790");
void func_0010A848() __asm__("func_0010A848");
void func_0010A8D0() __asm__("func_0010A8D0");
void func_0010A9E8() __asm__("func_0010A9E8");
void func_0010AA90() __asm__("func_0010AA90");
void func_0010AB00() __asm__("func_0010AB00");
void func_0010AB88() __asm__("func_0010AB88");
void func_0010ABF8() __asm__("func_0010ABF8");
void func_0010AC80() __asm__("func_0010AC80");
void func_0010AD08() __asm__("func_0010AD08");
void func_0010ADB8() __asm__("func_0010ADB8");
void func_0010AE28() __asm__("func_0010AE28");
void func_0010AFB0() __asm__("func_0010AFB0");
void func_0010B028() __asm__("func_0010B028");
void func_0010B0B8() __asm__("func_0010B0B8");
void func_0010B140() __asm__("func_0010B140");
void func_0010B1B8() __asm__("func_0010B1B8");
void func_0010B230() __asm__("func_0010B230");
void func_0010B340() __asm__("func_0010B340");
void func_0010B510() __asm__("func_0010B510");
void func_0010B598() __asm__("func_0010B598");
void func_0010B618() __asm__("func_0010B618");
void func_0010B6D0() __asm__("func_0010B6D0");
void func_0010B760() __asm__("func_0010B760");
void func_0010B8F8() __asm__("func_0010B8F8");
void func_0010B970() __asm__("func_0010B970");
void func_0010B9E8() __asm__("func_0010B9E8");
void func_0010BB38() __asm__("func_0010BB38");
void func_0010BD10() __asm__("func_0010BD10");
void func_0010BE70() __asm__("func_0010BE70");
void func_0010C0A8() __asm__("func_0010C0A8");
void func_0010C120() __asm__("func_0010C120");
void func_0010C2E8() __asm__("func_0010C2E8");
void func_0010C5F0() __asm__("func_0010C5F0");
void func_0010C6A8() __asm__("func_0010C6A8");
void func_0010C820() __asm__("func_0010C820");
void func_0010C8C8() __asm__("func_0010C8C8");
void func_0010C950() __asm__("func_0010C950");
void func_0010C9C8() __asm__("func_0010C9C8");
void func_0010CA50() __asm__("func_0010CA50");
void func_0010CAD8() __asm__("func_0010CAD8");
void func_0010CB60() __asm__("func_0010CB60");
void func_0010CBE8() __asm__("func_0010CBE8");
void func_0010CC70() __asm__("func_0010CC70");
void func_0010CCF8() __asm__("func_0010CCF8");
void func_0010CD80() __asm__("func_0010CD80");
void func_0010CE08() __asm__("func_0010CE08");
void func_0010CE90() __asm__("func_0010CE90");
void func_0010CF18() __asm__("func_0010CF18");
void func_0010CFA0() __asm__("func_0010CFA0");
void func_0010D028() __asm__("func_0010D028");
void func_0010D0B0() __asm__("func_0010D0B0");
void func_0010D158() __asm__("func_0010D158");
void func_0010D1D8() __asm__("func_0010D1D8");
void func_0010D2E8() __asm__("func_0010D2E8");
void func_0010D370() __asm__("func_0010D370");
void func_0010D3E8() __asm__("func_0010D3E8");
void func_0010D468() __asm__("func_0010D468");
void func_0010D4E0() __asm__("func_0010D4E0");
void func_0010D558() __asm__("func_0010D558");
void func_0010D5D0() __asm__("func_0010D5D0");
void func_0010D660() __asm__("func_0010D660");
void func_0010D6E0() __asm__("func_0010D6E0");
void func_0010D798() __asm__("func_0010D798");
void func_0010D840() __asm__("func_0010D840");
void func_0010D8E0() __asm__("func_0010D8E0");
void func_0010D958() __asm__("func_0010D958");
void func_0010D9E0() __asm__("func_0010D9E0");
void func_0010DA68() __asm__("func_0010DA68");
void func_0010DDB0() __asm__("func_0010DDB0");
void func_0010DEF8() __asm__("func_0010DEF8");
void func_0010DF80() __asm__("func_0010DF80");
void func_0010E0C0() __asm__("func_0010E0C0");
void func_0010E1C0() __asm__("func_0010E1C0");
void func_0010E4A8() __asm__("func_0010E4A8");
void func_0010E550() __asm__("func_0010E550");
void func_0010E5D8() __asm__("func_0010E5D8");
void func_0010E690() __asm__("func_0010E690");
void func_0010E758() __asm__("func_0010E758");
void func_0010E820() __asm__("func_0010E820");
void func_0010E998() __asm__("func_0010E998");
void func_0010EA20() __asm__("func_0010EA20");
void func_0010EA98() __asm__("func_0010EA98");
void func_0010EB10() __asm__("func_0010EB10");
void func_0010EC00() __asm__("func_0010EC00");
void func_0010EDA8() __asm__("func_0010EDA8");
void func_0010EFB0() __asm__("func_0010EFB0");
void func_0010F030() __asm__("func_0010F030");
void func_0010F208() __asm__("func_0010F208");
void func_0010F280() __asm__("func_0010F280");
void func_0010F2F8() __asm__("func_0010F2F8");
void func_0010F370() __asm__("func_0010F370");
void func_0010F6E8() __asm__("func_0010F6E8");
void func_0010F7B0() __asm__("func_0010F7B0");
void func_0010F918() __asm__("func_0010F918");
void func_0010FA38() __asm__("func_0010FA38");
void func_0010FAC0() __asm__("func_0010FAC0");
void func_0010FB48() __asm__("func_0010FB48");
void func_0010FBF0() __asm__("func_0010FBF0");
void func_0010FC80() __asm__("func_0010FC80");
void func_0010FDA8() __asm__("func_0010FDA8");
void func_0010FE60() __asm__("func_0010FE60");
void func_0010FF18() __asm__("func_0010FF18");
void func_0010FFE8() __asm__("func_0010FFE8");
void func_001100B8() __asm__("func_001100B8");
void func_00110140() __asm__("func_00110140");
void func_001102C0() __asm__("func_001102C0");
void func_001103B0() __asm__("func_001103B0");
void func_00110478() __asm__("func_00110478");
void func_001104F0() __asm__("func_001104F0");
void func_001105B8() __asm__("func_001105B8");
void func_00110680() __asm__("func_00110680");
void func_00110738() __asm__("func_00110738");
void func_001107B8() __asm__("func_001107B8");
void func_00110838() __asm__("func_00110838");
void func_001108E0() __asm__("func_001108E0");
void func_00110998() __asm__("func_00110998");
void func_0012EE78() __asm__("func_0012EE78");
void func_00132ED0() __asm__("func_00132ED0");
void func_0013BC60() __asm__("func_0013BC60");
void func_0013C4D8() __asm__("func_0013C4D8");
void func_00144068() __asm__("func_00144068");
void func_001443E8() __asm__("func_001443E8");
void func_001443F8() __asm__("func_001443F8");
void func_00144408() __asm__("func_00144408");
void func_00149248() __asm__("func_00149248");
void func_001497E0() __asm__("func_001497E0");
void func_00149848() __asm__("func_00149848");
void func_00156870() __asm__("func_00156870");
void func_0015C650() __asm__("func_0015C650");
void func_001628B0() __asm__("func_001628B0");
void func_001732E0() __asm__("func_001732E0");
void func_00173318() __asm__("func_00173318");
void func_00173358() __asm__("func_00173358");
void func_00173398() __asm__("func_00173398");
void func_001733C8() __asm__("func_001733C8");
void func_001813C0() __asm__("func_001813C0");
void func_001813F0() __asm__("func_001813F0");
void func_00184028() __asm__("func_00184028");
void func_00184040() __asm__("func_00184040");
void func_001A35F0() __asm__("func_001A35F0");
void func_001A3610() __asm__("func_001A3610");
void func_001A3630() __asm__("func_001A3630");
void func_001A3650() __asm__("func_001A3650");
void func_001A3680() __asm__("func_001A3680");
void func_001A36D8() __asm__("func_001A36D8");
void func_001A62E8() __asm__("func_001A62E8");
void func_001A6318() __asm__("func_001A6318");
void func_001A6340() __asm__("func_001A6340");
void func_001A63B0() __asm__("func_001A63B0");
void func_001A6C90() __asm__("func_001A6C90");
void func_001A6E38() __asm__("func_001A6E38");
void func_001A6E70() __asm__("func_001A6E70");
void func_001B1328() __asm__("func_001B1328");
void func_001BBA60() __asm__("func_001BBA60");
void func_001BBA90() __asm__("func_001BBA90");
void func_001BBB50() __asm__("func_001BBB50");
void func_001BBB80() __asm__("func_001BBB80");
void func_001BEAA8() __asm__("func_001BEAA8");
void func_001BEAD0() __asm__("func_001BEAD0");
void func_001C20E8() __asm__("func_001C20E8");
void func_001C2148() __asm__("func_001C2148");
void SetPipeGrabEnabled() __asm__("SetPipeGrabEnabled");
void func_001C4098() __asm__("func_001C4098");
void func_001C4A60() __asm__("func_001C4A60");
void func_001C7A40() __asm__("func_001C7A40");
void func_001C7A58() __asm__("func_001C7A58");
void func_001C7B10() __asm__("func_001C7B10");
void func_001CB2E0() __asm__("func_001CB2E0");
void func_001CB360() __asm__("func_001CB360");
void func_001CB658() __asm__("func_001CB658");
void func_001E0128() __asm__("func_001E0128");
void func_001E0158() __asm__("func_001E0158");
void func_001E01C8() __asm__("func_001E01C8");
void func_001E7840() __asm__("func_001E7840");
void func_001E7930() __asm__("func_001E7930");
void func_001E7968() __asm__("func_001E7968");
void func_001E79A0() __asm__("func_001E79A0");
void func_001E79F0() __asm__("func_001E79F0");
void func_001E7FF0() __asm__("func_001E7FF0");
void func_001F07E8() __asm__("func_001F07E8");
void func_001F1EB8() __asm__("func_001F1EB8");
void func_001F2688() __asm__("func_001F2688");
void func_001F2718() __asm__("func_001F2718");
void func_001F2740() __asm__("func_001F2740");
void func_001F4848() __asm__("func_001F4848");
void func_001F4870() __asm__("func_001F4870");
void func_001F4898() __asm__("func_001F4898");
void func_001F4930() __asm__("func_001F4930");
void func_001F6838() __asm__("func_001F6838");
void func_001F6860() __asm__("func_001F6860");
void func_001F6918() __asm__("func_001F6918");
void func_001F6960() __asm__("func_001F6960");
void func_001F7A68() __asm__("func_001F7A68");
void func_001F7B40() __asm__("func_001F7B40");
void func_001F7B58() __asm__("func_001F7B58");
void func_001F7B68() __asm__("func_001F7B68");
void func_001F7B78() __asm__("func_001F7B78");
void func_001F7B88() __asm__("func_001F7B88");
void func_001F7B98() __asm__("func_001F7B98");
void func_001F7BF0() __asm__("func_001F7BF0");
void func_001F7EC8() __asm__("func_001F7EC8");
void func_001F7EF0() __asm__("func_001F7EF0");
void func_001F7F08() __asm__("func_001F7F08");
void func_001F7F20() __asm__("func_001F7F20");
void func_001F7F48() __asm__("func_001F7F48");
void func_001F81E0() __asm__("func_001F81E0");
void func_001F83B0() __asm__("func_001F83B0");
void func_001F83E0() __asm__("func_001F83E0");
void func_001F8410() __asm__("func_001F8410");
void func_001F8440() __asm__("func_001F8440");
void func_001F8470() __asm__("func_001F8470");
void func_001F84A0() __asm__("func_001F84A0");
void func_001F8668() __asm__("func_001F8668");
void func_001F87A0() __asm__("func_001F87A0");
void func_001F87C8() __asm__("func_001F87C8");
void func_001F8808() __asm__("func_001F8808");
void func_001F8830() __asm__("func_001F8830");
void func_001F8870() __asm__("func_001F8870");
void func_001F8898() __asm__("func_001F8898");
void func_001F88D8() __asm__("func_001F88D8");
void func_001FB1F8() __asm__("func_001FB1F8");
void func_001FE888() __asm__("func_001FE888");
void func_00203FA0() __asm__("func_00203FA0");
void func_00204D48() __asm__("func_00204D48");
void func_00204DA8() __asm__("func_00204DA8");
void func_00206458() __asm__("func_00206458");
void func_00206488() __asm__("func_00206488");
void func_002064A0() __asm__("func_002064A0");
void func_00208538() __asm__("func_00208538");
void func_00209A88() __asm__("func_00209A88");
void func_0020A010() __asm__("func_0020A010");
void func_0020A518() __asm__("func_0020A518");
void func_0020A540() __asm__("func_0020A540");
void func_0020A9C0() __asm__("func_0020A9C0");
void func_0020AF08() __asm__("func_0020AF08");
void func_0020F2B8() __asm__("func_0020F2B8");
void func_00210978() __asm__("func_00210978");
void func_00221818() __asm__("func_00221818");
void func_00221948() __asm__("func_00221948");
void func_00224610() __asm__("func_00224610");
void func_00224638() __asm__("func_00224638");
void func_00226370() __asm__("func_00226370");
void func_00228518() __asm__("func_00228518");
}

/* The pointer types of the emitter.h function templates whose instances the records store (EOPID_PMFT): g++ 2.95
 * picks a template instance by deduction from the type it is converted to, and a `const void *` gives it none. */
template <class T> struct EOPIDT
{
    typedef EMITB *(*PemitbEnsure)(T *, ENSK);
    typedef EMITV *(*func_00258898)(T *, ENSK);
    typedef EMITV *(*func_00258920)(T *, ENSK);
    typedef void (*func_002589A8)(T *, int);
    typedef int (*func_002589F0)(T *);
    typedef void (*func_00258A00)(T *, int);
    typedef int (*func_00258AB8)(T *);
    typedef int *(*func_00258AC8)(T *, ENSK);
    typedef int (*func_00258B00)(T *, ENSK);
    typedef int *(*func_00258B38)(T *, ENSK);
    typedef void (*func_00258B70)(T *, int);
    typedef int (*func_00258C40)(T *);
    typedef void *(*func_00258C60)(T *, ENSK);
    typedef void *(*func_00258CC8)(T *, ENSK);
    typedef void *(*func_00258D30)(T *, ENSK);
    typedef void (*func_00258D98)(T *, int);
    typedef int (*func_00258E40)(T *);
    typedef void *(*func_00258E60)(T *, ENSK);
    typedef void *(*func_00258EC8)(T *, ENSK);
};

/* PemitbEnsure's two instances are emitter.cpp's explicit instantiations (0x258658, 0x2590F0): referred to here, not
 * instantiated a second time (g++'s `extern template`). */
extern template EMITB *PemitbEnsure(EMITTER *, ENSK);
extern template EMITB *PemitbEnsure(EXPLO *, ENSK);

namespace M_eopid { extern "C" {
/* One 8-byte word of an EOPID (see above). */
struct EOPIDW { const void *pv0; const void *pv1; };
#define EOPID_NONE { 0, 0 }
#define EOPID_INT(n) { (const void *)(n), 0 }                                     /* offset or count */
#define EOPID_PMF(fn) { (const void *)0xFFFF0000, (const void *)&EOPIDFN::fn }   /* delta 0, index -1, pfn */
#define EOPID_PMFI(fn) { (const void *)0xFFFF0000, (const void *)&::fn }        /* the same, an inline accessor of the headers */
#define EOPID_PMFT(fn, T) { (const void *)0xFFFF0000, (const void *)(EOPIDT<T>::fn)&::fn }  /* the same, a function template instance */
#define EOPID_VPMF(i, d2) { (const void *)((i) << 16), (const void *)(d2) }      /* delta 0, vtable index i, delta2 */
#define EOPID_FN(fn) { (const void *)&EOPIDFN::fn, 0 }                          /* a Splice thunk */
struct EOPIDROW { int otyp; int grfeopid; EOPIDW get; EOPIDW set; EOPIDW file; EOPIDW obj; };
typedef char assert_EOPIDROW_size[sizeof(EOPIDROW) == 0x28 ? 1 : -1];

/* g_aeopid (.data). */
EOPIDROW D_002AC880[1449] = {
    {7, 4, EOPID_PMF(CidBasic), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [0] opt 1, BASIC (+198) */
    {0x2000, 4, EOPID_PMF(func_00132ED0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1] opt 2, BASIC (+198) */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunkBasicDERIVED_FROMQ), EOPID_INT(1), EOPID_NONE}, /* [2] opt 3, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_BOOL_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_BOOL_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [3] opt 4, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_INT_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_INT_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [4] opt 5, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_FLOAT_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_FLOAT_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [5] opt 6, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_CLQ_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_CLQ_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [6] opt 7, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_LM_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_LM_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [7] opt 8, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_OID_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_OID_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [8] opt 9, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(RefThunkBasicCUSTOM_VECTOR_PROPERTY), EOPID_FN(RefThunkBasicCUSTOM_VECTOR_PROPERTY), EOPID_INT(2), EOPID_NONE}, /* [9] opt 10, BASIC (+198) */
    {0x12, 0x401, EOPID_FN(func_00100760), EOPID_FN(func_00100760), EOPID_INT(1), EOPID_NONE}, /* [10] opt 11, BASIC (+198) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001007E8), EOPID_NONE, EOPID_NONE}, /* [11] opt 12, BASIC (+198) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00100860), EOPID_NONE, EOPID_NONE}, /* [12] opt 13, BASIC (+198) */
    {1, 0x122, EOPID_INT(0x38), EOPID_VPMF(0x2B, 0xC), EOPID_VPMF(0x2B, 0xC), EOPID_NONE}, /* [13] opt 14, LO (+131) */
    {0x1008, 0x124, EOPID_PMF(func_001A6C90), EOPID_PMF(SetLoBusyk), EOPID_PMF(SetLoBusyk), EOPID_NONE}, /* [14] opt 15, LO (+131) */
    {6, 2, EOPID_INT(0x18), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [15] opt 28, LO (+131) */
    {6, 4, EOPID_PMF(OidProxyLo), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [16] opt 29, LO (+131) */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunkLoMATCHES_NAMEQ), EOPID_INT(1), EOPID_NONE}, /* [17] opt 30, LO (+131) */
    {0x2006, 4, EOPID_PMF(func_001CB658), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [18] opt 31, LO (+131) */
    {0x2030, 2, EOPID_INT(0x10), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [19] opt 32, LO (+131) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetLoParent), EOPID_INT(2), EOPID_NONE}, /* [20] opt 33, LO (+131) */
    {2, 0x24, EOPID_PMF(func_001A62E8), EOPID_VPMF(0x1E, 0xC), EOPID_NONE, EOPID_NONE}, /* [21] opt 34, LO (+131) */
    {4, 0x24, EOPID_PMF(func_001A6318), EOPID_VPMF(0x1F, 0xC), EOPID_NONE, EOPID_NONE}, /* [22] opt 35, LO (+131) */
    {2, 0x24, EOPID_PMF(func_001A6340), EOPID_PMF(func_001A63B0), EOPID_NONE, EOPID_NONE}, /* [23] opt 36, LO (+131) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00100A00), EOPID_INT(1), EOPID_NONE}, /* [24] opt 37, LO (+131) */
    {0x12, 0x4401, EOPID_FN(RefThunkLoADD), EOPID_FN(RefThunkLoADD), EOPID_NONE, EOPID_NONE}, /* [25] opt 39, LO (+131) */
    {0x12, 0x4401, EOPID_FN(RefThunkLoREMOVEFnUser), EOPID_FN(RefThunkLoREMOVEFn), EOPID_NONE, EOPID_NONE}, /* [26] opt 40, LO (+131) */
    {0, 4, EOPID_PMFI(func_00259AE8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [27] opt 41, LO (+131) */
    {6, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [28] opt 60, LO (+131) */
    {2, 0x124, EOPID_VPMF(0x22, 0xC), EOPID_VPMF(0x24, 0xC), EOPID_VPMF(0x24, 0xC), EOPID_NONE}, /* [29] opt 63, LO (+131) */
    {2, 0x124, EOPID_VPMF(0x23, 0xC), EOPID_VPMF(0x25, 0xC), EOPID_VPMF(0x25, 0xC), EOPID_NONE}, /* [30] opt 64, LO (+131) */
    {5, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [31] opt 65, LO (+131) */
    {0, 0x104, EOPID_PMF(func_001A6E70), EOPID_NONE, EOPID_PMF(func_001A6E38), EOPID_NONE}, /* [32] opt 67, LO (+131) */
    {0x1001, 0x120, EOPID_NONE, EOPID_PMF(SetSoCnstrForce), EOPID_PMF(SetSoCnstrForce), EOPID_NONE}, /* [33] opt 69, SO (+60) */
    {0x1001, 0x120, EOPID_NONE, EOPID_PMF(SetSoCnstrTorque), EOPID_PMF(SetSoCnstrTorque), EOPID_NONE}, /* [34] opt 70, SO (+60) */
    {2, 0x124, EOPID_PMF(GetSoDvGravity), EOPID_PMF(SetSoDvGravity), EOPID_PMF(SetSoDvGravity), EOPID_NONE}, /* [35] opt 71, SO (+60) */
    {1, 0xA2, EOPID_INT(0x13C), EOPID_PMF(func_001F2688), EOPID_INT(0x13C), EOPID_NONE}, /* [36] opt 87, SO (+60) */
    {0, 0x124, EOPID_PMF(func_001F6960), EOPID_PMF(func_001F6918), EOPID_PMF(func_001F6918), EOPID_NONE}, /* [37] opt 88, SO (+60) */
    {0, 0x24, EOPID_PMF(func_001F6838), EOPID_PMF(func_001F6860), EOPID_NONE, EOPID_NONE}, /* [38] opt 90, SO (+60) */
    {0, 0x120, EOPID_NONE, EOPID_PMF(func_001F7F48), EOPID_PMF(func_001F7F48), EOPID_NONE}, /* [39] opt 91, SO (+60) */
    {0, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001F8668), EOPID_NONE}, /* [40] opt 102, SO (+60) */
    {6, 0x120, EOPID_NONE, EOPID_PMF(func_001F81E0), EOPID_PMF(func_001F81E0), EOPID_NONE}, /* [41] opt 104, SO (+60) */
    {0x12, 0x401, EOPID_FN(func_00100C08), EOPID_FN(func_00100C08), EOPID_NONE, EOPID_NONE}, /* [42] opt 105, SO (+60) */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunkSoPOINT_IN_BSP_QUICK), EOPID_INT(1), EOPID_NONE}, /* [43] opt 108, SO (+60) */
    {0x12, 0x401, EOPID_FN(RefThunkSo_ShowSoGlobsById), EOPID_FN(RefThunkSo_ShowSoGlobsById), EOPID_INT(1), EOPID_NONE}, /* [44] opt 110, SO (+60) */
    {0x12, 0x401, EOPID_FN(RefThunkSo_HideSoGlobsById), EOPID_FN(RefThunkSo_HideSoGlobsById), EOPID_INT(1), EOPID_NONE}, /* [45] opt 111, SO (+60) */
    {0x12, 0x401, EOPID_FN(RefThunkSo_ShowSoPartsById), EOPID_FN(RefThunkSo_ShowSoPartsById), EOPID_INT(1), EOPID_NONE}, /* [46] opt 112, SO (+60) */
    {0x12, 0x401, EOPID_FN(RefThunkSo_HideSoPartsById), EOPID_FN(RefThunkSo_HideSoPartsById), EOPID_INT(1), EOPID_NONE}, /* [47] opt 113, SO (+60) */
    {0x12, 0x401, EOPID_FN(func_00100F38), EOPID_FN(func_00100F38), EOPID_INT(2), EOPID_NONE}, /* [48] opt 114, SO (+60) */
    {0x1011, 0x2092, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_PMF(func_001F7B98)}, /* [49] opt 119, SO (+60) */
    {1, 0x2092, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_PMF(func_001F7B98)}, /* [50] opt 120, SO (+60) */
    {0, 0x2092, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_PMF(func_001F7B98)}, /* [51] opt 121, SO (+60) */
    {0, 0x124, EOPID_PMF(GetSoIceable), EOPID_PMF(SetSoIceable__FP2SOi), EOPID_PMF(SetSoIceable__FP2SOi), EOPID_NONE}, /* [52] opt 124, SO (+60) */
    {0x12, 0x401, EOPID_FN(func_00100FE0), EOPID_FN(func_00100FE0), EOPID_INT(2), EOPID_NONE}, /* [53] opt 125, SO (+60) */
    {5, 1, EOPID_NONE, EOPID_FN(RefThunk_GrfzpdFromSoGrfzpk), EOPID_INT(1), EOPID_NONE}, /* [54] opt 126, SO (+60) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSoFADE_OUT), EOPID_INT(1), EOPID_NONE}, /* [55] opt 127, SO (+60) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSoFADE_IN), EOPID_INT(1), EOPID_NONE}, /* [56] opt 128, SO (+60) */
    {1, 0x122, EOPID_INT(0x12C), EOPID_PMF(SetSoAlpha), EOPID_PMF(SetSoAlpha), EOPID_NONE}, /* [57] opt 129, SO (+60) */
    {1, 0x122, EOPID_INT(0x130), EOPID_PMF(SetSoFadeRate), EOPID_PMF(SetSoFadeRate), EOPID_NONE}, /* [58] opt 130, SO (+60) */
    {1, 0x2092, EOPID_INT(0xD4), EOPID_INT(0xD4), EOPID_INT(0xD4), EOPID_PMF(func_001F7BF0)}, /* [59] opt 131, SO (+60) */
    {1, 0x2092, EOPID_INT(0xD8), EOPID_INT(0xD8), EOPID_INT(0xD8), EOPID_PMF(func_001F7BF0)}, /* [60] opt 132, SO (+60) */
    {0x1032, 0x124, EOPID_PMF(func_001F7EF0), EOPID_PMF(func_001F7EC8), EOPID_PMF(func_001F7EC8), EOPID_NONE}, /* [61] opt 139, SO (+60) */
    {0x103D, 0x104, EOPID_PMF(func_001F2740), EOPID_NONE, EOPID_PMF(func_001F2718), EOPID_NONE}, /* [62] opt 140, SO (+60) */
    {0, 0x124, EOPID_PMF(func_001F7F08), EOPID_PMF(func_001F7F20), EOPID_PMF(func_001F7F20), EOPID_NONE}, /* [63] opt 141, SO (+60) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00101218), EOPID_INT(2), EOPID_NONE}, /* [64] opt 166, SO (+60) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001012C0), EOPID_INT(2), EOPID_NONE}, /* [65] opt 167, SO (+60) */
    {0x208A, 1, EOPID_NONE, EOPID_FN(RefThunk_PsaaFindSoOid), EOPID_INT(1), EOPID_NONE}, /* [66] opt 174, SO (+60) */
    {1, 0x92, EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_NONE}, /* [67] opt 176, SO (+60) */
    {2, 0x92, EOPID_INT(0x90), EOPID_INT(0x90), EOPID_INT(0x90), EOPID_NONE}, /* [68] opt 177, SO (+60) */
    {0, 0x124, EOPID_PMF(GetSoIgnoreLocked), EOPID_PMF(SetSoIgnoreLocked__FP2SOi), EOPID_PMF(SetSoIgnoreLocked__FP2SOi), EOPID_NONE}, /* [69] opt 178, SO (+60) */
    {0x1040, 0x124, EOPID_PMF(func_001F7B40), EOPID_PMF(func_001F7A68), EOPID_PMF(func_001F7A68), EOPID_NONE}, /* [70] opt 179, SO (+60) */
    {5, 0x124, EOPID_PMF(func_001F7B68), EOPID_PMF(func_001F7B58), EOPID_PMF(func_001F7B58), EOPID_NONE}, /* [71] opt 180, SO (+60) */
    {5, 0x124, EOPID_PMF(func_001F7B88), EOPID_PMF(func_001F7B78), EOPID_PMF(func_001F7B78), EOPID_NONE}, /* [72] opt 181, SO (+60) */
    {0x2086, 1, EOPID_NONE, EOPID_FN(func_001013E8), EOPID_INT(1), EOPID_NONE}, /* [73] opt 182, SO (+60) */
    {0x2088, 1, EOPID_NONE, EOPID_FN(func_00101468), EOPID_INT(1), EOPID_NONE}, /* [74] opt 183, SO (+60) */
    {2, 0x124, EOPID_PMF(func_001F4848), EOPID_PMF(func_001F4898), EOPID_PMF(func_001F4898), EOPID_NONE}, /* [75] opt 187, SO (+60) */
    {2, 0x124, EOPID_PMF(func_001F4870), EOPID_PMF(func_001F4930), EOPID_PMF(func_001F4930), EOPID_NONE}, /* [76] opt 188, SO (+60) */
    {0x2095, 1, EOPID_NONE, EOPID_FN(func_001014E8), EOPID_INT(2), EOPID_NONE}, /* [77] opt 194, SO (+60) */
    {0x2096, 1, EOPID_NONE, EOPID_FN(func_00101588), EOPID_INT(1), EOPID_NONE}, /* [78] opt 195, SO (+60) */
    {0x12, 0x401, EOPID_FN(func_00101608), EOPID_FN(func_00101608), EOPID_INT(2), EOPID_NONE}, /* [79] opt 196, SO (+60) */
    {1, 0x124, EOPID_PMF(func_001F83E0), EOPID_PMF(func_001F83B0), EOPID_PMF(func_001F83B0), EOPID_NONE}, /* [80] opt 197, SO (+60) */
    {1, 0x124, EOPID_PMF(func_001F8440), EOPID_PMF(func_001F8410), EOPID_PMF(func_001F8410), EOPID_NONE}, /* [81] opt 198, SO (+60) */
    {1, 0x124, EOPID_PMF(func_001F84A0), EOPID_PMF(func_001F8470), EOPID_PMF(func_001F8470), EOPID_NONE}, /* [82] opt 199, SO (+60) */
    {0x2011, 4, EOPID_PMFI(func_00259B20), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [83] opt 200, SO (+60) */
    {1, 4, EOPID_VPMF(0xA, 0xC), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [84] opt 202, SAA (+6) */
    {0xA, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [85] opt 143, LOOP */
    {0xA, 0x12, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE, EOPID_NONE}, /* [86] opt 144, LOOP */
    {0xA, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [87] opt 147, PINGPONG */
    {0xA, 0x12, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE, EOPID_NONE}, /* [88] opt 148, PINGPONG */
    {0xA, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [89] opt 151, SHUFFLE */
    {1, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [90] opt 158, EYES */
    {0xA, 0x12, EOPID_INT(0x54), EOPID_INT(0x54), EOPID_NONE, EOPID_NONE}, /* [91] opt 159, EYES */
    {1, 0x12, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE, EOPID_NONE}, /* [92] opt 203, EYES */
    {1, 0x22, EOPID_INT(0x78), EOPID_PMF(SetEyesClosed), EOPID_NONE, EOPID_NONE}, /* [93] opt 204, EYES */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetScrollerMasterSpeeds), EOPID_INT(2), EOPID_NONE}, /* [94] opt 205, SCROLLER */
    {1, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [95] opt 206, CIRCLER */
    {1, 0x12, EOPID_INT(0x54), EOPID_INT(0x54), EOPID_NONE, EOPID_NONE}, /* [96] opt 170, CIRCLER */
    {1, 0x12, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE, EOPID_NONE}, /* [97] opt 171, CIRCLER */
    {1, 0x12, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE, EOPID_NONE}, /* [98] opt 172, CIRCLER */
    {0x12, 0x400, EOPID_FN(func_00101768), EOPID_NONE, EOPID_INT(5), EOPID_NONE}, /* [99] opt 207, CID43 */
    {0x12, 0x400, EOPID_FN(func_00101848), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [100] opt 208, CID43 */
    {0xE, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_0012EE78)}, /* [101] opt 209, CID43 */
    {1, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(4), EOPID_PMF(func_0012EE78)}, /* [102] opt 210, CID43 */
    {1, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(8), EOPID_PMF(func_0012EE78)}, /* [103] opt 211, CID43 */
    {1, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0xC), EOPID_PMF(func_0012EE78)}, /* [104] opt 212, CID43 */
    {1, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x10), EOPID_PMF(func_0012EE78)}, /* [105] opt 213, CID43 */
    {0x2086, 1, EOPID_NONE, EOPID_FN(RefThunkAsegAPPLY), EOPID_INT(4), EOPID_NONE}, /* [106] opt 214, ASEG (+1) */
    {0x12, 0x400, EOPID_FN(func_001019D0), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [107] opt 215, ASEG (+1) */
    {0x2086, 1, EOPID_NONE, EOPID_FN(RefThunkAsegENSURE), EOPID_INT(5), EOPID_NONE}, /* [108] opt 216, ASEG (+1) */
    {1, 0xA2, EOPID_INT(0x78), EOPID_PMF(SetAsegMasterSpeed), EOPID_INT(0x78), EOPID_NONE}, /* [109] opt 217, ASEG (+1) */
    {9, 0x82, EOPID_INT(0x80), EOPID_NONE, EOPID_INT(0x80), EOPID_NONE}, /* [110] opt 218, ASEG (+1) */
    {0xA, 0x82, EOPID_INT(0x90), EOPID_NONE, EOPID_INT(0x90), EOPID_NONE}, /* [111] opt 219, ASEG (+1) */
    {5, 0x92, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE}, /* [112] opt 220, ASEG (+1) */
    {0, 0x92, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE}, /* [113] opt 221, ASEG (+1) */
    {1, 2, EOPID_INT(0x74), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [114] opt 222, ASEG (+1) */
    {1, 1, EOPID_NONE, EOPID_FN(RefThunkAsegFIND_LABEL), EOPID_INT(1), EOPID_NONE}, /* [115] opt 223, ASEG (+1) */
    {0x206D, 2, EOPID_INT(0x14), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [116] opt 250, ASEGA */
    {0x2030, 2, EOPID_INT(0x18), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [117] opt 251, ASEGA */
    {1, 2, EOPID_INT(0x1C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [118] opt 252, ASEGA */
    {1, 0x22, EOPID_INT(0x44), EOPID_PMF(SetAsegaSpeed__FP5ASEGAf), EOPID_NONE, EOPID_NONE}, /* [119] opt 253, ASEGA */
    {1, 0x22, EOPID_INT(0x48), EOPID_PMF(SetAsegaMasterSpeed), EOPID_NONE, EOPID_NONE}, /* [120] opt 217, ASEGA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetAsegaPriority), EOPID_INT(1), EOPID_NONE}, /* [121] opt 254, ASEGA */
    {0, 0x22, EOPID_INT(0x58), EOPID_PMF(SetAsegaHandsOff), EOPID_NONE, EOPID_NONE}, /* [122] opt 221, ASEGA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkAsegaSEEK), EOPID_INT(3), EOPID_NONE}, /* [123] opt 236, ASEGA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkAsegaRETRACT), EOPID_NONE, EOPID_NONE}, /* [124] opt 237, ASEGA */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00101D98), EOPID_INT(1), EOPID_NONE}, /* [125] opt 248, ASEGA */
    {0, 4, EOPID_PMF(FAsegaRetracted), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [126] opt 255, ASEGA */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00101E18), EOPID_NONE, EOPID_NONE}, /* [127] opt 256, CID151 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ResumeSnd), EOPID_NONE, EOPID_NONE}, /* [128] opt 257, CID151 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_StopSnd), EOPID_INT(1), EOPID_NONE}, /* [129] opt 258, CID151 */
    {1, 0x22, EOPID_INT(0x10), EOPID_PMF(func_001FB1F8), EOPID_NONE, EOPID_NONE}, /* [130] opt 212, CID151 */
    {5, 0x22, EOPID_INT(0x14), EOPID_PMF(SetSndPan), EOPID_NONE, EOPID_NONE}, /* [131] opt 259, CID151 */
    {1, 0x22, EOPID_INT(0x18), EOPID_PMF(SetSndPitch), EOPID_NONE, EOPID_NONE}, /* [132] opt 213, CID151 */
    {0x2002, 0x22, EOPID_INT(0x1C), EOPID_PMF(SetSndSourceLo), EOPID_NONE, EOPID_NONE}, /* [133] opt 260, CID151 */
    {2, 0x22, EOPID_INT(0x20), EOPID_PMF(SetSndSourcePos), EOPID_NONE, EOPID_NONE}, /* [134] opt 261, CID151 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetSndRange), EOPID_INT(2), EOPID_NONE}, /* [135] opt 262, CID151 */
    {0xA, 2, EOPID_INT(0x30), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [136] opt 263, CID151 */
    {0x2097, 1, EOPID_NONE, EOPID_FN(RefThunk_StartDialog), EOPID_INT(2), EOPID_NONE}, /* [137] opt 264, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_StopDialog), EOPID_NONE, EOPID_NONE}, /* [138] opt 265, CID152 */
    {0, 1, EOPID_NONE, EOPID_FN(func_00102150), EOPID_INT(1), EOPID_NONE}, /* [139] opt 266, CID152 */
    {1, 4, EOPID_PMF(func_001FE888), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [140] opt 267, CID152 */
    {1, 1, EOPID_NONE, EOPID_FN(func_001021D0), EOPID_INT(1), EOPID_NONE}, /* [141] opt 268, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00102250), EOPID_NONE, EOPID_NONE}, /* [142] opt 269, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001022C8), EOPID_NONE, EOPID_NONE}, /* [143] opt 270, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00102340), EOPID_NONE, EOPID_NONE}, /* [144] opt 271, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001023B8), EOPID_NONE, EOPID_NONE}, /* [145] opt 272, CID152 */
    {0x2097, 1, EOPID_NONE, EOPID_FN(RefThunk_PushMusic), EOPID_INT(1), EOPID_NONE}, /* [146] opt 273, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001024B0), EOPID_INT(1), EOPID_NONE}, /* [147] opt 274, CID152 */
    {1, 1, EOPID_NONE, EOPID_FN(RefThunk_TMusicCur), EOPID_NONE, EOPID_NONE}, /* [148] opt 275, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetSndmgrMasterVolume), EOPID_INT(1), EOPID_NONE}, /* [149] opt 276, CID152 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetSndmgrChannelVolume), EOPID_INT(2), EOPID_NONE}, /* [150] opt 277, CID152 */
    {0x1007, 0x122, EOPID_INT(0xE0), EOPID_PMF(SetLightKind), EOPID_PMF(SetLightKind), EOPID_NONE}, /* [151] opt 278, LIGHT */
    {2, 0xA2, EOPID_INT(0xF0), EOPID_PMF(func_001A35F0), EOPID_INT(0xF0), EOPID_NONE}, /* [152] opt 279, LIGHT */
    {1, 0xA2, EOPID_INT(0x150), EOPID_PMF(func_001A3610), EOPID_INT(0x150), EOPID_NONE}, /* [153] opt 280, LIGHT */
    {1, 0xA2, EOPID_INT(0x154), EOPID_PMF(func_001A3630), EOPID_INT(0x154), EOPID_NONE}, /* [154] opt 281, LIGHT */
    {0xA, 0xA2, EOPID_INT(0x164), EOPID_PMF(func_001A3650), EOPID_INT(0x164), EOPID_NONE}, /* [155] opt 282, LIGHT */
    {0, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [156] opt 283, LIGHT */
    {0, 0xA2, EOPID_INT(0x15C), EOPID_PMF(func_001A3680), EOPID_INT(0x15C), EOPID_NONE}, /* [157] opt 290, LIGHT */
    {0, 0xA2, EOPID_INT(0x160), EOPID_PMF(func_001A36D8), EOPID_INT(0x160), EOPID_NONE}, /* [158] opt 291, LIGHT */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkCmJOLT), EOPID_INT(1), EOPID_NONE}, /* [159] opt 233, CM */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkCmCUT), EOPID_NONE, EOPID_NONE}, /* [160] opt 292, CM */
    {1, 0x122, EOPID_INT(0x20), EOPID_PMF(SetCmFov__FP2CMf), EOPID_PMF(SetCmFov__FP2CMf), EOPID_NONE}, /* [161] opt 293, CM */
    {1, 0x122, EOPID_INT(0x24), EOPID_PMF(SetCmBaseFovTarget), EOPID_PMF(SetCmBaseFovTarget), EOPID_NONE}, /* [162] opt 294, CM */
    {0xC, 0x92, EOPID_INT(0x2C), EOPID_INT(0x2C), EOPID_INT(0x2C), EOPID_NONE}, /* [163] opt 295, CM */
    {1, 0x122, EOPID_INT(0x4C), EOPID_PMF(SetCmBaseNearClip), EOPID_PMF(SetCmBaseNearClip), EOPID_NONE}, /* [164] opt 296, CM */
    {1, 0x122, EOPID_INT(0x50), EOPID_PMF(SetCmFarClip__FP2CMf), EOPID_PMF(SetCmFarClip__FP2CMf), EOPID_NONE}, /* [165] opt 297, CM */
    {1, 0x122, EOPID_INT(0x240), EOPID_PMF(SetCmNearFog__FP2CMf), EOPID_PMF(SetCmNearFog__FP2CMf), EOPID_NONE}, /* [166] opt 298, CM */
    {1, 0x122, EOPID_INT(0x244), EOPID_PMF(SetCmFarFog__FP2CMf), EOPID_PMF(SetCmFarFog__FP2CMf), EOPID_NONE}, /* [167] opt 299, CM */
    {0xD, 0xA2, EOPID_INT(0x24C), EOPID_PMF(SetCmRgbaFog__FP2CMP4RGBA), EOPID_INT(0x24C), EOPID_NONE}, /* [168] opt 300, CM */
    {1, 0x122, EOPID_INT(0x248), EOPID_PMF(SetCmUFogMax__FP2CMf), EOPID_PMF(SetCmUFogMax__FP2CMf), EOPID_NONE}, /* [169] opt 301, CM */
    {1, 0x122, EOPID_INT(0x18), EOPID_PMF(SetCmMrdRatio__FP2CMf), EOPID_PMF(SetCmMrdRatio__FP2CMf), EOPID_NONE}, /* [170] opt 302, CM */
    {2, 0x24, EOPID_PMFI(func_00256298), EOPID_PMF(SetCmPos), EOPID_NONE, EOPID_NONE}, /* [171] opt 34, CM */
    {4, 0x24, EOPID_PMFI(func_002562A0), EOPID_PMFI(func_00256278), EOPID_NONE, EOPID_NONE}, /* [172] opt 35, CM */
    {2, 0x24, EOPID_PMF(GetCmRotDeg), EOPID_PMF(SetCmRotDeg), EOPID_NONE, EOPID_NONE}, /* [173] opt 36, CM */
    {1, 0x92, EOPID_INT(0x620), EOPID_INT(0x624), EOPID_INT(0x624), EOPID_NONE}, /* [174] opt 303, CM */
    {5, 0x92, EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_NONE}, /* [175] opt 220, CID5 */
    {7, 0x124, EOPID_PMF(func_00221948), EOPID_PMF(func_00221818), EOPID_PMF(func_00221818), EOPID_NONE}, /* [176] opt 307, TN */
    {0x12, 0x400, EOPID_FN(func_001027E0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [177] opt 308, TN */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMF(PtnfnFromTn)}, /* [178] opt 309, TN */
    {9, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMF(PtnfnFromTn)}, /* [179] opt 310, TN */
    {0xA, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMF(PtnfnFromTn)}, /* [180] opt 311, TN */
    {9, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMF(PtnfnFromTn)}, /* [181] opt 312, TN */
    {0xA, 0x2092, EOPID_INT(0x48), EOPID_INT(0x48), EOPID_INT(0x48), EOPID_PMF(PtnfnFromTn)}, /* [182] opt 313, TN */
    {9, 0x2092, EOPID_INT(0x30), EOPID_INT(0x30), EOPID_INT(0x30), EOPID_PMF(PtnfnFromTn)}, /* [183] opt 314, TN */
    {0xA, 0x2092, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_PMF(PtnfnFromTn)}, /* [184] opt 315, TN */
    {1, 0x2092, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_PMF(PtnfnFromTn)}, /* [185] opt 316, TN */
    {1, 0x2092, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_PMF(PtnfnFromTn)}, /* [186] opt 317, TN */
    {1, 0x2092, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_PMF(PtnfnFromTn)}, /* [187] opt 318, TN */
    {1, 0x2092, EOPID_INT(0x64), EOPID_INT(0x64), EOPID_INT(0x64), EOPID_PMF(PtnfnFromTn)}, /* [188] opt 319, TN */
    {1, 0x2092, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_PMF(PtnfnFromTn)}, /* [189] opt 320, TN */
    {1, 0x124, EOPID_PMF(GetTnFocusX), EOPID_PMF(SetTnFocusRatio), EOPID_PMF(SetTnFocusRatio), EOPID_NONE}, /* [190] opt 321, TN */
    {2, 0x2092, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_PMF(PtnfnFromTn)}, /* [191] opt 322, TN */
    {1, 0x2092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_PMF(PtnfnFromTn)}, /* [192] opt 323, TN */
    {1, 0x2092, EOPID_INT(0x84), EOPID_INT(0x84), EOPID_INT(0x84), EOPID_PMF(PtnfnFromTn)}, /* [193] opt 324, TN */
    {0xA, 0x2092, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_PMF(PtnfnFromTn)}, /* [194] opt 325, TN */
    {1, 0x92, EOPID_INT(0x130), EOPID_INT(0x130), EOPID_INT(0x130), EOPID_NONE}, /* [195] opt 326, TN */
    {0, 0x92, EOPID_INT(0x120), EOPID_INT(0x120), EOPID_INT(0x120), EOPID_NONE}, /* [196] opt 327, TN */
    {0x100E, 0x92, EOPID_INT(0x164), EOPID_INT(0x164), EOPID_INT(0x164), EOPID_NONE}, /* [197] opt 328, TN */
    {0, 0x92, EOPID_INT(0x168), EOPID_INT(0x168), EOPID_INT(0x168), EOPID_NONE}, /* [198] opt 329, TN */
    {0, 0x92, EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_NONE}, /* [199] opt 330, TN */
    {5, 0x92, EOPID_INT(0x128), EOPID_INT(0x128), EOPID_INT(0x128), EOPID_NONE}, /* [200] opt 220, TN */
    {0, 0x92, EOPID_INT(0x124), EOPID_INT(0x124), EOPID_INT(0x124), EOPID_NONE}, /* [201] opt 331, TN */
    {0x100A, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [202] opt 332, TN */
    {1, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [203] opt 333, TN */
    {1, 0x92, EOPID_INT(0x174), EOPID_INT(0x174), EOPID_INT(0x174), EOPID_NONE}, /* [204] opt 334, TN */
    {1, 0x92, EOPID_INT(0x178), EOPID_INT(0x178), EOPID_INT(0x178), EOPID_NONE}, /* [205] opt 335, TN */
    {1, 0x92, EOPID_INT(0x17C), EOPID_INT(0x17C), EOPID_INT(0x17C), EOPID_NONE}, /* [206] opt 336, TN */
    {0, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [207] opt 337, TN */
    {1, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [208] opt 293, TN */
    {0, 0x92, EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_NONE}, /* [209] opt 338, TN */
    {0x2002, 0x92, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE}, /* [210] opt 339, TN */
    {9, 0x92, EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_NONE}, /* [211] opt 340, TN */
    {0xA, 0x92, EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_NONE}, /* [212] opt 341, TN */
    {0x100F, 2, EOPID_INT(0x134), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [213] opt 342, TN */
    {0x100F, 0xA2, EOPID_INT(0x13C), EOPID_PMF(SetTnTnsOverride), EOPID_INT(0x13C), EOPID_NONE}, /* [214] opt 343, TN */
    {0x1011, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [215] opt 119, TARGET (+1) */
    {1, 0x92, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_NONE}, /* [216] opt 120, TARGET (+1) */
    {0, 0x92, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_NONE}, /* [217] opt 121, TARGET (+1) */
    {6, 0x92, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_NONE}, /* [218] opt 344, TARGET (+1) */
    {0xA, 0x92, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_NONE}, /* [219] opt 345, CID21 */
    {1, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [220] opt 346, CID21 */
    {0x1053, 0x92, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_NONE}, /* [221] opt 347, CID21 */
    {2, 0x92, EOPID_INT(0x240), EOPID_INT(0x240), EOPID_INT(0x240), EOPID_NONE}, /* [222] opt 348, STEP (+17) */
    {1, 0x92, EOPID_INT(0x220), EOPID_INT(0x220), EOPID_INT(0x220), EOPID_NONE}, /* [223] opt 349, STEP (+17) */
    {1, 0x122, EOPID_INT(0x1F0), EOPID_PMF(func_00204D48), EOPID_PMF(func_00204D48), EOPID_NONE}, /* [224] opt 350, STEP (+17) */
    {1, 0x122, EOPID_INT(0x260), EOPID_PMF(func_00204DA8), EOPID_PMF(func_00204DA8), EOPID_NONE}, /* [225] opt 351, STEP (+17) */
    {6, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0x264), EOPID_NONE}, /* [226] opt 352, STEP (+17) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00102858), EOPID_INT(2), EOPID_NONE}, /* [227] opt 353, STEP (+17) */
    {0, 0xA2, EOPID_INT(0x204), EOPID_PMF(func_00203FA0), EOPID_INT(0x204), EOPID_NONE}, /* [228] opt 354, STEP (+17) */
    {1, 0x22, EOPID_INT(0x1A8), EOPID_PMFI(func_00259D18), EOPID_NONE, EOPID_NONE}, /* [229] opt 355, STEP (+17) */
    {2, 0x12, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE, EOPID_NONE}, /* [230] opt 356, STEP (+17) */
    {0, 0x92, EOPID_INT(0x298), EOPID_INT(0x298), EOPID_INT(0x298), EOPID_NONE}, /* [231] opt 357, STEP (+17) */
    {0, 0x92, EOPID_INT(0x29C), EOPID_INT(0x29C), EOPID_INT(0x29C), EOPID_NONE}, /* [232] opt 358, STEP (+17) */
    {0x12, 0x401, EOPID_FN(func_00102900), EOPID_FN(func_00102988), EOPID_INT(1), EOPID_NONE}, /* [233] opt 359, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00102A10), EOPID_FN(func_00102A10), EOPID_NONE, EOPID_NONE}, /* [234] opt 360, STEPGUARD (+11) */
    {0xA, 0x2092, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_PMF(func_0020A9C0)}, /* [235] opt 361, STEPGUARD (+11) */
    {0xA, 0x2092, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_PMF(func_0020A9C0)}, /* [236] opt 362, STEPGUARD (+11) */
    {0xA, 0x2092, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_PMF(func_0020A9C0)}, /* [237] opt 363, STEPGUARD (+11) */
    {1, 0x2120, EOPID_NONE, EOPID_PMF(func_0020AF08), EOPID_PMF(func_0020AF08), EOPID_PMF(func_0020A9C0)}, /* [238] opt 364, STEPGUARD (+11) */
    {1, 0x2092, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_PMF(func_0020A9C0)}, /* [239] opt 365, STEPGUARD (+11) */
    {0, 0x2092, EOPID_INT(0x78), EOPID_INT(0x78), EOPID_INT(0x78), EOPID_PMF(func_0020A9C0)}, /* [240] opt 366, STEPGUARD (+11) */
    {0, 0x2092, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_PMF(func_0020A9C0)}, /* [241] opt 367, STEPGUARD (+11) */
    {0, 0x2092, EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_PMF(func_0020A9C0)}, /* [242] opt 368, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00102A88), EOPID_FN(func_00102B60), EOPID_INT(3), EOPID_NONE}, /* [243] opt 369, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00102C38), EOPID_FN(func_00102D30), EOPID_INT(5), EOPID_NONE}, /* [244] opt 370, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00102E28), EOPID_FN(func_00102F38), EOPID_INT(6), EOPID_NONE}, /* [245] opt 371, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103048), EOPID_FN(func_00103100), EOPID_INT(3), EOPID_NONE}, /* [246] opt 372, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_001031B8), EOPID_FN(func_00103270), EOPID_INT(3), EOPID_NONE}, /* [247] opt 373, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103328), EOPID_FN(func_001033E0), EOPID_INT(3), EOPID_NONE}, /* [248] opt 374, STEPGUARD (+11) */
    {0x1011, 0x92, EOPID_INT(0x350), EOPID_INT(0x350), EOPID_INT(0x350), EOPID_NONE}, /* [249] opt 375, STEPGUARD (+11) */
    {0x1011, 0x92, EOPID_INT(0x354), EOPID_INT(0x354), EOPID_INT(0x354), EOPID_NONE}, /* [250] opt 376, STEPGUARD (+11) */
    {0x1011, 0x92, EOPID_INT(0x358), EOPID_INT(0x358), EOPID_INT(0x358), EOPID_NONE}, /* [251] opt 377, STEPGUARD (+11) */
    {5, 0x124, EOPID_PMF(func_00206458), EOPID_PMF(func_00206488), EOPID_PMF(func_00206488), EOPID_NONE}, /* [252] opt 378, STEPGUARD (+11) */
    {5, 0x124, EOPID_PMF(func_002064A0), EOPID_PMF(SetStepguardHpLevel), EOPID_PMF(SetStepguardHpLevel), EOPID_NONE}, /* [253] opt 379, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x338), EOPID_INT(0x338), EOPID_INT(0x338), EOPID_NONE}, /* [254] opt 380, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x33C), EOPID_INT(0x33C), EOPID_INT(0x33C), EOPID_NONE}, /* [255] opt 381, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x340), EOPID_INT(0x340), EOPID_INT(0x340), EOPID_NONE}, /* [256] opt 382, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x340), EOPID_INT(0x340), EOPID_INT(0x340), EOPID_NONE}, /* [257] opt 383, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x344), EOPID_INT(0x344), EOPID_INT(0x344), EOPID_NONE}, /* [258] opt 384, STEPGUARD (+11) */
    {0x206D, 0x92, EOPID_INT(0x348), EOPID_INT(0x348), EOPID_INT(0x348), EOPID_NONE}, /* [259] opt 385, STEPGUARD (+11) */
    {0x2002, 0x22, EOPID_INT(0x38C), EOPID_PMF(func_00208538), EOPID_NONE, EOPID_NONE}, /* [260] opt 386, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103498), EOPID_INT(1), EOPID_NONE}, /* [261] opt 387, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103520), EOPID_FN(func_00103520), EOPID_INT(1), EOPID_NONE}, /* [262] opt 388, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_001035A8), EOPID_FN(func_001035A8), EOPID_INT(1), EOPID_NONE}, /* [263] opt 389, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103630), EOPID_FN(func_00103630), EOPID_NONE, EOPID_NONE}, /* [264] opt 390, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001036A8), EOPID_INT(1), EOPID_NONE}, /* [265] opt 391, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103730), EOPID_NONE, EOPID_NONE}, /* [266] opt 392, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_001037A8), EOPID_FN(func_001037A8), EOPID_INT(1), EOPID_NONE}, /* [267] opt 393, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103830), EOPID_FN(func_00103830), EOPID_INT(1), EOPID_NONE}, /* [268] opt 394, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_001038B8), EOPID_FN(func_001038B8), EOPID_NONE, EOPID_NONE}, /* [269] opt 395, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103930), EOPID_FN(func_00103930), EOPID_INT(1), EOPID_NONE}, /* [270] opt 396, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_001039B8), EOPID_FN(func_001039B8), EOPID_INT(1), EOPID_NONE}, /* [271] opt 397, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103A40), EOPID_FN(func_00103A40), EOPID_NONE, EOPID_NONE}, /* [272] opt 398, STEPGUARD (+11) */
    {0x2071, 0xA2, EOPID_INT(0x2E8), EOPID_PMF(func_0020A540), EOPID_INT(0x2E8), EOPID_NONE}, /* [273] opt 399, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x2CC), EOPID_INT(0x2CC), EOPID_INT(0x2CC), EOPID_NONE}, /* [274] opt 400, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x2D4), EOPID_INT(0x2D4), EOPID_INT(0x2D4), EOPID_NONE}, /* [275] opt 401, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x3D8), EOPID_INT(0x3D8), EOPID_INT(0x3D8), EOPID_NONE}, /* [276] opt 402, STEPGUARD (+11) */
    {2, 0x92, EOPID_INT(0x3E0), EOPID_INT(0x3E0), EOPID_INT(0x3E0), EOPID_NONE}, /* [277] opt 403, STEPGUARD (+11) */
    {0x9010, 4, EOPID_PMF(func_00209A88), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [278] opt 404, STEPGUARD (+11) */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_FStepguardHasSgf), EOPID_INT(1), EOPID_NONE}, /* [279] opt 405, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0xF38), EOPID_INT(0xF38), EOPID_INT(0xF38), EOPID_NONE}, /* [280] opt 406, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0xE64), EOPID_INT(0xE64), EOPID_INT(0xE64), EOPID_NONE}, /* [281] opt 407, STEPGUARD (+11) */
    {0x2075, 0x92, EOPID_INT(0xE60), EOPID_INT(0xE60), EOPID_INT(0xE60), EOPID_NONE}, /* [282] opt 408, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0xE7C), EOPID_INT(0xE7C), EOPID_INT(0xE7C), EOPID_NONE}, /* [283] opt 409, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x364), EOPID_INT(0x364), EOPID_INT(0x364), EOPID_NONE}, /* [284] opt 410, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x370), EOPID_INT(0x370), EOPID_INT(0x370), EOPID_NONE}, /* [285] opt 411, STEPGUARD (+11) */
    {1, 0x92, EOPID_INT(0x378), EOPID_INT(0x378), EOPID_INT(0x378), EOPID_NONE}, /* [286] opt 412, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x37C), EOPID_INT(0x37C), EOPID_INT(0x37C), EOPID_NONE}, /* [287] opt 413, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x380), EOPID_INT(0x380), EOPID_INT(0x380), EOPID_NONE}, /* [288] opt 345, STEPGUARD (+11) */
    {1, 0x92, EOPID_INT(0x388), EOPID_INT(0x388), EOPID_INT(0x388), EOPID_NONE}, /* [289] opt 346, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x368), EOPID_INT(0x368), EOPID_INT(0x368), EOPID_NONE}, /* [290] opt 414, STEPGUARD (+11) */
    {0, 0x12, EOPID_INT(0x4A0), EOPID_INT(0x4A0), EOPID_NONE, EOPID_NONE}, /* [291] opt 415, STEPGUARD (+11) */
    {1, 0x92, EOPID_INT(0x314), EOPID_INT(0x314), EOPID_INT(0x314), EOPID_NONE}, /* [292] opt 416, STEPGUARD (+11) */
    {1, 0x92, EOPID_INT(0x318), EOPID_INT(0x318), EOPID_INT(0x318), EOPID_NONE}, /* [293] opt 417, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0xE84), EOPID_INT(0xE84), EOPID_INT(0xE84), EOPID_NONE}, /* [294] opt 418, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0xE88), EOPID_INT(0xE88), EOPID_INT(0xE88), EOPID_NONE}, /* [295] opt 419, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x31C), EOPID_INT(0x31C), EOPID_INT(0x31C), EOPID_NONE}, /* [296] opt 420, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0x324), EOPID_INT(0x324), EOPID_INT(0x324), EOPID_NONE}, /* [297] opt 421, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0xF48), EOPID_INT(0xF48), EOPID_INT(0xF48), EOPID_NONE}, /* [298] opt 422, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0xF50), EOPID_INT(0xF50), EOPID_INT(0xF50), EOPID_NONE}, /* [299] opt 423, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0xF58), EOPID_INT(0xF58), EOPID_INT(0xF58), EOPID_NONE}, /* [300] opt 424, STEPGUARD (+11) */
    {0x2011, 0x120, EOPID_NONE, EOPID_PMF(func_0020A518), EOPID_PMF(func_0020A518), EOPID_NONE}, /* [301] opt 425, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103B38), EOPID_INT(2), EOPID_NONE}, /* [302] opt 426, STEPGUARD (+11) */
    {0x2016, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [303] opt 427, STEPGUARD (+11) */
    {0x2016, 0x92, EOPID_INT(0x3F4), EOPID_INT(0x3F4), EOPID_INT(0x3F4), EOPID_NONE}, /* [304] opt 428, STEPGUARD (+11) */
    {0x2016, 0x92, EOPID_INT(0x3F8), EOPID_INT(0x3F8), EOPID_INT(0x3F8), EOPID_NONE}, /* [305] opt 429, STEPGUARD (+11) */
    {0x2063, 0x92, EOPID_INT(0x494), EOPID_INT(0x494), EOPID_INT(0x494), EOPID_NONE}, /* [306] opt 430, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x490), EOPID_INT(0x490), EOPID_INT(0x490), EOPID_NONE}, /* [307] opt 431, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103C18), EOPID_FN(func_00103C18), EOPID_INT(1), EOPID_NONE}, /* [308] opt 432, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103CA0), EOPID_FN(func_00103CA0), EOPID_INT(1), EOPID_NONE}, /* [309] opt 433, STEPGUARD (+11) */
    {0x12, 0x401, EOPID_FN(func_00103D28), EOPID_FN(func_00103D28), EOPID_INT(1), EOPID_NONE}, /* [310] opt 434, STEPGUARD (+11) */
    {0x8006, 4, EOPID_PMF(func_001F1EB8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [311] opt 435, STEPGUARD (+11) */
    {0x1016, 0x2092, EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_PMF(PdetectorGetStepguard)}, /* [312] opt 436, STEPGUARD (+11) */
    {0, 0x2092, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_PMF(PdetectorGetStepguard)}, /* [313] opt 437, STEPGUARD (+11) */
    {0, 0x2092, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_PMF(PdetectorGetStepguard)}, /* [314] opt 438, STEPGUARD (+11) */
    {0x2016, 0x124, EOPID_PMFI(func_00259D60), EOPID_PMFI(func_00259D38), EOPID_PMFI(func_00259D38), EOPID_NONE}, /* [315] opt 439, STEPGUARD (+11) */
    {0x2016, 0x124, EOPID_PMFI(func_00259D70), EOPID_PMFI(func_00259D48), EOPID_PMFI(func_00259D48), EOPID_NONE}, /* [316] opt 440, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103DB0), EOPID_INT(1), EOPID_NONE}, /* [317] opt 441, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103E38), EOPID_INT(1), EOPID_NONE}, /* [318] opt 442, STEPGUARD (+11) */
    {0, 1, EOPID_NONE, EOPID_FN(func_00103EC0), EOPID_INT(1), EOPID_NONE}, /* [319] opt 443, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0xF30), EOPID_INT(0xF30), EOPID_INT(0xF30), EOPID_NONE}, /* [320] opt 444, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0xF34), EOPID_INT(0xF34), EOPID_INT(0xF34), EOPID_NONE}, /* [321] opt 445, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103F40), EOPID_INT(2), EOPID_NONE}, /* [322] opt 446, STEPGUARD (+11) */
    {0x1047, 0x92, EOPID_INT(0xE68), EOPID_INT(0xE68), EOPID_INT(0xE68), EOPID_NONE}, /* [323] opt 447, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0xF40), EOPID_INT(0xF40), EOPID_INT(0xF40), EOPID_NONE}, /* [324] opt 448, STEPGUARD (+11) */
    {0x2003, 0x92, EOPID_INT(0xF90), EOPID_INT(0xF90), EOPID_INT(0xF90), EOPID_NONE}, /* [325] opt 449, STEPGUARD (+11) */
    {5, 4, EOPID_PMF(func_0020A010), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [326] opt 450, STEPGUARD (+11) */
    {0x2003, 0x92, EOPID_INT(0xE74), EOPID_INT(0xE74), EOPID_INT(0xE74), EOPID_NONE}, /* [327] opt 451, STEPGUARD (+11) */
    {0x1062, 0x92, EOPID_INT(0xE78), EOPID_INT(0xE78), EOPID_INT(0xE78), EOPID_NONE}, /* [328] opt 452, STEPGUARD (+11) */
    {0xA, 0x92, EOPID_INT(0xE6C), EOPID_INT(0xE6C), EOPID_INT(0xE6C), EOPID_NONE}, /* [329] opt 453, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00103FE8), EOPID_NONE, EOPID_NONE}, /* [330] opt 454, STEPGUARD (+11) */
    {0x2024, 0x92, EOPID_INT(0xFEC), EOPID_INT(0xFEC), EOPID_INT(0xFEC), EOPID_NONE}, /* [331] opt 455, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x334), EOPID_INT(0x334), EOPID_INT(0x334), EOPID_NONE}, /* [332] opt 456, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x3D0), EOPID_INT(0x3D0), EOPID_INT(0x3D0), EOPID_NONE}, /* [333] opt 457, STEPGUARD (+11) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkStepguard_SetDetectorTargetStateById), EOPID_INT(2), EOPID_NONE}, /* [334] opt 458, STEPGUARD (+11) */
    {0x106B, 1, EOPID_NONE, EOPID_FN(RefThunkStepguard_GetDetectorTargetStateById), EOPID_INT(1), EOPID_NONE}, /* [335] opt 459, STEPGUARD (+11) */
    {0, 0x12, EOPID_INT(0xFC8), EOPID_INT(0xFC8), EOPID_NONE, EOPID_NONE}, /* [336] opt 460, STEPGUARD (+11) */
    {0x2002, 0x12, EOPID_INT(0xFCC), EOPID_INT(0xFCC), EOPID_NONE, EOPID_NONE}, /* [337] opt 461, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x4A4), EOPID_INT(0x4A4), EOPID_INT(0x4A4), EOPID_NONE}, /* [338] opt 462, STEPGUARD (+11) */
    {5, 0x92, EOPID_INT(0x3F0), EOPID_INT(0x3F0), EOPID_INT(0x3F0), EOPID_NONE}, /* [339] opt 463, STEPGUARD (+11) */
    {0, 0x92, EOPID_INT(0x3D4), EOPID_INT(0x3D4), EOPID_INT(0x3D4), EOPID_NONE}, /* [340] opt 464, STEPGUARD (+11) */
    {0x106F, 0x92, EOPID_INT(0x1000), EOPID_INT(0x1000), EOPID_INT(0x1000), EOPID_NONE}, /* [341] opt 465, STEPGUARD (+11) */
    {0x1016, 0x92, EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_NONE}, /* [342] opt 436, DETECTOR */
    {0, 0x92, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_NONE}, /* [343] opt 437, DETECTOR */
    {0, 0x92, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_NONE}, /* [344] opt 438, DETECTOR */
    {0x2016, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [345] opt 439, DETECTOR */
    {0x2016, 0x92, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_NONE}, /* [346] opt 440, DETECTOR */
    {0x106B, 1, EOPID_NONE, EOPID_FN(RefThunk_KCalculateDetectorTarget), EOPID_INT(2), EOPID_NONE}, /* [347] opt 466, DETECTOR */
    {0x2002, 0x92, EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_NONE}, /* [348] opt 467, DETECTOR */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(AddDetectorTarget), EOPID_PMF(AddDetectorTarget), EOPID_NONE}, /* [349] opt 468, DETECTOR */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(RemoveDetectorTarget), EOPID_PMF(RemoveDetectorTarget), EOPID_NONE}, /* [350] opt 469, DETECTOR */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkDetector_SetDetectorTargetStateById), EOPID_INT(2), EOPID_NONE}, /* [351] opt 458, DETECTOR */
    {0x106B, 1, EOPID_NONE, EOPID_FN(RefThunkDetector_GetDetectorTargetStateById), EOPID_INT(1), EOPID_NONE}, /* [352] opt 459, DETECTOR */
    {0xA, 0x92, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE}, /* [353] opt 361, CID120 */
    {0xA, 0x92, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE}, /* [354] opt 362, CID120 */
    {0xA, 0x92, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE}, /* [355] opt 363, CID120 */
    {1, 0x120, EOPID_NONE, EOPID_PMF(func_0020AF08), EOPID_PMF(func_0020AF08), EOPID_NONE}, /* [356] opt 364, CID120 */
    {1, 0x92, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_NONE}, /* [357] opt 365, CID120 */
    {0, 0x92, EOPID_INT(0x78), EOPID_INT(0x78), EOPID_INT(0x78), EOPID_NONE}, /* [358] opt 366, CID120 */
    {0, 0x92, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE}, /* [359] opt 470, CID120 */
    {1, 0x92, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE}, /* [360] opt 471, CID120 */
    {0, 0x92, EOPID_INT(0x74), EOPID_INT(0x74), EOPID_INT(0x74), EOPID_NONE}, /* [361] opt 472, CID120 */
    {0x12, 0x401, EOPID_FN(func_00104350), EOPID_FN(func_00104350), EOPID_INT(3), EOPID_NONE}, /* [362] opt 473, CID120 */
    {0, 0x92, EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_NONE}, /* [363] opt 368, CID120 */
    {0x12, 0x401, EOPID_FN(func_00104408), EOPID_FN(func_001044B0), EOPID_INT(2), EOPID_NONE}, /* [364] opt 369, CID120 */
    {0x12, 0x401, EOPID_FN(func_00104558), EOPID_FN(func_00104638), EOPID_INT(5), EOPID_NONE}, /* [365] opt 371, CID120 */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunkVolCHECK_POINT), EOPID_INT(1), EOPID_NONE}, /* [366] opt 108, VOL (+5) */
    {0, 1, EOPID_NONE, EOPID_FN(func_001047A0), EOPID_INT(2), EOPID_NONE}, /* [367] opt 474, VOL (+5) */
    {0x2088, 1, EOPID_NONE, EOPID_FN(RefThunkSmAPPLY), EOPID_INT(3), EOPID_NONE}, /* [368] opt 214, SM */
    {0x2088, 1, EOPID_NONE, EOPID_FN(RefThunk_PsmaApplySmEx), EOPID_INT(4), EOPID_NONE}, /* [369] opt 477, SM */
    {0x12, 0x400, EOPID_FN(RefThunk_SetSmPendingApply), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [370] opt 215, SM */
    {0x2030, 2, EOPID_INT(0x28), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [371] opt 251, SMA */
    {6, 0x24, EOPID_PMF(GetSmaGoal), EOPID_PMF(SetSmaGoal), EOPID_NONE, EOPID_NONE}, /* [372] opt 478, SMA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetSmaGoalIfReachable), EOPID_INT(1), EOPID_NONE}, /* [373] opt 479, SMA */
    {1, 1, EOPID_NONE, EOPID_FN(RefThunk_DtSmaToState), EOPID_INT(1), EOPID_NONE}, /* [374] opt 480, SMA */
    {6, 4, EOPID_PMF(func_001F07E8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [375] opt 481, SMA */
    {6, 4, EOPID_PMF(GetSmaNext), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [376] opt 482, SMA */
    {1, 0x22, EOPID_INT(0x44), EOPID_PMF(SetSmaSvt), EOPID_NONE, EOPID_NONE}, /* [377] opt 483, SMA */
    {0x2086, 2, EOPID_INT(0x30), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [378] opt 182, SMA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSmaSEEK), EOPID_INT(1), EOPID_NONE}, /* [379] opt 236, SMA */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSmaRETRACT), EOPID_NONE, EOPID_NONE}, /* [380] opt 237, SMA */
    {0x206F, 2, EOPID_INT(0x24), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [381] opt 484, SMA */
    {0x1054, 0x22, EOPID_INT(0x48), EOPID_PMF(SetSmaMask), EOPID_NONE, EOPID_NONE}, /* [382] opt 485, SMA */
    {1, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [383] opt 486, BOMB (+3) */
    {1, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [384] opt 487, BOMB (+3) */
    {0x1018, 0x92, EOPID_INT(0x150), EOPID_INT(0x150), EOPID_INT(0x150), EOPID_NONE}, /* [385] opt 488, BOMB (+3) */
    {0x1019, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [386] opt 489, BOMB (+3) */
    {0, 0x92, EOPID_INT(0x164), EOPID_INT(0x164), EOPID_INT(0x164), EOPID_NONE}, /* [387] opt 490, BOMB (+3) */
    {1, 0x92, EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_NONE}, /* [388] opt 491, BOMB (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkBombPRIME), EOPID_INT(1), EOPID_NONE}, /* [389] opt 492, BOMB (+3) */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkBombDETONATE), EOPID_NONE, EOPID_NONE}, /* [390] opt 493, BOMB (+3) */
    {0x1011, 0x92, EOPID_INT(0x1EC), EOPID_INT(0x1EC), EOPID_INT(0x1EC), EOPID_NONE}, /* [391] opt 494, BOMB (+3) */
    {1, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [392] opt 495, BOMB (+3) */
    {0, 0x82, EOPID_INT(0x1F4), EOPID_NONE, EOPID_INT(0x1F4), EOPID_NONE}, /* [393] opt 496, BOMB (+3) */
    {0x201E, 0x92, EOPID_INT(0x168), EOPID_INT(0x168), EOPID_INT(0x168), EOPID_NONE}, /* [394] opt 497, BOMB (+3) */
    {5, 0x92, EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_NONE}, /* [395] opt 498, BOMB (+3) */
    {0, 0x92, EOPID_INT(0x1E0), EOPID_INT(0x1E0), EOPID_INT(0x1E0), EOPID_NONE}, /* [396] opt 499, BOMB (+3) */
    {0x2002, 0x22, EOPID_INT(0x200), EOPID_PMF(func_0013BC60), EOPID_NONE, EOPID_NONE}, /* [397] opt 500, BOMB (+3) */
    {0, 0x92, EOPID_INT(0x1E4), EOPID_INT(0x1E4), EOPID_INT(0x1E4), EOPID_NONE}, /* [398] opt 501, BOMB (+3) */
    {0x2030, 2, EOPID_INT(0x1E8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [399] opt 502, BOMB (+3) */
    {0x2002, 0x12, EOPID_INT(0x210), EOPID_INT(0x210), EOPID_NONE, EOPID_NONE}, /* [400] opt 503, MISSILE (+2) */
    {2, 0x12, EOPID_INT(0x220), EOPID_INT(0x220), EOPID_NONE, EOPID_NONE}, /* [401] opt 504, MISSILE (+2) */
    {0, 0x92, EOPID_INT(0x264), EOPID_INT(0x264), EOPID_INT(0x264), EOPID_NONE}, /* [402] opt 505, MISSILE (+2) */
    {0x12, 0x400, EOPID_FN(func_00104D70), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [403] opt 506, MISSILE (+2) */
    {0x12, 0x400, EOPID_FN(func_00104DF8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [404] opt 507, MISSILE (+2) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_FireMissile), EOPID_INT(2), EOPID_NONE}, /* [405] opt 508, MISSILE (+2) */
    {1, 0x92, EOPID_INT(0x260), EOPID_INT(0x260), EOPID_INT(0x260), EOPID_NONE}, /* [406] opt 509, MISSILE (+2) */
    {1, 0x92, EOPID_INT(0x26C), EOPID_INT(0x26C), EOPID_INT(0x26C), EOPID_NONE}, /* [407] opt 510, MISSILE (+2) */
    {1, 0x92, EOPID_INT(0x274), EOPID_INT(0x274), EOPID_INT(0x274), EOPID_NONE}, /* [408] opt 511, ACCMISS */
    {1, 0x92, EOPID_INT(0x270), EOPID_INT(0x270), EOPID_INT(0x270), EOPID_NONE}, /* [409] opt 356, ACCMISS */
    {1, 0x92, EOPID_INT(0x270), EOPID_INT(0x270), EOPID_INT(0x270), EOPID_NONE}, /* [410] opt 512, CID82 */
    {1, 0x92, EOPID_INT(0x274), EOPID_INT(0x274), EOPID_INT(0x274), EOPID_NONE}, /* [411] opt 513, CID82 */
    {2, 0x92, EOPID_INT(0x280), EOPID_INT(0x280), EOPID_INT(0x280), EOPID_NONE}, /* [412] opt 514, CID82 */
    {0x101A, 0x82, EOPID_INT(0x2A8), EOPID_NONE, EOPID_INT(0x2A8), EOPID_NONE}, /* [413] opt 515, BUTTON */
    {0x2013, 0x92, EOPID_INT(0x2F4), EOPID_INT(0x2F4), EOPID_INT(0x2F4), EOPID_NONE}, /* [414] opt 516, BUTTON */
    {0x12, 0x400, EOPID_FN(RefThunk_AddButtonAseg), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [415] opt 517, BUTTON */
    {0x12, 0x400, EOPID_FN(RefThunk_AddButtonOffAseg), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [416] opt 518, BUTTON */
    {0x2030, 0x120, EOPID_NONE, EOPID_PMF(AddButtonPushObject), EOPID_PMF(AddButtonPushObject), EOPID_NONE}, /* [417] opt 519, BUTTON */
    {7, 0x120, EOPID_NONE, EOPID_PMF(func_00149248), EOPID_PMF(func_00149248), EOPID_NONE}, /* [418] opt 520, BUTTON */
    {1, 0x92, EOPID_INT(0x1E8), EOPID_INT(0x1E8), EOPID_INT(0x1E8), EOPID_NONE}, /* [419] opt 521, BUTTON */
    {1, 0x92, EOPID_INT(0x1DC), EOPID_INT(0x1DC), EOPID_INT(0x1DC), EOPID_NONE}, /* [420] opt 522, BUTTON */
    {0, 0x92, EOPID_INT(0x1EC), EOPID_INT(0x1EC), EOPID_INT(0x1EC), EOPID_NONE}, /* [421] opt 523, BUTTON */
    {0, 0x92, EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_NONE}, /* [422] opt 524, BUTTON */
    {0, 0x92, EOPID_INT(0x1F4), EOPID_INT(0x1F4), EOPID_INT(0x1F4), EOPID_NONE}, /* [423] opt 525, BUTTON */
    {0x12, 0x401, EOPID_FN(RefThunkButtonSET_SM_GOAL), EOPID_FN(RefThunkButtonSET_SM_GOAL), EOPID_INT(4), EOPID_NONE}, /* [424] opt 526, BUTTON */
    {0x12, 0x401, EOPID_FN(func_00105158), EOPID_FN(func_00105158), EOPID_INT(2), EOPID_NONE}, /* [425] opt 527, BUTTON */
    {0x101B, 0x22, EOPID_INT(0x1E0), EOPID_PMF(SetButtonButtons), EOPID_NONE, EOPID_NONE}, /* [426] opt 528, BUTTON */
    {0x2030, 2, EOPID_INT(0x2F8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [427] opt 529, BUTTON */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkButtonTRIGGER), EOPID_NONE, EOPID_NONE}, /* [428] opt 235, BUTTON */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkButtonUNTRIGGER), EOPID_NONE, EOPID_NONE}, /* [429] opt 530, BUTTON */
    {0x12, 0x400, EOPID_FN(RefThunk_AddVolbtnAseg), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [430] opt 517, VOLBTN (+1) */
    {0x12, 0x400, EOPID_FN(RefThunk_AddVolbtnOffAseg), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [431] opt 518, VOLBTN (+1) */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(AddVolbtnPushObject), EOPID_PMF(AddVolbtnPushObject), EOPID_NONE}, /* [432] opt 519, VOLBTN (+1) */
    {7, 0x120, EOPID_NONE, EOPID_PMF(func_001497E0), EOPID_PMF(func_001497E0), EOPID_NONE}, /* [433] opt 520, VOLBTN (+1) */
    {1, 0x92, EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_NONE}, /* [434] opt 521, VOLBTN (+1) */
    {1, 0x92, EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_NONE}, /* [435] opt 522, VOLBTN (+1) */
    {0, 0x92, EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_NONE}, /* [436] opt 523, VOLBTN (+1) */
    {0, 0x92, EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_NONE}, /* [437] opt 524, VOLBTN (+1) */
    {0, 0x92, EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_NONE}, /* [438] opt 525, VOLBTN (+1) */
    {0x12, 0x401, EOPID_FN(RefThunkVolbtnSET_SM_GOAL), EOPID_FN(RefThunkVolbtnSET_SM_GOAL), EOPID_INT(4), EOPID_NONE}, /* [439] opt 526, VOLBTN (+1) */
    {0x12, 0x401, EOPID_FN(func_00105508), EOPID_FN(func_00105508), EOPID_INT(2), EOPID_NONE}, /* [440] opt 527, VOLBTN (+1) */
    {0x2072, 0x92, EOPID_INT(0x1DC), EOPID_INT(0x1DC), EOPID_INT(0x1DC), EOPID_NONE}, /* [441] opt 531, VOLBTN (+1) */
    {0x101B, 0x22, EOPID_INT(0x1C0), EOPID_PMF(SetVolbtnButtons), EOPID_NONE, EOPID_NONE}, /* [442] opt 528, VOLBTN (+1) */
    {0, 0x82, EOPID_INT(0x2C8), EOPID_NONE, EOPID_INT(0x2C8), EOPID_NONE}, /* [443] opt 532, VOLBTN (+1) */
    {0xA002, 4, EOPID_PMF(func_00149848), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [444] opt 533, VOLBTN (+1) */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkVolbtnTRIGGER), EOPID_NONE, EOPID_NONE}, /* [445] opt 235, VOLBTN (+1) */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkVolbtnUNTRIGGER), EOPID_NONE, EOPID_NONE}, /* [446] opt 530, VOLBTN (+1) */
    {0x2002, 2, EOPID_INT(0x1D8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [447] opt 534, VOLBTN (+1) */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(AddSensorNoTriggerObject), EOPID_NONE}, /* [448] opt 535, SENSOR (+1) */
    {7, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(AddSensorNoTriggerClass), EOPID_NONE}, /* [449] opt 536, SENSOR (+1) */
    {0x2030, 0x92, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE}, /* [450] opt 537, LASEN */
    {0x2030, 0x92, EOPID_INT(0x194), EOPID_INT(0x194), EOPID_INT(0x194), EOPID_NONE}, /* [451] opt 538, LASEN */
    {0x2030, 0x92, EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_NONE}, /* [452] opt 539, LASEN */
    {0x2030, 0x92, EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_NONE}, /* [453] opt 540, LASEN */
    {1, 0x92, EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_NONE}, /* [454] opt 217, LASEN */
    {0x12, 0x400, EOPID_FN(RefThunk_AddLasenBeam), EOPID_NONE, EOPID_INT(0xA), EOPID_NONE}, /* [455] opt 541, LASEN */
    {0x12, 0x400, EOPID_FN(RefThunk_AddBarrierObject), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [456] opt 542, BARRIER */
    {0x12, 0x400, EOPID_FN(RefThunk_AddBarrierClass), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [457] opt 543, BARRIER */
    {0, 0x82, EOPID_INT(0x178), EOPID_NONE, EOPID_INT(0x178), EOPID_NONE}, /* [458] opt 544, BARRIER */
    {0x2030, 2, EOPID_INT(0x17C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [459] opt 534, BARRIER */
    {0x2002, 0x92, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE}, /* [460] opt 503, CID112 */
    {2, 0x92, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE}, /* [461] opt 504, CID112 */
    {9, 0x92, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE}, /* [462] opt 545, CID112 */
    {0xA, 0x92, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_NONE}, /* [463] opt 546, CID112 */
    {9, 0x92, EOPID_INT(0x90), EOPID_INT(0x90), EOPID_INT(0x90), EOPID_NONE}, /* [464] opt 547, CID112 */
    {0xA, 0x92, EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_NONE}, /* [465] opt 548, CID112 */
    {0xA, 0x92, EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_NONE}, /* [466] opt 549, CID112 */
    {1, 0x92, EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_NONE}, /* [467] opt 550, CID112 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_LaunchSoFromLauncher), EOPID_INT(1), EOPID_NONE}, /* [468] opt 508, CID112 */
    {5, 0x122, EOPID_INT(0xF4), EOPID_PMF(SetEmitterParticleCount__FP7EMITTERi), EOPID_PMF(SetEmitterParticleCount__FP7EMITTERi), EOPID_NONE}, /* [469] opt 551, EMITTER */
    {0, 0x122, EOPID_INT(0x114), EOPID_PMF(SetEmitterAutoPause__FP7EMITTERi), EOPID_PMF(SetEmitterAutoPause__FP7EMITTERi), EOPID_NONE}, /* [470] opt 552, EMITTER */
    {0x1025, 0x2092, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_PMF(PemitterEnsureEmitter__FP7EMITTER4ENSK)}, /* [471] opt 553, EMITTER */
    {0xA, 0x2092, EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_PMF(PemitterEnsureEmitter__FP7EMITTER4ENSK)}, /* [472] opt 554, EMITTER */
    {1, 0x2092, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_PMF(PemitterEnsureEmitter__FP7EMITTER4ENSK)}, /* [473] opt 555, EMITTER */
    {1, 0x2092, EOPID_INT(0x104), EOPID_INT(0x104), EOPID_INT(0x104), EOPID_PMF(PemitterEnsureEmitter__FP7EMITTER4ENSK)}, /* [474] opt 556, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x108), EOPID_INT(0x108), EOPID_INT(0x108), EOPID_PMF(PemitterEnsureEmitter__FP7EMITTER4ENSK)}, /* [475] opt 557, EMITTER */
    {0, 0x82, EOPID_INT(0x100), EOPID_NONE, EOPID_INT(0x100), EOPID_NONE}, /* [476] opt 558, EMITTER */
    {6, 0x82, EOPID_INT(0x118), EOPID_NONE, EOPID_INT(0x118), EOPID_NONE}, /* [477] opt 559, EMITTER */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkEmitterPAUSE), EOPID_INT(1), EOPID_NONE}, /* [478] opt 256, EMITTER */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkEmitterPAUSE_FOREVER), EOPID_NONE, EOPID_NONE}, /* [479] opt 560, EMITTER */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkEmitterUNPAUSE), EOPID_NONE, EOPID_NONE}, /* [480] opt 561, EMITTER */
    {0, 4, EOPID_PMF(FPausedEmitter__FP7EMITTER), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [481] opt 562, EMITTER */
    {0x1021, 0x104, EOPID_PMFT(func_002589F0, EMITTER), EOPID_NONE, EOPID_PMFT(func_002589A8, EMITTER), EOPID_NONE}, /* [482] opt 563, EMITTER */
    {0x1022, 0x104, EOPID_PMFT(func_00258AB8, EMITTER), EOPID_NONE, EOPID_PMFT(func_00258A00, EMITTER), EOPID_NONE}, /* [483] opt 564, EMITTER */
    {2, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(func_00258AC8, EMITTER)}, /* [484] opt 565, EMITTER */
    {2, 0x2082, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258B00, EMITTER)}, /* [485] opt 566, EMITTER */
    {2, 0x2082, EOPID_INT(0x10), EOPID_NONE, EOPID_INT(0x10), EOPID_PMFT(func_00258B00, EMITTER)}, /* [486] opt 567, EMITTER */
    {6, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(func_00258B38, EMITTER)}, /* [487] opt 569, EMITTER */
    {6, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [488] opt 570, EMITTER */
    {0x12, 0x400, EOPID_FN(func_00105AF0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [489] opt 571, EMITTER */
    {0x1023, 0x2082, EOPID_INT(0x14), EOPID_NONE, EOPID_INT(0x14), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [490] opt 572, EMITTER */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [491] opt 573, EMITTER */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [492] opt 574, EMITTER */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [493] opt 575, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x24), EOPID_INT(0x24), EOPID_INT(0x24), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [494] opt 576, EMITTER */
    {0x1024, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [495] opt 577, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [496] opt 578, EMITTER */
    {1, 0x2092, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [497] opt 579, EMITTER */
    {2, 0x2092, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [498] opt 580, EMITTER */
    {1, 0x2092, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [499] opt 581, EMITTER */
    {1, 0x2092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [500] opt 582, EMITTER */
    {2, 0x2092, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [501] opt 583, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x90), EOPID_INT(0x90), EOPID_INT(0x90), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [502] opt 325, EMITTER */
    {5, 0x2092, EOPID_INT(0x84), EOPID_INT(0x84), EOPID_INT(0x84), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [503] opt 584, EMITTER */
    {1, 0x2092, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [504] opt 585, EMITTER */
    {1, 0x2092, EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [505] opt 586, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x98), EOPID_INT(0x98), EOPID_INT(0x98), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [506] opt 587, EMITTER */
    {0xA, 0x2092, EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [507] opt 588, EMITTER */
    {9, 0x2092, EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_PMFT(PemitbEnsure, EMITTER)}, /* [508] opt 589, EMITTER */
    {0x101C, 0x104, EOPID_PMFT(func_00258C40, EMITTER), EOPID_NONE, EOPID_PMFT(func_00258B70, EMITTER), EOPID_NONE}, /* [509] opt 590, EMITTER */
    {0x101D, 0x2082, EOPID_INT(4), EOPID_NONE, EOPID_INT(4), EOPID_PMFT(func_00258920, EMITTER)}, /* [510] opt 591, EMITTER */
    {6, 0x2082, EOPID_INT(8), EOPID_NONE, EOPID_INT(8), EOPID_PMFT(func_00258920, EMITTER)}, /* [511] opt 592, EMITTER */
    {6, 0x2082, EOPID_INT(0xC), EOPID_NONE, EOPID_INT(0xC), EOPID_PMFT(func_00258920, EMITTER)}, /* [512] opt 593, EMITTER */
    {6, 0x2082, EOPID_INT(0x10), EOPID_NONE, EOPID_INT(0x10), EOPID_PMFT(func_00258920, EMITTER)}, /* [513] opt 594, EMITTER */
    {1, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFT(func_00258920, EMITTER)}, /* [514] opt 595, EMITTER */
    {0xA, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(func_00258920, EMITTER)}, /* [515] opt 596, EMITTER */
    {9, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMFT(func_00258920, EMITTER)}, /* [516] opt 597, EMITTER */
    {6, 0x2082, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258C60, EMITTER)}, /* [517] opt 598, EMITTER */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258C60, EMITTER)}, /* [518] opt 599, EMITTER */
    {0, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258CC8, EMITTER)}, /* [519] opt 600, EMITTER */
    {1, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258D30, EMITTER)}, /* [520] opt 601, EMITTER */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258D30, EMITTER)}, /* [521] opt 602, EMITTER */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258D30, EMITTER)}, /* [522] opt 603, EMITTER */
    {0x101F, 0x124, EOPID_PMFT(func_00258E40, EMITTER), EOPID_PMFT(func_00258D98, EMITTER), EOPID_PMFT(func_00258D98, EMITTER), EOPID_NONE}, /* [523] opt 604, EMITTER */
    {0x101E, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258898, EMITTER)}, /* [524] opt 605, EMITTER */
    {0, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258898, EMITTER)}, /* [525] opt 606, EMITTER */
    {6, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFT(func_00258898, EMITTER)}, /* [526] opt 607, EMITTER */
    {6, 0x2082, EOPID_INT(0x14), EOPID_NONE, EOPID_INT(0x14), EOPID_PMFT(func_00258898, EMITTER)}, /* [527] opt 608, EMITTER */
    {1, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(func_00258898, EMITTER)}, /* [528] opt 609, EMITTER */
    {9, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMFT(func_00258898, EMITTER)}, /* [529] opt 611, EMITTER */
    {0x1020, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258E60, EMITTER)}, /* [530] opt 612, EMITTER */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258E60, EMITTER)}, /* [531] opt 613, EMITTER */
    {0xA, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258E60, EMITTER)}, /* [532] opt 614, EMITTER */
    {1, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258EC8, EMITTER)}, /* [533] opt 615, EMITTER */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkExplEXPLODE), EOPID_NONE, EOPID_NONE}, /* [534] opt 244, EXPL (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkExplEXPLODE_OVR), EOPID_INT(6), EOPID_NONE}, /* [535] opt 616, EXPL (+3) */
    {0x1022, 0x104, EOPID_PMFT(func_00258AB8, EXPLO), EOPID_NONE, EOPID_PMFT(func_00258A00, EXPLO), EOPID_NONE}, /* [536] opt 564, EXPLO (+1) */
    {2, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(func_00258AC8, EXPLO)}, /* [537] opt 565, EXPLO (+1) */
    {2, 0x2082, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258B00, EXPLO)}, /* [538] opt 566, EXPLO (+1) */
    {2, 0x2082, EOPID_INT(0x10), EOPID_NONE, EOPID_INT(0x10), EOPID_PMFT(func_00258B00, EXPLO)}, /* [539] opt 567, EXPLO (+1) */
    {6, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(func_00258B38, EXPLO)}, /* [540] opt 569, EXPLO (+1) */
    {6, 0x2082, EOPID_INT(0x20), EOPID_NONE, EOPID_INT(0x20), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [541] opt 570, EXPLO (+1) */
    {0x1023, 0x2082, EOPID_INT(0x14), EOPID_NONE, EOPID_INT(0x14), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [542] opt 572, EXPLO (+1) */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [543] opt 573, EXPLO (+1) */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [544] opt 574, EXPLO (+1) */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [545] opt 575, EXPLO (+1) */
    {0xA, 0x2092, EOPID_INT(0x24), EOPID_INT(0x24), EOPID_INT(0x24), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [546] opt 576, EXPLO (+1) */
    {0x1024, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [547] opt 577, EXPLO (+1) */
    {0xA, 0x2092, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [548] opt 578, EXPLO (+1) */
    {1, 0x2092, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [549] opt 579, EXPLO (+1) */
    {0xA, 0x2092, EOPID_INT(0x90), EOPID_INT(0x90), EOPID_INT(0x90), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [550] opt 325, EXPLO (+1) */
    {5, 0x2092, EOPID_INT(0x84), EOPID_INT(0x84), EOPID_INT(0x84), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [551] opt 584, EXPLO (+1) */
    {1, 0x2092, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [552] opt 585, EXPLO (+1) */
    {0xA, 0x2092, EOPID_INT(0x98), EOPID_INT(0x98), EOPID_INT(0x98), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [553] opt 587, EXPLO (+1) */
    {0x101C, 0x104, EOPID_PMFT(func_00258C40, EXPLO), EOPID_NONE, EOPID_PMFT(func_00258B70, EXPLO), EOPID_NONE}, /* [554] opt 590, EXPLO (+1) */
    {0xA, 0x92, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_NONE}, /* [555] opt 551, EXPLS */
    {0, 0x92, EOPID_INT(0xFC), EOPID_INT(0xFC), EOPID_INT(0xFC), EOPID_NONE}, /* [556] opt 617, EXPLS */
    {0x1021, 0x104, EOPID_PMFT(func_002589F0, EXPLO), EOPID_NONE, EOPID_PMFT(func_002589A8, EXPLO), EOPID_NONE}, /* [557] opt 563, EXPLS */
    {2, 0x2092, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [558] opt 580, EXPLS */
    {1, 0x2092, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [559] opt 581, EXPLS */
    {1, 0x2092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [560] opt 582, EXPLS */
    {2, 0x2092, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [561] opt 583, EXPLS */
    {1, 0x2092, EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [562] opt 586, EXPLS */
    {0xA, 0x2092, EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [563] opt 588, EXPLS */
    {9, 0x2092, EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_INT(0xB0), EOPID_PMFT(PemitbEnsure, EXPLO)}, /* [564] opt 589, EXPLS */
    {0x101D, 0x2082, EOPID_INT(4), EOPID_NONE, EOPID_INT(4), EOPID_PMFT(func_00258920, EXPLO)}, /* [565] opt 591, EXPLS */
    {6, 0x2082, EOPID_INT(8), EOPID_NONE, EOPID_INT(8), EOPID_PMFT(func_00258920, EXPLO)}, /* [566] opt 592, EXPLS */
    {6, 0x2082, EOPID_INT(0xC), EOPID_NONE, EOPID_INT(0xC), EOPID_PMFT(func_00258920, EXPLO)}, /* [567] opt 593, EXPLS */
    {6, 0x2082, EOPID_INT(0x10), EOPID_NONE, EOPID_INT(0x10), EOPID_PMFT(func_00258920, EXPLO)}, /* [568] opt 594, EXPLS */
    {1, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFT(func_00258920, EXPLO)}, /* [569] opt 595, EXPLS */
    {0xA, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(func_00258920, EXPLO)}, /* [570] opt 596, EXPLS */
    {9, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMFT(func_00258920, EXPLO)}, /* [571] opt 597, EXPLS */
    {6, 0x2082, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258C60, EXPLO)}, /* [572] opt 598, EXPLS */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258C60, EXPLO)}, /* [573] opt 599, EXPLS */
    {0, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258CC8, EXPLO)}, /* [574] opt 600, EXPLS */
    {1, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258D30, EXPLO)}, /* [575] opt 601, EXPLS */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258D30, EXPLO)}, /* [576] opt 602, EXPLS */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258D30, EXPLO)}, /* [577] opt 603, EXPLS */
    {0x101F, 0x124, EOPID_PMFT(func_00258E40, EXPLO), EOPID_PMFT(func_00258D98, EXPLO), EOPID_PMFT(func_00258D98, EXPLO), EOPID_NONE}, /* [578] opt 604, EXPLS */
    {0x101E, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258898, EXPLO)}, /* [579] opt 605, EXPLS */
    {0, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258898, EXPLO)}, /* [580] opt 606, EXPLS */
    {6, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFT(func_00258898, EXPLO)}, /* [581] opt 607, EXPLS */
    {6, 0x2082, EOPID_INT(0x14), EOPID_NONE, EOPID_INT(0x14), EOPID_PMFT(func_00258898, EXPLO)}, /* [582] opt 608, EXPLS */
    {1, 0x2092, EOPID_INT(0x18), EOPID_INT(0x18), EOPID_INT(0x18), EOPID_PMFT(func_00258898, EXPLO)}, /* [583] opt 609, EXPLS */
    {9, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMFT(func_00258898, EXPLO)}, /* [584] opt 611, EXPLS */
    {0x1020, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258E60, EXPLO)}, /* [585] opt 612, EXPLS */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFT(func_00258E60, EXPLO)}, /* [586] opt 613, EXPLS */
    {0xA, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFT(func_00258E60, EXPLO)}, /* [587] opt 614, EXPLS */
    {1, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFT(func_00258EC8, EXPLO)}, /* [588] opt 615, EXPLS */
    {0xE, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_0015C650)}, /* [589] opt 209, EXPLS */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMF(func_0015C650)}, /* [590] opt 618, EXPLS */
    {1, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMF(func_0015C650)}, /* [591] opt 619, EXPLS */
    {1, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMF(func_0015C650)}, /* [592] opt 620, EXPLS */
    {1, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMF(func_0015C650)}, /* [593] opt 621, EXPLS */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00105CD0), EOPID_INT(1), EOPID_NONE}, /* [594] opt 214, CID113 */
    {0x12, 0x401, EOPID_FN(func_00105D58), EOPID_FN(func_00105D58), EOPID_INT(1), EOPID_NONE}, /* [595] opt 622, CID113 */
    {0x12, 0x401, EOPID_FN(func_00105DE0), EOPID_FN(func_00105DE0), EOPID_INT(1), EOPID_NONE}, /* [596] opt 623, CID113 */
    {0x12, 0x401, EOPID_FN(func_00105E70), EOPID_FN(func_00105E70), EOPID_INT(1), EOPID_NONE}, /* [597] opt 624, CID113 */
    {0x12, 0x401, EOPID_FN(func_00105F00), EOPID_FN(func_00105F00), EOPID_INT(1), EOPID_NONE}, /* [598] opt 292, CID113 */
    {0x12, 0x401, EOPID_FN(func_00105F88), EOPID_FN(func_00105F88), EOPID_INT(1), EOPID_NONE}, /* [599] opt 243, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106010), EOPID_FN(func_00106010), EOPID_INT(1), EOPID_NONE}, /* [600] opt 256, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106098), EOPID_FN(func_00106098), EOPID_INT(3), EOPID_NONE}, /* [601] opt 625, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106150), EOPID_FN(func_00106150), EOPID_INT(3), EOPID_NONE}, /* [602] opt 626, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106208), EOPID_FN(func_00106208), EOPID_INT(1), EOPID_NONE}, /* [603] opt 627, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106290), EOPID_FN(func_00106290), EOPID_INT(1), EOPID_NONE}, /* [604] opt 242, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106318), EOPID_FN(func_00106318), EOPID_INT(1), EOPID_NONE}, /* [605] opt 236, CID113 */
    {0x12, 0x401, EOPID_FN(func_001063A0), EOPID_FN(func_001063A0), EOPID_INT(2), EOPID_NONE}, /* [606] opt 628, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106448), EOPID_FN(func_00106448), EOPID_NONE, EOPID_NONE}, /* [607] opt 629, CID113 */
    {0x12, 0x401, EOPID_FN(func_001064C0), EOPID_FN(func_001064C0), EOPID_NONE, EOPID_NONE}, /* [608] opt 630, CID113 */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [609] opt 631, CID113 */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [610] opt 632, CID113 */
    {0x12, 0x401, EOPID_FN(func_00106538), EOPID_FN(func_00106538), EOPID_INT(2), EOPID_NONE}, /* [611] opt 633, CID113 */
    {0x12, 0x401, EOPID_FN(func_001065E0), EOPID_FN(func_001065E0), EOPID_INT(2), EOPID_NONE}, /* [612] opt 634, CID113 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkRelayTRIGGER), EOPID_INT(1), EOPID_NONE}, /* [613] opt 235, RELAY */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_00106710), EOPID_NONE, EOPID_NONE}, /* [614] opt 638, RELAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkRelayUNTRIGGER), EOPID_NONE, EOPID_NONE}, /* [615] opt 530, RELAY */
    {0, 4, EOPID_PMF(FRelayFiring), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [616] opt 639, RELAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_EnsureRelayAlarm), EOPID_INT(1), EOPID_NONE}, /* [617] opt 640, RELAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_RemoveRelayAlarm), EOPID_INT(1), EOPID_NONE}, /* [618] opt 641, RELAY */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_FRelayHasAlarm), EOPID_INT(1), EOPID_NONE}, /* [619] opt 642, RELAY */
    {0x2002, 2, EOPID_INT(0x6C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [620] opt 643, RELAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ClearRelayAlarms), EOPID_NONE, EOPID_NONE}, /* [621] opt 644, RELAY */
    {5, 0x102, EOPID_INT(0x58), EOPID_NONE, EOPID_PMF(func_001E01C8), EOPID_NONE}, /* [622] opt 645, RELAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ResetRelay), EOPID_NONE, EOPID_NONE}, /* [623] opt 646, RELAY */
    {0x2002, 0x24, EOPID_PMF(func_001E0128), EOPID_PMF(func_001E0158), EOPID_NONE, EOPID_NONE}, /* [624] opt 647, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayBlockBegin), EOPID_FN(RefThunk_AddRelayBlockBegin), EOPID_INT(1), EOPID_NONE}, /* [625] opt 648, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayBlockEnd), EOPID_FN(RefThunk_AddRelayBlockEnd), EOPID_NONE, EOPID_NONE}, /* [626] opt 649, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStopBlock), EOPID_FN(RefThunk_AddRelayStopBlock), EOPID_INT(1), EOPID_NONE}, /* [627] opt 650, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWaitBlocks), EOPID_FN(RefThunk_AddRelayWaitBlocks), EOPID_INT(1), EOPID_NONE}, /* [628] opt 651, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWaitAlarm), EOPID_FN(RefThunk_AddRelayWaitAlarm), EOPID_INT(2), EOPID_NONE}, /* [629] opt 652, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRaiseAlarm), EOPID_FN(RefThunk_AddRelayRaiseAlarm), EOPID_INT(2), EOPID_NONE}, /* [630] opt 634, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStopRelay), EOPID_FN(RefThunk_AddRelayStopRelay), EOPID_INT(1), EOPID_NONE}, /* [631] opt 653, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySetCtlState), EOPID_FN(RefThunk_AddRelaySetCtlState), EOPID_INT(1), EOPID_NONE}, /* [632] opt 654, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySetActor), EOPID_FN(RefThunk_AddRelaySetActor), EOPID_INT(1), EOPID_NONE}, /* [633] opt 655, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySignal), EOPID_FN(RefThunk_AddRelaySignal), EOPID_INT(1), EOPID_NONE}, /* [634] opt 243, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayPlayAseg), EOPID_FN(RefThunk_AddRelayPlayAseg), EOPID_INT(4), EOPID_NONE}, /* [635] opt 627, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayFireRelay), EOPID_FN(RefThunk_AddRelayFireRelay), EOPID_INT(3), EOPID_NONE}, /* [636] opt 656, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayCONTEXT), EOPID_FN(RefThunkRelayCONTEXT), EOPID_INT(1), EOPID_NONE}, /* [637] opt 622, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelaySET_SM_GOAL), EOPID_FN(RefThunkRelaySET_SM_GOAL), EOPID_INT(5), EOPID_NONE}, /* [638] opt 526, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWidgetShow), EOPID_FN(RefThunk_AddRelayWidgetShow), EOPID_INT(3), EOPID_NONE}, /* [639] opt 657, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWidgetHide), EOPID_FN(RefThunk_AddRelayWidgetHide), EOPID_INT(1), EOPID_NONE}, /* [640] opt 658, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayCLOCK), EOPID_FN(RefThunkRelayCLOCK), EOPID_INT(1), EOPID_NONE}, /* [641] opt 659, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySetTaskState), EOPID_FN(RefThunk_AddRelaySetTaskState), EOPID_INT(2), EOPID_NONE}, /* [642] opt 527, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayHandsOff), EOPID_FN(RefThunk_AddRelayHandsOff), EOPID_INT(2), EOPID_NONE}, /* [643] opt 221, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayLABEL), EOPID_FN(RefThunkRelayLABEL), EOPID_INT(1), EOPID_NONE}, /* [644] opt 242, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelaySEEK), EOPID_FN(RefThunkRelaySEEK), EOPID_INT(1), EOPID_NONE}, /* [645] opt 236, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayGotoLabelIf), EOPID_FN(RefThunk_AddRelayGotoLabelIf), EOPID_INT(2), EOPID_NONE}, /* [646] opt 660, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayCUT), EOPID_FN(RefThunkRelayCUT), EOPID_INT(3), EOPID_NONE}, /* [647] opt 292, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayCutCamera), EOPID_FN(RefThunk_AddRelayCutCamera), EOPID_NONE, EOPID_NONE}, /* [648] opt 661, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayTUNNEL), EOPID_FN(RefThunkRelayTUNNEL), EOPID_INT(2), EOPID_NONE}, /* [649] opt 662, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayFOCUS), EOPID_FN(RefThunkRelayFOCUS), EOPID_INT(2), EOPID_NONE}, /* [650] opt 663, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayPipeTo), EOPID_FN(RefThunk_AddRelayPipeTo), EOPID_INT(4), EOPID_NONE}, /* [651] opt 664, RELAY */
    {0x12, 0x401, EOPID_FN(func_00107B18), EOPID_FN(func_00107B18), EOPID_INT(1), EOPID_NONE}, /* [652] opt 665, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayHeroState), EOPID_FN(RefThunk_AddRelayHeroState), EOPID_INT(4), EOPID_NONE}, /* [653] opt 666, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd1D), EOPID_FN(RefThunk_AddRelayStepCmd1D), EOPID_INT(2), EOPID_NONE}, /* [654] opt 667, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd1E), EOPID_FN(RefThunk_AddRelayStepCmd1E), EOPID_INT(3), EOPID_NONE}, /* [655] opt 668, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd1F), EOPID_FN(RefThunk_AddRelayStepCmd1F), EOPID_INT(2), EOPID_NONE}, /* [656] opt 669, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd20), EOPID_FN(RefThunk_AddRelayStepCmd20), EOPID_INT(2), EOPID_NONE}, /* [657] opt 670, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd21), EOPID_FN(RefThunk_AddRelayStepCmd21), EOPID_INT(2), EOPID_NONE}, /* [658] opt 671, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd22), EOPID_FN(RefThunk_AddRelayStepCmd22), EOPID_INT(4), EOPID_NONE}, /* [659] opt 672, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayPAUSE), EOPID_FN(RefThunkRelayPAUSE), EOPID_INT(3), EOPID_NONE}, /* [660] opt 256, RELAY */
    {0x12, 0x401, EOPID_FN(func_00108140), EOPID_FN(func_00108140), EOPID_INT(6), EOPID_NONE}, /* [661] opt 673, RELAY */
    {0x12, 0x401, EOPID_FN(func_00108230), EOPID_FN(func_00108230), EOPID_INT(6), EOPID_NONE}, /* [662] opt 674, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRunOn), EOPID_FN(RefThunk_AddRelayRunOn), EOPID_NONE, EOPID_NONE}, /* [663] opt 631, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRunOff), EOPID_FN(RefThunk_AddRelayRunOff), EOPID_NONE, EOPID_NONE}, /* [664] opt 632, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayFollowPath), EOPID_FN(RefThunk_AddRelayFollowPath), EOPID_INT(6), EOPID_NONE}, /* [665] opt 625, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWalkTo), EOPID_FN(RefThunk_AddRelayWalkTo), EOPID_INT(6), EOPID_NONE}, /* [666] opt 626, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStepCmd29), EOPID_FN(RefThunk_AddRelayStepCmd29), EOPID_INT(5), EOPID_NONE}, /* [667] opt 675, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRespawn), EOPID_FN(RefThunk_AddRelayRespawn), EOPID_INT(5), EOPID_NONE}, /* [668] opt 628, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayLaunchTo), EOPID_FN(RefThunk_AddRelayLaunchTo), EOPID_INT(5), EOPID_NONE}, /* [669] opt 676, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayAddObject), EOPID_FN(RefThunk_AddRelayAddObject), EOPID_INT(3), EOPID_NONE}, /* [670] opt 238, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRemoveObject), EOPID_FN(RefThunk_AddRelayRemoveObject), EOPID_INT(3), EOPID_NONE}, /* [671] opt 239, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySetPartFlag), EOPID_FN(RefThunk_AddRelaySetPartFlag), EOPID_INT(2), EOPID_NONE}, /* [672] opt 110, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayClearPartFlag), EOPID_FN(RefThunk_AddRelayClearPartFlag), EOPID_INT(2), EOPID_NONE}, /* [673] opt 111, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayShowPart), EOPID_FN(RefThunk_AddRelayShowPart), EOPID_INT(2), EOPID_NONE}, /* [674] opt 112, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayHidePart), EOPID_FN(RefThunk_AddRelayHidePart), EOPID_INT(2), EOPID_NONE}, /* [675] opt 113, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayJOLT), EOPID_FN(RefThunkRelayJOLT), EOPID_INT(2), EOPID_NONE}, /* [676] opt 233, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayRumble), EOPID_FN(RefThunk_AddRelayRumble), EOPID_INT(3), EOPID_NONE}, /* [677] opt 234, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunkRelayEXPLODE), EOPID_FN(RefThunkRelayEXPLODE), EOPID_INT(4), EOPID_NONE}, /* [678] opt 244, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayEyes), EOPID_FN(RefThunk_AddRelayEyes), EOPID_INT(3), EOPID_NONE}, /* [679] opt 247, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySound), EOPID_FN(RefThunk_AddRelaySound), EOPID_INT(7), EOPID_NONE}, /* [680] opt 231, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStopSound), EOPID_FN(RefThunk_AddRelayStopSound), EOPID_INT(1), EOPID_NONE}, /* [681] opt 232, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayCamBlendFrom), EOPID_FN(RefThunk_AddRelayCamBlendFrom), EOPID_INT(5), EOPID_NONE}, /* [682] opt 677, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayCamBlendTo), EOPID_FN(RefThunk_AddRelayCamBlendTo), EOPID_INT(5), EOPID_NONE}, /* [683] opt 678, RELAY */
    {0x12, 0x401, EOPID_FN(func_001092D0), EOPID_FN(func_001092D0), EOPID_INT(1), EOPID_NONE}, /* [684] opt 679, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStartJtMode), EOPID_FN(RefThunk_AddRelayStartJtMode), EOPID_INT(4), EOPID_NONE}, /* [685] opt 680, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayEndJtMode), EOPID_FN(RefThunk_AddRelayEndJtMode), EOPID_INT(1), EOPID_NONE}, /* [686] opt 681, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayTweenPosObj), EOPID_FN(RefThunk_AddRelayTweenPosObj), EOPID_INT(2), EOPID_NONE}, /* [687] opt 682, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayTweenAimObj), EOPID_FN(RefThunk_AddRelayTweenAimObj), EOPID_INT(2), EOPID_NONE}, /* [688] opt 683, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayTweenTarget), EOPID_FN(RefThunk_AddRelayTweenTarget), EOPID_INT(3), EOPID_NONE}, /* [689] opt 684, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayTweenTime), EOPID_FN(RefThunk_AddRelayTweenTime), EOPID_INT(2), EOPID_NONE}, /* [690] opt 685, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayTweenStart), EOPID_FN(RefThunk_AddRelayTweenStart), EOPID_NONE, EOPID_NONE}, /* [691] opt 686, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayButtonPrompt), EOPID_FN(RefThunk_AddRelayButtonPrompt), EOPID_INT(2), EOPID_NONE}, /* [692] opt 687, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109868), EOPID_FN(func_00109868), EOPID_INT(2), EOPID_NONE}, /* [693] opt 688, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStopLine), EOPID_FN(RefThunk_AddRelayStopLine), EOPID_NONE, EOPID_NONE}, /* [694] opt 689, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayLine), EOPID_FN(RefThunk_AddRelayLine), EOPID_INT(3), EOPID_NONE}, /* [695] opt 690, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayClearSubtitle), EOPID_FN(RefThunk_AddRelayClearSubtitle), EOPID_NONE, EOPID_NONE}, /* [696] opt 691, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109AB8), EOPID_FN(func_00109AB8), EOPID_INT(3), EOPID_NONE}, /* [697] opt 692, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109B70), EOPID_FN(func_00109B70), EOPID_INT(3), EOPID_NONE}, /* [698] opt 693, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109C28), EOPID_FN(func_00109C28), EOPID_INT(1), EOPID_NONE}, /* [699] opt 694, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109CB0), EOPID_FN(func_00109CB0), EOPID_INT(4), EOPID_NONE}, /* [700] opt 695, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayPlayAsegDefault), EOPID_FN(RefThunk_AddRelayPlayAsegDefault), EOPID_INT(2), EOPID_NONE}, /* [701] opt 696, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStartMusic), EOPID_FN(RefThunk_AddRelayStartMusic), EOPID_INT(1), EOPID_NONE}, /* [702] opt 273, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayStopMusic), EOPID_FN(RefThunk_AddRelayStopMusic), EOPID_INT(1), EOPID_NONE}, /* [703] opt 274, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayWaitTimer), EOPID_FN(RefThunk_AddRelayWaitTimer), EOPID_INT(1), EOPID_NONE}, /* [704] opt 697, RELAY */
    {0x12, 0x401, EOPID_FN(func_00109FB8), EOPID_FN(func_00109FB8), EOPID_INT(6), EOPID_NONE}, /* [705] opt 698, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A0A8), EOPID_FN(func_0010A0A8), EOPID_INT(1), EOPID_NONE}, /* [706] opt 699, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A130), EOPID_FN(func_0010A130), EOPID_INT(1), EOPID_NONE}, /* [707] opt 700, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A1B8), EOPID_FN(func_0010A1B8), EOPID_INT(1), EOPID_NONE}, /* [708] opt 701, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayHintBox), EOPID_FN(RefThunk_AddRelayHintBox), EOPID_INT(3), EOPID_NONE}, /* [709] opt 702, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayClearHintBox), EOPID_FN(RefThunk_AddRelayClearHintBox), EOPID_INT(1), EOPID_NONE}, /* [710] opt 703, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelaySetSsgGoal), EOPID_FN(RefThunk_AddRelaySetSsgGoal), EOPID_INT(3), EOPID_NONE}, /* [711] opt 704, RELAY */
    {0x12, 0x401, EOPID_FN(RefThunk_AddRelayResetStepGoal), EOPID_FN(RefThunk_AddRelayResetStepGoal), EOPID_INT(2), EOPID_NONE}, /* [712] opt 705, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A4E0), EOPID_FN(func_0010A4E0), EOPID_INT(1), EOPID_NONE}, /* [713] opt 706, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A568), EOPID_FN(func_0010A568), EOPID_INT(3), EOPID_NONE}, /* [714] opt 707, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A620), EOPID_FN(func_0010A620), EOPID_INT(3), EOPID_NONE}, /* [715] opt 708, RELAY */
    {0x12, 0x401, EOPID_FN(func_0010A6D8), EOPID_FN(func_0010A6D8), EOPID_INT(3), EOPID_NONE}, /* [716] opt 709, RELAY */
    {0x2002, 0x92, EOPID_INT(0x420), EOPID_INT(0x420), EOPID_INT(0x420), EOPID_NONE}, /* [717] opt 710, STEPPLAYER (+3) */
    {1, 0x92, EOPID_INT(0x424), EOPID_INT(0x424), EOPID_INT(0x424), EOPID_NONE}, /* [718] opt 711, STEPPLAYER (+3) */
    {1, 0x92, EOPID_INT(0x103C), EOPID_INT(0x103C), EOPID_INT(0x103C), EOPID_NONE}, /* [719] opt 712, STEPPLAYER (+3) */
    {2, 0x92, EOPID_INT(0x3B0), EOPID_INT(0x3B0), EOPID_INT(0x3B0), EOPID_NONE}, /* [720] opt 713, STEPPLAYER (+3) */
    {1, 0x92, EOPID_INT(0x3C8), EOPID_INT(0x3C8), EOPID_INT(0x3C8), EOPID_NONE}, /* [721] opt 714, STEPPLAYER (+3) */
    {0xA, 0x92, EOPID_INT(0x3CC), EOPID_INT(0x3CC), EOPID_INT(0x3CC), EOPID_NONE}, /* [722] opt 715, STEPPLAYER (+3) */
    {1, 0x92, EOPID_INT(0x3D4), EOPID_INT(0x3D4), EOPID_INT(0x3D4), EOPID_NONE}, /* [723] opt 716, STEPPLAYER (+3) */
    {0xA, 0x92, EOPID_INT(0x3D8), EOPID_INT(0x3D8), EOPID_INT(0x3D8), EOPID_NONE}, /* [724] opt 717, STEPPLAYER (+3) */
    {0, 0x92, EOPID_INT(0x10F0), EOPID_INT(0x10F0), EOPID_INT(0x10F0), EOPID_NONE}, /* [725] opt 410, STEPPLAYER (+3) */
    {5, 0x92, EOPID_INT(0xDFC), EOPID_INT(0xDFC), EOPID_INT(0xDFC), EOPID_NONE}, /* [726] opt 380, STEPPLAYER (+3) */
    {5, 0x24, EOPID_PMFI(func_00258280), EOPID_PMF(SetStepplayerHp), EOPID_NONE, EOPID_NONE}, /* [727] opt 381, STEPPLAYER (+3) */
    {5, 0x92, EOPID_INT(0x1300), EOPID_INT(0x1300), EOPID_INT(0x1300), EOPID_NONE}, /* [728] opt 718, STEPPLAYER (+3) */
    {5, 0x24, EOPID_PMF(MpStepplayerCur), EOPID_PMF(SetStepplayerMp), EOPID_NONE, EOPID_NONE}, /* [729] opt 719, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010A790), EOPID_INT(3), EOPID_NONE}, /* [730] opt 720, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010A848), EOPID_INT(1), EOPID_NONE}, /* [731] opt 667, STEPPLAYER (+3) */
    {0, 1, EOPID_NONE, EOPID_FN(func_0010A8D0), EOPID_NONE, EOPID_NONE}, /* [732] opt 721, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_StartStepplayerFollow), EOPID_INT(2), EOPID_NONE}, /* [733] opt 446, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010A9E8), EOPID_INT(2), EOPID_NONE}, /* [734] opt 722, STEPPLAYER (+3) */
    {0, 1, EOPID_NONE, EOPID_FN(func_0010AA90), EOPID_NONE, EOPID_NONE}, /* [735] opt 723, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010AB00), EOPID_INT(1), EOPID_NONE}, /* [736] opt 724, STEPPLAYER (+3) */
    {0, 1, EOPID_NONE, EOPID_FN(func_0010AB88), EOPID_NONE, EOPID_NONE}, /* [737] opt 725, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010ABF8), EOPID_INT(1), EOPID_NONE}, /* [738] opt 670, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010AC80), EOPID_INT(1), EOPID_NONE}, /* [739] opt 671, STEPPLAYER (+3) */
    {0x2030, 2, EOPID_INT(0x9A0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [740] opt 726, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010AD08), EOPID_INT(2), EOPID_NONE}, /* [741] opt 672, STEPPLAYER (+3) */
    {0, 1, EOPID_NONE, EOPID_FN(func_0010ADB8), EOPID_NONE, EOPID_NONE}, /* [742] opt 727, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010AE28), EOPID_INT(3), EOPID_NONE}, /* [743] opt 249, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ForceStepplayerLeanTarget), EOPID_INT(2), EOPID_NONE}, /* [744] opt 728, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010AFB0), EOPID_NONE, EOPID_NONE}, /* [745] opt 729, STEPPLAYER (+3) */
    {0xA, 0x92, EOPID_INT(0x1138), EOPID_INT(0x1138), EOPID_INT(0x1138), EOPID_NONE}, /* [746] opt 730, STEPPLAYER (+3) */
    {0xA, 0x92, EOPID_INT(0x1140), EOPID_INT(0x1140), EOPID_INT(0x1140), EOPID_NONE}, /* [747] opt 731, STEPPLAYER (+3) */
    {0, 0x12, EOPID_INT(0x1104), EOPID_INT(0x1104), EOPID_NONE, EOPID_NONE}, /* [748] opt 732, STEPPLAYER (+3) */
    {2, 0x12, EOPID_INT(0x1110), EOPID_INT(0x1110), EOPID_NONE, EOPID_NONE}, /* [749] opt 733, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B028), EOPID_INT(1), EOPID_NONE}, /* [750] opt 734, STEPPLAYER (+3) */
    {0x1057, 4, EOPID_VPMF(0x5A, 0xC), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [751] opt 342, STEPPLAYER (+3) */
    {0x2067, 2, EOPID_INT(0x11A8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [752] opt 735, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B0B8), EOPID_INT(1), EOPID_NONE}, /* [753] opt 736, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B140), EOPID_NONE, EOPID_NONE}, /* [754] opt 737, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B1B8), EOPID_NONE, EOPID_NONE}, /* [755] opt 738, STEPPLAYER (+3) */
    {0x2038, 2, EOPID_INT(0x10DC), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [756] opt 739, STEPPLAYER (+3) */
    {0x1053, 2, EOPID_INT(0x10E4), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [757] opt 740, STEPPLAYER (+3) */
    {0, 0x92, EOPID_INT(0x11AC), EOPID_INT(0x11AC), EOPID_INT(0x11AC), EOPID_NONE}, /* [758] opt 741, STEPPLAYER (+3) */
    {0, 4, EOPID_PMF(func_00144068), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [759] opt 742, STEPPLAYER (+3) */
    {0x12, 0x401, EOPID_FN(func_0010B230), EOPID_FN(func_0010B230), EOPID_INT(7), EOPID_NONE}, /* [760] opt 743, STEPPLAYER (+3) */
    {0, 0x92, EOPID_INT(0x12F0), EOPID_INT(0x12F0), EOPID_INT(0x12F0), EOPID_NONE}, /* [761] opt 744, STEPPLAYER (+3) */
    {0, 4, EOPID_PMF(FStepplayerInJtmode), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [762] opt 745, STEPPLAYER (+3) */
    {0, 4, EOPID_PMF(func_001443E8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [763] opt 746, STEPPLAYER (+3) */
    {0, 4, EOPID_PMF(func_001443F8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [764] opt 747, STEPPLAYER (+3) */
    {0, 4, EOPID_PMF(func_00144408), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [765] opt 748, STEPPLAYER (+3) */
    {0x12, 0x401, EOPID_FN(func_0010B340), EOPID_FN(func_0010B340), EOPID_INT(2), EOPID_NONE}, /* [766] opt 749, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetStepplayerGadget), EOPID_INT(2), EOPID_NONE}, /* [767] opt 750, STEPPLAYER (+3) */
    {5, 1, EOPID_NONE, EOPID_FN(RefThunk_IGadgetSlotFromId), EOPID_INT(1), EOPID_NONE}, /* [768] opt 751, STEPPLAYER (+3) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B510), EOPID_INT(1), EOPID_NONE}, /* [769] opt 752, JT */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010B598), EOPID_INT(1), EOPID_NONE}, /* [770] opt 753, JT */
    {0x12, 0x401, EOPID_FN(func_0010B618), EOPID_FN(func_0010B618), EOPID_INT(3), EOPID_NONE}, /* [771] opt 754, JT */
    {0x12, 0x401, EOPID_FN(func_0010B6D0), EOPID_FN(func_0010B6D0), EOPID_INT(1), EOPID_NONE}, /* [772] opt 755, JT */
    {0x2030, 2, EOPID_INT(0x1BCC), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [773] opt 756, JT */
    {0x105C, 0x92, EOPID_INT(0x16D8), EOPID_INT(0x16D8), EOPID_INT(0x16D8), EOPID_NONE}, /* [774] opt 757, BENTLEY */
    {0, 0x92, EOPID_INT(0x1818), EOPID_INT(0x1818), EOPID_INT(0x1818), EOPID_NONE}, /* [775] opt 758, BENTLEY */
    {0, 0x92, EOPID_INT(0x181C), EOPID_INT(0x181C), EOPID_INT(0x181C), EOPID_NONE}, /* [776] opt 759, BENTLEY */
    {0x1049, 0x92, EOPID_INT(0x1870), EOPID_INT(0x1870), EOPID_INT(0x1870), EOPID_NONE}, /* [777] opt 760, BENTLEY */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010B760), EOPID_NONE, EOPID_NONE}, /* [778] opt 235, CID41 */
    {6, 0x92, EOPID_INT(0x150), EOPID_INT(0x150), EOPID_INT(0x150), EOPID_NONE}, /* [779] opt 761, EXIT */
    {6, 0x92, EOPID_INT(0x154), EOPID_INT(0x154), EOPID_INT(0x154), EOPID_NONE}, /* [780] opt 764, EXIT */
    {1, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [781] opt 765, EXIT */
    {0, 0x92, EOPID_INT(0x130), EOPID_INT(0x130), EOPID_INT(0x130), EOPID_NONE}, /* [782] opt 766, EXIT */
    {0, 0x92, EOPID_INT(0x134), EOPID_INT(0x134), EOPID_INT(0x134), EOPID_NONE}, /* [783] opt 767, EXIT */
    {0, 0x92, EOPID_INT(0x138), EOPID_INT(0x138), EOPID_INT(0x138), EOPID_NONE}, /* [784] opt 768, EXIT */
    {1, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [785] opt 769, EXIT */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunkExitTRIGGER), EOPID_NONE, EOPID_NONE}, /* [786] opt 235, EXIT */
    {0x1033, 0x22, EOPID_INT(0x13C), EOPID_PMF(SetExitExits), EOPID_NONE, EOPID_NONE}, /* [787] opt 770, EXIT */
    {0x12, 0x400, EOPID_FN(RefThunk_AddExitRsmg), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [788] opt 527, EXIT */
    {0xF, 4, EOPID_PMF(func_00228518), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [789] opt 771, EXIT */
    {0x12, 0x400, EOPID_FN(func_0010B8F8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [790] opt 766, WARP */
    {0x12, 0x400, EOPID_FN(func_0010B970), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [791] opt 772, WARP */
    {0x12, 0x400, EOPID_FN(func_0010B9E8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [792] opt 20, WARP */
    {0x12, 0x400, EOPID_FN(RefThunkWarpSET_SM_GOAL), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [793] opt 526, WARP */
    {0x12, 0x400, EOPID_FN(func_0010BB38), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [794] opt 527, WARP */
    {0x12, 0x400, EOPID_FN(RefThunk_AddWarpOidPair), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [795] opt 773, WARP */
    {1, 0x92, EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_NONE}, /* [796] opt 774, CID34 */
    {0, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [797] opt 775, CID34 */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A08)}, /* [798] opt 776, CID34 */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A08)}, /* [799] opt 777, CID34 */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A08)}, /* [800] opt 778, CID34 */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A08)}, /* [801] opt 141, CID34 */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A08)}, /* [802] opt 779, CID34 */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A08)}, /* [803] opt 780, CID34 */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A08)}, /* [804] opt 781, CID34 */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A08)}, /* [805] opt 782, CID34 */
    {0x1031, 0x92, EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_NONE}, /* [806] opt 783, CID12 */
    {0, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [807] opt 775, CID12 */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_002599F8)}, /* [808] opt 776, CID12 */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_002599F8)}, /* [809] opt 777, CID12 */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_002599F8)}, /* [810] opt 778, CID12 */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_002599F8)}, /* [811] opt 141, CID12 */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_002599F8)}, /* [812] opt 779, CID12 */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_002599F8)}, /* [813] opt 780, CID12 */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_002599F8)}, /* [814] opt 781, CID12 */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_002599F8)}, /* [815] opt 782, CID12 */
    {0x102C, 0x92, EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_NONE}, /* [816] opt 784, CID38 */
    {0, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [817] opt 775, CID38 */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A18)}, /* [818] opt 776, CID38 */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A18)}, /* [819] opt 777, CID38 */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A18)}, /* [820] opt 778, CID38 */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A18)}, /* [821] opt 141, CID38 */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A18)}, /* [822] opt 779, CID38 */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A18)}, /* [823] opt 780, CID38 */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A18)}, /* [824] opt 781, CID38 */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A18)}, /* [825] opt 782, CID38 */
    {1, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [826] opt 774, BARREL */
    {5, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [827] opt 785, BARREL */
    {0, 0x92, EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_NONE}, /* [828] opt 786, BARREL */
    {0x2034, 2, EOPID_INT(0x190), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [829] opt 787, BARREL */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A28)}, /* [830] opt 776, BARREL */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A28)}, /* [831] opt 777, BARREL */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A28)}, /* [832] opt 778, BARREL */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A28)}, /* [833] opt 141, BARREL */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A28)}, /* [834] opt 779, BARREL */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A28)}, /* [835] opt 780, BARREL */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A28)}, /* [836] opt 781, BARREL */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A28)}, /* [837] opt 782, BARREL */
    {2, 0x92, EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_NONE}, /* [838] opt 788, PIPE */
    {0xE, 0x92, EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_NONE}, /* [839] opt 789, PIPE */
    {0xE, 0x92, EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_NONE}, /* [840] opt 790, PIPE */
    {0, 0x120, EOPID_NONE, EOPID_PMF(SetPipeGrabEnabled), EOPID_PMF(SetPipeGrabEnabled), EOPID_NONE}, /* [841] opt 791, PIPE */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259B00)}, /* [842] opt 776, PIPE */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259B00)}, /* [843] opt 777, PIPE */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259B00)}, /* [844] opt 778, PIPE */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259B00)}, /* [845] opt 141, PIPE */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259B00)}, /* [846] opt 779, PIPE */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259B00)}, /* [847] opt 780, PIPE */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259B00)}, /* [848] opt 781, PIPE */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259B00)}, /* [849] opt 782, PIPE */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259C10)}, /* [850] opt 776, CID39 */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259C10)}, /* [851] opt 777, CID39 */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259C10)}, /* [852] opt 778, CID39 */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259C10)}, /* [853] opt 141, CID39 */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259C10)}, /* [854] opt 779, CID39 */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259C10)}, /* [855] opt 780, CID39 */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259C10)}, /* [856] opt 781, CID39 */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259C10)}, /* [857] opt 782, CID39 */
    {0, 0x92, EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_INT(0x1BC), EOPID_NONE}, /* [858] opt 792, RAIL */
    {1, 0x92, EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_NONE}, /* [859] opt 793, RAIL */
    {1, 0x92, EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_NONE}, /* [860] opt 351, RAIL */
    {6, 0x92, EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_NONE}, /* [861] opt 794, RAIL */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259B10)}, /* [862] opt 776, RAIL */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259B10)}, /* [863] opt 777, RAIL */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259B10)}, /* [864] opt 778, RAIL */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259B10)}, /* [865] opt 141, RAIL */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259B10)}, /* [866] opt 779, RAIL */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259B10)}, /* [867] opt 780, RAIL */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259B10)}, /* [868] opt 781, RAIL */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259B10)}, /* [869] opt 782, RAIL */
    {1, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [870] opt 795, CID40 */
    {1, 0x92, EOPID_INT(0x108), EOPID_INT(0x108), EOPID_INT(0x108), EOPID_NONE}, /* [871] opt 778, CID9 */
    {0, 0x92, EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_NONE}, /* [872] opt 141, CID9 */
    {0x200F, 0x92, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_NONE}, /* [873] opt 796, CID9 */
    {0x201E, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [874] opt 797, WATER */
    {0x201E, 0x92, EOPID_INT(0x174), EOPID_INT(0x174), EOPID_INT(0x174), EOPID_NONE}, /* [875] opt 798, WATER */
    {5, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [876] opt 666, WATER */
    {0x1011, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [877] opt 799, WATER */
    {2, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [878] opt 800, WATER */
    {0, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [879] opt 801, WATER */
    {0x2030, 0x92, EOPID_INT(0x178), EOPID_INT(0x178), EOPID_INT(0x178), EOPID_NONE}, /* [880] opt 802, WATER */
    {0x2030, 0x92, EOPID_INT(0x17C), EOPID_INT(0x17C), EOPID_INT(0x17C), EOPID_NONE}, /* [881] opt 803, WATER */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_FInvulnerablePo), EOPID_INT(1), EOPID_NONE}, /* [882] opt 804, PO (+28) */
    {0x102D, 4, EOPID_VPMF(0x4E, 0xC), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [883] opt 805, PO (+28) */
    {0x1037, 0x24, EOPID_PMF(GetPoCtlState), EOPID_VPMF(0x50, 0xC), EOPID_NONE, EOPID_NONE}, /* [884] opt 806, PO (+28) */
    {0, 4, EOPID_PMF(func_001C4098), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [885] opt 807, PO (+28) */
    {0, 4, EOPID_PMF(FIsPoCur), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [886] opt 808, PO (+28) */
    {0, 4, EOPID_PMF(FIsPoActive), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [887] opt 809, PO (+28) */
    {0x2089, 2, EOPID_INT(0x150), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [888] opt 810, PO (+28) */
    {0x103C, 0x92, EOPID_INT(0x154), EOPID_INT(0x154), EOPID_INT(0x154), EOPID_NONE}, /* [889] opt 811, PO (+28) */
    {0x200F, 0x122, EOPID_INT(0x16C), EOPID_PMF(SetPoTnCam), EOPID_PMF(SetPoTnCam), EOPID_NONE}, /* [890] opt 812, PO (+28) */
    {6, 0x82, EOPID_INT(0x204), EOPID_NONE, EOPID_INT(0x204), EOPID_NONE}, /* [891] opt 813, DPRIZE (+5) */
    {6, 0x82, EOPID_INT(0x20C), EOPID_NONE, EOPID_INT(0x20C), EOPID_NONE}, /* [892] opt 814, DPRIZE (+5) */
    {6, 0x82, EOPID_INT(0x214), EOPID_NONE, EOPID_INT(0x214), EOPID_NONE}, /* [893] opt 815, DPRIZE (+5) */
    {0x12, 0x400, EOPID_FN(func_0010BD10), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [894] opt 816, DPRIZE (+5) */
    {6, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [895] opt 817, DPRIZE (+5) */
    {1, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [896] opt 818, DPRIZE (+5) */
    {0x1034, 0x22, EOPID_INT(0x150), EOPID_VPMF(0x4D, 0xC), EOPID_NONE, EOPID_NONE}, /* [897] opt 819, DPRIZE (+5) */
    {0x1034, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [898] opt 820, DPRIZE (+5) */
    {1, 0x92, EOPID_INT(0x21C), EOPID_INT(0x21C), EOPID_INT(0x21C), EOPID_NONE}, /* [899] opt 821, DPRIZE (+5) */
    {0, 0x92, EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_NONE}, /* [900] opt 822, DPRIZE (+5) */
    {0x10, 0x92, EOPID_INT(0x224), EOPID_INT(0x224), EOPID_INT(0x224), EOPID_NONE}, /* [901] opt 823, DPRIZE (+5) */
    {0xA, 0x92, EOPID_INT(0x230), EOPID_INT(0x230), EOPID_INT(0x230), EOPID_NONE}, /* [902] opt 824, COIN */
    {0xA, 0x92, EOPID_INT(0x230), EOPID_INT(0x230), EOPID_INT(0x230), EOPID_NONE}, /* [903] opt 824, MEDICINE */
    {5, 0x92, EOPID_INT(0x23C), EOPID_INT(0x23C), EOPID_INT(0x23C), EOPID_NONE}, /* [904] opt 825, MEDICINE */
    {5, 0x92, EOPID_INT(0x240), EOPID_INT(0x240), EOPID_INT(0x240), EOPID_NONE}, /* [905] opt 826, MEDICINE */
    {0x2085, 0x92, EOPID_INT(0x230), EOPID_INT(0x230), EOPID_INT(0x230), EOPID_NONE}, /* [906] opt 827, KEY */
    {0x12, 0x400, EOPID_FN(RefThunk_AddLootEntry), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [907] opt 828, LOOT */
    {0x12, 0x400, EOPID_FN(func_0010BE70), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [908] opt 829, LOOT */
    {0x12, 0x400, EOPID_FN(RefThunk_AddLootEntryOid), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [909] opt 830, LOOT */
    {6, 0x24, EOPID_PMF(func_001C7A40), EOPID_PMF(func_001C7A58), EOPID_NONE, EOPID_NONE}, /* [910] opt 831, LOOT */
    {6, 0x2002, EOPID_INT(4), EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001C7B10)}, /* [911] opt 832, LOOT */
    {0xA, 0x92, EOPID_INT(0x230), EOPID_INT(0x230), EOPID_INT(0x230), EOPID_NONE}, /* [912] opt 824, LOOT */
    {0xF, 0x92, EOPID_INT(0x3C80), EOPID_INT(0x3C80), EOPID_INT(0x3C80), EOPID_NONE}, /* [913] opt 771, SW */
    {2, 0xA2, EOPID_INT(0x3E10), EOPID_PMF(func_0020F2B8), EOPID_INT(0x3E10), EOPID_NONE}, /* [914] opt 833, SW */
    {1, 0xA2, EOPID_INT(0x3E24), EOPID_PMF(SetSwIllumShadow), EOPID_INT(0x3E24), EOPID_NONE}, /* [915] opt 834, SW */
    {1, 0xA2, EOPID_INT(0x3E20), EOPID_PMF(SetSwIllum), EOPID_INT(0x3E20), EOPID_NONE}, /* [916] opt 835, SW */
    {2, 0x2092, EOPID_INT(0x3E70), EOPID_INT(0x3E70), EOPID_INT(0x3E70), EOPID_PMF(func_00210978)}, /* [917] opt 836, SW */
    {1, 0x2092, EOPID_INT(0x3E64), EOPID_INT(0x3E64), EOPID_INT(0x3E64), EOPID_PMF(func_00210978)}, /* [918] opt 837, SW */
    {2, 0x2092, EOPID_INT(0x3E80), EOPID_INT(0x3E80), EOPID_INT(0x3E80), EOPID_PMF(func_00210978)}, /* [919] opt 838, SW */
    {1, 0x2092, EOPID_INT(0x3E68), EOPID_INT(0x3E68), EOPID_INT(0x3E68), EOPID_PMF(func_00210978)}, /* [920] opt 839, SW */
    {2, 0x2092, EOPID_INT(0x3E90), EOPID_INT(0x3E90), EOPID_INT(0x3E90), EOPID_PMF(func_00210978)}, /* [921] opt 840, SW */
    {1, 0x2092, EOPID_INT(0x3E6C), EOPID_INT(0x3E6C), EOPID_INT(0x3E6C), EOPID_PMF(func_00210978)}, /* [922] opt 841, SW */
    {0xD, 0x92, EOPID_INT(0x3E00), EOPID_INT(0x3E00), EOPID_INT(0x3E00), EOPID_NONE}, /* [923] opt 842, SW */
    {0, 0x82, EOPID_INT(0x3E38), EOPID_NONE, EOPID_INT(0x3E38), EOPID_NONE}, /* [924] opt 768, SW */
    {0, 0x92, EOPID_INT(0x3E3C), EOPID_INT(0x3E3C), EOPID_INT(0x3E3C), EOPID_NONE}, /* [925] opt 843, SW */
    {0x2032, 4, EOPID_PMF(PpoHero), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [926] opt 844, SW */
    {0x2032, 4, EOPID_PMF(PpoCur), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [927] opt 845, SW */
    {0x2032, 0x22, EOPID_INT(0x3F00), EOPID_PMF(func_001C4A60), EOPID_NONE, EOPID_NONE}, /* [928] opt 846, SW */
    {0x2032, 1, EOPID_NONE, EOPID_FN(RefThunk_PpoClosestSw), EOPID_INT(1), EOPID_NONE}, /* [929] opt 847, SW */
    {1, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [930] opt 848, SW */
    {0xA, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [931] opt 849, SW */
    {0x2093, 1, EOPID_NONE, EOPID_FN(RefThunk_EmitSwNoise), EOPID_INT(3), EOPID_NONE}, /* [932] opt 850, SW */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010C0A8), EOPID_NONE, EOPID_NONE}, /* [933] opt 454, SW */
    {0x12, 0x400, EOPID_FN(func_0010C120), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [934] opt 852, SW */
    {9, 0x92, EOPID_INT(0x45D0), EOPID_INT(0x45D0), EOPID_INT(0x45D0), EOPID_NONE}, /* [935] opt 853, SW */
    {0xA, 0x92, EOPID_INT(0x45E0), EOPID_INT(0x45E0), EOPID_INT(0x45E0), EOPID_NONE}, /* [936] opt 854, SW */
    {6, 0x12, EOPID_INT(0x3C88), EOPID_INT(0x3C88), EOPID_NONE, EOPID_NONE}, /* [937] opt 855, SW */
    {6, 0x12, EOPID_INT(0x3C8C), EOPID_INT(0x3C8C), EOPID_NONE, EOPID_NONE}, /* [938] opt 856, SW */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSwHANDS_OFF), EOPID_INT(1), EOPID_NONE}, /* [939] opt 857, SW */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkSwHANDS_ON), EOPID_INT(1), EOPID_NONE}, /* [940] opt 858, SW */
    {0, 4, EOPID_PMF(FSwHandsOff), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [941] opt 859, SW */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010C2E8), EOPID_INT(1), EOPID_NONE}, /* [942] opt 861, SW */
    {5, 2, EOPID_INT(0x4768), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [943] opt 862, SW */
    {5, 2, EOPID_INT(0x4764), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [944] opt 863, SW */
    {0x12, 0x400, EOPID_FN(RefThunkWrADD_CIRCLE_WARP), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [945] opt 864, WR */
    {0x12, 0x400, EOPID_FN(RefThunkWrADD_BEND_WARP), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [946] opt 865, WR */
    {2, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [947] opt 573, WR */
    {1, 0x124, EOPID_PMF(GetWrWavelength), EOPID_PMF(SetWrWavelength__FP2WRf), EOPID_PMF(SetWrWavelength__FP2WRf), EOPID_NONE}, /* [948] opt 866, WR */
    {1, 0x2092, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [949] opt 867, WR */
    {1, 0x2092, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [950] opt 868, WR */
    {2, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [951] opt 869, WR */
    {2, 0x2092, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_INT(0x40), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [952] opt 870, WR */
    {0xA, 0x2092, EOPID_INT(0x30), EOPID_INT(0x30), EOPID_INT(0x30), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [953] opt 871, WR */
    {0xA, 0x2092, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_PMF(PwreGetWrCircle__FP2WR4ENSK)}, /* [954] opt 872, WR */
    {2, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [955] opt 873, WR */
    {0xA, 0x2092, EOPID_INT(0x30), EOPID_INT(0x30), EOPID_INT(0x30), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [956] opt 874, WR */
    {1, 0x2092, EOPID_INT(0x38), EOPID_INT(0x38), EOPID_INT(0x38), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [957] opt 875, WR */
    {0x12, 0x400, EOPID_FN(RefThunkWrADD_BEND_NOISE), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [958] opt 876, WR */
    {2, 0x2092, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_INT(0x20), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [959] opt 877, WR */
    {0xA, 0x2092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [960] opt 878, WR */
    {1, 0x2092, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_PMF(PwreGetWrBend__FP2WR4ENSK)}, /* [961] opt 879, WR */
    {0x12, 0x400, EOPID_FN(RefThunkWrADD_SWIVEL_NOISE), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [962] opt 880, WR */
    {5, 2, EOPID_INT(0x54), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [963] opt 881, WR */
    {5, 0x12, EOPID_INT(0x6E0), EOPID_INT(0x6E0), EOPID_NONE, EOPID_NONE}, /* [964] opt 882, WR */
    {0x12, 0x400, EOPID_FN(func_0010C5F0), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [965] opt 369, CID96 */
    {0x200A, 0x92, EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_NONE}, /* [966] opt 883, CID96 */
    {0x200A, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [967] opt 884, CID96 */
    {0x201E, 0x92, EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_INT(0x1C8), EOPID_NONE}, /* [968] opt 885, CID96 */
    {1, 0x92, EOPID_INT(0x150), EOPID_INT(0x150), EOPID_INT(0x150), EOPID_NONE}, /* [969] opt 886, CID96 */
    {1, 0x92, EOPID_INT(0x154), EOPID_INT(0x154), EOPID_INT(0x154), EOPID_NONE}, /* [970] opt 887, CID96 */
    {0xA, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [971] opt 888, CID96 */
    {0xA, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [972] opt 889, CID96 */
    {1, 0x92, EOPID_INT(0x168), EOPID_INT(0x168), EOPID_INT(0x168), EOPID_NONE}, /* [973] opt 890, CID96 */
    {1, 0x122, EOPID_INT(0x21C), EOPID_PMF(SetStepSpinRatio__FP4STEPf), EOPID_PMF(SetStepSpinRatio__FP4STEPf), EOPID_NONE}, /* [974] opt 350, CID96 */
    {0x1036, 0x22, EOPID_INT(0x220), EOPID_PMF(func_001B1328), EOPID_NONE, EOPID_NONE}, /* [975] opt 891, CID96 */
    {5, 0x92, EOPID_INT(0x230), EOPID_INT(0x230), EOPID_INT(0x230), EOPID_NONE}, /* [976] opt 402, CID96 */
    {2, 0x92, EOPID_INT(0x240), EOPID_INT(0x240), EOPID_INT(0x240), EOPID_NONE}, /* [977] opt 403, CID96 */
    {0, 0x92, EOPID_INT(0x150), EOPID_INT(0x150), EOPID_INT(0x150), EOPID_NONE}, /* [978] opt 892, SKY */
    {0x12, 0x4401, EOPID_FN(func_0010C6A8), EOPID_FN(func_0010C6A8), EOPID_NONE, EOPID_NONE}, /* [979] opt 235, CID116 */
    {0xD, 0x92, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE}, /* [980] opt 893, CID116 */
    {0xD, 0x92, EOPID_INT(0x54), EOPID_INT(0x54), EOPID_INT(0x54), EOPID_NONE}, /* [981] opt 894, CID116 */
    {0xD, 0x92, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE}, /* [982] opt 895, CID116 */
    {1, 0x92, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE}, /* [983] opt 896, CID116 */
    {1, 0x92, EOPID_INT(0x64), EOPID_INT(0x64), EOPID_INT(0x64), EOPID_NONE}, /* [984] opt 897, CID116 */
    {1, 0x92, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE}, /* [985] opt 898, CID116 */
    {0x103B, 0x92, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE}, /* [986] opt 899, CID116 */
    {1, 2, EOPID_INT(0x10), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [987] opt 900, POCTL */
    {1, 2, EOPID_INT(0x14), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [988] opt 901, POCTL */
    {1, 2, EOPID_INT(0x18), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [989] opt 902, POCTL */
    {1, 2, EOPID_INT(0x1C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [990] opt 903, POCTL */
    {1, 2, EOPID_INT(0x20), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [991] opt 904, POCTL */
    {1, 2, EOPID_INT(0x24), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [992] opt 905, POCTL */
    {1, 2, EOPID_INT(0x28), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [993] opt 906, POCTL */
    {1, 2, EOPID_INT(0x2C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [994] opt 907, POCTL */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_POCTL_FConsumePressed), EOPID_INT(1), EOPID_NONE}, /* [995] opt 908, POCTL */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_POCTL_FButtonDown), EOPID_INT(1), EOPID_NONE}, /* [996] opt 909, POCTL */
    {0x2030, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [997] opt 910, TURRET */
    {0, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [998] opt 911, TURRET */
    {0, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [999] opt 912, TURRET */
    {0xA, 0x92, EOPID_INT(0x19C), EOPID_INT(0x19C), EOPID_INT(0x19C), EOPID_NONE}, /* [1000] opt 325, TURRET */
    {0xA, 0x92, EOPID_INT(0x1A4), EOPID_INT(0x1A4), EOPID_INT(0x1A4), EOPID_NONE}, /* [1001] opt 913, TURRET */
    {0x2006, 0x92, EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_NONE}, /* [1002] opt 914, TURRET */
    {0x2070, 0x92, EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_NONE}, /* [1003] opt 915, TURRET */
    {0x2030, 2, EOPID_INT(0x1C8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1004] opt 916, TURRET */
    {1, 0x92, EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_NONE}, /* [1005] opt 917, TURRET */
    {1, 0x92, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE}, /* [1006] opt 918, TURRET */
    {0, 0xA2, EOPID_INT(0x1CC), EOPID_PMF(func_00226370), EOPID_INT(0x1CC), EOPID_NONE}, /* [1007] opt 919, TURRET */
    {0x2002, 0x92, EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_NONE}, /* [1008] opt 920, TURRET */
    {0, 0x92, EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_NONE}, /* [1009] opt 921, TURRET */
    {0xA, 0x92, EOPID_INT(0x1AC), EOPID_INT(0x1AC), EOPID_INT(0x1AC), EOPID_NONE}, /* [1010] opt 922, TURRET */
    {0x1030, 0x82, EOPID_INT(0x124), EOPID_NONE, EOPID_INT(0x124), EOPID_NONE}, /* [1011] opt 923, HND */
    {1, 0x92, EOPID_INT(0x128), EOPID_INT(0x128), EOPID_INT(0x128), EOPID_NONE}, /* [1012] opt 924, HND */
    {0xE, 0x92, EOPID_INT(0x12C), EOPID_INT(0x12C), EOPID_INT(0x12C), EOPID_NONE}, /* [1013] opt 789, HND */
    {0xE, 0x92, EOPID_INT(0x130), EOPID_INT(0x130), EOPID_INT(0x130), EOPID_NONE}, /* [1014] opt 790, HND */
    {1, 0x92, EOPID_INT(0x134), EOPID_INT(0x134), EOPID_INT(0x134), EOPID_NONE}, /* [1015] opt 925, HND */
    {0x2002, 0x92, EOPID_INT(0x138), EOPID_INT(0x138), EOPID_INT(0x138), EOPID_NONE}, /* [1016] opt 503, HND */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A78)}, /* [1017] opt 776, HND */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A78)}, /* [1018] opt 777, HND */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A78)}, /* [1019] opt 778, HND */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A78)}, /* [1020] opt 141, HND */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A78)}, /* [1021] opt 779, HND */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A78)}, /* [1022] opt 780, HND */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A78)}, /* [1023] opt 781, HND */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A78)}, /* [1024] opt 782, HND */
    {0x12, 0x400, EOPID_FN(func_0010C820), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1025] opt 926, CID98 */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1026] opt 927, CID98 */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1027] opt 928, CID98 */
    {0x2002, 2, EOPID_INT(0x16C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1028] opt 929, CID98 */
    {0x1011, 2, EOPID_INT(0x170), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1029] opt 930, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010C8C8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1030] opt 342, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010C950), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1031] opt 931, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010C9C8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1032] opt 932, CID98 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BBA60), EOPID_NONE}, /* [1033] opt 933, CID98 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BBA90), EOPID_NONE}, /* [1034] opt 244, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010CA50), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1035] opt 233, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010CAD8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1036] opt 234, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010CB60), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1037] opt 934, CID98 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BBB50), EOPID_NONE}, /* [1038] opt 243, CID98 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BBB80), EOPID_NONE}, /* [1039] opt 238, CID98 */
    {0x12, 0x401, EOPID_FN(func_0010CBE8), EOPID_FN(func_0010CC70), EOPID_INT(1), EOPID_NONE}, /* [1040] opt 110, CID98 */
    {0x12, 0x401, EOPID_FN(func_0010CCF8), EOPID_FN(func_0010CD80), EOPID_INT(1), EOPID_NONE}, /* [1041] opt 112, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010CE08), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1042] opt 239, CID98 */
    {0x12, 0x401, EOPID_FN(func_0010CE90), EOPID_FN(func_0010CF18), EOPID_INT(1), EOPID_NONE}, /* [1043] opt 111, CID98 */
    {0x12, 0x401, EOPID_FN(func_0010CFA0), EOPID_FN(func_0010D028), EOPID_INT(1), EOPID_NONE}, /* [1044] opt 113, CID98 */
    {0x12, 0x400, EOPID_FN(func_0010D0B0), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1045] opt 527, CID98 */
    {0x104A, 0x12, EOPID_INT(0x168), EOPID_INT(0x168), EOPID_NONE, EOPID_NONE}, /* [1046] opt 935, CID98 */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010D158), EOPID_NONE, EOPID_NONE}, /* [1047] opt 936, CID98 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D1D8), EOPID_INT(1), EOPID_NONE}, /* [1048] opt 236, CID98 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_StartMurrayLiftGrabspot), EOPID_INT(1), EOPID_NONE}, /* [1049] opt 937, MURRAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D2E8), EOPID_INT(1), EOPID_NONE}, /* [1050] opt 938, MURRAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D370), EOPID_NONE, EOPID_NONE}, /* [1051] opt 939, MURRAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D3E8), EOPID_INT(1), EOPID_NONE}, /* [1052] opt 940, MURRAY */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D468), EOPID_NONE, EOPID_NONE}, /* [1053] opt 941, MURRAY */
    {1, 0x82, EOPID_INT(0x130), EOPID_NONE, EOPID_INT(0x130), EOPID_NONE}, /* [1054] opt 293, CID44 */
    {0x2002, 0x92, EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_NONE}, /* [1055] opt 503, CID44 */
    {2, 0x92, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_NONE}, /* [1056] opt 942, CID44 */
    {2, 0x92, EOPID_INT(0x100), EOPID_INT(0x100), EOPID_INT(0x100), EOPID_NONE}, /* [1057] opt 943, CID44 */
    {2, 0x92, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_NONE}, /* [1058] opt 944, CID44 */
    {0, 0x92, EOPID_INT(0x120), EOPID_INT(0x120), EOPID_INT(0x120), EOPID_NONE}, /* [1059] opt 945, CID44 */
    {1, 0x82, EOPID_INT(0x11C), EOPID_NONE, EOPID_INT(0x11C), EOPID_NONE}, /* [1060] opt 293, CID45 */
    {1, 0x82, EOPID_INT(0x120), EOPID_NONE, EOPID_INT(0x120), EOPID_NONE}, /* [1061] opt 296, CID45 */
    {1, 0x82, EOPID_INT(0x124), EOPID_NONE, EOPID_INT(0x124), EOPID_NONE}, /* [1062] opt 297, CID45 */
    {0x2002, 0x92, EOPID_INT(0x118), EOPID_INT(0x118), EOPID_INT(0x118), EOPID_NONE}, /* [1063] opt 946, CID45 */
    {0x2030, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BEAA8), EOPID_NONE}, /* [1064] opt 238, CID45 */
    {0x2004, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001BEAD0), EOPID_NONE}, /* [1065] opt 947, CID45 */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010D4E0), EOPID_NONE, EOPID_NONE}, /* [1066] opt 214, CID45 */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010D558), EOPID_NONE, EOPID_NONE}, /* [1067] opt 237, CID45 */
    {0x2002, 0x92, EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_NONE}, /* [1068] opt 534, CID35 (+2) */
    {1, 0x92, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_NONE}, /* [1069] opt 948, CID35 (+2) */
    {0x2072, 0x92, EOPID_INT(0x114), EOPID_INT(0x114), EOPID_INT(0x114), EOPID_NONE}, /* [1070] opt 531, CID35 (+2) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D5D0), EOPID_INT(1), EOPID_NONE}, /* [1071] opt 235, CID35 (+2) */
    {0, 1, EOPID_NONE, EOPID_FN(func_0010D660), EOPID_INT(1), EOPID_NONE}, /* [1072] opt 949, CID35 (+2) */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(func_00184028), EOPID_PMF(func_00184028), EOPID_NONE}, /* [1073] opt 519, CID35 (+2) */
    {7, 0x120, EOPID_NONE, EOPID_PMF(func_00184040), EOPID_PMF(func_00184040), EOPID_NONE}, /* [1074] opt 520, CID35 (+2) */
    {0x12, 0x401, EOPID_FN(func_0010D6E0), EOPID_FN(func_0010D6E0), EOPID_INT(3), EOPID_NONE}, /* [1075] opt 526, CID35 (+2) */
    {0x12, 0x401, EOPID_FN(func_0010D798), EOPID_FN(func_0010D798), EOPID_INT(2), EOPID_NONE}, /* [1076] opt 527, CID35 (+2) */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A38)}, /* [1077] opt 776, CID35 (+2) */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A38)}, /* [1078] opt 777, CID35 (+2) */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A38)}, /* [1079] opt 778, CID35 (+2) */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A38)}, /* [1080] opt 141, CID35 (+2) */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A38)}, /* [1081] opt 779, CID35 (+2) */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A38)}, /* [1082] opt 780, CID35 (+2) */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A38)}, /* [1083] opt 781, CID35 (+2) */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A38)}, /* [1084] opt 782, CID35 (+2) */
    {0x2030, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [1085] opt 950, CID36 */
    {6, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0x184), EOPID_NONE}, /* [1086] opt 951, CID36 */
    {6, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18C), EOPID_NONE}, /* [1087] opt 952, CID36 */
    {6, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0x194), EOPID_NONE}, /* [1088] opt 953, CID36 */
    {6, 0x92, EOPID_INT(0x19C), EOPID_INT(0x19C), EOPID_INT(0x19C), EOPID_NONE}, /* [1089] opt 344, CID36 */
    {0x1046, 0x92, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE}, /* [1090] opt 954, CID117 */
    {1, 0x92, EOPID_INT(0x54), EOPID_INT(0x54), EOPID_INT(0x54), EOPID_NONE}, /* [1091] opt 795, CID117 */
    {0xA, 0x92, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE}, /* [1092] opt 955, CID117 */
    {1, 0x92, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE}, /* [1093] opt 956, CID117 */
    {0x1047, 0x92, EOPID_INT(0x64), EOPID_INT(0x64), EOPID_INT(0x64), EOPID_NONE}, /* [1094] opt 957, CID117 */
    {0x2093, 1, EOPID_NONE, EOPID_FN(func_0010D840), EOPID_INT(2), EOPID_NONE}, /* [1095] opt 214, CID117 */
    {0x2075, 2, EOPID_INT(0x14), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1096] opt 958, CID147 */
    {2, 0x12, EOPID_INT(0x20), EOPID_INT(0x20), EOPID_NONE, EOPID_NONE}, /* [1097] opt 34, CID147 */
    {2, 0x12, EOPID_INT(0x30), EOPID_INT(0x30), EOPID_NONE, EOPID_NONE}, /* [1098] opt 959, CID147 */
    {0x1047, 0x12, EOPID_INT(0x54), EOPID_INT(0x54), EOPID_NONE, EOPID_NONE}, /* [1099] opt 957, CID147 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D8E0), EOPID_NONE, EOPID_NONE}, /* [1100] opt 237, CID147 */
    {0, 4, EOPID_PMF(func_00156870), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1101] opt 255, CID147 */
    {0x206F, 0x92, EOPID_INT(0x164), EOPID_INT(0x164), EOPID_INT(0x164), EOPID_NONE}, /* [1102] opt 484, CID100 (+2) */
    {0x2088, 2, EOPID_INT(0x168), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1103] opt 183, CID100 (+2) */
    {0x1049, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [1104] opt 960, CID100 (+2) */
    {0, 4, EOPID_PMF(func_00224610), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1105] opt 961, CID100 (+2) */
    {0, 4, EOPID_PMF(func_00224638), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1106] opt 501, CID100 (+2) */
    {0x1011, 0x92, EOPID_INT(0x174), EOPID_INT(0x174), EOPID_INT(0x174), EOPID_NONE}, /* [1107] opt 962, CID100 (+2) */
    {5, 0x92, EOPID_INT(0x178), EOPID_INT(0x178), EOPID_INT(0x178), EOPID_NONE}, /* [1108] opt 498, CID100 (+2) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D958), EOPID_INT(1), EOPID_NONE}, /* [1109] opt 963, CID100 (+2) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010D9E0), EOPID_INT(1), EOPID_NONE}, /* [1110] opt 964, CID100 (+2) */
    {0x2002, 0x92, EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_INT(0x16C), EOPID_NONE}, /* [1111] opt 965, CID100 (+2) */
    {1, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [1112] opt 966, CID100 (+2) */
    {0x2002, 0x92, EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_NONE}, /* [1113] opt 37, CID102 */
    {0x2016, 0x92, EOPID_INT(0x1010), EOPID_INT(0x1010), EOPID_INT(0x1010), EOPID_NONE}, /* [1114] opt 967, NEELA */
    {0x2016, 0x92, EOPID_INT(0x1014), EOPID_INT(0x1014), EOPID_INT(0x1014), EOPID_NONE}, /* [1115] opt 968, NEELA */
    {0x2030, 0x92, EOPID_INT(0x1050), EOPID_INT(0x1050), EOPID_INT(0x1050), EOPID_NONE}, /* [1116] opt 969, NEELA */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010DA68), EOPID_INT(1), EOPID_NONE}, /* [1117] opt 970, NEELA */
    {0x206D, 0x92, EOPID_INT(0x1064), EOPID_INT(0x1064), EOPID_INT(0x1064), EOPID_NONE}, /* [1118] opt 971, NEELA */
    {9, 0x92, EOPID_INT(0x1070), EOPID_INT(0x1070), EOPID_INT(0x1070), EOPID_NONE}, /* [1119] opt 972, NEELA */
    {0xA, 0x92, EOPID_INT(0x1080), EOPID_INT(0x1080), EOPID_INT(0x1080), EOPID_NONE}, /* [1120] opt 973, NEELA */
    {1, 0x92, EOPID_INT(0x1088), EOPID_INT(0x1088), EOPID_INT(0x1088), EOPID_NONE}, /* [1121] opt 974, NEELA */
    {0, 0x90, EOPID_NONE, EOPID_INT(0x6D), EOPID_INT(0x6D), EOPID_NONE}, /* [1122] opt 766, WIDGET (+5) */
    {0x1068, 0x82, EOPID_INT(0xA4), EOPID_NONE, EOPID_INT(0xA4), EOPID_NONE}, /* [1123] opt 976, WIDGET (+5) */
    {0, 0x82, EOPID_INT(0xAC), EOPID_NONE, EOPID_INT(0xAC), EOPID_NONE}, /* [1124] opt 977, WIDGET (+5) */
    {1, 0x92, EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_NONE}, /* [1125] opt 978, WIDGET (+5) */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunk_ShowWidget), EOPID_NONE, EOPID_NONE}, /* [1126] opt 979, WIDGET (+5) */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(RefThunk_HideWidget), EOPID_NONE, EOPID_NONE}, /* [1127] opt 980, WIDGET (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_PlayWidgetAnim), EOPID_INT(1), EOPID_NONE}, /* [1128] opt 981, WIDGET (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_StopWidgetAnim), EOPID_INT(1), EOPID_NONE}, /* [1129] opt 258, WIDGET (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_SetWidgetAnimPhase), EOPID_INT(2), EOPID_NONE}, /* [1130] opt 982, WIDGET (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010DDB0), EOPID_INT(2), EOPID_NONE}, /* [1131] opt 983, WIDGET (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ShowWidgetArgsFromSplice), EOPID_INT(1), EOPID_NONE}, /* [1132] opt 984, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xCC), EOPID_INT(0xCC), EOPID_INT(0xCC), EOPID_NONE}, /* [1133] opt 985, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xD0), EOPID_INT(0xD0), EOPID_INT(0xD0), EOPID_NONE}, /* [1134] opt 986, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xD4), EOPID_INT(0xD4), EOPID_INT(0xD4), EOPID_NONE}, /* [1135] opt 987, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xD8), EOPID_INT(0xD8), EOPID_INT(0xD8), EOPID_NONE}, /* [1136] opt 988, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xDC), EOPID_INT(0xDC), EOPID_INT(0xDC), EOPID_NONE}, /* [1137] opt 989, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_NONE}, /* [1138] opt 990, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_NONE}, /* [1139] opt 991, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [1140] opt 992, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_NONE}, /* [1141] opt 993, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_INT(0xF0), EOPID_NONE}, /* [1142] opt 994, WIDGET (+5) */
    {0x10, 0x92, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_NONE}, /* [1143] opt 996, WIDGET130 */
    {0x10, 0x92, EOPID_INT(0x114), EOPID_INT(0x114), EOPID_INT(0x114), EOPID_NONE}, /* [1144] opt 997, WIDGET130 */
    {0x10, 0x92, EOPID_INT(0x118), EOPID_INT(0x118), EOPID_INT(0x118), EOPID_NONE}, /* [1145] opt 998, WIDGET130 */
    {0x10, 0x92, EOPID_INT(0x11C), EOPID_INT(0x11C), EOPID_INT(0x11C), EOPID_NONE}, /* [1146] opt 999, WIDGET130 */
    {0x10, 0x92, EOPID_INT(0x120), EOPID_INT(0x120), EOPID_INT(0x120), EOPID_NONE}, /* [1147] opt 1000, WIDGET130 */
    {0, 2, EOPID_INT(0x110), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1148] opt 1001, WIDGET127 */
    {0, 0x92, EOPID_INT(0x114), EOPID_INT(0x114), EOPID_INT(0x114), EOPID_NONE}, /* [1149] opt 1002, WIDGET127 */
    {0x12, 0x400, EOPID_FN(func_0010DEF8), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1150] opt 1003, WIDGET129 */
    {0x12, 0x400, EOPID_FN(func_0010DF80), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1151] opt 1004, WIDGET129 */
    {0x12, 0x400, EOPID_FN(RefThunk_AddBootyEntry), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1152] opt 828, BOOTY */
    {0x12, 0x400, EOPID_FN(func_0010E0C0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1153] opt 829, BOOTY */
    {0x12, 0x400, EOPID_FN(RefThunk_AddBootyEntryOid), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1154] opt 830, BOOTY */
    {6, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMF(func_0013C4D8)}, /* [1155] opt 352, BOOTY */
    {0x201E, 0x92, EOPID_INT(0x164), EOPID_INT(0x164), EOPID_INT(0x164), EOPID_NONE}, /* [1156] opt 1005, BOOTY */
    {2, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [1157] opt 1006, BOOTY */
    {6, 0x24, EOPID_PMF(IdBootyCur), EOPID_PMF(SelectBootyEntry), EOPID_NONE, EOPID_NONE}, /* [1158] opt 831, BOOTY */
    {6, 0x2002, EOPID_INT(0x14), EOPID_NONE, EOPID_NONE, EOPID_PMF(func_0013C4D8)}, /* [1159] opt 1007, BOOTY */
    {6, 0x2002, EOPID_INT(4), EOPID_NONE, EOPID_NONE, EOPID_PMF(func_0013C4D8)}, /* [1160] opt 832, BOOTY */
    {0, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0xC), EOPID_PMF(func_0013C4D8)}, /* [1161] opt 1008, BOOTY */
    {0, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(8), EOPID_PMF(func_0013C4D8)}, /* [1162] opt 1009, BOOTY */
    {0, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x10), EOPID_PMF(func_0013C4D8)}, /* [1163] opt 1010, BOOTY */
    {0x1042, 2, EOPID_INT(0x184), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1164] opt 342, BOOTY */
    {0x2025, 2, EOPID_INT(0x15C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1165] opt 1011, BOOTY */
    {0x2034, 2, EOPID_INT(0x160), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1166] opt 1012, BOOTY */
    {6, 0x82, EOPID_INT(0x184), EOPID_NONE, EOPID_INT(0x184), EOPID_NONE}, /* [1167] opt 1013, CID37 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E1C0), EOPID_INT(1), EOPID_NONE}, /* [1168] opt 736, CID37 */
    {0, 0x92, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE}, /* [1169] opt 1014, CID37 */
    {0x2067, 2, EOPID_INT(0x194), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1170] opt 735, CID37 */
    {6, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [1171] opt 1007, CID37 */
    {6, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1172] opt 1015, CID37 */
    {1, 0x92, EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_INT(0x18C), EOPID_NONE}, /* [1173] opt 1016, CID37 */
    {0, 1, EOPID_NONE, EOPID_FN(RefThunk_FCanAcquireProxy), EOPID_NONE, EOPID_NONE}, /* [1174] opt 1017, PROXY (+1) */
    {0x2002, 1, EOPID_NONE, EOPID_FN(RefThunk_PloAcquireProxy), EOPID_NONE, EOPID_NONE}, /* [1175] opt 1019, PROXY (+1) */
    {0x2002, 4, EOPID_PMF(func_001CB2E0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1176] opt 1020, PROXY (+1) */
    {0x2002, 1, EOPID_NONE, EOPID_FN(RefThunk_EnsureProxyCopy), EOPID_NONE, EOPID_NONE}, /* [1177] opt 1021, PROXY (+1) */
    {0, 0x92, EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_NONE}, /* [1178] opt 1022, PROXY (+1) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_LockProxyPlo), EOPID_INT(1), EOPID_NONE}, /* [1179] opt 1023, PROXY (+1) */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_UnlockProxyPlo), EOPID_INT(1), EOPID_NONE}, /* [1180] opt 1024, PROXY (+1) */
    {0xA002, 4, EOPID_PMF(func_001CB360), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1181] opt 1025, PROXY (+1) */
    {0x2003, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [1182] opt 1026, CID27 */
    {0x200F, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMFI(func_00259A48)}, /* [1183] opt 776, CID27 */
    {0, 0x2092, EOPID_INT(4), EOPID_INT(4), EOPID_INT(4), EOPID_PMFI(func_00259A48)}, /* [1184] opt 777, CID27 */
    {1, 0x2092, EOPID_INT(8), EOPID_INT(8), EOPID_INT(8), EOPID_PMFI(func_00259A48)}, /* [1185] opt 778, CID27 */
    {0, 0x2092, EOPID_INT(0xC), EOPID_INT(0xC), EOPID_INT(0xC), EOPID_PMFI(func_00259A48)}, /* [1186] opt 141, CID27 */
    {0x106A, 0x2092, EOPID_INT(0x10), EOPID_INT(0x10), EOPID_INT(0x10), EOPID_PMFI(func_00259A48)}, /* [1187] opt 779, CID27 */
    {0x10, 0x2092, EOPID_INT(0x14), EOPID_INT(0x14), EOPID_INT(0x14), EOPID_PMFI(func_00259A48)}, /* [1188] opt 780, CID27 */
    {0x2002, 0x2080, EOPID_NONE, EOPID_NONE, EOPID_INT(0x18), EOPID_PMFI(func_00259A48)}, /* [1189] opt 781, CID27 */
    {0x106C, 0x2092, EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_INT(0x1C), EOPID_PMFI(func_00259A48)}, /* [1190] opt 782, CID27 */
    {1, 0x82, EOPID_INT(0x50), EOPID_NONE, EOPID_INT(0x50), EOPID_NONE}, /* [1191] opt 1027, CID118 */
    {1, 0x82, EOPID_INT(0x54), EOPID_NONE, EOPID_INT(0x54), EOPID_NONE}, /* [1192] opt 1028, CID118 */
    {1, 0x82, EOPID_INT(0x58), EOPID_NONE, EOPID_INT(0x58), EOPID_NONE}, /* [1193] opt 1029, CID118 */
    {1, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1194] opt 1030, CID46 */
    {1, 0x92, EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_NONE}, /* [1195] opt 1031, CID46 */
    {1, 0x92, EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_INT(0xE4), EOPID_NONE}, /* [1196] opt 1032, CID46 */
    {1, 0x92, EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_INT(0xE8), EOPID_NONE}, /* [1197] opt 1033, CID46 */
    {0x10, 0x92, EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_INT(0xF4), EOPID_NONE}, /* [1198] opt 1034, CID46 */
    {0x10, 0x92, EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_INT(0xF8), EOPID_NONE}, /* [1199] opt 1035, CID46 */
    {0x10, 0x92, EOPID_INT(0xFC), EOPID_INT(0xFC), EOPID_INT(0xFC), EOPID_NONE}, /* [1200] opt 1036, CID46 */
    {0x2085, 0x92, EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_INT(0xEC), EOPID_NONE}, /* [1201] opt 827, CID46 */
    {0x206B, 0x92, EOPID_INT(0x100), EOPID_INT(0x100), EOPID_INT(0x100), EOPID_NONE}, /* [1202] opt 1037, CID46 */
    {1, 0x92, EOPID_INT(0x104), EOPID_INT(0x104), EOPID_INT(0x104), EOPID_NONE}, /* [1203] opt 1038, CID46 */
    {0, 4, EOPID_PMF(FReconTargetDone), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1204] opt 1039, CID46 */
    {0, 0x92, EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_INT(0x10C), EOPID_NONE}, /* [1205] opt 1040, CID46 */
    {0x10, 0x92, EOPID_INT(0x110), EOPID_INT(0x110), EOPID_INT(0x110), EOPID_NONE}, /* [1206] opt 1041, CID46 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E4A8), EOPID_INT(2), EOPID_NONE}, /* [1207] opt 235, CID104 */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(func_001E7930), EOPID_PMF(func_001E7930), EOPID_NONE}, /* [1208] opt 1042, SHAPE (+4) */
    {0x2002, 0x120, EOPID_NONE, EOPID_PMF(func_001E7968), EOPID_PMF(func_001E7968), EOPID_NONE}, /* [1209] opt 1043, SHAPE (+4) */
    {1, 0x124, EOPID_PMF(func_001E79A0), EOPID_PMF(func_001E79F0), EOPID_PMF(func_001E79F0), EOPID_NONE}, /* [1210] opt 1044, SHAPE (+4) */
    {2, 0x2092, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001E7840)}, /* [1211] opt 1045, SHAPE (+4) */
    {0x104B, 0x124, EOPID_PMF(func_001E7FF0), EOPID_PMF(SetShapeChainMode), EOPID_PMF(SetShapeChainMode), EOPID_NONE}, /* [1212] opt 1046, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x64), EOPID_INT(0x64), EOPID_INT(0x64), EOPID_PMF(func_001E7840)}, /* [1213] opt 1047, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_PMF(func_001E7840)}, /* [1214] opt 1048, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x74), EOPID_INT(0x74), EOPID_INT(0x74), EOPID_PMF(func_001E7840)}, /* [1215] opt 1049, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x7C), EOPID_INT(0x7C), EOPID_INT(0x7C), EOPID_PMF(func_001E7840)}, /* [1216] opt 1050, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_PMF(func_001E7840)}, /* [1217] opt 1051, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_PMF(func_001E7840)}, /* [1218] opt 1052, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_PMF(func_001E7840)}, /* [1219] opt 1053, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x78), EOPID_INT(0x78), EOPID_INT(0x78), EOPID_PMF(func_001E7840)}, /* [1220] opt 1054, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_PMF(func_001E7840)}, /* [1221] opt 1055, SHAPE (+4) */
    {1, 0x2092, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_PMF(func_001E7840)}, /* [1222] opt 1056, SHAPE (+4) */
    {2, 1, EOPID_NONE, EOPID_FN(func_0010E550), EOPID_INT(1), EOPID_NONE}, /* [1223] opt 1057, SHAPE (+4) */
    {1, 0x24, EOPID_PMF(func_001F87A0), EOPID_PMF(func_001F87C8), EOPID_NONE, EOPID_NONE}, /* [1224] opt 323, CID149 */
    {1, 0x24, EOPID_PMF(func_001F8808), EOPID_PMF(func_001F8830), EOPID_NONE, EOPID_NONE}, /* [1225] opt 324, CID149 */
    {1, 0x24, EOPID_PMF(func_001F8870), EOPID_PMF(func_001F8898), EOPID_NONE, EOPID_NONE}, /* [1226] opt 1058, CID149 */
    {1, 0x2012, EOPID_INT(0xA8), EOPID_INT(0xA8), EOPID_NONE, EOPID_PMF(func_001F88D8)}, /* [1227] opt 1059, CID149 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E5D8), EOPID_INT(3), EOPID_NONE}, /* [1228] opt 1060, CID149 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E690), EOPID_INT(3), EOPID_NONE}, /* [1229] opt 1061, CID149 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E758), EOPID_INT(4), EOPID_NONE}, /* [1230] opt 1062, CID149 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010E820), EOPID_INT(6), EOPID_NONE}, /* [1231] opt 1063, CID149 */
    {0x1011, 0x24, EOPID_PMFI(func_00256350), EOPID_PMFI(func_00256328), EOPID_NONE, EOPID_NONE}, /* [1232] opt 122, CID150 */
    {5, 0x24, EOPID_PMFI(func_00256318), EOPID_PMFI(func_002562F0), EOPID_NONE, EOPID_NONE}, /* [1233] opt 123, CID150 */
    {1, 0x24, EOPID_PMFI(func_002562E0), EOPID_PMFI(func_002562B8), EOPID_NONE, EOPID_NONE}, /* [1234] opt 795, CID150 */
    {0, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001628B0), EOPID_NONE}, /* [1235] opt 1064, JOB */
    {0x12, 0x400, EOPID_FN(RefThunk_AddJobCompleteEvent), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1236] opt 1065, JOB */
    {0x12, 0x400, EOPID_FN(func_0010E998), EOPID_NONE, EOPID_INT(1), EOPID_NONE}, /* [1237] opt 1066, JOB */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010EA20), EOPID_NONE, EOPID_NONE}, /* [1238] opt 1067, JOB */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010EA98), EOPID_NONE, EOPID_NONE}, /* [1239] opt 1068, JOB */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010EB10), EOPID_NONE, EOPID_NONE}, /* [1240] opt 1069, JOB */
    {0, 0x92, EOPID_INT(0x74), EOPID_INT(0x74), EOPID_INT(0x74), EOPID_NONE}, /* [1241] opt 1070, JOB */
    {0, 0x92, EOPID_INT(0x78), EOPID_INT(0x78), EOPID_INT(0x78), EOPID_NONE}, /* [1242] opt 1071, JOB */
    {0x2085, 0x92, EOPID_INT(0x64), EOPID_INT(0x64), EOPID_INT(0x64), EOPID_NONE}, /* [1243] opt 1072, JOB */
    {0x2085, 0x92, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_INT(0x68), EOPID_NONE}, /* [1244] opt 1073, JOB */
    {0x10, 0x92, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE}, /* [1245] opt 1074, JOB */
    {0x10, 0x92, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE}, /* [1246] opt 1075, JOB */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1247] opt 1076, JOB */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1248] opt 1077, JOB */
    {5, 2, EOPID_INT(0x7C), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1249] opt 1078, JOB */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ResetJobTasks), EOPID_NONE, EOPID_NONE}, /* [1250] opt 1079, JOB */
    {0, 4, EOPID_PMF(FJobComplete), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1251] opt 1080, JOB */
    {0, 4, EOPID_PMF(FIsCurrentJob), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1252] opt 1081, JOB */
    {0x1051, 2, EOPID_INT(0xF0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1253] opt 1082, JOB */
    {0x2002, 2, EOPID_INT(0xF4), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1254] opt 1083, JOB */
    {0x12, 0x400, EOPID_FN(func_0010EC00), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1255] opt 1084, JOB */
    {0x12, 0x400, EOPID_FN(RefThunkJobSET_SM_GOAL), EOPID_NONE, EOPID_INT(5), EOPID_NONE}, /* [1256] opt 526, JOB */
    {0xA, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0x9C), EOPID_NONE}, /* [1257] opt 1085, JOB */
    {0x12, 0x400, EOPID_FN(func_0010EDA8), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1258] opt 1086, JOB */
    {0x201A, 0x8092, EOPID_INT(0xCC), EOPID_INT(0xCC), EOPID_INT(0xCC), EOPID_NONE}, /* [1259] opt 1087, JOB */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xE0), EOPID_NONE}, /* [1260] opt 1088, JOB */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xE8), EOPID_NONE}, /* [1261] opt 1089, JOB */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xD0), EOPID_NONE}, /* [1262] opt 1090, JOB */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xD8), EOPID_NONE}, /* [1263] opt 1091, JOB */
    {0x2084, 0x92, EOPID_INT(0x7C), EOPID_INT(0x7C), EOPID_INT(0x7C), EOPID_NONE}, /* [1264] opt 1093, TASK */
    {0x10, 0x92, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE}, /* [1265] opt 1074, TASK */
    {0, 0x92, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE}, /* [1266] opt 1076, TASK */
    {5, 2, EOPID_INT(0x64), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1267] opt 1078, TASK */
    {0, 4, EOPID_PMF(FTaskComplete), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1268] opt 1080, TASK */
    {0, 4, EOPID_PMF(FIsCurrentTask), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1269] opt 1081, TASK */
    {6, 4, EOPID_PMF(OidFromTaskState), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1270] opt 1094, TASK */
    {0, 0x92, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE}, /* [1271] opt 1095, TASK */
    {0, 0x92, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE}, /* [1272] opt 1096, TASK */
    {0, 0x92, EOPID_INT(0x74), EOPID_INT(0x74), EOPID_INT(0x74), EOPID_NONE}, /* [1273] opt 1097, TASK */
    {6, 0x92, EOPID_INT(0xB8), EOPID_INT(0xB8), EOPID_INT(0xB8), EOPID_NONE}, /* [1274] opt 1098, TASK */
    {0xF, 0x92, EOPID_INT(0xBC), EOPID_INT(0xBC), EOPID_INT(0xBC), EOPID_NONE}, /* [1275] opt 1099, TASK */
    {6, 0x92, EOPID_INT(0xC0), EOPID_INT(0xC0), EOPID_INT(0xC0), EOPID_NONE}, /* [1276] opt 1100, TASK */
    {0, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1277] opt 1101, TASK */
    {0x201A, 0x8092, EOPID_INT(0xC4), EOPID_INT(0xC4), EOPID_INT(0xC4), EOPID_NONE}, /* [1278] opt 1102, TASK */
    {0x206B, 0x8092, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_NONE}, /* [1279] opt 1103, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_RequestTaskState), EOPID_INT(2), EOPID_NONE}, /* [1280] opt 1104, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ForceTaskState), EOPID_INT(2), EOPID_NONE}, /* [1281] opt 1105, TASK */
    {0x1051, 2, EOPID_INT(0xF4), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1282] opt 1082, TASK */
    {0x2002, 2, EOPID_INT(0xF8), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1283] opt 1083, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010EFB0), EOPID_INT(1), EOPID_NONE}, /* [1284] opt 1106, TASK */
    {1, 0x92, EOPID_INT(0x78), EOPID_INT(0x78), EOPID_INT(0x78), EOPID_NONE}, /* [1285] opt 1107, TASK */
    {0x104F, 2, EOPID_INT(0x54), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1286] opt 342, TASK */
    {0x12, 0x400, EOPID_FN(func_0010F030), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1287] opt 1108, TASK */
    {0x12, 0x400, EOPID_FN(RefThunkTaskSET_SM_GOAL), EOPID_NONE, EOPID_INT(6), EOPID_NONE}, /* [1288] opt 526, TASK */
    {0, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1289] opt 1109, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010F208), EOPID_NONE, EOPID_NONE}, /* [1290] opt 1110, TASK */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010F280), EOPID_NONE, EOPID_NONE}, /* [1291] opt 1111, TASK */
    {0x12, 0x4001, EOPID_NONE, EOPID_FN(func_0010F2F8), EOPID_NONE, EOPID_NONE}, /* [1292] opt 1112, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010F370), EOPID_NONE, EOPID_NONE}, /* [1293] opt 1113, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ActivateTask), EOPID_NONE, EOPID_NONE}, /* [1294] opt 1114, TASK */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xC8), EOPID_NONE}, /* [1295] opt 1090, TASK */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xD0), EOPID_NONE}, /* [1296] opt 1115, TASK */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xD8), EOPID_NONE}, /* [1297] opt 1091, TASK */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xE0), EOPID_NONE}, /* [1298] opt 1116, TASK */
    {0x16, 0x80, EOPID_NONE, EOPID_NONE, EOPID_INT(0xE8), EOPID_NONE}, /* [1299] opt 1117, TASK */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_PostNotice), EOPID_INT(2), EOPID_NONE}, /* [1300] opt 979, CID154 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_CancelNotice), EOPID_INT(1), EOPID_NONE}, /* [1301] opt 980, CID154 */
    {0x10, 2, EOPID_INT(0x18), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1302] opt 1118, CID154 */
    {0x10, 2, EOPID_INT(0x20), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1303] opt 1119, CID154 */
    {1, 0x82, EOPID_INT(0x150), EOPID_NONE, EOPID_INT(0x150), EOPID_NONE}, /* [1304] opt 1120, CID105 */
    {5, 0x82, EOPID_INT(0x158), EOPID_NONE, EOPID_INT(0x158), EOPID_NONE}, /* [1305] opt 1121, CID105 */
    {0, 0x82, EOPID_INT(0x154), EOPID_NONE, EOPID_INT(0x154), EOPID_NONE}, /* [1306] opt 1122, CID105 */
    {2, 0x92, EOPID_INT(0x170), EOPID_INT(0x170), EOPID_INT(0x170), EOPID_NONE}, /* [1307] opt 1123, CID105 */
    {1, 0x92, EOPID_INT(0x168), EOPID_INT(0x168), EOPID_INT(0x168), EOPID_NONE}, /* [1308] opt 1124, CID105 */
    {0x2003, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [1309] opt 1125, CID105 */
    {1, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [1310] opt 1126, CID105 */
    {0x200A, 0x82, EOPID_INT(0x150), EOPID_NONE, EOPID_INT(0x150), EOPID_NONE}, /* [1311] opt 1127, CID106 */
    {6, 0x82, EOPID_INT(0x110), EOPID_NONE, EOPID_INT(0x110), EOPID_NONE}, /* [1312] opt 1007, PATHZONE */
    {7, 0x92, EOPID_INT(0x154), EOPID_INT(0x154), EOPID_INT(0x154), EOPID_NONE}, /* [1313] opt 1129, WAYPT */
    {0xA, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [1314] opt 1130, WAYPT */
    {0x2085, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [1315] opt 1131, WAYPT */
    {0x1055, 0x92, EOPID_INT(0x150), EOPID_INT(0x150), EOPID_INT(0x150), EOPID_NONE}, /* [1316] opt 1132, WAYPT */
    {0x2003, 0x82, EOPID_INT(0x168), EOPID_NONE, EOPID_INT(0x168), EOPID_NONE}, /* [1317] opt 1133, WAYPT */
    {0x2030, 0x82, EOPID_INT(0x16C), EOPID_NONE, EOPID_INT(0x16C), EOPID_NONE}, /* [1318] opt 1134, WAYPT */
    {0x2030, 0x82, EOPID_INT(0x170), EOPID_NONE, EOPID_INT(0x170), EOPID_NONE}, /* [1319] opt 537, WAYPT */
    {0x2030, 0x82, EOPID_INT(0x174), EOPID_NONE, EOPID_INT(0x174), EOPID_NONE}, /* [1320] opt 1135, WAYPT */
    {0x12, 0, EOPID_NONE, EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1321] opt 1136, WAYPT */
    {0x2084, 0x82, EOPID_INT(0x188), EOPID_NONE, EOPID_INT(0x188), EOPID_NONE}, /* [1322] opt 1137, WAYPT */
    {0x1049, 0x82, EOPID_INT(0x184), EOPID_NONE, EOPID_INT(0x184), EOPID_NONE}, /* [1323] opt 1138, WAYPT */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001732E0), EOPID_NONE}, /* [1324] opt 1139, CID119 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_00173318), EOPID_NONE}, /* [1325] opt 1140, CID119 */
    {6, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_00173358), EOPID_NONE}, /* [1326] opt 1141, CID119 */
    {5, 0x92, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE}, /* [1327] opt 1142, CID119 */
    {0x2006, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_00173398), EOPID_NONE}, /* [1328] opt 1143, CID119 */
    {0, 0x92, EOPID_INT(0x84), EOPID_INT(0x84), EOPID_INT(0x84), EOPID_NONE}, /* [1329] opt 1144, CID119 */
    {0, 0x92, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_INT(0x88), EOPID_NONE}, /* [1330] opt 1145, CID119 */
    {0, 0x92, EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_INT(0x8C), EOPID_NONE}, /* [1331] opt 1146, CID119 */
    {1, 0x92, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_INT(0x80), EOPID_NONE}, /* [1332] opt 1147, CID119 */
    {2, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001733C8), EOPID_NONE}, /* [1333] opt 1148, CID119 */
    {1, 0x92, EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_INT(0xA0), EOPID_NONE}, /* [1334] opt 1149, CID119 */
    {1, 0x92, EOPID_INT(0xA4), EOPID_INT(0xA4), EOPID_INT(0xA4), EOPID_NONE}, /* [1335] opt 1150, CID119 */
    {0x1067, 0x92, EOPID_INT(0xAC), EOPID_INT(0xAC), EOPID_INT(0xAC), EOPID_NONE}, /* [1336] opt 1151, CID119 */
    {0x206F, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [1337] opt 1152, SAFEHOUSE */
    {0x12, 0x400, EOPID_FN(RefThunk_AddSafehouseEntry), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1338] opt 1153, SAFEHOUSE */
    {0x12, 0x401, EOPID_FN(RefThunk_SetPoChoices), EOPID_FN(RefThunk_SetPoChoices), EOPID_INT(1), EOPID_NONE}, /* [1339] opt 1154, SAFEHOUSE */
    {0x12, 0x400, EOPID_FN(func_0010F6E8), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1340] opt 1155, DIMITRI */
    {2, 1, EOPID_NONE, EOPID_FN(func_0010F7B0), EOPID_INT(1), EOPID_NONE}, /* [1341] opt 1156, DIMITRI */
    {0x206D, 0x92, EOPID_INT(0x2C0), EOPID_INT(0x2C0), EOPID_INT(0x2C0), EOPID_NONE}, /* [1342] opt 1157, DANCER */
    {0x12, 0x400, EOPID_FN(RefThunk_AddDancerMove), EOPID_NONE, EOPID_INT(5), EOPID_NONE}, /* [1343] opt 1158, DANCER */
    {1, 0x92, EOPID_INT(0x58), EOPID_INT(0x58), EOPID_INT(0x58), EOPID_NONE}, /* [1344] opt 1159, CID121 */
    {1, 0x92, EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_INT(0x5C), EOPID_NONE}, /* [1345] opt 1160, CID121 */
    {0x12, 0x400, EOPID_FN(func_0010F918), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1346] opt 1161, CID121 */
    {0xA, 0x82, EOPID_INT(0x234), EOPID_NONE, EOPID_INT(0x234), EOPID_NONE}, /* [1347] opt 1162, CID121 */
    {5, 0x92, EOPID_INT(0x280), EOPID_INT(0x280), EOPID_INT(0x280), EOPID_NONE}, /* [1348] opt 1163, CID121 */
    {0xB, 0x92, EOPID_INT(0x28C), EOPID_INT(0x28C), EOPID_INT(0x28C), EOPID_NONE}, /* [1349] opt 1164, CID121 */
    {1, 0x92, EOPID_INT(0x298), EOPID_INT(0x298), EOPID_INT(0x298), EOPID_NONE}, /* [1350] opt 1165, CID121 */
    {1, 0x92, EOPID_INT(0x29C), EOPID_INT(0x29C), EOPID_INT(0x29C), EOPID_NONE}, /* [1351] opt 1166, CID121 */
    {1, 0x92, EOPID_INT(0x2A0), EOPID_INT(0x2A0), EOPID_INT(0x2A0), EOPID_NONE}, /* [1352] opt 1167, CID121 */
    {1, 0x92, EOPID_INT(0x2A4), EOPID_INT(0x2A4), EOPID_INT(0x2A4), EOPID_NONE}, /* [1353] opt 1168, CID121 */
    {0x1059, 0x122, EOPID_INT(0x50), EOPID_PMF(SetRhythmMgrState), EOPID_PMF(SetRhythmMgrState), EOPID_NONE}, /* [1354] opt 342, CID121 */
    {1, 2, EOPID_INT(0x284), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1355] opt 1169, CID121 */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunk_ResetRhythmScore), EOPID_NONE, EOPID_NONE}, /* [1356] opt 1170, CID121 */
    {0x2033, 2, EOPID_INT(0x10), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1357] opt 1171, BHV (+43) */
    {0x2071, 2, EOPID_INT(0x30), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1358] opt 1172, BHV169 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010FA38), EOPID_INT(1), EOPID_NONE}, /* [1359] opt 236, BHV169 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010FAC0), EOPID_INT(1), EOPID_NONE}, /* [1360] opt 1173, CARMELITA */
    {0x105A, 0x92, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE}, /* [1361] opt 278, CID123 */
    {0x12, 0x400, EOPID_FN(func_0010FB48), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1362] opt 1174, CID123 */
    {5, 0x92, EOPID_INT(0x180), EOPID_INT(0x180), EOPID_INT(0x180), EOPID_NONE}, /* [1363] opt 380, HACK (+5) */
    {5, 0x92, EOPID_INT(0x184), EOPID_INT(0x184), EOPID_INT(0x184), EOPID_NONE}, /* [1364] opt 381, HACK (+5) */
    {5, 0x92, EOPID_INT(0x188), EOPID_INT(0x188), EOPID_INT(0x188), EOPID_NONE}, /* [1365] opt 123, HACK (+5) */
    {0x201E, 0x92, EOPID_INT(0x190), EOPID_INT(0x190), EOPID_INT(0x190), EOPID_NONE}, /* [1366] opt 497, HACK (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010FBF0), EOPID_INT(1), EOPID_NONE}, /* [1367] opt 493, HACK (+5) */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_0010FC80), EOPID_NONE, EOPID_NONE}, /* [1368] opt 454, HACK (+5) */
    {9, 0x92, EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_INT(0x1A0), EOPID_NONE}, /* [1369] opt 1176, HACK (+5) */
    {0xA, 0x92, EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_INT(0x1B0), EOPID_NONE}, /* [1370] opt 1177, HACK (+5) */
    {0x12, 0x400, EOPID_FN(RefThunk_AddHackshipOidPair), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1371] opt 1178, HACKSHIP */
    {0x12, 0x400, EOPID_FN(func_0010FDA8), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1372] opt 369, HACKSHIP */
    {0x207E, 0x92, EOPID_INT(0x2A0), EOPID_INT(0x2A0), EOPID_INT(0x2A0), EOPID_NONE}, /* [1373] opt 1179, HACKSHIP */
    {0xE, 0x92, EOPID_INT(0x2A8), EOPID_INT(0x2A8), EOPID_INT(0x2A8), EOPID_NONE}, /* [1374] opt 1180, HACKSHIP */
    {0x12, 0x400, EOPID_FN(func_0010FE60), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1375] opt 369, HACKENEMY */
    {1, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001813C0), EOPID_NONE}, /* [1376] opt 350, HACKENEMY */
    {1, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001813F0), EOPID_NONE}, /* [1377] opt 1181, HACKENEMY */
    {0x2011, 0x92, EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_NONE}, /* [1378] opt 425, HACKENEMY */
    {0x2002, 0x92, EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_NONE}, /* [1379] opt 386, HACKENEMY */
    {0x12, 0x401, EOPID_FN(func_0010FF18), EOPID_FN(func_0010FF18), EOPID_INT(3), EOPID_NONE}, /* [1380] opt 1182, HACKENEMY */
    {0x12, 0x401, EOPID_FN(func_0010FFE8), EOPID_FN(func_0010FFE8), EOPID_INT(3), EOPID_NONE}, /* [1381] opt 1183, HACKENEMY */
    {1, 0x92, EOPID_INT(0x338), EOPID_INT(0x338), EOPID_INT(0x338), EOPID_NONE}, /* [1382] opt 1184, HACKENEMY */
    {9, 0x92, EOPID_INT(0x340), EOPID_INT(0x340), EOPID_INT(0x340), EOPID_NONE}, /* [1383] opt 1185, HACKENEMY */
    {9, 0x92, EOPID_INT(0x350), EOPID_INT(0x350), EOPID_INT(0x350), EOPID_NONE}, /* [1384] opt 1186, HACKENEMY */
    {9, 0x92, EOPID_INT(0x360), EOPID_INT(0x360), EOPID_INT(0x360), EOPID_NONE}, /* [1385] opt 1187, HACKENEMY */
    {0xA, 0x92, EOPID_INT(0x370), EOPID_INT(0x370), EOPID_INT(0x370), EOPID_NONE}, /* [1386] opt 1188, HACKENEMY */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001100B8), EOPID_INT(1), EOPID_NONE}, /* [1387] opt 508, HACKBULLET */
    {0, 0x92, EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_NONE}, /* [1388] opt 1189, HACKBULLET */
    {1, 0x92, EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_NONE}, /* [1389] opt 1190, HACKBULLET */
    {0x2030, 0x92, EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_INT(0x1D0), EOPID_NONE}, /* [1390] opt 1191, HACKBULLET */
    {1, 0x92, EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_INT(0x1D4), EOPID_NONE}, /* [1391] opt 1192, HACKBULLET */
    {0x12, 0x400, EOPID_FN(func_00110140), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1392] opt 369, HACKLOCK */
    {0x12, 0x400, EOPID_FN(RefThunkHacklockSET_SM_GOAL), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1393] opt 526, HACKLOCK */
    {0x2003, 0x92, EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_INT(0x1CC), EOPID_NONE}, /* [1394] opt 1193, HACKLOCK */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_001102C0), EOPID_NONE, EOPID_NONE}, /* [1395] opt 1194, HACKLOCK */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkHacklockTRIGGER), EOPID_NONE, EOPID_NONE}, /* [1396] opt 235, HACKLOCK */
    {0x2002, 0x92, EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_INT(0x1C0), EOPID_NONE}, /* [1397] opt 386, HACKWALL */
    {0x12, 0x400, EOPID_FN(func_001103B0), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1398] opt 1195, HACKWALL */
    {0x2030, 0x92, EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_INT(0x1C4), EOPID_NONE}, /* [1399] opt 1196, HACKWALL */
    {0x103C, 0x12, EOPID_INT(0x24), EOPID_INT(0x24), EOPID_NONE, EOPID_NONE}, /* [1400] opt 1197, JSG */
    {0x103C, 0x12, EOPID_INT(0x28), EOPID_INT(0x28), EOPID_NONE, EOPID_NONE}, /* [1401] opt 1198, JSG */
    {0xA, 0x12, EOPID_INT(0x40), EOPID_INT(0x40), EOPID_NONE, EOPID_NONE}, /* [1402] opt 1199, JSG */
    {9, 0x12, EOPID_INT(0x50), EOPID_INT(0x50), EOPID_NONE, EOPID_NONE}, /* [1403] opt 1200, JSG */
    {0xA, 0x12, EOPID_INT(0x60), EOPID_INT(0x60), EOPID_NONE, EOPID_NONE}, /* [1404] opt 1201, JSG */
    {0xA, 0x12, EOPID_INT(0x48), EOPID_INT(0x48), EOPID_NONE, EOPID_NONE}, /* [1405] opt 1202, JSG */
    {1, 0x12, EOPID_INT(0x68), EOPID_INT(0x68), EOPID_NONE, EOPID_NONE}, /* [1406] opt 1203, JSG */
    {1, 2, EOPID_INT(0x74), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1407] opt 1204, JSG */
    {1, 2, EOPID_INT(0x78), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1408] opt 253, JSG */
    {0x1064, 0x12, EOPID_INT(0x80), EOPID_INT(0x80), EOPID_NONE, EOPID_NONE}, /* [1409] opt 1205, JSG */
    {1, 0x12, EOPID_INT(0x7C), EOPID_INT(0x7C), EOPID_NONE, EOPID_NONE}, /* [1410] opt 1206, JSG */
    {0x2016, 0x12, EOPID_INT(0x84), EOPID_INT(0x84), EOPID_NONE, EOPID_NONE}, /* [1411] opt 1207, JSG */
    {0xA, 0x12, EOPID_INT(0x88), EOPID_INT(0x88), EOPID_NONE, EOPID_NONE}, /* [1412] opt 1208, JSG */
    {0xA, 0x12, EOPID_INT(0x90), EOPID_INT(0x90), EOPID_NONE, EOPID_NONE}, /* [1413] opt 1209, JSG */
    {0xA, 0x12, EOPID_INT(0x98), EOPID_INT(0x98), EOPID_NONE, EOPID_NONE}, /* [1414] opt 1210, JSG */
    {0, 2, EOPID_INT(0xA0), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1415] opt 1211, JSG */
    {0x1065, 0x12, EOPID_INT(0x6C), EOPID_INT(0x6C), EOPID_NONE, EOPID_NONE}, /* [1416] opt 1212, JSG */
    {5, 0x12, EOPID_INT(0x70), EOPID_INT(0x70), EOPID_NONE, EOPID_NONE}, /* [1417] opt 1213, JSG */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00110478), EOPID_NONE, EOPID_NONE}, /* [1418] opt 1214, JSG */
    {0x12, 0x400, EOPID_FN(func_001104F0), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1419] opt 1215, ROBOTTANK */
    {0x12, 0x400, EOPID_FN(func_001105B8), EOPID_NONE, EOPID_INT(4), EOPID_NONE}, /* [1420] opt 1216, ROBOTTANK */
    {0x12, 0x400, EOPID_FN(func_00110680), EOPID_NONE, EOPID_INT(2), EOPID_NONE}, /* [1421] opt 1178, ROBOTTANK */
    {0x2099, 1, EOPID_NONE, EOPID_FN(func_00110738), EOPID_INT(1), EOPID_NONE}, /* [1422] opt 1217, ROBOTTANK */
    {0x2002, 0x92, EOPID_INT(0x5A4), EOPID_INT(0x5A4), EOPID_INT(0x5A4), EOPID_NONE}, /* [1423] opt 386, ROBOTTANK */
    {0x2011, 0x92, EOPID_INT(0x5A8), EOPID_INT(0x5A8), EOPID_INT(0x5A8), EOPID_NONE}, /* [1424] opt 425, ROBOTTANK */
    {0x1063, 0x22, EOPID_INT(0x180), EOPID_PMF(SetRobottankState), EOPID_NONE, EOPID_NONE}, /* [1425] opt 342, ROBOTTANK */
    {5, 0x82, EOPID_INT(0x55C), EOPID_NONE, EOPID_INT(0x55C), EOPID_NONE}, /* [1426] opt 380, ROBOTTANK */
    {5, 2, EOPID_INT(0x560), EOPID_NONE, EOPID_NONE, EOPID_NONE}, /* [1427] opt 381, ROBOTTANK */
    {0x207E, 0x92, EOPID_INT(0x568), EOPID_INT(0x568), EOPID_INT(0x568), EOPID_NONE}, /* [1428] opt 1179, ROBOTTANK */
    {0, 0x92, EOPID_INT(0x564), EOPID_INT(0x564), EOPID_INT(0x564), EOPID_NONE}, /* [1429] opt 1218, ROBOTTANK */
    {0x206F, 0x92, EOPID_INT(0x56C), EOPID_INT(0x56C), EOPID_INT(0x56C), EOPID_NONE}, /* [1430] opt 484, ROBOTTANK */
    {5, 1, EOPID_NONE, EOPID_FN(func_001107B8), EOPID_INT(1), EOPID_NONE}, /* [1431] opt 1219, ROBOTTANK */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00110838), EOPID_INT(2), EOPID_NONE}, /* [1432] opt 1220, ROBOTTANK */
    {0x12, 0x400, EOPID_FN(func_001108E0), EOPID_NONE, EOPID_INT(3), EOPID_NONE}, /* [1433] opt 1221, ROBOTTANK */
    {0xE, 0x92, EOPID_INT(0x59C), EOPID_INT(0x59C), EOPID_INT(0x59C), EOPID_NONE}, /* [1434] opt 1222, ROBOTTANK */
    {0x1065, 0x92, EOPID_INT(0x158), EOPID_INT(0x158), EOPID_INT(0x158), EOPID_NONE}, /* [1435] opt 1212, CID108 */
    {5, 0x92, EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_INT(0x15C), EOPID_NONE}, /* [1436] opt 1223, CID108 */
    {0x206F, 0x92, EOPID_INT(0x160), EOPID_INT(0x160), EOPID_INT(0x160), EOPID_NONE}, /* [1437] opt 484, CID108 */
    {0x12, 1, EOPID_NONE, EOPID_FN(func_00110998), EOPID_INT(1), EOPID_NONE}, /* [1438] opt 1224, RAJAN */
    {5, 0x92, EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_INT(0x1F0), EOPID_NONE}, /* [1439] opt 1225, VAULT */
    {0x10, 0x92, EOPID_INT(0x1F4), EOPID_INT(0x1F4), EOPID_INT(0x1F4), EOPID_NONE}, /* [1440] opt 1226, VAULT */
    {0x10, 0x92, EOPID_INT(0x1F8), EOPID_INT(0x1F8), EOPID_INT(0x1F8), EOPID_NONE}, /* [1441] opt 1227, VAULT */
    {0x1049, 0x92, EOPID_INT(0x200), EOPID_INT(0x200), EOPID_INT(0x200), EOPID_NONE}, /* [1442] opt 1228, VAULT */
    {0x10, 0x92, EOPID_INT(0x1FC), EOPID_INT(0x1FC), EOPID_INT(0x1FC), EOPID_NONE}, /* [1443] opt 1229, VAULT */
    {0x12, 1, EOPID_NONE, EOPID_FN(RefThunkVaultTRIGGER), EOPID_INT(1), EOPID_NONE}, /* [1444] opt 235, VAULT */
    {0x106C, 0x92, EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_INT(0xE0), EOPID_NONE}, /* [1445] opt 1230, CID20 */
    {0x106E, 0x92, EOPID_INT(0x120), EOPID_INT(0x120), EOPID_INT(0x120), EOPID_NONE}, /* [1446] opt 1231, PATHLINK */
    {0x2002, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001C20E8), EOPID_NONE}, /* [1447] opt 238, PATHLINK */
    {2, 0x100, EOPID_NONE, EOPID_NONE, EOPID_PMF(func_001C2148), EOPID_NONE}, /* [1448] opt 1232, PATHLINK */
};
#undef EOPID_NONE
#undef EOPID_INT
#undef EOPID_PMF
#undef EOPID_VPMF
#undef EOPID_FN

/* The per-class option arrays (.rodata.early), CID order = address order. */
struct SPLOPTROW { int opid; const EOPIDROW *peopid; };
#define SPLOPT_RODATA __attribute__((section(".rodata.early")))
extern const SPLOPTROW D_00262380[13] SPLOPT_RODATA = {   /* BASIC (cid 0) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]},
};
extern const SPLOPTROW D_002623E8[45] SPLOPT_RODATA = {   /* SW (cid 1) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {454, &D_002AC880[933]}, {768, &D_002AC880[924]},
    {771, &D_002AC880[913]}, {833, &D_002AC880[914]}, {834, &D_002AC880[915]}, {835, &D_002AC880[916]}, {836, &D_002AC880[917]},
    {837, &D_002AC880[918]}, {838, &D_002AC880[919]}, {839, &D_002AC880[920]}, {840, &D_002AC880[921]}, {841, &D_002AC880[922]},
    {842, &D_002AC880[923]}, {843, &D_002AC880[925]}, {844, &D_002AC880[926]}, {845, &D_002AC880[927]}, {846, &D_002AC880[928]},
    {847, &D_002AC880[929]}, {848, &D_002AC880[930]}, {849, &D_002AC880[931]}, {850, &D_002AC880[932]}, {852, &D_002AC880[934]},
    {853, &D_002AC880[935]}, {854, &D_002AC880[936]}, {855, &D_002AC880[937]}, {856, &D_002AC880[938]}, {857, &D_002AC880[939]},
    {858, &D_002AC880[940]}, {859, &D_002AC880[941]}, {861, &D_002AC880[942]}, {862, &D_002AC880[943]}, {863, &D_002AC880[944]},
};
extern const SPLOPTROW D_00262550[33] SPLOPT_RODATA = {   /* LO (cid 2) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00262658[33] SPLOPT_RODATA = {   /* XFM (cid 3) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00262760[41] SPLOPT_RODATA = {   /* LIGHT (cid 4) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {278, &D_002AC880[151]}, {279, &D_002AC880[152]},
    {280, &D_002AC880[153]}, {281, &D_002AC880[154]}, {282, &D_002AC880[155]}, {283, &D_002AC880[156]}, {290, &D_002AC880[157]},
    {291, &D_002AC880[158]},
};
extern const SPLOPTROW D_002628A8[34] SPLOPT_RODATA = {   /* CID5 (cid 5) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {220, &D_002AC880[175]},
};
extern const SPLOPTROW D_002629B8[41] SPLOPT_RODATA = {   /* PROXY (cid 6) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1017, &D_002AC880[1174]}, {1019, &D_002AC880[1175]},
    {1020, &D_002AC880[1176]}, {1021, &D_002AC880[1177]}, {1022, &D_002AC880[1178]}, {1023, &D_002AC880[1179]}, {1024, &D_002AC880[1180]},
    {1025, &D_002AC880[1181]},
};
extern const SPLOPTROW D_00262B00[41] SPLOPT_RODATA = {   /* CLUEPROXY (cid 7) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1017, &D_002AC880[1174]}, {1019, &D_002AC880[1175]},
    {1020, &D_002AC880[1176]}, {1021, &D_002AC880[1177]}, {1022, &D_002AC880[1178]}, {1023, &D_002AC880[1179]}, {1024, &D_002AC880[1180]},
    {1025, &D_002AC880[1181]},
};
extern const SPLOPTROW D_00262C48[33] SPLOPT_RODATA = {   /* CID8 (cid 8) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00262D50[36] SPLOPT_RODATA = {   /* CID9 (cid 9) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[872]}, {778, &D_002AC880[871]},
    {796, &D_002AC880[873]},
};
extern const SPLOPTROW D_00262E70[49] SPLOPT_RODATA = {   /* SHAPE (cid 10) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1042, &D_002AC880[1208]}, {1043, &D_002AC880[1209]},
    {1044, &D_002AC880[1210]}, {1045, &D_002AC880[1211]}, {1046, &D_002AC880[1212]}, {1047, &D_002AC880[1213]}, {1048, &D_002AC880[1214]},
    {1049, &D_002AC880[1215]}, {1050, &D_002AC880[1216]}, {1051, &D_002AC880[1217]}, {1052, &D_002AC880[1218]}, {1053, &D_002AC880[1219]},
    {1054, &D_002AC880[1220]}, {1055, &D_002AC880[1221]}, {1056, &D_002AC880[1222]}, {1057, &D_002AC880[1223]},
};
extern const SPLOPTROW D_00262FF8[61] SPLOPT_RODATA = {   /* PIPE (cid 11) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[845]}, {776, &D_002AC880[842]},
    {777, &D_002AC880[843]}, {778, &D_002AC880[844]}, {779, &D_002AC880[846]}, {780, &D_002AC880[847]}, {781, &D_002AC880[848]},
    {782, &D_002AC880[849]}, {788, &D_002AC880[838]}, {789, &D_002AC880[839]}, {790, &D_002AC880[840]}, {791, &D_002AC880[841]},
    {1042, &D_002AC880[1208]}, {1043, &D_002AC880[1209]}, {1044, &D_002AC880[1210]}, {1045, &D_002AC880[1211]}, {1046, &D_002AC880[1212]},
    {1047, &D_002AC880[1213]}, {1048, &D_002AC880[1214]}, {1049, &D_002AC880[1215]}, {1050, &D_002AC880[1216]}, {1051, &D_002AC880[1217]},
    {1052, &D_002AC880[1218]}, {1053, &D_002AC880[1219]}, {1054, &D_002AC880[1220]}, {1055, &D_002AC880[1221]}, {1056, &D_002AC880[1222]},
    {1057, &D_002AC880[1223]},
};
extern const SPLOPTROW D_002631E0[59] SPLOPT_RODATA = {   /* CID12 (cid 12) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[811]}, {775, &D_002AC880[807]},
    {776, &D_002AC880[808]}, {777, &D_002AC880[809]}, {778, &D_002AC880[810]}, {779, &D_002AC880[812]}, {780, &D_002AC880[813]},
    {781, &D_002AC880[814]}, {782, &D_002AC880[815]}, {783, &D_002AC880[806]}, {1042, &D_002AC880[1208]}, {1043, &D_002AC880[1209]},
    {1044, &D_002AC880[1210]}, {1045, &D_002AC880[1211]}, {1046, &D_002AC880[1212]}, {1047, &D_002AC880[1213]}, {1048, &D_002AC880[1214]},
    {1049, &D_002AC880[1215]}, {1050, &D_002AC880[1216]}, {1051, &D_002AC880[1217]}, {1052, &D_002AC880[1218]}, {1053, &D_002AC880[1219]},
    {1054, &D_002AC880[1220]}, {1055, &D_002AC880[1221]}, {1056, &D_002AC880[1222]}, {1057, &D_002AC880[1223]},
};
extern const SPLOPTROW D_002633B8[61] SPLOPT_RODATA = {   /* RAIL (cid 13) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[865]}, {351, &D_002AC880[860]},
    {776, &D_002AC880[862]}, {777, &D_002AC880[863]}, {778, &D_002AC880[864]}, {779, &D_002AC880[866]}, {780, &D_002AC880[867]},
    {781, &D_002AC880[868]}, {782, &D_002AC880[869]}, {792, &D_002AC880[858]}, {793, &D_002AC880[859]}, {794, &D_002AC880[861]},
    {1042, &D_002AC880[1208]}, {1043, &D_002AC880[1209]}, {1044, &D_002AC880[1210]}, {1045, &D_002AC880[1211]}, {1046, &D_002AC880[1212]},
    {1047, &D_002AC880[1213]}, {1048, &D_002AC880[1214]}, {1049, &D_002AC880[1215]}, {1050, &D_002AC880[1216]}, {1051, &D_002AC880[1217]},
    {1052, &D_002AC880[1218]}, {1053, &D_002AC880[1219]}, {1054, &D_002AC880[1220]}, {1055, &D_002AC880[1221]}, {1056, &D_002AC880[1222]},
    {1057, &D_002AC880[1223]},
};
extern const SPLOPTROW D_002635A0[49] SPLOPT_RODATA = {   /* CID14 (cid 14) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1042, &D_002AC880[1208]}, {1043, &D_002AC880[1209]},
    {1044, &D_002AC880[1210]}, {1045, &D_002AC880[1211]}, {1046, &D_002AC880[1212]}, {1047, &D_002AC880[1213]}, {1048, &D_002AC880[1214]},
    {1049, &D_002AC880[1215]}, {1050, &D_002AC880[1216]}, {1051, &D_002AC880[1217]}, {1052, &D_002AC880[1218]}, {1053, &D_002AC880[1219]},
    {1054, &D_002AC880[1220]}, {1055, &D_002AC880[1221]}, {1056, &D_002AC880[1222]}, {1057, &D_002AC880[1223]},
};
extern const SPLOPTROW D_00263728[72] SPLOPT_RODATA = {   /* TN (cid 15) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {220, &D_002AC880[200]}, {293, &D_002AC880[208]},
    {307, &D_002AC880[176]}, {308, &D_002AC880[177]}, {309, &D_002AC880[178]}, {310, &D_002AC880[179]}, {311, &D_002AC880[180]},
    {312, &D_002AC880[181]}, {313, &D_002AC880[182]}, {314, &D_002AC880[183]}, {315, &D_002AC880[184]}, {316, &D_002AC880[185]},
    {317, &D_002AC880[186]}, {318, &D_002AC880[187]}, {319, &D_002AC880[188]}, {320, &D_002AC880[189]}, {321, &D_002AC880[190]},
    {322, &D_002AC880[191]}, {323, &D_002AC880[192]}, {324, &D_002AC880[193]}, {325, &D_002AC880[194]}, {326, &D_002AC880[195]},
    {327, &D_002AC880[196]}, {328, &D_002AC880[197]}, {329, &D_002AC880[198]}, {330, &D_002AC880[199]}, {331, &D_002AC880[201]},
    {332, &D_002AC880[202]}, {333, &D_002AC880[203]}, {334, &D_002AC880[204]}, {335, &D_002AC880[205]}, {336, &D_002AC880[206]},
    {337, &D_002AC880[207]}, {338, &D_002AC880[209]}, {339, &D_002AC880[210]}, {340, &D_002AC880[211]}, {341, &D_002AC880[212]},
    {342, &D_002AC880[213]}, {343, &D_002AC880[214]},
};
extern const SPLOPTROW D_00263968[44] SPLOPT_RODATA = {   /* DETECTOR (cid 16) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {436, &D_002AC880[342]}, {437, &D_002AC880[343]},
    {438, &D_002AC880[344]}, {439, &D_002AC880[345]}, {440, &D_002AC880[346]}, {458, &D_002AC880[351]}, {459, &D_002AC880[352]},
    {466, &D_002AC880[347]}, {467, &D_002AC880[348]}, {468, &D_002AC880[349]}, {469, &D_002AC880[350]},
};
extern const SPLOPTROW D_00263AC8[34] SPLOPT_RODATA = {   /* PATHZONE (cid 17) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1007, &D_002AC880[1312]},
};
extern const SPLOPTROW D_00263BD8[37] SPLOPT_RODATA = {   /* TARGET (cid 18) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {119, &D_002AC880[215]}, {120, &D_002AC880[216]},
    {121, &D_002AC880[217]}, {344, &D_002AC880[218]},
};
extern const SPLOPTROW D_00263D00[51] SPLOPT_RODATA = {   /* HND (cid 19) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {119, &D_002AC880[215]}, {120, &D_002AC880[216]},
    {121, &D_002AC880[217]}, {141, &D_002AC880[1020]}, {344, &D_002AC880[218]}, {503, &D_002AC880[1016]}, {776, &D_002AC880[1017]},
    {777, &D_002AC880[1018]}, {778, &D_002AC880[1019]}, {779, &D_002AC880[1021]}, {780, &D_002AC880[1022]}, {781, &D_002AC880[1023]},
    {782, &D_002AC880[1024]}, {789, &D_002AC880[1013]}, {790, &D_002AC880[1014]}, {923, &D_002AC880[1011]}, {924, &D_002AC880[1012]},
    {925, &D_002AC880[1015]},
};
extern const SPLOPTROW D_00263E98[34] SPLOPT_RODATA = {   /* CID20 (cid 20) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1230, &D_002AC880[1445]},
};
extern const SPLOPTROW D_00263FA8[36] SPLOPT_RODATA = {   /* CID21 (cid 21) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {345, &D_002AC880[219]}, {346, &D_002AC880[220]},
    {347, &D_002AC880[221]},
};
extern const SPLOPTROW D_002640C8[35] SPLOPT_RODATA = {   /* VOL (cid 22) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {474, &D_002AC880[367]},
};
extern const SPLOPTROW D_002641E0[35] SPLOPT_RODATA = {   /* CID23 (cid 23) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {474, &D_002AC880[367]},
};
extern const SPLOPTROW D_002642F8[53] SPLOPT_RODATA = {   /* VOLBTN (cid 24) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {235, &D_002AC880[445]},
    {474, &D_002AC880[367]}, {517, &D_002AC880[430]}, {518, &D_002AC880[431]}, {519, &D_002AC880[432]}, {520, &D_002AC880[433]},
    {521, &D_002AC880[434]}, {522, &D_002AC880[435]}, {523, &D_002AC880[436]}, {524, &D_002AC880[437]}, {525, &D_002AC880[438]},
    {526, &D_002AC880[439]}, {527, &D_002AC880[440]}, {528, &D_002AC880[442]}, {530, &D_002AC880[446]}, {531, &D_002AC880[441]},
    {532, &D_002AC880[443]}, {533, &D_002AC880[444]}, {534, &D_002AC880[447]},
};
extern const SPLOPTROW D_002644A0[53] SPLOPT_RODATA = {   /* CID25 (cid 25) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {235, &D_002AC880[445]},
    {474, &D_002AC880[367]}, {517, &D_002AC880[430]}, {518, &D_002AC880[431]}, {519, &D_002AC880[432]}, {520, &D_002AC880[433]},
    {521, &D_002AC880[434]}, {522, &D_002AC880[435]}, {523, &D_002AC880[436]}, {524, &D_002AC880[437]}, {525, &D_002AC880[438]},
    {526, &D_002AC880[439]}, {527, &D_002AC880[440]}, {528, &D_002AC880[442]}, {530, &D_002AC880[446]}, {531, &D_002AC880[441]},
    {532, &D_002AC880[443]}, {533, &D_002AC880[444]}, {534, &D_002AC880[447]},
};
extern const SPLOPTROW D_00264648[46] SPLOPT_RODATA = {   /* EXIT (cid 26) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {235, &D_002AC880[786]},
    {474, &D_002AC880[367]}, {527, &D_002AC880[788]}, {761, &D_002AC880[779]}, {764, &D_002AC880[780]}, {765, &D_002AC880[781]},
    {766, &D_002AC880[782]}, {767, &D_002AC880[783]}, {768, &D_002AC880[784]}, {769, &D_002AC880[785]}, {770, &D_002AC880[787]},
    {771, &D_002AC880[789]},
};
extern const SPLOPTROW D_002647B8[44] SPLOPT_RODATA = {   /* CID27 (cid 27) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {108, &D_002AC880[366]}, {141, &D_002AC880[1186]},
    {474, &D_002AC880[367]}, {776, &D_002AC880[1183]}, {777, &D_002AC880[1184]}, {778, &D_002AC880[1185]}, {779, &D_002AC880[1187]},
    {780, &D_002AC880[1188]}, {781, &D_002AC880[1189]}, {782, &D_002AC880[1190]}, {1026, &D_002AC880[1182]},
};
extern const SPLOPTROW D_00264918[33] SPLOPT_RODATA = {   /* BLIPG (cid 28) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00264A20[98] SPLOPT_RODATA = {   /* EMITTER (cid 29) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {256, &D_002AC880[478]}, {325, &D_002AC880[502]},
    {551, &D_002AC880[469]}, {552, &D_002AC880[470]}, {553, &D_002AC880[471]}, {554, &D_002AC880[472]}, {555, &D_002AC880[473]},
    {556, &D_002AC880[474]}, {557, &D_002AC880[475]}, {558, &D_002AC880[476]}, {559, &D_002AC880[477]}, {560, &D_002AC880[479]},
    {561, &D_002AC880[480]}, {562, &D_002AC880[481]}, {563, &D_002AC880[482]}, {564, &D_002AC880[483]}, {565, &D_002AC880[484]},
    {566, &D_002AC880[485]}, {567, &D_002AC880[486]}, {569, &D_002AC880[487]}, {570, &D_002AC880[488]}, {571, &D_002AC880[489]},
    {572, &D_002AC880[490]}, {573, &D_002AC880[491]}, {574, &D_002AC880[492]}, {575, &D_002AC880[493]}, {576, &D_002AC880[494]},
    {577, &D_002AC880[495]}, {578, &D_002AC880[496]}, {579, &D_002AC880[497]}, {580, &D_002AC880[498]}, {581, &D_002AC880[499]},
    {582, &D_002AC880[500]}, {583, &D_002AC880[501]}, {584, &D_002AC880[503]}, {585, &D_002AC880[504]}, {586, &D_002AC880[505]},
    {587, &D_002AC880[506]}, {588, &D_002AC880[507]}, {589, &D_002AC880[508]}, {590, &D_002AC880[509]}, {591, &D_002AC880[510]},
    {592, &D_002AC880[511]}, {593, &D_002AC880[512]}, {594, &D_002AC880[513]}, {595, &D_002AC880[514]}, {596, &D_002AC880[515]},
    {597, &D_002AC880[516]}, {598, &D_002AC880[517]}, {599, &D_002AC880[518]}, {600, &D_002AC880[519]}, {601, &D_002AC880[520]},
    {602, &D_002AC880[521]}, {603, &D_002AC880[522]}, {604, &D_002AC880[523]}, {605, &D_002AC880[524]}, {606, &D_002AC880[525]},
    {607, &D_002AC880[526]}, {608, &D_002AC880[527]}, {609, &D_002AC880[528]}, {611, &D_002AC880[529]}, {612, &D_002AC880[530]},
    {613, &D_002AC880[531]}, {614, &D_002AC880[532]}, {615, &D_002AC880[533]},
};
extern const SPLOPTROW D_00264D30[35] SPLOPT_RODATA = {   /* EXPL (cid 30) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {244, &D_002AC880[534]}, {616, &D_002AC880[535]},
};
extern const SPLOPTROW D_00264E48[35] SPLOPT_RODATA = {   /* EXPLG (cid 31) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {244, &D_002AC880[534]}, {616, &D_002AC880[535]},
};
extern const SPLOPTROW D_00264F60[54] SPLOPT_RODATA = {   /* EXPLO (cid 32) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {244, &D_002AC880[534]}, {325, &D_002AC880[550]},
    {564, &D_002AC880[536]}, {565, &D_002AC880[537]}, {566, &D_002AC880[538]}, {567, &D_002AC880[539]}, {569, &D_002AC880[540]},
    {570, &D_002AC880[541]}, {572, &D_002AC880[542]}, {573, &D_002AC880[543]}, {574, &D_002AC880[544]}, {575, &D_002AC880[545]},
    {576, &D_002AC880[546]}, {577, &D_002AC880[547]}, {578, &D_002AC880[548]}, {579, &D_002AC880[549]}, {584, &D_002AC880[551]},
    {585, &D_002AC880[552]}, {587, &D_002AC880[553]}, {590, &D_002AC880[554]}, {616, &D_002AC880[535]},
};
extern const SPLOPTROW D_00265110[93] SPLOPT_RODATA = {   /* EXPLS (cid 33) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {209, &D_002AC880[589]}, {244, &D_002AC880[534]},
    {325, &D_002AC880[550]}, {551, &D_002AC880[555]}, {563, &D_002AC880[557]}, {564, &D_002AC880[536]}, {565, &D_002AC880[537]},
    {566, &D_002AC880[538]}, {567, &D_002AC880[539]}, {569, &D_002AC880[540]}, {570, &D_002AC880[541]}, {572, &D_002AC880[542]},
    {573, &D_002AC880[543]}, {574, &D_002AC880[544]}, {575, &D_002AC880[545]}, {576, &D_002AC880[546]}, {577, &D_002AC880[547]},
    {578, &D_002AC880[548]}, {579, &D_002AC880[549]}, {580, &D_002AC880[558]}, {581, &D_002AC880[559]}, {582, &D_002AC880[560]},
    {583, &D_002AC880[561]}, {584, &D_002AC880[551]}, {585, &D_002AC880[552]}, {586, &D_002AC880[562]}, {587, &D_002AC880[553]},
    {588, &D_002AC880[563]}, {589, &D_002AC880[564]}, {590, &D_002AC880[554]}, {591, &D_002AC880[565]}, {592, &D_002AC880[566]},
    {593, &D_002AC880[567]}, {594, &D_002AC880[568]}, {595, &D_002AC880[569]}, {596, &D_002AC880[570]}, {597, &D_002AC880[571]},
    {598, &D_002AC880[572]}, {599, &D_002AC880[573]}, {600, &D_002AC880[574]}, {601, &D_002AC880[575]}, {602, &D_002AC880[576]},
    {603, &D_002AC880[577]}, {604, &D_002AC880[578]}, {605, &D_002AC880[579]}, {606, &D_002AC880[580]}, {607, &D_002AC880[581]},
    {608, &D_002AC880[582]}, {609, &D_002AC880[583]}, {611, &D_002AC880[584]}, {612, &D_002AC880[585]}, {613, &D_002AC880[586]},
    {614, &D_002AC880[587]}, {615, &D_002AC880[588]}, {616, &D_002AC880[535]}, {617, &D_002AC880[556]}, {618, &D_002AC880[590]},
    {619, &D_002AC880[591]}, {620, &D_002AC880[592]}, {621, &D_002AC880[593]},
};
extern const SPLOPTROW D_002653F8[43] SPLOPT_RODATA = {   /* CID34 (cid 34) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[801]}, {774, &D_002AC880[796]},
    {775, &D_002AC880[797]}, {776, &D_002AC880[798]}, {777, &D_002AC880[799]}, {778, &D_002AC880[800]}, {779, &D_002AC880[802]},
    {780, &D_002AC880[803]}, {781, &D_002AC880[804]}, {782, &D_002AC880[805]},
};
extern const SPLOPTROW D_00265550[50] SPLOPT_RODATA = {   /* CID35 (cid 35) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[1080]}, {235, &D_002AC880[1071]},
    {519, &D_002AC880[1073]}, {520, &D_002AC880[1074]}, {526, &D_002AC880[1075]}, {527, &D_002AC880[1076]}, {531, &D_002AC880[1070]},
    {534, &D_002AC880[1068]}, {776, &D_002AC880[1077]}, {777, &D_002AC880[1078]}, {778, &D_002AC880[1079]}, {779, &D_002AC880[1081]},
    {780, &D_002AC880[1082]}, {781, &D_002AC880[1083]}, {782, &D_002AC880[1084]}, {948, &D_002AC880[1069]}, {949, &D_002AC880[1072]},
};
extern const SPLOPTROW D_002656E0[55] SPLOPT_RODATA = {   /* CID36 (cid 36) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[1080]}, {235, &D_002AC880[1071]},
    {344, &D_002AC880[1089]}, {519, &D_002AC880[1073]}, {520, &D_002AC880[1074]}, {526, &D_002AC880[1075]}, {527, &D_002AC880[1076]},
    {531, &D_002AC880[1070]}, {534, &D_002AC880[1068]}, {776, &D_002AC880[1077]}, {777, &D_002AC880[1078]}, {778, &D_002AC880[1079]},
    {779, &D_002AC880[1081]}, {780, &D_002AC880[1082]}, {781, &D_002AC880[1083]}, {782, &D_002AC880[1084]}, {948, &D_002AC880[1069]},
    {949, &D_002AC880[1072]}, {950, &D_002AC880[1085]}, {951, &D_002AC880[1086]}, {952, &D_002AC880[1087]}, {953, &D_002AC880[1088]},
};
extern const SPLOPTROW D_00265898[57] SPLOPT_RODATA = {   /* CID37 (cid 37) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[1080]}, {235, &D_002AC880[1071]},
    {519, &D_002AC880[1073]}, {520, &D_002AC880[1074]}, {526, &D_002AC880[1075]}, {527, &D_002AC880[1076]}, {531, &D_002AC880[1070]},
    {534, &D_002AC880[1068]}, {735, &D_002AC880[1170]}, {736, &D_002AC880[1168]}, {776, &D_002AC880[1077]}, {777, &D_002AC880[1078]},
    {778, &D_002AC880[1079]}, {779, &D_002AC880[1081]}, {780, &D_002AC880[1082]}, {781, &D_002AC880[1083]}, {782, &D_002AC880[1084]},
    {948, &D_002AC880[1069]}, {949, &D_002AC880[1072]}, {1007, &D_002AC880[1171]}, {1013, &D_002AC880[1167]}, {1014, &D_002AC880[1169]},
    {1015, &D_002AC880[1172]}, {1016, &D_002AC880[1173]},
};
extern const SPLOPTROW D_00265A60[43] SPLOPT_RODATA = {   /* CID38 (cid 38) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[821]}, {775, &D_002AC880[817]},
    {776, &D_002AC880[818]}, {777, &D_002AC880[819]}, {778, &D_002AC880[820]}, {779, &D_002AC880[822]}, {780, &D_002AC880[823]},
    {781, &D_002AC880[824]}, {782, &D_002AC880[825]}, {784, &D_002AC880[816]},
};
extern const SPLOPTROW D_00265BB8[41] SPLOPT_RODATA = {   /* CID39 (cid 39) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {141, &D_002AC880[853]}, {776, &D_002AC880[850]},
    {777, &D_002AC880[851]}, {778, &D_002AC880[852]}, {779, &D_002AC880[854]}, {780, &D_002AC880[855]}, {781, &D_002AC880[856]},
    {782, &D_002AC880[857]},
};
extern const SPLOPTROW D_00265D00[34] SPLOPT_RODATA = {   /* CID40 (cid 40) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {795, &D_002AC880[870]},
};
extern const SPLOPTROW D_00265E10[34] SPLOPT_RODATA = {   /* CID41 (cid 41) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {235, &D_002AC880[778]},
};
extern const SPLOPTROW D_00265F20[39] SPLOPT_RODATA = {   /* WARP (cid 42) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {20, &D_002AC880[792]}, {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]},
    {32, &D_002AC880[19]}, {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]},
    {37, &D_002AC880[24]}, {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]},
    {63, &D_002AC880[29]}, {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {526, &D_002AC880[793]},
    {527, &D_002AC880[794]}, {766, &D_002AC880[790]}, {772, &D_002AC880[791]}, {773, &D_002AC880[795]},
};
extern const SPLOPTROW D_00266058[40] SPLOPT_RODATA = {   /* CID43 (cid 43) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {207, &D_002AC880[99]}, {208, &D_002AC880[100]},
    {209, &D_002AC880[101]}, {210, &D_002AC880[102]}, {211, &D_002AC880[103]}, {212, &D_002AC880[104]}, {213, &D_002AC880[105]},
};
extern const SPLOPTROW D_00266198[39] SPLOPT_RODATA = {   /* CID44 (cid 44) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {293, &D_002AC880[1054]}, {503, &D_002AC880[1055]},
    {942, &D_002AC880[1056]}, {943, &D_002AC880[1057]}, {944, &D_002AC880[1058]}, {945, &D_002AC880[1059]},
};
extern const SPLOPTROW D_002662D0[41] SPLOPT_RODATA = {   /* CID45 (cid 45) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[1066]}, {237, &D_002AC880[1067]},
    {238, &D_002AC880[1064]}, {293, &D_002AC880[1060]}, {296, &D_002AC880[1061]}, {297, &D_002AC880[1062]}, {946, &D_002AC880[1063]},
    {947, &D_002AC880[1065]},
};
extern const SPLOPTROW D_00266418[46] SPLOPT_RODATA = {   /* CID46 (cid 46) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {827, &D_002AC880[1201]}, {1030, &D_002AC880[1194]},
    {1031, &D_002AC880[1195]}, {1032, &D_002AC880[1196]}, {1033, &D_002AC880[1197]}, {1034, &D_002AC880[1198]}, {1035, &D_002AC880[1199]},
    {1036, &D_002AC880[1200]}, {1037, &D_002AC880[1202]}, {1038, &D_002AC880[1203]}, {1039, &D_002AC880[1204]}, {1040, &D_002AC880[1205]},
    {1041, &D_002AC880[1206]},
};
extern const SPLOPTROW D_00266588[33] SPLOPT_RODATA = {   /* CID47 (cid 47) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00266690[84] SPLOPT_RODATA = {   /* SO (cid 48) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
};
extern const SPLOPTROW D_00266930[84] SPLOPT_RODATA = {   /* MS (cid 49) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
};
extern const SPLOPTROW D_00266BD0[93] SPLOPT_RODATA = {   /* PO (cid 50) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_00266EB8[104] SPLOPT_RODATA = {   /* STEP (cid 51) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {348, &D_002AC880[222]},
    {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]}, {352, &D_002AC880[226]}, {353, &D_002AC880[227]},
    {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]}, {357, &D_002AC880[231]}, {358, &D_002AC880[232]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_002671F8[156] SPLOPT_RODATA = {   /* STEPPLAYER (cid 52) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {249, &D_002AC880[743]},
    {342, &D_002AC880[751]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {380, &D_002AC880[726]}, {381, &D_002AC880[727]}, {410, &D_002AC880[725]},
    {446, &D_002AC880[733]}, {667, &D_002AC880[731]}, {670, &D_002AC880[738]}, {671, &D_002AC880[739]}, {672, &D_002AC880[741]},
    {710, &D_002AC880[717]}, {711, &D_002AC880[718]}, {712, &D_002AC880[719]}, {713, &D_002AC880[720]}, {714, &D_002AC880[721]},
    {715, &D_002AC880[722]}, {716, &D_002AC880[723]}, {717, &D_002AC880[724]}, {718, &D_002AC880[728]}, {719, &D_002AC880[729]},
    {720, &D_002AC880[730]}, {721, &D_002AC880[732]}, {722, &D_002AC880[734]}, {723, &D_002AC880[735]}, {724, &D_002AC880[736]},
    {725, &D_002AC880[737]}, {726, &D_002AC880[740]}, {727, &D_002AC880[742]}, {728, &D_002AC880[744]}, {729, &D_002AC880[745]},
    {730, &D_002AC880[746]}, {731, &D_002AC880[747]}, {732, &D_002AC880[748]}, {733, &D_002AC880[749]}, {734, &D_002AC880[750]},
    {735, &D_002AC880[752]}, {736, &D_002AC880[753]}, {737, &D_002AC880[754]}, {738, &D_002AC880[755]}, {739, &D_002AC880[756]},
    {740, &D_002AC880[757]}, {741, &D_002AC880[758]}, {742, &D_002AC880[759]}, {743, &D_002AC880[760]}, {744, &D_002AC880[761]},
    {745, &D_002AC880[762]}, {746, &D_002AC880[763]}, {747, &D_002AC880[764]}, {748, &D_002AC880[765]}, {749, &D_002AC880[766]},
    {750, &D_002AC880[767]}, {751, &D_002AC880[768]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]},
    {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]},
    {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_002676D8[161] SPLOPT_RODATA = {   /* JT (cid 53) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {249, &D_002AC880[743]},
    {342, &D_002AC880[751]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {380, &D_002AC880[726]}, {381, &D_002AC880[727]}, {410, &D_002AC880[725]},
    {446, &D_002AC880[733]}, {667, &D_002AC880[731]}, {670, &D_002AC880[738]}, {671, &D_002AC880[739]}, {672, &D_002AC880[741]},
    {710, &D_002AC880[717]}, {711, &D_002AC880[718]}, {712, &D_002AC880[719]}, {713, &D_002AC880[720]}, {714, &D_002AC880[721]},
    {715, &D_002AC880[722]}, {716, &D_002AC880[723]}, {717, &D_002AC880[724]}, {718, &D_002AC880[728]}, {719, &D_002AC880[729]},
    {720, &D_002AC880[730]}, {721, &D_002AC880[732]}, {722, &D_002AC880[734]}, {723, &D_002AC880[735]}, {724, &D_002AC880[736]},
    {725, &D_002AC880[737]}, {726, &D_002AC880[740]}, {727, &D_002AC880[742]}, {728, &D_002AC880[744]}, {729, &D_002AC880[745]},
    {730, &D_002AC880[746]}, {731, &D_002AC880[747]}, {732, &D_002AC880[748]}, {733, &D_002AC880[749]}, {734, &D_002AC880[750]},
    {735, &D_002AC880[752]}, {736, &D_002AC880[753]}, {737, &D_002AC880[754]}, {738, &D_002AC880[755]}, {739, &D_002AC880[756]},
    {740, &D_002AC880[757]}, {741, &D_002AC880[758]}, {742, &D_002AC880[759]}, {743, &D_002AC880[760]}, {744, &D_002AC880[761]},
    {745, &D_002AC880[762]}, {746, &D_002AC880[763]}, {747, &D_002AC880[764]}, {748, &D_002AC880[765]}, {749, &D_002AC880[766]},
    {750, &D_002AC880[767]}, {751, &D_002AC880[768]}, {752, &D_002AC880[769]}, {753, &D_002AC880[770]}, {754, &D_002AC880[771]},
    {755, &D_002AC880[772]}, {756, &D_002AC880[773]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]},
    {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]},
    {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_00267BE0[161] SPLOPT_RODATA = {   /* MURRAY (cid 54) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {249, &D_002AC880[743]},
    {342, &D_002AC880[751]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {380, &D_002AC880[726]}, {381, &D_002AC880[727]}, {410, &D_002AC880[725]},
    {446, &D_002AC880[733]}, {667, &D_002AC880[731]}, {670, &D_002AC880[738]}, {671, &D_002AC880[739]}, {672, &D_002AC880[741]},
    {710, &D_002AC880[717]}, {711, &D_002AC880[718]}, {712, &D_002AC880[719]}, {713, &D_002AC880[720]}, {714, &D_002AC880[721]},
    {715, &D_002AC880[722]}, {716, &D_002AC880[723]}, {717, &D_002AC880[724]}, {718, &D_002AC880[728]}, {719, &D_002AC880[729]},
    {720, &D_002AC880[730]}, {721, &D_002AC880[732]}, {722, &D_002AC880[734]}, {723, &D_002AC880[735]}, {724, &D_002AC880[736]},
    {725, &D_002AC880[737]}, {726, &D_002AC880[740]}, {727, &D_002AC880[742]}, {728, &D_002AC880[744]}, {729, &D_002AC880[745]},
    {730, &D_002AC880[746]}, {731, &D_002AC880[747]}, {732, &D_002AC880[748]}, {733, &D_002AC880[749]}, {734, &D_002AC880[750]},
    {735, &D_002AC880[752]}, {736, &D_002AC880[753]}, {737, &D_002AC880[754]}, {738, &D_002AC880[755]}, {739, &D_002AC880[756]},
    {740, &D_002AC880[757]}, {741, &D_002AC880[758]}, {742, &D_002AC880[759]}, {743, &D_002AC880[760]}, {744, &D_002AC880[761]},
    {745, &D_002AC880[762]}, {746, &D_002AC880[763]}, {747, &D_002AC880[764]}, {748, &D_002AC880[765]}, {749, &D_002AC880[766]},
    {750, &D_002AC880[767]}, {751, &D_002AC880[768]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]},
    {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]},
    {812, &D_002AC880[890]}, {937, &D_002AC880[1049]}, {938, &D_002AC880[1050]}, {939, &D_002AC880[1051]}, {940, &D_002AC880[1052]},
    {941, &D_002AC880[1053]},
};
extern const SPLOPTROW D_002680E8[160] SPLOPT_RODATA = {   /* BENTLEY (cid 55) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {249, &D_002AC880[743]},
    {342, &D_002AC880[751]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {380, &D_002AC880[726]}, {381, &D_002AC880[727]}, {410, &D_002AC880[725]},
    {446, &D_002AC880[733]}, {667, &D_002AC880[731]}, {670, &D_002AC880[738]}, {671, &D_002AC880[739]}, {672, &D_002AC880[741]},
    {710, &D_002AC880[717]}, {711, &D_002AC880[718]}, {712, &D_002AC880[719]}, {713, &D_002AC880[720]}, {714, &D_002AC880[721]},
    {715, &D_002AC880[722]}, {716, &D_002AC880[723]}, {717, &D_002AC880[724]}, {718, &D_002AC880[728]}, {719, &D_002AC880[729]},
    {720, &D_002AC880[730]}, {721, &D_002AC880[732]}, {722, &D_002AC880[734]}, {723, &D_002AC880[735]}, {724, &D_002AC880[736]},
    {725, &D_002AC880[737]}, {726, &D_002AC880[740]}, {727, &D_002AC880[742]}, {728, &D_002AC880[744]}, {729, &D_002AC880[745]},
    {730, &D_002AC880[746]}, {731, &D_002AC880[747]}, {732, &D_002AC880[748]}, {733, &D_002AC880[749]}, {734, &D_002AC880[750]},
    {735, &D_002AC880[752]}, {736, &D_002AC880[753]}, {737, &D_002AC880[754]}, {738, &D_002AC880[755]}, {739, &D_002AC880[756]},
    {740, &D_002AC880[757]}, {741, &D_002AC880[758]}, {742, &D_002AC880[759]}, {743, &D_002AC880[760]}, {744, &D_002AC880[761]},
    {745, &D_002AC880[762]}, {746, &D_002AC880[763]}, {747, &D_002AC880[764]}, {748, &D_002AC880[765]}, {749, &D_002AC880[766]},
    {750, &D_002AC880[767]}, {751, &D_002AC880[768]}, {757, &D_002AC880[774]}, {758, &D_002AC880[775]}, {759, &D_002AC880[776]},
    {760, &D_002AC880[777]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]},
    {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_002685E8[213] SPLOPT_RODATA = {   /* STEPGUARD (cid 56) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_00268C90[213] SPLOPT_RODATA = {   /* GOMER (cid 57) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_00269338[213] SPLOPT_RODATA = {   /* GUARD (cid 58) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_002699E0[215] SPLOPT_RODATA = {   /* DIMITRI (cid 59) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1155, &D_002AC880[1340]}, {1156, &D_002AC880[1341]},
};
extern const SPLOPTROW D_0026A098[221] SPLOPT_RODATA = {   /* NEELA (cid 60) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {967, &D_002AC880[1114]}, {968, &D_002AC880[1115]},
    {969, &D_002AC880[1116]}, {970, &D_002AC880[1117]}, {971, &D_002AC880[1118]}, {972, &D_002AC880[1119]}, {973, &D_002AC880[1120]},
    {974, &D_002AC880[1121]},
};
extern const SPLOPTROW D_0026A780[214] SPLOPT_RODATA = {   /* RAJAN (cid 61) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1224, &D_002AC880[1438]},
};
extern const SPLOPTROW D_0026AE30[213] SPLOPT_RODATA = {   /* JEANBISON (cid 62) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_0026B4D8[214] SPLOPT_RODATA = {   /* CARMELITA (cid 63) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1173, &D_002AC880[1360]},
};
extern const SPLOPTROW D_0026BB88[213] SPLOPT_RODATA = {   /* CONTESSA (cid 64) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_0026C230[213] SPLOPT_RODATA = {   /* ARPEGGIO (cid 65) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_0026C8D8[213] SPLOPT_RODATA = {   /* BEAR (cid 66) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_0026CF80[213] SPLOPT_RODATA = {   /* BEARCUB (cid 67) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {345, &D_002AC880[288]},
    {346, &D_002AC880[289]}, {348, &D_002AC880[222]}, {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]},
    {352, &D_002AC880[226]}, {353, &D_002AC880[227]}, {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]},
    {357, &D_002AC880[231]}, {358, &D_002AC880[232]}, {359, &D_002AC880[233]}, {360, &D_002AC880[234]}, {361, &D_002AC880[235]},
    {362, &D_002AC880[236]}, {363, &D_002AC880[237]}, {364, &D_002AC880[238]}, {365, &D_002AC880[239]}, {366, &D_002AC880[240]},
    {367, &D_002AC880[241]}, {368, &D_002AC880[242]}, {369, &D_002AC880[243]}, {370, &D_002AC880[244]}, {371, &D_002AC880[245]},
    {372, &D_002AC880[246]}, {373, &D_002AC880[247]}, {374, &D_002AC880[248]}, {375, &D_002AC880[249]}, {376, &D_002AC880[250]},
    {377, &D_002AC880[251]}, {378, &D_002AC880[252]}, {379, &D_002AC880[253]}, {380, &D_002AC880[254]}, {381, &D_002AC880[255]},
    {382, &D_002AC880[256]}, {383, &D_002AC880[257]}, {384, &D_002AC880[258]}, {385, &D_002AC880[259]}, {386, &D_002AC880[260]},
    {387, &D_002AC880[261]}, {388, &D_002AC880[262]}, {389, &D_002AC880[263]}, {390, &D_002AC880[264]}, {391, &D_002AC880[265]},
    {392, &D_002AC880[266]}, {393, &D_002AC880[267]}, {394, &D_002AC880[268]}, {395, &D_002AC880[269]}, {396, &D_002AC880[270]},
    {397, &D_002AC880[271]}, {398, &D_002AC880[272]}, {399, &D_002AC880[273]}, {400, &D_002AC880[274]}, {401, &D_002AC880[275]},
    {402, &D_002AC880[276]}, {403, &D_002AC880[277]}, {404, &D_002AC880[278]}, {405, &D_002AC880[279]}, {406, &D_002AC880[280]},
    {407, &D_002AC880[281]}, {408, &D_002AC880[282]}, {409, &D_002AC880[283]}, {410, &D_002AC880[284]}, {411, &D_002AC880[285]},
    {412, &D_002AC880[286]}, {413, &D_002AC880[287]}, {414, &D_002AC880[290]}, {415, &D_002AC880[291]}, {416, &D_002AC880[292]},
    {417, &D_002AC880[293]}, {418, &D_002AC880[294]}, {419, &D_002AC880[295]}, {420, &D_002AC880[296]}, {421, &D_002AC880[297]},
    {422, &D_002AC880[298]}, {423, &D_002AC880[299]}, {424, &D_002AC880[300]}, {425, &D_002AC880[301]}, {426, &D_002AC880[302]},
    {427, &D_002AC880[303]}, {428, &D_002AC880[304]}, {429, &D_002AC880[305]}, {430, &D_002AC880[306]}, {431, &D_002AC880[307]},
    {432, &D_002AC880[308]}, {433, &D_002AC880[309]}, {434, &D_002AC880[310]}, {435, &D_002AC880[311]}, {436, &D_002AC880[312]},
    {437, &D_002AC880[313]}, {438, &D_002AC880[314]}, {439, &D_002AC880[315]}, {440, &D_002AC880[316]}, {441, &D_002AC880[317]},
    {442, &D_002AC880[318]}, {443, &D_002AC880[319]}, {444, &D_002AC880[320]}, {445, &D_002AC880[321]}, {446, &D_002AC880[322]},
    {447, &D_002AC880[323]}, {448, &D_002AC880[324]}, {449, &D_002AC880[325]}, {450, &D_002AC880[326]}, {451, &D_002AC880[327]},
    {452, &D_002AC880[328]}, {453, &D_002AC880[329]}, {454, &D_002AC880[330]}, {455, &D_002AC880[331]}, {456, &D_002AC880[332]},
    {457, &D_002AC880[333]}, {458, &D_002AC880[334]}, {459, &D_002AC880[335]}, {460, &D_002AC880[336]}, {461, &D_002AC880[337]},
    {462, &D_002AC880[338]}, {463, &D_002AC880[339]}, {464, &D_002AC880[340]}, {465, &D_002AC880[341]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
};
extern const SPLOPTROW D_0026D628[106] SPLOPT_RODATA = {   /* DANCER (cid 68) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {348, &D_002AC880[222]},
    {349, &D_002AC880[223]}, {350, &D_002AC880[224]}, {351, &D_002AC880[225]}, {352, &D_002AC880[226]}, {353, &D_002AC880[227]},
    {354, &D_002AC880[228]}, {355, &D_002AC880[229]}, {356, &D_002AC880[230]}, {357, &D_002AC880[231]}, {358, &D_002AC880[232]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1157, &D_002AC880[1342]},
    {1158, &D_002AC880[1343]},
};
extern const SPLOPTROW D_0026D978[107] SPLOPT_RODATA = {   /* TURRET (cid 69) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {325, &D_002AC880[1000]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {910, &D_002AC880[997]},
    {911, &D_002AC880[998]}, {912, &D_002AC880[999]}, {913, &D_002AC880[1001]}, {914, &D_002AC880[1002]}, {915, &D_002AC880[1003]},
    {916, &D_002AC880[1004]}, {917, &D_002AC880[1005]}, {918, &D_002AC880[1006]}, {919, &D_002AC880[1007]}, {920, &D_002AC880[1008]},
    {921, &D_002AC880[1009]}, {922, &D_002AC880[1010]},
};
extern const SPLOPTROW D_0026DCD0[96] SPLOPT_RODATA = {   /* SAFEHOUSE (cid 70) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1152, &D_002AC880[1337]}, {1153, &D_002AC880[1338]},
    {1154, &D_002AC880[1339]},
};
extern const SPLOPTROW D_0026DFD0[101] SPLOPT_RODATA = {   /* HACK (cid 71) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {454, &D_002AC880[1368]}, {493, &D_002AC880[1367]}, {497, &D_002AC880[1366]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1176, &D_002AC880[1369]},
    {1177, &D_002AC880[1370]},
};
extern const SPLOPTROW D_0026E2F8[105] SPLOPT_RODATA = {   /* HACKSHIP (cid 72) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {369, &D_002AC880[1372]}, {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {454, &D_002AC880[1368]}, {493, &D_002AC880[1367]},
    {497, &D_002AC880[1366]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]},
    {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
    {1176, &D_002AC880[1369]}, {1177, &D_002AC880[1370]}, {1178, &D_002AC880[1371]}, {1179, &D_002AC880[1373]}, {1180, &D_002AC880[1374]},
};
extern const SPLOPTROW D_0026E640[113] SPLOPT_RODATA = {   /* HACKENEMY (cid 73) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {350, &D_002AC880[1376]}, {369, &D_002AC880[1375]}, {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {386, &D_002AC880[1379]},
    {425, &D_002AC880[1378]}, {454, &D_002AC880[1368]}, {493, &D_002AC880[1367]}, {497, &D_002AC880[1366]}, {804, &D_002AC880[882]},
    {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]},
    {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1176, &D_002AC880[1369]}, {1177, &D_002AC880[1370]},
    {1181, &D_002AC880[1377]}, {1182, &D_002AC880[1380]}, {1183, &D_002AC880[1381]}, {1184, &D_002AC880[1382]}, {1185, &D_002AC880[1383]},
    {1186, &D_002AC880[1384]}, {1187, &D_002AC880[1385]}, {1188, &D_002AC880[1386]},
};
extern const SPLOPTROW D_0026E9C8[106] SPLOPT_RODATA = {   /* HACKBULLET (cid 74) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {454, &D_002AC880[1368]}, {493, &D_002AC880[1367]}, {497, &D_002AC880[1366]},
    {508, &D_002AC880[1387]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]},
    {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
    {1176, &D_002AC880[1369]}, {1177, &D_002AC880[1370]}, {1189, &D_002AC880[1388]}, {1190, &D_002AC880[1389]}, {1191, &D_002AC880[1390]},
    {1192, &D_002AC880[1391]},
};
extern const SPLOPTROW D_0026ED18[106] SPLOPT_RODATA = {   /* HACKLOCK (cid 75) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {235, &D_002AC880[1396]}, {369, &D_002AC880[1392]}, {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {454, &D_002AC880[1368]},
    {493, &D_002AC880[1367]}, {497, &D_002AC880[1366]}, {526, &D_002AC880[1393]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]},
    {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]},
    {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1176, &D_002AC880[1369]}, {1177, &D_002AC880[1370]}, {1193, &D_002AC880[1394]},
    {1194, &D_002AC880[1395]},
};
extern const SPLOPTROW D_0026F068[104] SPLOPT_RODATA = {   /* HACKWALL (cid 76) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {123, &D_002AC880[1365]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {380, &D_002AC880[1363]}, {381, &D_002AC880[1364]}, {386, &D_002AC880[1397]}, {454, &D_002AC880[1368]}, {493, &D_002AC880[1367]},
    {497, &D_002AC880[1366]}, {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]},
    {808, &D_002AC880[886]}, {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]},
    {1176, &D_002AC880[1369]}, {1177, &D_002AC880[1370]}, {1195, &D_002AC880[1398]}, {1196, &D_002AC880[1399]},
};
extern const SPLOPTROW D_0026F3A8[109] SPLOPT_RODATA = {   /* ROBOTTANK (cid 77) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {342, &D_002AC880[1425]},
    {380, &D_002AC880[1426]}, {381, &D_002AC880[1427]}, {386, &D_002AC880[1423]}, {425, &D_002AC880[1424]}, {484, &D_002AC880[1430]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1178, &D_002AC880[1421]},
    {1179, &D_002AC880[1428]}, {1215, &D_002AC880[1419]}, {1216, &D_002AC880[1420]}, {1217, &D_002AC880[1422]}, {1218, &D_002AC880[1429]},
    {1219, &D_002AC880[1431]}, {1220, &D_002AC880[1432]}, {1221, &D_002AC880[1433]}, {1222, &D_002AC880[1434]},
};
extern const SPLOPTROW D_0026F710[99] SPLOPT_RODATA = {   /* VAULT (cid 78) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {235, &D_002AC880[1444]},
    {804, &D_002AC880[882]}, {805, &D_002AC880[883]}, {806, &D_002AC880[884]}, {807, &D_002AC880[885]}, {808, &D_002AC880[886]},
    {809, &D_002AC880[887]}, {810, &D_002AC880[888]}, {811, &D_002AC880[889]}, {812, &D_002AC880[890]}, {1225, &D_002AC880[1439]},
    {1226, &D_002AC880[1440]}, {1227, &D_002AC880[1441]}, {1228, &D_002AC880[1442]}, {1229, &D_002AC880[1443]},
};
extern const SPLOPTROW D_0026FA28[101] SPLOPT_RODATA = {   /* BOMB (cid 79) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {486, &D_002AC880[383]},
    {487, &D_002AC880[384]}, {488, &D_002AC880[385]}, {489, &D_002AC880[386]}, {490, &D_002AC880[387]}, {491, &D_002AC880[388]},
    {492, &D_002AC880[389]}, {493, &D_002AC880[390]}, {494, &D_002AC880[391]}, {495, &D_002AC880[392]}, {496, &D_002AC880[393]},
    {497, &D_002AC880[394]}, {498, &D_002AC880[395]}, {499, &D_002AC880[396]}, {500, &D_002AC880[397]}, {501, &D_002AC880[398]},
    {502, &D_002AC880[399]},
};
extern const SPLOPTROW D_0026FD50[109] SPLOPT_RODATA = {   /* MISSILE (cid 80) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {486, &D_002AC880[383]},
    {487, &D_002AC880[384]}, {488, &D_002AC880[385]}, {489, &D_002AC880[386]}, {490, &D_002AC880[387]}, {491, &D_002AC880[388]},
    {492, &D_002AC880[389]}, {493, &D_002AC880[390]}, {494, &D_002AC880[391]}, {495, &D_002AC880[392]}, {496, &D_002AC880[393]},
    {497, &D_002AC880[394]}, {498, &D_002AC880[395]}, {499, &D_002AC880[396]}, {500, &D_002AC880[397]}, {501, &D_002AC880[398]},
    {502, &D_002AC880[399]}, {503, &D_002AC880[400]}, {504, &D_002AC880[401]}, {505, &D_002AC880[402]}, {506, &D_002AC880[403]},
    {507, &D_002AC880[404]}, {508, &D_002AC880[405]}, {509, &D_002AC880[406]}, {510, &D_002AC880[407]},
};
extern const SPLOPTROW D_002700B8[111] SPLOPT_RODATA = {   /* ACCMISS (cid 81) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {356, &D_002AC880[409]},
    {486, &D_002AC880[383]}, {487, &D_002AC880[384]}, {488, &D_002AC880[385]}, {489, &D_002AC880[386]}, {490, &D_002AC880[387]},
    {491, &D_002AC880[388]}, {492, &D_002AC880[389]}, {493, &D_002AC880[390]}, {494, &D_002AC880[391]}, {495, &D_002AC880[392]},
    {496, &D_002AC880[393]}, {497, &D_002AC880[394]}, {498, &D_002AC880[395]}, {499, &D_002AC880[396]}, {500, &D_002AC880[397]},
    {501, &D_002AC880[398]}, {502, &D_002AC880[399]}, {503, &D_002AC880[400]}, {504, &D_002AC880[401]}, {505, &D_002AC880[402]},
    {506, &D_002AC880[403]}, {507, &D_002AC880[404]}, {508, &D_002AC880[405]}, {509, &D_002AC880[406]}, {510, &D_002AC880[407]},
    {511, &D_002AC880[408]},
};
extern const SPLOPTROW D_00270430[112] SPLOPT_RODATA = {   /* CID82 (cid 82) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {486, &D_002AC880[383]},
    {487, &D_002AC880[384]}, {488, &D_002AC880[385]}, {489, &D_002AC880[386]}, {490, &D_002AC880[387]}, {491, &D_002AC880[388]},
    {492, &D_002AC880[389]}, {493, &D_002AC880[390]}, {494, &D_002AC880[391]}, {495, &D_002AC880[392]}, {496, &D_002AC880[393]},
    {497, &D_002AC880[394]}, {498, &D_002AC880[395]}, {499, &D_002AC880[396]}, {500, &D_002AC880[397]}, {501, &D_002AC880[398]},
    {502, &D_002AC880[399]}, {503, &D_002AC880[400]}, {504, &D_002AC880[401]}, {505, &D_002AC880[402]}, {506, &D_002AC880[403]},
    {507, &D_002AC880[404]}, {508, &D_002AC880[405]}, {509, &D_002AC880[406]}, {510, &D_002AC880[407]}, {512, &D_002AC880[410]},
    {513, &D_002AC880[411]}, {514, &D_002AC880[412]},
};
extern const SPLOPTROW D_002707B0[101] SPLOPT_RODATA = {   /* BUTTON (cid 83) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {235, &D_002AC880[428]},
    {515, &D_002AC880[413]}, {516, &D_002AC880[414]}, {517, &D_002AC880[415]}, {518, &D_002AC880[416]}, {519, &D_002AC880[417]},
    {520, &D_002AC880[418]}, {521, &D_002AC880[419]}, {522, &D_002AC880[420]}, {523, &D_002AC880[421]}, {524, &D_002AC880[422]},
    {525, &D_002AC880[423]}, {526, &D_002AC880[424]}, {527, &D_002AC880[425]}, {528, &D_002AC880[426]}, {529, &D_002AC880[427]},
    {530, &D_002AC880[429]},
};
extern const SPLOPTROW D_00270AD8[86] SPLOPT_RODATA = {   /* SENSOR (cid 84) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {535, &D_002AC880[448]},
    {536, &D_002AC880[449]},
};
extern const SPLOPTROW D_00270D88[92] SPLOPT_RODATA = {   /* LASEN (cid 85) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {217, &D_002AC880[454]},
    {535, &D_002AC880[448]}, {536, &D_002AC880[449]}, {537, &D_002AC880[450]}, {538, &D_002AC880[451]}, {539, &D_002AC880[452]},
    {540, &D_002AC880[453]}, {541, &D_002AC880[455]},
};
extern const SPLOPTROW D_00271068[88] SPLOPT_RODATA = {   /* BARRIER (cid 86) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {534, &D_002AC880[459]},
    {542, &D_002AC880[456]}, {543, &D_002AC880[457]}, {544, &D_002AC880[458]},
};
extern const SPLOPTROW D_00271328[84] SPLOPT_RODATA = {   /* RIPG (cid 87) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
};
extern const SPLOPTROW D_002715C8[96] SPLOPT_RODATA = {   /* BARREL (cid 88) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[833]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {774, &D_002AC880[826]}, {776, &D_002AC880[830]}, {777, &D_002AC880[831]}, {778, &D_002AC880[832]}, {779, &D_002AC880[834]},
    {780, &D_002AC880[835]}, {781, &D_002AC880[836]}, {782, &D_002AC880[837]}, {785, &D_002AC880[827]}, {786, &D_002AC880[828]},
    {787, &D_002AC880[829]},
};
extern const SPLOPTROW D_002718C8[92] SPLOPT_RODATA = {   /* WATER (cid 89) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {666, &D_002AC880[876]},
    {797, &D_002AC880[874]}, {798, &D_002AC880[875]}, {799, &D_002AC880[877]}, {800, &D_002AC880[878]}, {801, &D_002AC880[879]},
    {802, &D_002AC880[880]}, {803, &D_002AC880[881]},
};
extern const SPLOPTROW D_00271BA8[95] SPLOPT_RODATA = {   /* DPRIZE (cid 90) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
};
extern const SPLOPTROW D_00271EA0[96] SPLOPT_RODATA = {   /* COIN (cid 91) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
    {824, &D_002AC880[902]},
};
extern const SPLOPTROW D_002721A0[96] SPLOPT_RODATA = {   /* KEY (cid 92) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
    {827, &D_002AC880[906]},
};
extern const SPLOPTROW D_002724A0[98] SPLOPT_RODATA = {   /* MEDICINE (cid 93) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
    {824, &D_002AC880[903]}, {825, &D_002AC880[904]}, {826, &D_002AC880[905]},
};
extern const SPLOPTROW D_002727B0[101] SPLOPT_RODATA = {   /* LOOT (cid 94) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
    {824, &D_002AC880[912]}, {828, &D_002AC880[907]}, {829, &D_002AC880[908]}, {830, &D_002AC880[909]}, {831, &D_002AC880[910]},
    {832, &D_002AC880[911]},
};
extern const SPLOPTROW D_00272AD8[95] SPLOPT_RODATA = {   /* CLUE (cid 95) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {813, &D_002AC880[891]},
    {814, &D_002AC880[892]}, {815, &D_002AC880[893]}, {816, &D_002AC880[894]}, {817, &D_002AC880[895]}, {818, &D_002AC880[896]},
    {819, &D_002AC880[897]}, {820, &D_002AC880[898]}, {821, &D_002AC880[899]}, {822, &D_002AC880[900]}, {823, &D_002AC880[901]},
};
extern const SPLOPTROW D_00272DD0[97] SPLOPT_RODATA = {   /* CID96 (cid 96) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {350, &D_002AC880[974]},
    {369, &D_002AC880[965]}, {402, &D_002AC880[976]}, {403, &D_002AC880[977]}, {883, &D_002AC880[966]}, {884, &D_002AC880[967]},
    {885, &D_002AC880[968]}, {886, &D_002AC880[969]}, {887, &D_002AC880[970]}, {888, &D_002AC880[971]}, {889, &D_002AC880[972]},
    {890, &D_002AC880[973]}, {891, &D_002AC880[975]},
};
extern const SPLOPTROW D_002730D8[85] SPLOPT_RODATA = {   /* SKY (cid 97) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {892, &D_002AC880[978]},
};
extern const SPLOPTROW D_00273380[108] SPLOPT_RODATA = {   /* CID98 (cid 98) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[1040]},
    {110, &D_002AC880[44]}, {111, &D_002AC880[1043]}, {111, &D_002AC880[45]}, {112, &D_002AC880[1041]}, {112, &D_002AC880[46]},
    {113, &D_002AC880[1044]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]}, {120, &D_002AC880[50]},
    {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]}, {127, &D_002AC880[55]},
    {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]}, {132, &D_002AC880[60]},
    {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]}, {167, &D_002AC880[65]},
    {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]}, {179, &D_002AC880[70]},
    {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]}, {187, &D_002AC880[75]},
    {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]}, {197, &D_002AC880[80]},
    {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {233, &D_002AC880[1035]}, {234, &D_002AC880[1036]},
    {236, &D_002AC880[1048]}, {238, &D_002AC880[1039]}, {239, &D_002AC880[1042]}, {243, &D_002AC880[1038]}, {244, &D_002AC880[1034]},
    {342, &D_002AC880[1030]}, {527, &D_002AC880[1045]}, {926, &D_002AC880[1025]}, {927, &D_002AC880[1026]}, {928, &D_002AC880[1027]},
    {929, &D_002AC880[1028]}, {930, &D_002AC880[1029]}, {931, &D_002AC880[1031]}, {932, &D_002AC880[1032]}, {933, &D_002AC880[1033]},
    {934, &D_002AC880[1037]}, {935, &D_002AC880[1046]}, {936, &D_002AC880[1047]},
};
extern const SPLOPTROW D_002736E0[84] SPLOPT_RODATA = {   /* CID99 (cid 99) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
};
extern const SPLOPTROW D_00273980[95] SPLOPT_RODATA = {   /* CID100 (cid 100) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[1103]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {484, &D_002AC880[1102]}, {498, &D_002AC880[1108]}, {501, &D_002AC880[1106]}, {960, &D_002AC880[1104]}, {961, &D_002AC880[1105]},
    {962, &D_002AC880[1107]}, {963, &D_002AC880[1109]}, {964, &D_002AC880[1110]}, {965, &D_002AC880[1111]}, {966, &D_002AC880[1112]},
};
extern const SPLOPTROW D_00273C78[95] SPLOPT_RODATA = {   /* CID101 (cid 101) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[1103]},
    {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]},
    {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]},
    {484, &D_002AC880[1102]}, {498, &D_002AC880[1108]}, {501, &D_002AC880[1106]}, {960, &D_002AC880[1104]}, {961, &D_002AC880[1105]},
    {962, &D_002AC880[1107]}, {963, &D_002AC880[1109]}, {964, &D_002AC880[1110]}, {965, &D_002AC880[1111]}, {966, &D_002AC880[1112]},
};
extern const SPLOPTROW D_00273F70[96] SPLOPT_RODATA = {   /* CID102 (cid 102) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[1113]},
    {37, &D_002AC880[24]}, {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]},
    {63, &D_002AC880[29]}, {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]},
    {70, &D_002AC880[34]}, {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]},
    {91, &D_002AC880[39]}, {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]},
    {110, &D_002AC880[44]}, {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]},
    {119, &D_002AC880[49]}, {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]},
    {126, &D_002AC880[54]}, {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]},
    {131, &D_002AC880[59]}, {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]},
    {166, &D_002AC880[64]}, {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]},
    {178, &D_002AC880[69]}, {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]},
    {183, &D_002AC880[1103]}, {183, &D_002AC880[74]}, {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]},
    {195, &D_002AC880[78]}, {196, &D_002AC880[79]}, {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]},
    {200, &D_002AC880[83]}, {484, &D_002AC880[1102]}, {498, &D_002AC880[1108]}, {501, &D_002AC880[1106]}, {960, &D_002AC880[1104]},
    {961, &D_002AC880[1105]}, {962, &D_002AC880[1107]}, {963, &D_002AC880[1109]}, {964, &D_002AC880[1110]}, {965, &D_002AC880[1111]},
    {966, &D_002AC880[1112]},
};
extern const SPLOPTROW D_00274270[99] SPLOPT_RODATA = {   /* BOOTY (cid 103) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {342, &D_002AC880[1164]},
    {352, &D_002AC880[1155]}, {828, &D_002AC880[1152]}, {829, &D_002AC880[1153]}, {830, &D_002AC880[1154]}, {831, &D_002AC880[1158]},
    {832, &D_002AC880[1160]}, {1005, &D_002AC880[1156]}, {1006, &D_002AC880[1157]}, {1007, &D_002AC880[1159]}, {1008, &D_002AC880[1161]},
    {1009, &D_002AC880[1162]}, {1010, &D_002AC880[1163]}, {1011, &D_002AC880[1165]}, {1012, &D_002AC880[1166]},
};
extern const SPLOPTROW D_00274588[85] SPLOPT_RODATA = {   /* CID104 (cid 104) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {235, &D_002AC880[1207]},
};
extern const SPLOPTROW D_00274830[91] SPLOPT_RODATA = {   /* CID105 (cid 105) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {1120, &D_002AC880[1304]},
    {1121, &D_002AC880[1305]}, {1122, &D_002AC880[1306]}, {1123, &D_002AC880[1307]}, {1124, &D_002AC880[1308]}, {1125, &D_002AC880[1309]},
    {1126, &D_002AC880[1310]},
};
extern const SPLOPTROW D_00274B08[85] SPLOPT_RODATA = {   /* CID106 (cid 106) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {1127, &D_002AC880[1311]},
};
extern const SPLOPTROW D_00274DB0[95] SPLOPT_RODATA = {   /* WAYPT (cid 107) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {537, &D_002AC880[1319]},
    {1129, &D_002AC880[1313]}, {1130, &D_002AC880[1314]}, {1131, &D_002AC880[1315]}, {1132, &D_002AC880[1316]}, {1133, &D_002AC880[1317]},
    {1134, &D_002AC880[1318]}, {1135, &D_002AC880[1320]}, {1136, &D_002AC880[1321]}, {1137, &D_002AC880[1322]}, {1138, &D_002AC880[1323]},
};
extern const SPLOPTROW D_002750A8[87] SPLOPT_RODATA = {   /* CID108 (cid 108) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {69, &D_002AC880[33]}, {70, &D_002AC880[34]},
    {71, &D_002AC880[35]}, {87, &D_002AC880[36]}, {88, &D_002AC880[37]}, {90, &D_002AC880[38]}, {91, &D_002AC880[39]},
    {102, &D_002AC880[40]}, {104, &D_002AC880[41]}, {105, &D_002AC880[42]}, {108, &D_002AC880[43]}, {110, &D_002AC880[44]},
    {111, &D_002AC880[45]}, {112, &D_002AC880[46]}, {113, &D_002AC880[47]}, {114, &D_002AC880[48]}, {119, &D_002AC880[49]},
    {120, &D_002AC880[50]}, {121, &D_002AC880[51]}, {124, &D_002AC880[52]}, {125, &D_002AC880[53]}, {126, &D_002AC880[54]},
    {127, &D_002AC880[55]}, {128, &D_002AC880[56]}, {129, &D_002AC880[57]}, {130, &D_002AC880[58]}, {131, &D_002AC880[59]},
    {132, &D_002AC880[60]}, {139, &D_002AC880[61]}, {140, &D_002AC880[62]}, {141, &D_002AC880[63]}, {166, &D_002AC880[64]},
    {167, &D_002AC880[65]}, {174, &D_002AC880[66]}, {176, &D_002AC880[67]}, {177, &D_002AC880[68]}, {178, &D_002AC880[69]},
    {179, &D_002AC880[70]}, {180, &D_002AC880[71]}, {181, &D_002AC880[72]}, {182, &D_002AC880[73]}, {183, &D_002AC880[74]},
    {187, &D_002AC880[75]}, {188, &D_002AC880[76]}, {194, &D_002AC880[77]}, {195, &D_002AC880[78]}, {196, &D_002AC880[79]},
    {197, &D_002AC880[80]}, {198, &D_002AC880[81]}, {199, &D_002AC880[82]}, {200, &D_002AC880[83]}, {484, &D_002AC880[1437]},
    {1212, &D_002AC880[1435]}, {1223, &D_002AC880[1436]},
};
extern const SPLOPTROW D_00275360[43] SPLOPT_RODATA = {   /* ASEG (cid 109) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[106]}, {215, &D_002AC880[107]},
    {216, &D_002AC880[108]}, {217, &D_002AC880[109]}, {218, &D_002AC880[110]}, {219, &D_002AC880[111]}, {220, &D_002AC880[112]},
    {221, &D_002AC880[113]}, {222, &D_002AC880[114]}, {223, &D_002AC880[115]},
};
extern const SPLOPTROW D_002754B8[43] SPLOPT_RODATA = {   /* ASEGBL (cid 110) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[106]}, {215, &D_002AC880[107]},
    {216, &D_002AC880[108]}, {217, &D_002AC880[109]}, {218, &D_002AC880[110]}, {219, &D_002AC880[111]}, {220, &D_002AC880[112]},
    {221, &D_002AC880[113]}, {222, &D_002AC880[114]}, {223, &D_002AC880[115]},
};
extern const SPLOPTROW D_00275610[36] SPLOPT_RODATA = {   /* SM (cid 111) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[368]}, {215, &D_002AC880[370]},
    {477, &D_002AC880[369]},
};
extern const SPLOPTROW D_00275730[42] SPLOPT_RODATA = {   /* CID112 (cid 112) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {503, &D_002AC880[460]}, {504, &D_002AC880[461]},
    {508, &D_002AC880[468]}, {545, &D_002AC880[462]}, {546, &D_002AC880[463]}, {547, &D_002AC880[464]}, {548, &D_002AC880[465]},
    {549, &D_002AC880[466]}, {550, &D_002AC880[467]},
};
extern const SPLOPTROW D_00275880[52] SPLOPT_RODATA = {   /* CID113 (cid 113) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[594]}, {236, &D_002AC880[605]},
    {242, &D_002AC880[604]}, {243, &D_002AC880[599]}, {256, &D_002AC880[600]}, {292, &D_002AC880[598]}, {622, &D_002AC880[595]},
    {623, &D_002AC880[596]}, {624, &D_002AC880[597]}, {625, &D_002AC880[601]}, {626, &D_002AC880[602]}, {627, &D_002AC880[603]},
    {628, &D_002AC880[606]}, {629, &D_002AC880[607]}, {630, &D_002AC880[608]}, {631, &D_002AC880[609]}, {632, &D_002AC880[610]},
    {633, &D_002AC880[611]}, {634, &D_002AC880[612]},
};
extern const SPLOPTROW D_00275A20[137] SPLOPT_RODATA = {   /* RELAY (cid 114) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {110, &D_002AC880[672]}, {111, &D_002AC880[673]},
    {112, &D_002AC880[674]}, {113, &D_002AC880[675]}, {221, &D_002AC880[643]}, {231, &D_002AC880[680]}, {232, &D_002AC880[681]},
    {233, &D_002AC880[676]}, {234, &D_002AC880[677]}, {235, &D_002AC880[613]}, {236, &D_002AC880[645]}, {238, &D_002AC880[670]},
    {239, &D_002AC880[671]}, {242, &D_002AC880[644]}, {243, &D_002AC880[634]}, {244, &D_002AC880[678]}, {247, &D_002AC880[679]},
    {256, &D_002AC880[660]}, {273, &D_002AC880[702]}, {274, &D_002AC880[703]}, {292, &D_002AC880[647]}, {526, &D_002AC880[638]},
    {527, &D_002AC880[642]}, {530, &D_002AC880[615]}, {622, &D_002AC880[637]}, {625, &D_002AC880[665]}, {626, &D_002AC880[666]},
    {627, &D_002AC880[635]}, {628, &D_002AC880[668]}, {631, &D_002AC880[663]}, {632, &D_002AC880[664]}, {634, &D_002AC880[630]},
    {638, &D_002AC880[614]}, {639, &D_002AC880[616]}, {640, &D_002AC880[617]}, {641, &D_002AC880[618]}, {642, &D_002AC880[619]},
    {643, &D_002AC880[620]}, {644, &D_002AC880[621]}, {645, &D_002AC880[622]}, {646, &D_002AC880[623]}, {647, &D_002AC880[624]},
    {648, &D_002AC880[625]}, {649, &D_002AC880[626]}, {650, &D_002AC880[627]}, {651, &D_002AC880[628]}, {652, &D_002AC880[629]},
    {653, &D_002AC880[631]}, {654, &D_002AC880[632]}, {655, &D_002AC880[633]}, {656, &D_002AC880[636]}, {657, &D_002AC880[639]},
    {658, &D_002AC880[640]}, {659, &D_002AC880[641]}, {660, &D_002AC880[646]}, {661, &D_002AC880[648]}, {662, &D_002AC880[649]},
    {663, &D_002AC880[650]}, {664, &D_002AC880[651]}, {665, &D_002AC880[652]}, {666, &D_002AC880[653]}, {667, &D_002AC880[654]},
    {668, &D_002AC880[655]}, {669, &D_002AC880[656]}, {670, &D_002AC880[657]}, {671, &D_002AC880[658]}, {672, &D_002AC880[659]},
    {673, &D_002AC880[661]}, {674, &D_002AC880[662]}, {675, &D_002AC880[667]}, {676, &D_002AC880[669]}, {677, &D_002AC880[682]},
    {678, &D_002AC880[683]}, {679, &D_002AC880[684]}, {680, &D_002AC880[685]}, {681, &D_002AC880[686]}, {682, &D_002AC880[687]},
    {683, &D_002AC880[688]}, {684, &D_002AC880[689]}, {685, &D_002AC880[690]}, {686, &D_002AC880[691]}, {687, &D_002AC880[692]},
    {688, &D_002AC880[693]}, {689, &D_002AC880[694]}, {690, &D_002AC880[695]}, {691, &D_002AC880[696]}, {692, &D_002AC880[697]},
    {693, &D_002AC880[698]}, {694, &D_002AC880[699]}, {695, &D_002AC880[700]}, {696, &D_002AC880[701]}, {697, &D_002AC880[704]},
    {698, &D_002AC880[705]}, {699, &D_002AC880[706]}, {700, &D_002AC880[707]}, {701, &D_002AC880[708]}, {702, &D_002AC880[709]},
    {703, &D_002AC880[710]}, {704, &D_002AC880[711]}, {705, &D_002AC880[712]}, {706, &D_002AC880[713]}, {707, &D_002AC880[714]},
    {708, &D_002AC880[715]}, {709, &D_002AC880[716]},
};
extern const SPLOPTROW D_00275E68[53] SPLOPT_RODATA = {   /* WR (cid 115) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {573, &D_002AC880[947]}, {864, &D_002AC880[945]},
    {865, &D_002AC880[946]}, {866, &D_002AC880[948]}, {867, &D_002AC880[949]}, {868, &D_002AC880[950]}, {869, &D_002AC880[951]},
    {870, &D_002AC880[952]}, {871, &D_002AC880[953]}, {872, &D_002AC880[954]}, {873, &D_002AC880[955]}, {874, &D_002AC880[956]},
    {875, &D_002AC880[957]}, {876, &D_002AC880[958]}, {877, &D_002AC880[959]}, {878, &D_002AC880[960]}, {879, &D_002AC880[961]},
    {880, &D_002AC880[962]}, {881, &D_002AC880[963]}, {882, &D_002AC880[964]},
};
extern const SPLOPTROW D_00276010[41] SPLOPT_RODATA = {   /* CID116 (cid 116) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {235, &D_002AC880[979]}, {893, &D_002AC880[980]},
    {894, &D_002AC880[981]}, {895, &D_002AC880[982]}, {896, &D_002AC880[983]}, {897, &D_002AC880[984]}, {898, &D_002AC880[985]},
    {899, &D_002AC880[986]},
};
extern const SPLOPTROW D_00276158[39] SPLOPT_RODATA = {   /* CID117 (cid 117) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {214, &D_002AC880[1095]}, {795, &D_002AC880[1091]},
    {954, &D_002AC880[1090]}, {955, &D_002AC880[1092]}, {956, &D_002AC880[1093]}, {957, &D_002AC880[1094]},
};
extern const SPLOPTROW D_00276290[36] SPLOPT_RODATA = {   /* CID118 (cid 118) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1027, &D_002AC880[1191]}, {1028, &D_002AC880[1192]},
    {1029, &D_002AC880[1193]},
};
extern const SPLOPTROW D_002763B0[46] SPLOPT_RODATA = {   /* CID119 (cid 119) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {1139, &D_002AC880[1324]}, {1140, &D_002AC880[1325]},
    {1141, &D_002AC880[1326]}, {1142, &D_002AC880[1327]}, {1143, &D_002AC880[1328]}, {1144, &D_002AC880[1329]}, {1145, &D_002AC880[1330]},
    {1146, &D_002AC880[1331]}, {1147, &D_002AC880[1332]}, {1148, &D_002AC880[1333]}, {1149, &D_002AC880[1334]}, {1150, &D_002AC880[1335]},
    {1151, &D_002AC880[1336]},
};
extern const SPLOPTROW D_00276520[46] SPLOPT_RODATA = {   /* CID120 (cid 120) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {361, &D_002AC880[353]}, {362, &D_002AC880[354]},
    {363, &D_002AC880[355]}, {364, &D_002AC880[356]}, {365, &D_002AC880[357]}, {366, &D_002AC880[358]}, {368, &D_002AC880[363]},
    {369, &D_002AC880[364]}, {371, &D_002AC880[365]}, {470, &D_002AC880[359]}, {471, &D_002AC880[360]}, {472, &D_002AC880[361]},
    {473, &D_002AC880[362]},
};
extern const SPLOPTROW D_00276690[46] SPLOPT_RODATA = {   /* CID121 (cid 121) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {342, &D_002AC880[1354]}, {1159, &D_002AC880[1344]},
    {1160, &D_002AC880[1345]}, {1161, &D_002AC880[1346]}, {1162, &D_002AC880[1347]}, {1163, &D_002AC880[1348]}, {1164, &D_002AC880[1349]},
    {1165, &D_002AC880[1350]}, {1166, &D_002AC880[1351]}, {1167, &D_002AC880[1352]}, {1168, &D_002AC880[1353]}, {1169, &D_002AC880[1355]},
    {1170, &D_002AC880[1356]},
};
extern const SPLOPTROW D_00276800[33] SPLOPT_RODATA = {   /* CID122 (cid 122) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00276908[35] SPLOPT_RODATA = {   /* CID123 (cid 123) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {278, &D_002AC880[1361]}, {1174, &D_002AC880[1362]},
};
extern const SPLOPTROW D_00276A20[36] SPLOPT_RODATA = {   /* PATHLINK (cid 124) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {238, &D_002AC880[1447]}, {1231, &D_002AC880[1446]},
    {1232, &D_002AC880[1448]},
};
extern const SPLOPTROW D_00276B40[33] SPLOPT_RODATA = {   /* CID125 (cid 125) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]},
};
extern const SPLOPTROW D_00276C48[54] SPLOPT_RODATA = {   /* WIDGET (cid 126) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]},
};
extern const SPLOPTROW D_00276DF8[56] SPLOPT_RODATA = {   /* WIDGET127 (cid 127) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]}, {1001, &D_002AC880[1148]},
    {1002, &D_002AC880[1149]},
};
extern const SPLOPTROW D_00276FB8[54] SPLOPT_RODATA = {   /* WIDGET128 (cid 128) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]},
};
extern const SPLOPTROW D_00277168[56] SPLOPT_RODATA = {   /* WIDGET129 (cid 129) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]}, {1003, &D_002AC880[1150]},
    {1004, &D_002AC880[1151]},
};
extern const SPLOPTROW D_00277328[59] SPLOPT_RODATA = {   /* WIDGET130 (cid 130) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]}, {996, &D_002AC880[1143]},
    {997, &D_002AC880[1144]}, {998, &D_002AC880[1145]}, {999, &D_002AC880[1146]}, {1000, &D_002AC880[1147]},
};
extern const SPLOPTROW D_00277500[54] SPLOPT_RODATA = {   /* WIDGET131 (cid 131) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {258, &D_002AC880[1129]}, {766, &D_002AC880[1122]},
    {976, &D_002AC880[1123]}, {977, &D_002AC880[1124]}, {978, &D_002AC880[1125]}, {979, &D_002AC880[1126]}, {980, &D_002AC880[1127]},
    {981, &D_002AC880[1128]}, {982, &D_002AC880[1130]}, {983, &D_002AC880[1131]}, {984, &D_002AC880[1132]}, {985, &D_002AC880[1133]},
    {986, &D_002AC880[1134]}, {987, &D_002AC880[1135]}, {988, &D_002AC880[1136]}, {989, &D_002AC880[1137]}, {990, &D_002AC880[1138]},
    {991, &D_002AC880[1139]}, {992, &D_002AC880[1140]}, {993, &D_002AC880[1141]}, {994, &D_002AC880[1142]},
};
extern const SPLOPTROW D_002776B0[62] SPLOPT_RODATA = {   /* JOB (cid 132) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {526, &D_002AC880[1256]}, {1064, &D_002AC880[1235]},
    {1065, &D_002AC880[1236]}, {1066, &D_002AC880[1237]}, {1067, &D_002AC880[1238]}, {1068, &D_002AC880[1239]}, {1069, &D_002AC880[1240]},
    {1070, &D_002AC880[1241]}, {1071, &D_002AC880[1242]}, {1072, &D_002AC880[1243]}, {1073, &D_002AC880[1244]}, {1074, &D_002AC880[1245]},
    {1075, &D_002AC880[1246]}, {1076, &D_002AC880[1247]}, {1077, &D_002AC880[1248]}, {1078, &D_002AC880[1249]}, {1079, &D_002AC880[1250]},
    {1080, &D_002AC880[1251]}, {1081, &D_002AC880[1252]}, {1082, &D_002AC880[1253]}, {1083, &D_002AC880[1254]}, {1084, &D_002AC880[1255]},
    {1085, &D_002AC880[1257]}, {1086, &D_002AC880[1258]}, {1087, &D_002AC880[1259]}, {1088, &D_002AC880[1260]}, {1089, &D_002AC880[1261]},
    {1090, &D_002AC880[1262]}, {1091, &D_002AC880[1263]},
};
extern const SPLOPTROW D_002778A0[69] SPLOPT_RODATA = {   /* TASK (cid 133) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {14, &D_002AC880[13]}, {15, &D_002AC880[14]},
    {28, &D_002AC880[15]}, {29, &D_002AC880[16]}, {30, &D_002AC880[17]}, {31, &D_002AC880[18]}, {32, &D_002AC880[19]},
    {33, &D_002AC880[20]}, {34, &D_002AC880[21]}, {35, &D_002AC880[22]}, {36, &D_002AC880[23]}, {37, &D_002AC880[24]},
    {39, &D_002AC880[25]}, {40, &D_002AC880[26]}, {41, &D_002AC880[27]}, {60, &D_002AC880[28]}, {63, &D_002AC880[29]},
    {64, &D_002AC880[30]}, {65, &D_002AC880[31]}, {67, &D_002AC880[32]}, {342, &D_002AC880[1286]}, {526, &D_002AC880[1288]},
    {1074, &D_002AC880[1265]}, {1076, &D_002AC880[1266]}, {1078, &D_002AC880[1267]}, {1080, &D_002AC880[1268]}, {1081, &D_002AC880[1269]},
    {1082, &D_002AC880[1282]}, {1083, &D_002AC880[1283]}, {1090, &D_002AC880[1295]}, {1091, &D_002AC880[1297]}, {1093, &D_002AC880[1264]},
    {1094, &D_002AC880[1270]}, {1095, &D_002AC880[1271]}, {1096, &D_002AC880[1272]}, {1097, &D_002AC880[1273]}, {1098, &D_002AC880[1274]},
    {1099, &D_002AC880[1275]}, {1100, &D_002AC880[1276]}, {1101, &D_002AC880[1277]}, {1102, &D_002AC880[1278]}, {1103, &D_002AC880[1279]},
    {1104, &D_002AC880[1280]}, {1105, &D_002AC880[1281]}, {1106, &D_002AC880[1284]}, {1107, &D_002AC880[1285]}, {1108, &D_002AC880[1287]},
    {1109, &D_002AC880[1289]}, {1110, &D_002AC880[1290]}, {1111, &D_002AC880[1291]}, {1112, &D_002AC880[1292]}, {1113, &D_002AC880[1293]},
    {1114, &D_002AC880[1294]}, {1115, &D_002AC880[1296]}, {1116, &D_002AC880[1298]}, {1117, &D_002AC880[1299]},
};
extern const SPLOPTROW D_00277AC8[24] SPLOPT_RODATA = {   /* ASEGA (cid 134) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {217, &D_002AC880[120]}, {221, &D_002AC880[122]},
    {236, &D_002AC880[123]}, {237, &D_002AC880[124]}, {248, &D_002AC880[125]}, {250, &D_002AC880[116]}, {251, &D_002AC880[117]},
    {252, &D_002AC880[118]}, {253, &D_002AC880[119]}, {254, &D_002AC880[121]}, {255, &D_002AC880[126]},
};
extern const SPLOPTROW D_00277B88[29] SPLOPT_RODATA = {   /* CM (cid 135) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {34, &D_002AC880[171]}, {35, &D_002AC880[172]},
    {36, &D_002AC880[173]}, {233, &D_002AC880[159]}, {292, &D_002AC880[160]}, {293, &D_002AC880[161]}, {294, &D_002AC880[162]},
    {295, &D_002AC880[163]}, {296, &D_002AC880[164]}, {297, &D_002AC880[165]}, {298, &D_002AC880[166]}, {299, &D_002AC880[167]},
    {300, &D_002AC880[168]}, {301, &D_002AC880[169]}, {302, &D_002AC880[170]}, {303, &D_002AC880[174]},
};
extern const SPLOPTROW D_00277C70[25] SPLOPT_RODATA = {   /* SMA (cid 136) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {182, &D_002AC880[378]}, {236, &D_002AC880[379]},
    {237, &D_002AC880[380]}, {251, &D_002AC880[371]}, {478, &D_002AC880[372]}, {479, &D_002AC880[373]}, {480, &D_002AC880[374]},
    {481, &D_002AC880[375]}, {482, &D_002AC880[376]}, {483, &D_002AC880[377]}, {484, &D_002AC880[381]}, {485, &D_002AC880[382]},
};
extern const SPLOPTROW D_00277D38[23] SPLOPT_RODATA = {   /* POCTL (cid 137) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {900, &D_002AC880[987]}, {901, &D_002AC880[988]},
    {902, &D_002AC880[989]}, {903, &D_002AC880[990]}, {904, &D_002AC880[991]}, {905, &D_002AC880[992]}, {906, &D_002AC880[993]},
    {907, &D_002AC880[994]}, {908, &D_002AC880[995]}, {909, &D_002AC880[996]},
};
extern const SPLOPTROW D_00277DF0[14] SPLOPT_RODATA = {   /* SAA (cid 138) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {202, &D_002AC880[84]},
};
extern const SPLOPTROW D_00277E60[16] SPLOPT_RODATA = {   /* LOOP (cid 139) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {143, &D_002AC880[85]}, {144, &D_002AC880[86]},
    {202, &D_002AC880[84]},
};
extern const SPLOPTROW D_00277EE0[16] SPLOPT_RODATA = {   /* PINGPONG (cid 140) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {147, &D_002AC880[87]}, {148, &D_002AC880[88]},
    {202, &D_002AC880[84]},
};
extern const SPLOPTROW D_00277F60[15] SPLOPT_RODATA = {   /* SHUFFLE (cid 141) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {151, &D_002AC880[89]}, {202, &D_002AC880[84]},
};
extern const SPLOPTROW D_00277FD8[18] SPLOPT_RODATA = {   /* EYES (cid 142) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {158, &D_002AC880[90]}, {159, &D_002AC880[91]},
    {202, &D_002AC880[84]}, {203, &D_002AC880[92]}, {204, &D_002AC880[93]},
};
extern const SPLOPTROW D_00278068[15] SPLOPT_RODATA = {   /* SCROLLER (cid 143) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {202, &D_002AC880[84]}, {205, &D_002AC880[94]},
};
extern const SPLOPTROW D_002780E0[18] SPLOPT_RODATA = {   /* CIRCLER (cid 144) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {170, &D_002AC880[96]}, {171, &D_002AC880[97]},
    {172, &D_002AC880[98]}, {202, &D_002AC880[84]}, {206, &D_002AC880[95]},
};
extern const SPLOPTROW D_00278170[13] SPLOPT_RODATA = {   /* LEAP (cid 145) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]},
};
extern const SPLOPTROW D_002781D8[13] SPLOPT_RODATA = {   /* CID146 (cid 146) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]},
};
extern const SPLOPTROW D_00278240[19] SPLOPT_RODATA = {   /* CID147 (cid 147) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {34, &D_002AC880[1097]}, {237, &D_002AC880[1100]},
    {255, &D_002AC880[1101]}, {957, &D_002AC880[1099]}, {958, &D_002AC880[1096]}, {959, &D_002AC880[1098]},
};
extern const SPLOPTROW D_002782D8[13] SPLOPT_RODATA = {   /* CID148 (cid 148) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]},
};
extern const SPLOPTROW D_00278340[21] SPLOPT_RODATA = {   /* CID149 (cid 149) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {323, &D_002AC880[1224]}, {324, &D_002AC880[1225]},
    {1058, &D_002AC880[1226]}, {1059, &D_002AC880[1227]}, {1060, &D_002AC880[1228]}, {1061, &D_002AC880[1229]}, {1062, &D_002AC880[1230]},
    {1063, &D_002AC880[1231]},
};
extern const SPLOPTROW D_002783E8[16] SPLOPT_RODATA = {   /* CID150 (cid 150) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {122, &D_002AC880[1232]}, {123, &D_002AC880[1233]},
    {795, &D_002AC880[1234]},
};
extern const SPLOPTROW D_00278468[23] SPLOPT_RODATA = {   /* CID151 (cid 151) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {212, &D_002AC880[130]}, {213, &D_002AC880[132]},
    {256, &D_002AC880[127]}, {257, &D_002AC880[128]}, {258, &D_002AC880[129]}, {259, &D_002AC880[131]}, {260, &D_002AC880[133]},
    {261, &D_002AC880[134]}, {262, &D_002AC880[135]}, {263, &D_002AC880[136]},
};
extern const SPLOPTROW D_00278520[27] SPLOPT_RODATA = {   /* CID152 (cid 152) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {264, &D_002AC880[137]}, {265, &D_002AC880[138]},
    {266, &D_002AC880[139]}, {267, &D_002AC880[140]}, {268, &D_002AC880[141]}, {269, &D_002AC880[142]}, {270, &D_002AC880[143]},
    {271, &D_002AC880[144]}, {272, &D_002AC880[145]}, {273, &D_002AC880[146]}, {274, &D_002AC880[147]}, {275, &D_002AC880[148]},
    {276, &D_002AC880[149]}, {277, &D_002AC880[150]},
};
extern const SPLOPTROW D_002785F8[32] SPLOPT_RODATA = {   /* JSG (cid 153) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {253, &D_002AC880[1408]}, {1197, &D_002AC880[1400]},
    {1198, &D_002AC880[1401]}, {1199, &D_002AC880[1402]}, {1200, &D_002AC880[1403]}, {1201, &D_002AC880[1404]}, {1202, &D_002AC880[1405]},
    {1203, &D_002AC880[1406]}, {1204, &D_002AC880[1407]}, {1205, &D_002AC880[1409]}, {1206, &D_002AC880[1410]}, {1207, &D_002AC880[1411]},
    {1208, &D_002AC880[1412]}, {1209, &D_002AC880[1413]}, {1210, &D_002AC880[1414]}, {1211, &D_002AC880[1415]}, {1212, &D_002AC880[1416]},
    {1213, &D_002AC880[1417]}, {1214, &D_002AC880[1418]},
};
extern const SPLOPTROW D_002786F8[17] SPLOPT_RODATA = {   /* CID154 (cid 154) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {979, &D_002AC880[1300]}, {980, &D_002AC880[1301]},
    {1118, &D_002AC880[1302]}, {1119, &D_002AC880[1303]},
};
extern const SPLOPTROW D_00278780[14] SPLOPT_RODATA = {   /* BHV (cid 155) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002787F0[14] SPLOPT_RODATA = {   /* BHVGUARD (cid 156) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278860[14] SPLOPT_RODATA = {   /* BHVANIM (cid 157) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002788D0[14] SPLOPT_RODATA = {   /* BHVIDLE (cid 158) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278940[14] SPLOPT_RODATA = {   /* BHVSTRIKE (cid 159) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002789B0[14] SPLOPT_RODATA = {   /* BHVHIT (cid 160) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278A20[14] SPLOPT_RODATA = {   /* BHV161 (cid 161) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278A90[14] SPLOPT_RODATA = {   /* BHV162 (cid 162) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278B00[14] SPLOPT_RODATA = {   /* BHVKO (cid 163) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278B70[14] SPLOPT_RODATA = {   /* BHVATTACK (cid 164) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278BE0[14] SPLOPT_RODATA = {   /* BHVCOMBAT (cid 165) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278C50[14] SPLOPT_RODATA = {   /* BHVPATH (cid 166) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278CC0[14] SPLOPT_RODATA = {   /* BHVWALKPATH (cid 167) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278D30[14] SPLOPT_RODATA = {   /* BHVWALKGOAL (cid 168) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278DA0[16] SPLOPT_RODATA = {   /* BHV169 (cid 169) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {236, &D_002AC880[1359]}, {1171, &D_002AC880[1357]},
    {1172, &D_002AC880[1358]},
};
extern const SPLOPTROW D_00278E20[14] SPLOPT_RODATA = {   /* BHVPATROL (cid 170) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278E90[14] SPLOPT_RODATA = {   /* BHV171 (cid 171) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278F00[14] SPLOPT_RODATA = {   /* BHVLEAP (cid 172) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278F70[14] SPLOPT_RODATA = {   /* BHVSHOOT (cid 173) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00278FE0[14] SPLOPT_RODATA = {   /* BHV174 (cid 174) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279050[14] SPLOPT_RODATA = {   /* BHV175 (cid 175) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002790C0[14] SPLOPT_RODATA = {   /* BHV176 (cid 176) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279130[14] SPLOPT_RODATA = {   /* BHVJUGGLED (cid 177) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002791A0[14] SPLOPT_RODATA = {   /* BHV178 (cid 178) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279210[14] SPLOPT_RODATA = {   /* BHV179 (cid 179) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279280[14] SPLOPT_RODATA = {   /* BHV180 (cid 180) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002792F0[14] SPLOPT_RODATA = {   /* BHV181 (cid 181) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279360[14] SPLOPT_RODATA = {   /* BHV182 (cid 182) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002793D0[14] SPLOPT_RODATA = {   /* BHV183 (cid 183) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279440[14] SPLOPT_RODATA = {   /* BHVRAIL (cid 184) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002794B0[14] SPLOPT_RODATA = {   /* BHVPIPE (cid 185) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279520[14] SPLOPT_RODATA = {   /* BHVGOTO (cid 186) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279590[14] SPLOPT_RODATA = {   /* BHV187 (cid 187) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279600[14] SPLOPT_RODATA = {   /* BHV188 (cid 188) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279670[14] SPLOPT_RODATA = {   /* BHV189 (cid 189) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002796E0[14] SPLOPT_RODATA = {   /* BHVPATHLINK (cid 190) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279750[14] SPLOPT_RODATA = {   /* BHV191 (cid 191) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002797C0[14] SPLOPT_RODATA = {   /* BHVPATROLPATH (cid 192) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279830[14] SPLOPT_RODATA = {   /* BHV193 (cid 193) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002798A0[14] SPLOPT_RODATA = {   /* BHVFOLLOW (cid 194) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279910[14] SPLOPT_RODATA = {   /* BHV195 (cid 195) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279980[14] SPLOPT_RODATA = {   /* BHV196 (cid 196) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_002799F0[14] SPLOPT_RODATA = {   /* BHV197 (cid 197) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};
extern const SPLOPTROW D_00279A60[14] SPLOPT_RODATA = {   /* BHV198 (cid 198) */
    {1, &D_002AC880[0]}, {2, &D_002AC880[1]}, {3, &D_002AC880[2]}, {4, &D_002AC880[3]}, {5, &D_002AC880[4]},
    {6, &D_002AC880[5]}, {7, &D_002AC880[6]}, {8, &D_002AC880[7]}, {9, &D_002AC880[8]}, {10, &D_002AC880[9]},
    {11, &D_002AC880[10]}, {12, &D_002AC880[11]}, {13, &D_002AC880[12]}, {1171, &D_002AC880[1357]},
};

/* g_mpcidarysplopt: the option array of each CID (spliceobj.cpp's SPLOPTS). */
struct SPLOPTSROW { int csplopt; const SPLOPTROW *asplopt; };
#define SPLOPTS(a) { sizeof(a) / sizeof(a[0]), a }
extern const SPLOPTSROW D_00279AD0[199] SPLOPT_RODATA = {
    SPLOPTS(D_00262380), /* BASIC */
    SPLOPTS(D_002623E8), /* SW */
    SPLOPTS(D_00262550), /* LO */
    SPLOPTS(D_00262658), /* XFM */
    SPLOPTS(D_00262760), /* LIGHT */
    SPLOPTS(D_002628A8), /* CID5 */
    SPLOPTS(D_002629B8), /* PROXY */
    SPLOPTS(D_00262B00), /* CLUEPROXY */
    SPLOPTS(D_00262C48), /* CID8 */
    SPLOPTS(D_00262D50), /* CID9 */
    SPLOPTS(D_00262E70), /* SHAPE */
    SPLOPTS(D_00262FF8), /* PIPE */
    SPLOPTS(D_002631E0), /* CID12 */
    SPLOPTS(D_002633B8), /* RAIL */
    SPLOPTS(D_002635A0), /* CID14 */
    SPLOPTS(D_00263728), /* TN */
    SPLOPTS(D_00263968), /* DETECTOR */
    SPLOPTS(D_00263AC8), /* PATHZONE */
    SPLOPTS(D_00263BD8), /* TARGET */
    SPLOPTS(D_00263D00), /* HND */
    SPLOPTS(D_00263E98), /* CID20 */
    SPLOPTS(D_00263FA8), /* CID21 */
    SPLOPTS(D_002640C8), /* VOL */
    SPLOPTS(D_002641E0), /* CID23 */
    SPLOPTS(D_002642F8), /* VOLBTN */
    SPLOPTS(D_002644A0), /* CID25 */
    SPLOPTS(D_00264648), /* EXIT */
    SPLOPTS(D_002647B8), /* CID27 */
    SPLOPTS(D_00264918), /* BLIPG */
    SPLOPTS(D_00264A20), /* EMITTER */
    SPLOPTS(D_00264D30), /* EXPL */
    SPLOPTS(D_00264E48), /* EXPLG */
    SPLOPTS(D_00264F60), /* EXPLO */
    SPLOPTS(D_00265110), /* EXPLS */
    SPLOPTS(D_002653F8), /* CID34 */
    SPLOPTS(D_00265550), /* CID35 */
    SPLOPTS(D_002656E0), /* CID36 */
    SPLOPTS(D_00265898), /* CID37 */
    SPLOPTS(D_00265A60), /* CID38 */
    SPLOPTS(D_00265BB8), /* CID39 */
    SPLOPTS(D_00265D00), /* CID40 */
    SPLOPTS(D_00265E10), /* CID41 */
    SPLOPTS(D_00265F20), /* WARP */
    SPLOPTS(D_00266058), /* CID43 */
    SPLOPTS(D_00266198), /* CID44 */
    SPLOPTS(D_002662D0), /* CID45 */
    SPLOPTS(D_00266418), /* CID46 */
    SPLOPTS(D_00266588), /* CID47 */
    SPLOPTS(D_00266690), /* SO */
    SPLOPTS(D_00266930), /* MS */
    SPLOPTS(D_00266BD0), /* PO */
    SPLOPTS(D_00266EB8), /* STEP */
    SPLOPTS(D_002671F8), /* STEPPLAYER */
    SPLOPTS(D_002676D8), /* JT */
    SPLOPTS(D_00267BE0), /* MURRAY */
    SPLOPTS(D_002680E8), /* BENTLEY */
    SPLOPTS(D_002685E8), /* STEPGUARD */
    SPLOPTS(D_00268C90), /* GOMER */
    SPLOPTS(D_00269338), /* GUARD */
    SPLOPTS(D_002699E0), /* DIMITRI */
    SPLOPTS(D_0026A098), /* NEELA */
    SPLOPTS(D_0026A780), /* RAJAN */
    SPLOPTS(D_0026AE30), /* JEANBISON */
    SPLOPTS(D_0026B4D8), /* CARMELITA */
    SPLOPTS(D_0026BB88), /* CONTESSA */
    SPLOPTS(D_0026C230), /* ARPEGGIO */
    SPLOPTS(D_0026C8D8), /* BEAR */
    SPLOPTS(D_0026CF80), /* BEARCUB */
    SPLOPTS(D_0026D628), /* DANCER */
    SPLOPTS(D_0026D978), /* TURRET */
    SPLOPTS(D_0026DCD0), /* SAFEHOUSE */
    SPLOPTS(D_0026DFD0), /* HACK */
    SPLOPTS(D_0026E2F8), /* HACKSHIP */
    SPLOPTS(D_0026E640), /* HACKENEMY */
    SPLOPTS(D_0026E9C8), /* HACKBULLET */
    SPLOPTS(D_0026ED18), /* HACKLOCK */
    SPLOPTS(D_0026F068), /* HACKWALL */
    SPLOPTS(D_0026F3A8), /* ROBOTTANK */
    SPLOPTS(D_0026F710), /* VAULT */
    SPLOPTS(D_0026FA28), /* BOMB */
    SPLOPTS(D_0026FD50), /* MISSILE */
    SPLOPTS(D_002700B8), /* ACCMISS */
    SPLOPTS(D_00270430), /* CID82 */
    SPLOPTS(D_002707B0), /* BUTTON */
    SPLOPTS(D_00270AD8), /* SENSOR */
    SPLOPTS(D_00270D88), /* LASEN */
    SPLOPTS(D_00271068), /* BARRIER */
    SPLOPTS(D_00271328), /* RIPG */
    SPLOPTS(D_002715C8), /* BARREL */
    SPLOPTS(D_002718C8), /* WATER */
    SPLOPTS(D_00271BA8), /* DPRIZE */
    SPLOPTS(D_00271EA0), /* COIN */
    SPLOPTS(D_002721A0), /* KEY */
    SPLOPTS(D_002724A0), /* MEDICINE */
    SPLOPTS(D_002727B0), /* LOOT */
    SPLOPTS(D_00272AD8), /* CLUE */
    SPLOPTS(D_00272DD0), /* CID96 */
    SPLOPTS(D_002730D8), /* SKY */
    SPLOPTS(D_00273380), /* CID98 */
    SPLOPTS(D_002736E0), /* CID99 */
    SPLOPTS(D_00273980), /* CID100 */
    SPLOPTS(D_00273C78), /* CID101 */
    SPLOPTS(D_00273F70), /* CID102 */
    SPLOPTS(D_00274270), /* BOOTY */
    SPLOPTS(D_00274588), /* CID104 */
    SPLOPTS(D_00274830), /* CID105 */
    SPLOPTS(D_00274B08), /* CID106 */
    SPLOPTS(D_00274DB0), /* WAYPT */
    SPLOPTS(D_002750A8), /* CID108 */
    SPLOPTS(D_00275360), /* ASEG */
    SPLOPTS(D_002754B8), /* ASEGBL */
    SPLOPTS(D_00275610), /* SM */
    SPLOPTS(D_00275730), /* CID112 */
    SPLOPTS(D_00275880), /* CID113 */
    SPLOPTS(D_00275A20), /* RELAY */
    SPLOPTS(D_00275E68), /* WR */
    SPLOPTS(D_00276010), /* CID116 */
    SPLOPTS(D_00276158), /* CID117 */
    SPLOPTS(D_00276290), /* CID118 */
    SPLOPTS(D_002763B0), /* CID119 */
    SPLOPTS(D_00276520), /* CID120 */
    SPLOPTS(D_00276690), /* CID121 */
    SPLOPTS(D_00276800), /* CID122 */
    SPLOPTS(D_00276908), /* CID123 */
    SPLOPTS(D_00276A20), /* PATHLINK */
    SPLOPTS(D_00276B40), /* CID125 */
    SPLOPTS(D_00276C48), /* WIDGET */
    SPLOPTS(D_00276DF8), /* WIDGET127 */
    SPLOPTS(D_00276FB8), /* WIDGET128 */
    SPLOPTS(D_00277168), /* WIDGET129 */
    SPLOPTS(D_00277328), /* WIDGET130 */
    SPLOPTS(D_00277500), /* WIDGET131 */
    SPLOPTS(D_002776B0), /* JOB */
    SPLOPTS(D_002778A0), /* TASK */
    SPLOPTS(D_00277AC8), /* ASEGA */
    SPLOPTS(D_00277B88), /* CM */
    SPLOPTS(D_00277C70), /* SMA */
    SPLOPTS(D_00277D38), /* POCTL */
    SPLOPTS(D_00277DF0), /* SAA */
    SPLOPTS(D_00277E60), /* LOOP */
    SPLOPTS(D_00277EE0), /* PINGPONG */
    SPLOPTS(D_00277F60), /* SHUFFLE */
    SPLOPTS(D_00277FD8), /* EYES */
    SPLOPTS(D_00278068), /* SCROLLER */
    SPLOPTS(D_002780E0), /* CIRCLER */
    SPLOPTS(D_00278170), /* LEAP */
    SPLOPTS(D_002781D8), /* CID146 */
    SPLOPTS(D_00278240), /* CID147 */
    SPLOPTS(D_002782D8), /* CID148 */
    SPLOPTS(D_00278340), /* CID149 */
    SPLOPTS(D_002783E8), /* CID150 */
    SPLOPTS(D_00278468), /* CID151 */
    SPLOPTS(D_00278520), /* CID152 */
    SPLOPTS(D_002785F8), /* JSG */
    SPLOPTS(D_002786F8), /* CID154 */
    SPLOPTS(D_00278780), /* BHV */
    SPLOPTS(D_002787F0), /* BHVGUARD */
    SPLOPTS(D_00278860), /* BHVANIM */
    SPLOPTS(D_002788D0), /* BHVIDLE */
    SPLOPTS(D_00278940), /* BHVSTRIKE */
    SPLOPTS(D_002789B0), /* BHVHIT */
    SPLOPTS(D_00278A20), /* BHV161 */
    SPLOPTS(D_00278A90), /* BHV162 */
    SPLOPTS(D_00278B00), /* BHVKO */
    SPLOPTS(D_00278B70), /* BHVATTACK */
    SPLOPTS(D_00278BE0), /* BHVCOMBAT */
    SPLOPTS(D_00278C50), /* BHVPATH */
    SPLOPTS(D_00278CC0), /* BHVWALKPATH */
    SPLOPTS(D_00278D30), /* BHVWALKGOAL */
    SPLOPTS(D_00278DA0), /* BHV169 */
    SPLOPTS(D_00278E20), /* BHVPATROL */
    SPLOPTS(D_00278E90), /* BHV171 */
    SPLOPTS(D_00278F00), /* BHVLEAP */
    SPLOPTS(D_00278F70), /* BHVSHOOT */
    SPLOPTS(D_00278FE0), /* BHV174 */
    SPLOPTS(D_00279050), /* BHV175 */
    SPLOPTS(D_002790C0), /* BHV176 */
    SPLOPTS(D_00279130), /* BHVJUGGLED */
    SPLOPTS(D_002791A0), /* BHV178 */
    SPLOPTS(D_00279210), /* BHV179 */
    SPLOPTS(D_00279280), /* BHV180 */
    SPLOPTS(D_002792F0), /* BHV181 */
    SPLOPTS(D_00279360), /* BHV182 */
    SPLOPTS(D_002793D0), /* BHV183 */
    SPLOPTS(D_00279440), /* BHVRAIL */
    SPLOPTS(D_002794B0), /* BHVPIPE */
    SPLOPTS(D_00279520), /* BHVGOTO */
    SPLOPTS(D_00279590), /* BHV187 */
    SPLOPTS(D_00279600), /* BHV188 */
    SPLOPTS(D_00279670), /* BHV189 */
    SPLOPTS(D_002796E0), /* BHVPATHLINK */
    SPLOPTS(D_00279750), /* BHV191 */
    SPLOPTS(D_002797C0), /* BHVPATROLPATH */
    SPLOPTS(D_00279830), /* BHV193 */
    SPLOPTS(D_002798A0), /* BHVFOLLOW */
    SPLOPTS(D_00279910), /* BHV195 */
    SPLOPTS(D_00279980), /* BHV196 */
    SPLOPTS(D_002799F0), /* BHV197 */
    SPLOPTS(D_00279A60), /* BHV198 */
};
#undef SPLOPTS

/* s_aotypEvtParm, HandleBasicSpliceEvent's (basic.cpp) argument types: event k's arguments have the types
   D_0027A108[1 + s_aiEvtParm[k]] .. D_0027A108[s_aiEvtParm[k + 1]]. basic.cpp reads the types from 0x27A10C, one
   word into this array (an int array is 8-aligned, so 0x27A10C cannot start one): the first word, a BASIC * type
   (0x2002), is unreferenced (HYPOTHESIS: the type of the event's object, the handler's first parameter). */
extern const int D_0027A108[52] SPLOPT_RODATA = {
    0x2002, 0x2002, 0x2000, 0x2000, 0x2000, 6, 0x2002, 0x2002, 0x2038, 0x1010, 0x1010, 0x2088, 6,
    6, 0x2018, 0x2030, 0x2030, 0x1011, 0x2030, 0x2030, 0x1011, 0x2002, 5, 5, 0x2032, 0x2002,
    0x2002, 0x2002, 0x2002, 0x2062, 0x2002, 0x104E, 0x2002, 0x2002, 0x1050, 0x2002, 0x2002, 0x2085, 0x2085,
    0x2085, 0, 0x2038, 6, 0x2077, 0x2038, 0x2034, 0x204F, 0x2010, 0x2002, 0x106B, 0x106B, 0,
};
/* s_aiEvtParm: the first type of each of the 29 events (0..28) and the end. */
extern const int D_0027A1D8[30] SPLOPT_RODATA = {
    0, 1, 2, 3, 4, 6, 7, 8, 11, 14, 15, 18, 21, 24, 25, 27, 29, 30, 33, 35, 36, 37, 38, 39, 41, 43, 45, 46, 47, 51,
};
#undef SPLOPT_RODATA
} }
