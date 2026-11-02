struct S;
struct VTE { short d; short pad; int (*pfn)(); };
struct S { int x0, x4, x8; struct VTE *vt; char pad[0xE0 - 0x10]; void *pE0; char pad1[0x128 - 0xE4]; int x128; char pad2[0x134 - 0x12C]; int k; float t; };
extern char D_002DDF40[];
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void SetCmPolicy(void *a, int b, void *c, void *d, struct S *p);
void RevokeCmPolicy(void *a, int b, int c, int d, int e, struct S *p);
/** @sly2-annot addr=0x221178
 * Purpose: tns change: In -> SetCmPolicy(g_cm, pri+2, &g_cm.cptn(+0x3B0), ppo, ptn) + splice event 5; Out ->
 *   RevokeCmPolicy(8) + event 6; store tns and tTns=g_clock.t
 * Info: proposed name SetTnTns (T1); TU tn
 * Notes: names_v2 C proposed FChooseRobRoc__FP3ROBP3ROH (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void SetTnTns(struct S *p, int k)
{
    if (k == p->k) return;
    switch (k) {
    case 1:
        SetCmPolicy(D_002DDF40, p->x128 + 2, D_002DDF40 + 0x3B0, p->pE0, p);
        p->vt[3].pfn((char *)p + p->vt[3].d, 5, 1, 0, 0, 0);
        break;
    case 0:
        RevokeCmPolicy(D_002DDF40, 8, -1, 0, 0, p);
        p->vt[3].pfn((char *)p + p->vt[3].d, 6, 1, 0, 0, 0);
        break;
    }
    p->k = k;
    p->t = D_002DDED0.t;
}
