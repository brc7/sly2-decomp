struct VTE { short d; short pad; void (*pfn)(); };
struct O { int x0, x4, x8; struct VTE *vt; char pad[0x13C - 0x10]; int k; };
struct PR { int a, b; };
struct S { int x0, x4, x8; struct VTE *vt; char pad[0xE0 - 0x10]; int xE0; char aE4[0x40]; int x124; char a128[0x20]; int c; struct PR apr[1]; };
struct G;
extern struct G D_003DD490;
extern char D_002DEB30[];
extern char D_002DDF40[];
struct O *func_0015E938(struct G *pg, int k, int id, struct S *p);
void func_0020FF70(struct G *pg, int a, void *b, struct S *p, int c);
void func_00161F48(void *a, int b, void *c, struct S *p);
int func_0015E9C8(struct G *pg, int a, int b, int c, int n, struct O **apo);
void func_00228358(struct O *po, int k);
void func_0014D8C0(void *p);
/** @sly2-annot addr=0x228750
 * Purpose: WARP method (Sly 1 TriggerWarp): apply its oid pairs (+0x14C, vt38), trigger its RSMGs
 *   (+0xE0/+0xE4), apply list +0x124, reset every EXIT (SetExitExits 0), splice event 5
 * Info: proposed name TriggerWarp (T1); TU exit_warp
 * Notes: v3: superseded TeleportSwPlayer | class review: WARP fields xE0/aE4/x124/apr@0x14C, single WARP*
 *   arg; body matches Sly 1 TriggerWarp (TriggerRsmg + exit reset loop); v2 TeleportSwPlayer
 * Confidence: mid (draft) [T1 0.70] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   r8_21BB88+verdicts_A2+class_review(master_names_v2_1)
 * See: docs/engine/world-sw.md#4-transitions-warps-exits-wipes-boot-queue,
 *   docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_00228750(struct S *p)
{
    struct O *apo[32];
    int i, j;
    for (i = 0; i < p->c; i++) {
        struct PR *ppr = &p->apr[i];
        struct O *po = func_0015E938(&D_003DD490, 4, ppr->a, p);
        if (po) {
            struct O *po2 = func_0015E938(&D_003DD490, 4, ppr->b, p);
            if (po2) po->vt[38].pfn((char *)po + po->vt[38].d, po2);
        }
    }
    func_0020FF70(&D_003DD490, p->xE0, p->aE4, p, 1);
    func_00161F48(D_002DEB30, p->x124, p->a128, p);
    for (j = func_0015E9C8(&D_003DD490, 5, 0x1A, 0, 0x20, apo) - 1; j >= 0; j--) {
        if (apo[j]->k != 2) func_00228358(apo[j], 0);
    }
    func_0014D8C0(D_002DDF40);
    p->vt[3].pfn((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
}
