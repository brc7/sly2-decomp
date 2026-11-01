struct E { char pad[0x94]; int b; int pad2; int a; char pad4[0x60]; };
struct S { char pad[0x50]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F4C58
 * Purpose: Find XF in SO XF array (+0x50/+0x54, 0x100 stride) matching (+0x9C, +0x94) ids
 * Info: proposed name PxfFindSo (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
struct E *func_001F4C58(struct S *p, int a, int b)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->a == a && pe->b == b) return pe; }
    return 0;
}
