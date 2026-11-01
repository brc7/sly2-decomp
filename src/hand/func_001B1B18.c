struct E { int a; int b; int c; };
struct P { char pad[0x17C]; struct E ae[1]; };
int func_001B1B10(int n);
/** @sly2-annot addr=0x1B1B18
 * Purpose: Set link slot n (not 2/5) id and optional value
 * Info: proposed name SetCid96Link (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B1B18(struct P *p, int n, int a, int c)
{
    if (func_001B1B10(n) && n != 2 && n != 5) {
        struct E *pe = &p->ae[n];
        pe->a = a;
        if (c != -1)
            pe->c = c;
    }
}
