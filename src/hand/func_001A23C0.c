struct T { int x0; };
struct S { int x0; int x4; int pad[2]; struct T *x10; char pad2[0x7C]; int x90; };
/** @sly2-annot addr=0x1A23C0
 * Purpose: 0 none, 1 hurt, 2 dead (health<=0) while in a hurt state; else x90==1
 * Info: proposed name JtHurtStatus (T2); TU hurt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
int func_001A23C0(struct S *p)
{
    if (p->x4 != -1) return p->x10->x0 <= 0 ? 2 : 1;
    return p->x90 == 1;
}
