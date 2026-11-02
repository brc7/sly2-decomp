typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
extern char D_002DDF40[];
struct A { int n0; int pad[3]; void *pb; int n14; float g18; };
void ActivateCplcy(struct A *a, int n, void *b);
void SetCmNearClip__FP2CMf(void *pg, float g);
void SetCmFov__FP2CMf(void *pg, float g);
int func_0012DC18(int n, void *pb, int a, int b, int c, qword q);
/** @sly2-annot addr=0x151240
 * Purpose: Store pv (+0x10), near clip 25, fov 1.15, +0x14 = 0x12DC18(owner, pv, 1, 8, 3, 0), tNext=-1
 * Info: proposed name ActivateCphang (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void ActivateCphang(struct A *a, int n, void *b)
{
    void *pg;
    ActivateCplcy(a, n, b);
    a->pb = b;
    pg = D_002DDF40;
    SetCmNearClip__FP2CMf(pg, 25.0f);
    SetCmFov__FP2CMf(pg, 1.15f);
    a->n14 = func_0012DC18(a->n0, a->pb, 1, 8, 3, D_002A2DD0);
    a->g18 = -1.0f;
}
