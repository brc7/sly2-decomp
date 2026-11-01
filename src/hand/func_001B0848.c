struct G { char pad[0xFD50]; int f; };
extern struct G D_002E65E0;
/** @sly2-annot addr=0x1B0848
 * Purpose: Text id 0x47 or 0x46 depending on flag
 * Info: proposed name PauseItemTextQuit (T2); TU menu
 * Notes: names_v2 v1 proposed UpdateStep__FP4STEPf (0.001)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B0848(int *p)
{
    if (D_002E65E0.f) { *p = 0x47; return; }
    *p = 0x46;
}
