typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int n[4]; } VU;
typedef struct { VU r[3]; } M3;
typedef struct { VU r[4]; } M4;
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
struct S { M4 m; char pad[0x94 - 0x40]; int k; };
void *func_0023320C(void *d, void *s, int n);
M3 *func_0011B628(M3 *pm, qword v);
VU *func_00256D18(VU *pv, qword q);
/** @sly2-annot addr=0x22C8C0
 * Purpose: switch on kind: rotation/position override applied to matrix
 * Info: proposed name ApplyXfOverride (T2); TU xf
 * Notes: names_v2 C proposed ApplyAloProxy__FP3ALOP5PROXY (0.438)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
M4 *func_0022C8C0(M4 *pm, struct S *p, qword v)
{
    switch (p->k) {
    case 0: case 8: case 13: case 14: case 15:
        break;
    case 1: case 4: case 5: case 9: case 10:
        func_0023320C(pm, p, 0x40);
        __asm__("sw %1,%0" : "=m"(pm->r[3].f[0]) : "r"(v));
        pm->r[3].f[1] = GetY(v);
        pm->r[3].f[2] = GetZ(v);
        break;
    case 2: case 3:
        func_0023320C(pm, p, 0x40);
        break;
    case 6: case 7: {
        M3 m;
        func_0011B628(&m, v);
        func_00256D18(&pm->r[3], p->m.r[3].q);
        pm->r[0]=m.r[0];
        pm->r[1]=m.r[1];
        pm->r[2]=m.r[2];
        pm->r[3].f[3] = 1.0f;
        pm->r[2].n[3] = 0;
        pm->r[1].n[3] = 0;
        pm->r[0].n[3] = 0;
        break;
    }
    case 11:
        func_0023320C(pm, p, 0x40);
        __asm__("sw %1,%0" : "=m"(pm->r[1].f[0]) : "r"(v));
        pm->r[2].f[0] = GetY(v);
        pm->r[2].f[1] = GetZ(v);
        break;
    case 12:
        func_0023320C(pm, p, 0x40);
        __asm__("sw %1,%0" : "=m"(pm->r[0].f[0]) : "r"(v));
        pm->r[1].f[1] = GetY(v);
        pm->r[2].f[2] = GetZ(v);
        break;
    }
    return pm;
}
