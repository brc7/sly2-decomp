struct E { int id; int a, b; };
struct X { char pad[0x1180]; struct E ae[3]; };
extern int D_002C3258[];
/** @sly2-annot addr=0x141EA8
 * Purpose: Button mask (D_002C3258) of every slot holding gadget id
 * Info: proposed name GrfbtnGadget (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int GrfbtnGadget(struct X *p, int id)
{
    int i;
    int m = 0;
    for (i = 0; i < 3; i++) {
        if (id == p->ae[i].id) m |= D_002C3258[i];
    }
    return m;
}
