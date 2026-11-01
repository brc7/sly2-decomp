typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x30]; qword q; };
struct E { int a; struct O *po; };
struct S { char pad[0x20]; struct E *ae; };
void func_00226F28(int a, int b, int c, qword *d, int e);
/** @sly2-annot addr=0x1EC6D0
 * Purpose: Variant of 0x1EC688 for a single qword from an object
 * Info: proposed name AddShdVifUnpackQw (T2); TU shd
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
int func_001EC6D0(struct S *p, int i, int x, int y)
{
    struct E *pe = p->ae + i; qword q = pe->po->q;
    func_00226F28(y, 12, 1, &q, x);
    return 1;
}
