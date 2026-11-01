extern float D_002A7874;
struct V { float x, y, z; };
struct S { char p0[0x70]; struct V v70; int pad; int n80; float g84; char p1[8]; struct V v90; int pad2; int nA0; float gA4; char p2[8]; float gB0; };
void func_001A5270(struct S *p);
static inline void SetVector(struct V *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
/** @sly2-annot addr=0x1D6A90
 * Purpose: LO-subclass Init: InitLo then offset (0,0,0), up (0,1,0), coeffs 0.5 and D_002A7874
 * Info: proposed name InitLauncher (T2); class CID112 slot7; TU tu_1D6848
 * Notes: names_v2 C proposed LimitVectorLength__FP6VECTORfT0 (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#tu_1d6848-0x1d6848-0x1d7238
 * @end-sly2-annot */
void func_001D6A90(struct S *p) {
    func_001A5270(p);
    SetVector(&p->v70, 0, 0, 0);
    p->n80 = 0;
    p->g84 = D_002A7874;
    SetVector(&p->v90, 0, 1.0f, 0);
    p->gB0 = 0.5f;
    p->gA4 = D_002A7874;
    p->nA0 = 0;
}
