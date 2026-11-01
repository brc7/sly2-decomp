typedef int qword __attribute__((mode(TI)));
typedef struct { qword q[4]; } M4;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct O { int x0, x4, x8; struct VTE *vt; };
void InvertMatrix4Rigid(M4 *out, void *in);
void __ml__FRC7MATRIX4T0(M4 *out, void *a, M4 *b);
/** @sly2-annot addr=0x1A71B0
 * Purpose: Same for a 4x4 matrix (vt31 + inverse 0x11C318)
 * Info: proposed name ConvertAloMat4 (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
M4 ConvertAloMat4(struct O *o1, struct O *o2, M4 *m)
{
    M4 t;
    M4 r;
    M4 inv;
    struct VTE *e;
    if (o1) {
        e = &o1->vt[0xF8 / 8];
        __ml__FRC7MATRIX4T0(&r, e->pfn((char *)o1 + e->d), m);
        t = r;
    } else {
        t = *m;
    }
    if (o2) {
        M4 *pinv = &inv;
        M4 *pr = &r;
        e = &o2->vt[0xF8 / 8];
        InvertMatrix4Rigid(pinv, e->pfn((char *)o2 + e->d));
        __ml__FRC7MATRIX4T0(pr, pinv, &t);
        t = r;
    }
    return t;
}
