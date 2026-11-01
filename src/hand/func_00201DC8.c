extern char D_003DD490[];
struct S { char pad[0x30]; int n30; int a34[16]; char pad2[0xA4 - 0x74]; int nA4; int *pA8; int nAC; int nB0; };
int func_00132B10(int a, int b);
int func_0015E938(void *p, int a, int b, int c);
int func_0015E9C8(void *p, int a, int b, int c, int d, int *e);
void func_00201E88(struct S *p);
/** @sly2-annot addr=0x201DC8
 * Purpose: find widgets 0x362/0x365 (JT only) and LO 0x602; collect all CID 0x2E targets (max 16)
 * Info: proposed name PostReconSelLoad (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void func_00201DC8(struct S *p)
{
    if (func_00132B10(*p->pA8, 0x35)) {
        p->nAC = func_0015E938(D_003DD490, 0x405, 0x362, 0);
        p->nB0 = func_0015E938(D_003DD490, 0x405, 0x365, 0);
    }
    p->nA4 = func_0015E938(D_003DD490, 0x405, 0x602, 0);
    p->n30 = func_0015E9C8(D_003DD490, 0x405, 0x2E, 0, 16, p->a34);
    func_00201E88(p);
}
