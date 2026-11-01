typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
extern void *func_001A6318(void);
extern qword func_0011BFF0(void *p);
/** @sly2-annot addr=0x1A6340
 * Purpose: Decompose PmatLoWorld into Euler angles (0x11BFF0) and convert rad->deg (57.2958)
 * Info: proposed name GetLoEulerDeg (T2); TU lo
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
qword func_001A6340(void)
{
    union { qword q; float f; } r;
    qword v = func_0011BFF0(func_001A6318());
    r.q = Make(GetX(v) * 57.2957763671875f, GetY(v) * 57.2957763671875f, GetZ(v) * 57.2957763671875f);
    return r.q;
}
