struct A { void *p0; int pad[3]; float g10; float g14; int pad2[7]; void *p34; void *p38; int pad3[4]; float g4C; float g50; };
extern char D_002DDF40[];
void StopSnd(void *p, float g);
void func_00258040(void **pp, int n);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void SetCmCut(void *p, float a, float b);
void DeactivateCplcy(struct A *a, int n, void *b);
static inline void Release(void **pp, float g)
{
    if (pp && *pp) {
        StopSnd(*pp, g);
        func_00258040(pp, 0);
    }
}
/** @sly2-annot addr=0x150610
 * Purpose: Stop/release zoom sounds, SetCmCut with pan+PI/-tilt (camera continues behind), then
 *   CPLCY::Deactivate
 * Info: proposed name DeactivateCplook (T1); TU cplcy
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void DeactivateCplook(struct A *a, int n, void *b)
{
    Release(&a->p34, 0.0f);
    Release(&a->p38, 0.5f);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(a->p0, 0x34))
        SetCmCut(D_002DDF40, a->g10 + 3.1415927f, -a->g14);
    else
        SetCmCut(D_002DDF40, a->g4C + 3.1415927f, -a->g50);
    DeactivateCplcy(a, n, b);
}
