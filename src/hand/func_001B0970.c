extern char D_002DEB30[];
extern void *D_003E0F9C;
void BroadcastGameEvent(void *p, int n, void *q);
void func_00162E38(void *p);
/** @sly2-annot addr=0x1B0970
 * Purpose: Game event 0x2A; FailJob(current job)
 * Info: proposed name PauseItemRestartJob (T2); TU menu
 * Notes: names_v2 v1 proposed PresetStepAccel__FP4STEPf (0.005)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0970(void)
{
    BroadcastGameEvent(D_002DEB30, 0x2A, 0);
    func_00162E38(D_003E0F9C);
}
