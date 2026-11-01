typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
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
struct LO;
typedef struct { short d; short pad; void (*pfn)(struct LO *, M3 *); } VT;
struct LO { int x0, x4, x8; VT *vt; };
void func_0011B628(M3 *pm, qword v);
/** @sly2-annot addr=0x1A63B0
 * Purpose: Degrees->radians (0.0174533), build M3 (0x11B628), vt30 RotateToMat
 * Info: proposed name SetLoEulerDeg (T2); TU lo
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A63B0(struct LO *p, qword v)
{
    M3 c;
    union { qword q; float f; } r;
    M3 m;
    r.q = Make(GetX(v) * 0.017453294f, GetY(v) * 0.017453294f, GetZ(v) * 0.017453294f);
    func_0011B628(&m, r.q);
    c = m;
    p->vt[30].pfn((struct LO *)((char *)p + p->vt[30].d), &c);
}
