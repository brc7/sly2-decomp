typedef unsigned long ulong;
struct C { char pad[0x10]; ulong fl; };
struct E { char pad[0xC]; int fl; char pad2[0x14]; struct C *pc; };
struct A { char pad[0xB8]; int c; int pad2; struct E *ae; };
/** @sly2-annot addr=0x1F6A68
 * Purpose: Hide every glob and its linked part
 * Info: proposed name HideSoAllGlobs (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6A68(struct A *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->fl |= 0x800;
        if (pe->pc)
            pe->pc->fl |= 1L << 50;
    }
}
