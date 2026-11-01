typedef int qword __attribute__((mode(TI)));
struct B { char pad[0xA0]; int n; };
struct A { char pad[0x24]; struct B *pb; };
qword func_0022FC48(int a, int n, qword q);
void func_001F89E0(struct A *p, int a, int b, qword q);
/** @sly2-annot addr=0x1F8A78
 * Purpose: 0x1F89E0 with vector converted from XF parent space
 * Info: proposed name SetXfhActuator1World (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8A78(struct A *p, int a, int b, qword q)
{
    func_001F89E0(p, a, b, func_0022FC48(0, p->pb->n, q));
}
