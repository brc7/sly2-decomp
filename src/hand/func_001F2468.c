struct R { int pad[2]; int c; };
struct L { char pad[0x24]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0x134 - 0x74]; struct R *pr; };
struct Pool;
extern struct Pool D_003E12D4;
void func_0011CD98(struct Pool *, struct R *);
int func_00132B10(struct L *, int);
/** @sly2-annot addr=0x1F2468
 * Purpose: Drop the shared bounds record (+0x134, refcounted, pool D_003E12D4) of this SO and all SO children
 * Info: proposed name ReleaseSoBoundsHierarchy (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2468(struct L *p)
{
    struct L *q;
    struct R *pr = p->pr;
    if (pr) {
        if (--pr->c == 0) func_0011CD98(&D_003E12D4, pr);
        p->pr = 0;
    }
    for (q = p->first; q != p->end; q = q->next) {
        if (func_00132B10(q, 0x30)) func_001F2468(q);
    }
}
