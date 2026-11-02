struct S;
struct VTE { short d; short pad; void (*pfn)(); };
struct S { int x0, x4, x8; struct VTE *vt; char pad[0x134 - 0x10]; int x134; int pad1; int k; char pad2[0x158 - 0x140]; float t; int pad3; float dt; int x164; char a168[4]; };
struct G;
extern struct G D_003DD490;
extern char D_002DDF40[];
extern char D_002DEB30[];
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void SetExitExits(struct S *p, int k);
struct S *PexitDefault(struct G *pg);
void ApplyTaskStates(void *a, int b, void *c, struct S *p);
void RevokeCmPolicy(void *a, int b, int c, void *d, int e, int f);
/** @sly2-annot addr=0x2283D0
 * Purpose: state 3 handling: find default, heading, revoke cm policy
 * Info: proposed name UpdateWarpTriggered (T2); TU exit_warp
 * Notes: names_v2 B proposed UpdateStepguardSgs__FP9STEPGUARD (0.913)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_002283D0(struct S *p)
{
    if (p->k != 3) return;
    if (p->x134) {
        struct S *q;
        SetExitExits(p, 0);
        q = PexitDefault(&D_003DD490);
        if (q) func_002283D0(q);
        return;
    }
    ApplyTaskStates(D_002DEB30, p->x164, p->a168, p);
    RevokeCmPolicy(D_002DDF40, 2, -1, D_002DDF40 + 0x450, 0, 0);
    RevokeCmPolicy(D_002DDF40, 2, -1, D_002DDF40 + 0x520, 0, 0);
    SetExitExits(p, 4);
    p->t = D_002DDED0.t + p->dt;
    p->vt[3].pfn((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
}
