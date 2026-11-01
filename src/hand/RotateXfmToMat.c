// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { float x, y, z, w; } V4;
typedef struct { V4 r[3]; } M3 __attribute__((aligned(16)));
typedef struct { char pad[0x50]; int x50; char pad2[0xC]; M3 m; } S;
M3 ConvertXfMat(int, int, int); void func_002313F0(S *);
static inline void SetM(M3 *pm, M3 m)
{
    *pm = m;
    pm->r[0].w = 0;
    pm->r[1].w = 0;
    pm->r[2].w = 0;
}
/** @sly2-annot addr=0x231300
 * Purpose: store rotation rows (w=0), invalidate
 * Info: proposed name RotateXfmToMat (T1); class XFM slot30; TU xform
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
void RotateXfmToMat(S *p, int x)
{
    SetM(&p->m, ConvertXfMat(0, p->x50, x));
    func_002313F0(p);
}
