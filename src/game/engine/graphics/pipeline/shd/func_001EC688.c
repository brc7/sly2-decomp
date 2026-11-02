struct E { int a; int b; };
struct S { char pad[0x20]; struct E *ae; };
void func_00226F28(int a, int b, int c, int d, int e);
/** @sly2-annot addr=0x1EC688
 * Purpose: Emit a VIF unpack (0x226F28 kind 12) for record i; return its size
 * Info: proposed name AddShdVifUnpack (T2); TU shd
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
int func_001EC688(struct S *p, int i, int x, int y)
{
    struct E *pe = &p->ae[i];
    func_00226F28(y, 12, pe->a, pe->b, x);
    return pe->a;
}
