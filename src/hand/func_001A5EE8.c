typedef int qword __attribute__((mode(TI)));
typedef struct { qword q[4]; } M4;
struct VTE { short d; short pad; void *pfn; };
struct O { int x0, x4, x8; struct VTE *vt; };
void func_0011C318(M4 *out, void *in);
void func_0011C168(M4 *out, void *a, M4 *b);
/** @sly2-annot addr=0x1A5EE8
 * Purpose: Draw an LO hierarchy at an explicit world matrix: m * inverse(PmatWorld()) then RenderAll (vt20)
 *   with that matrix and two alpha/show factors
 * Info: proposed name RenderLoAtMat (T2); TU lo
 * Notes: v3: superseded SetLoMatWorldRel | top50_round (0.50): Body: vt31 PmatWorld, InvertMatrix4Rigid,
 *   MultiplyMatrix4Matrix4, vt20(&m,0,a,b). SO slot 20 is RenderSoAll. All callers are Render*Self functions
 *   drawing child SOs at computed matrices (RenderWaypt draws marker/base wit
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r4_17F090+top50_round
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A5EE8(struct O *o, void *m, float a, float b)
{
    M4 t;
    M4 r;
    M4 inv;
    M4 *pr = &r;
    M4 *pinv = &inv;
    struct VTE *e = &o->vt[0xF8 / 8];
    func_0011C318(pinv, ((void *(*)(void *))e->pfn)((char *)o + e->d));
    func_0011C168(pr, m, pinv);
    t = r;
    e = &o->vt[0xA0 / 8];
    ((void (*)(void *, M4 *, int, float, float))e->pfn)((char *)o + e->d, &t, 0, a, b);
}
