struct E { int a; int b; int c; };
struct S { char pad[0x17C]; struct E ae[6]; };
int func_001B1B98(struct S *p, int a);
/** @sly2-annot addr=0x1B1BF0
 * Purpose: Resolve the 6 link ids into object pointers
 * Info: proposed name ResolveCid96Links (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B1BF0(struct S *p)
{
    int i;
    struct E *pe;
    for (i = 0; i < 6; i++) {
        pe = &p->ae[i];
        if (pe->a != -1) {
            pe->b = func_001B1B98(p, pe->a);
            pe->a = -1;
        }
    }
}
