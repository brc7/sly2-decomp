typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
extern float D_003E1BE0;
struct G { char pad[0x24]; float g24; char pad2[0x60 - 0x28]; char mat[0x4A4 - 0x60]; int c4A4; };
extern struct G D_002DDF40;
struct A { char pad[0xA4]; float gA4; float gA8; int nAC; int nB0; float gB4; float gB8; float gBC; void *pC0; void *pC4; };
void func_0014FCC8(struct A *a, int n);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void func_00258040(void **pp, void *p);
void func_001FB018(void *p);
void DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(void *pmat, float *pa, float *pb);
void func_0014F610(struct G *pg, float g);
void func_00150EF0(struct A *a);
/** @sly2-annot addr=0x150938
 * Purpose: Zoom-cam enter: zoom sounds 0x26/0x25, pan/tilt from current camera mat, fov range, near clip 49,
 *   SnapCpzoom, shares cEnter counter (g_cm+0x4A4)
 * Info: proposed name ActivateCpzoom (T1); TU cplcy
 * Notes: runtime6 C7: CPZOOM activated as the recon camera (JTMODE kind 1, S57)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00150938(struct A *a, int n)
{
    void *p;
    struct G *pg;
    func_0014FCC8(a, n);
    p = func_001FBB90(0x26);
    func_001FB2F8(p, 0);
    func_00258040(&a->pC0, p);
    p = func_001FBB90(0x25);
    func_001FB2F8(p, 0);
    func_00258040(&a->pC4, p);
    func_001FB018(a->pC0);
    __asm__ __volatile__("" ::: "$6"); /* a2 is dead here: delays the a2 setup like the original */
    pg = &D_002DDF40;
    DecomposeRotateMatrixPanTilt__FP7MATRIX3PfT1(pg->mat, &a->gA4, &a->gA8);
    a->gA8 = 0;
    a->nAC = 0;
    a->nB0 = 0;
    a->gB8 = pg->g24 / a->gB4;
    a->gBC = pg->g24;
    func_0014F610(pg, 49.0f);
    func_00150EF0(a);
    pg->c4A4++;
    D_003E1BE0 = D_002DDED0.t;
}
