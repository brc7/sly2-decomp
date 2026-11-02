typedef int qword __attribute__((mode(TI)));
struct S { int pad[8]; qword q20; qword q30; };
qword PosCpzoomEye(struct S *p);
int func_0011D4D0(float a, float b, float eps);
static __inline__ float GetX(qword q)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q));
    return x;
}
static __inline__ float GetY(qword q)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(q));
    return y;
}
static __inline__ float GetZ(qword q)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q));
    return z;
}
static __inline__ int FNear(float a, float b) { return func_0011D4D0(a, b, 0.0001f); }
/** @sly2-annot addr=0x151018
 * Purpose: Smoothed eye (+0x30) == PosCpzoomEye within 1e-4
 * Info: proposed name FCpzoomEyeArrived (T2); TU cplcy
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
int func_00151018(struct S *p)
{
    int f = 0;
    qword a = p->q30;
    qword b = PosCpzoomEye(p);
    if (FNear(GetX(a), GetX(b)))
        if (FNear(GetY(a), GetY(b)))
            if (FNear(GetZ(a), GetZ(b)))
                f = 1;
    return f;
}
