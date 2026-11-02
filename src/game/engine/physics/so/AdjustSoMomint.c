typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { qword r[3]; } M3;
struct T { M3 m; qword v[3]; };
struct X { char pad[0xE8]; struct T *pt; };
void func_0023320C(void *d, const void *s, unsigned int cb);
void *func_002333B4(void *d, int c, unsigned int cb);
int FInvertMatrix3(struct T *pt, qword *pv);
/** @sly2-annot addr=0x1F35F8
 * Purpose: Scale SOPHYS inertia matrix by r and recompute its inverse (zero if singular)
 * Info: proposed name AdjustSoMomint (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AdjustSoMomint(struct X *p, float s)
{
    struct T *pt = p->pt;
    M3 m;
    U u;
    M3 r;
    __asm__("qmtc2.ni %1, %0" : "=j"(u.q) : "r"(s));
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r.r[0]) : "j"(u.q), "j"(pt->m.r[0]));
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r.r[1]) : "j"(u.q), "j"(pt->m.r[1]));
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r.r[2]) : "j"(u.q), "j"(pt->m.r[2]));
    func_0023320C(&m, &r, sizeof(M3));
    pt->m = m;
    if (!FInvertMatrix3(p->pt, p->pt->v))
        func_002333B4(p->pt->v, 0, 0x30);
}
