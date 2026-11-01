typedef int qword __attribute__((mode(TI)));
struct M3 { qword a[3]; };
struct O { char pad[0xA0]; int nA0; };
struct P { char pad[0x24]; struct O *po; };
void func_0022FD38(struct M3 *, int, int);
qword func_0011BFF0(struct M3 *);
void func_001F89E0(struct P *, int, int, qword);
/** @sly2-annot addr=0x1F8AD8
 * Purpose: 0x1F89E0 with value from matrix
 * Info: proposed name SetXfhActuator1Mat (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8AD8(struct P *p, int a, int b)
{
    struct M3 c, t;
    func_0022FD38(&t, 0, p->po->nA0);
    c = t;
    func_001F89E0(p, a, b, func_0011BFF0(&c));
}
