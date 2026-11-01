typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x14]; int n14; };
struct P { char pad[0x20]; void *p20; void *p24; };
struct R *func_001F8910(struct P *, int, int, int);
void func_0012D108(struct R *, qword);
void func_0012DC18(void *, void *, int, int, int, qword);
/** @sly2-annot addr=0x1F89E0
 * Purpose: Update/create kind-1 actuator with value n,m,q
 * Info: proposed name SetXfhActuator1 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F89E0(struct P *p, int n, int m, qword q)
{
    struct R *r = func_001F8910(p, 1, n, m);
    if (r) {
        r->n14 = n;
        func_0012D108(r, q);
    } else if (n != -1) {
        func_0012DC18(p->p20, p->p24, m, n, 2, q);
    }
}
