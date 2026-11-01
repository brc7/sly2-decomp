typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
struct S { char pad[0x94]; int k; };
void *func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x22CC00
 * Purpose: memset then switch on kind
 * Info: TU xf
 * Notes: descriptive only: xf: derivative matrix by kind | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
float *func_0022CC00(float *m, struct S *p, qword v)
{
    func_002333B4(m, 0, 0x40);
    switch (p->k) {
    case 0: case 15: break;
    case 1: case 4: case 5: case 9: case 10:
        __asm__("sw %1,%0" : "=m"(m[12]) : "r"(v));
        m[13] = GetY(v);
        m[14] = GetZ(v);
        break;
    case 11:
        __asm__("sw %1,%0" : "=m"(m[4]) : "r"(v));
        m[8] = GetY(v);
        m[9] = GetZ(v);
        break;
    case 12:
        __asm__("sw %1,%0" : "=m"(m[0]) : "r"(v));
        m[5] = GetY(v);
        m[10] = GetZ(v);
        break;
    }
    return m;
}
