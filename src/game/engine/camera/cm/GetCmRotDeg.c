typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
qword func_0011BFF0(void *p);
/** @sly2-annot addr=0x14F478
 * Purpose: Option getter (g_aeopid[173]): euler angles of CM::mat (0x11BFF0) in degrees
 * Info: proposed name GetCmRotDeg (T1); TU cm
 * Notes: names_v2 v1 proposed SetCmPosMat__FP2CMP6VECTORP7MATRIX3 (0.167)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources:
 *   r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
qword GetCmRotDeg(char *p)
{
    union { qword q; float f; } r;
    qword v = func_0011BFF0(p + 0x60);
    r.q = Make(GetX(v) * 57.295776f, GetY(v) * 57.295776f, GetZ(v) * 57.295776f);
    return r.q;
}
