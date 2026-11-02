typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct MAT { qword a[3]; };
struct Q { char pad[0xA0]; void *xA0; };
struct LO { char pad[0xC]; VTE *vt; int x10; char p1[0x58 - 0x14]; Q *x58; Q *x5C; };
struct S { char pad[0x84]; LO *x84; char p1[0x94 - 0x88]; int x94; int x98; int x9C; LO *xA0; };
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
extern float D_002C1EF4;
extern qword D_002A2DD0;
extern "C" {
void func_001CCAC0(S *p, int n);
void ReleaseAct(int *p);
void SetLoParent_1A58D0(LO *p, int a, int b);
qword func_001A62E8(LO *p);
qword func_0022FC48(int a, void *pv, qword q);
int func_0012DE58(LO *p, Q *q, int n, qword a, qword b, float g);
void *func_001A6318(LO *p);
int func_0012DC18(LO *p, Q *q, int a, int b, int c, qword d);
void func_001CD308(S *p, LO *po);
}
MAT ConvertXfMat(int a, void *pv, void *v);
qword func_0011BFF0(MAT m);
/** @sly2-annot addr=0x1CD308
 * Purpose: SetJtHandsTarget variant that parents the object and builds pos/rot acts
 * Info: proposed name SetJtHandsTargetAttached (T2); TU jthands
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CD308(S *p, LO *po)
{
    MAT mat;
    int unused[4];
    if (!p->xA0) return;
    func_001CCAC0(p, -1);
    ReleaseAct(&p->x94);
    ReleaseAct(&p->x98);
    ReleaseAct(&p->x9C);
    if (po->x10) {
        
        void *pv = ((void *(*)(void *, void *))po->vt[32].pf)((char *)po + po->vt[32].d, &mat);
        LO *pa = p->xA0;
        ((void (*)(void *, int, void *, int))pa->vt[10].pf)((char *)pa + pa->vt[10].d, po->x10, pv, 0xF);
    } else {
        SetLoParent_1A58D0(p->xA0, 0, 0xF);
    }
    Q *q = p->xA0->x58;
    p->x94 = func_0012DE58(p->xA0, q, 5, func_0022FC48(0, q->xA0, func_001A62E8(po)), D_002A2DD0, D_002DDED0.t + D_002C1EF4);
    q = p->xA0->x5C;
    mat = ConvertXfMat(0, q->xA0, func_001A6318(po));
    p->x9C = func_0012DC18(p->xA0, q, 5, 7, 2, func_0011BFF0(mat));
    p->x84 = p->xA0;
    func_001CCAC0(p, 1);
    p->xA0 = 0;
}
