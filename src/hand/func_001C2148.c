typedef int qword __attribute__((mode(TI)));
struct E { qword q; int pad[4]; };
struct S { char pad[0x50]; int c; char pad2[0x1c]; struct E a[6]; };
/** @sly2-annot addr=0x1C2148
 * Purpose: Append fixed endpoint vector, max 6
 * Info: proposed name AddPathlinkEndpointPos (T2); TU path
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C2148(struct S *p, qword q)
{
    if ((unsigned)p->c < 6) { struct E *pe = &p->a[p->c++]; pe->q = q; }
}
