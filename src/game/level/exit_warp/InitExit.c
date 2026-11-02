struct S { char pad[0x40]; int f; char pad2[0x10C]; int a, b; };
static inline int FIsPtr(int f) { return !(f & 1); }
void InitXfm(struct S *p);
void SetLoBusyk(struct S *p, int a);
/** @sly2-annot addr=0x2281E8
 * Purpose: EXIT (cid 0x1A) slot 7 Init: InitXfm; oidWarp +0x150 = 0x189, +0x154 = -1; busyk 0; grf |= 2
 * Info: proposed name InitExit (T1); class EXIT slot7; TU exit_warp
 * Notes: names_v2 C proposed DradEnemyStepguard__FP9STEPGUARD (0.438) | v3: superseded InitWarp | CONFIRMED
 *   (vtable slot + fields): EXIT slot 7, not WARP; v2 InitWarp
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void InitExit(struct S *p)
{
    int f;
    InitXfm(p);
    p->a = 0x189;
    p->b = -1;
    SetLoBusyk(p, 0);
    f = p->f;
    if (FIsPtr(f)) *(int *)f |= 2;
    else p->f = f | 2;
}
