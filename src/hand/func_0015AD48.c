typedef int qword __attribute__((mode(TI)));
struct M4 { qword r[4]; };
struct R { char pad[0x40]; struct M4 m; };
struct BUF { qword a[4]; };
struct VTE { short d; short pad; struct R *(*pfn)(void *, struct BUF *); };
struct SO { char pad[0xC]; struct VTE *vt; };
struct Y { int x0, x4, k; };
struct E { int x0, x4; struct SO **ppso; int xC; struct Y y; };
void func_0011C168(struct M4 *pm, struct M4 *pa, struct BUF *pb);
qword func_001A72D0(struct M4 *pm, int z, qword v);
qword func_001A7348(struct M4 *pm, int z, qword v);
qword func_0022D348(struct R *r, int z, qword v);
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }

/** @sly2-annot addr=0x15AD48
 * Purpose: Transforms a chosen particle position (and velocity, depending on mode) from the reference SO
 *   space to world space
 * Info: proposed name ConvertEmitoPosVec (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources:
 *   r3_1539A8+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015AD48(struct E *p, qword *pv, qword *pw)
{
    struct Y *py = &p->y;
    struct SO *pso = *p->ppso;
    struct BUF buf;
    struct M4 m;
    struct R *r;
    if (!pso) return;
    r = pso->vt[32].pfn((char *)pso + pso->vt[32].d, &buf);
    func_0011C168(&m, &r->m, &buf);
    *pv = func_001A72D0(&m, 0, *pv);
    if (py->k != 3) {
        *pw = func_001A7348(&m, 0, *pw);
        if (py->k != 2)
            *pw = Add(*pw, func_0022D348(r, 0, py->k == 1 ? *pv : r->m.r[3]));
    }
}
