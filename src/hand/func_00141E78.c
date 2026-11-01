struct E { int id; int a, b; };
struct X { char pad[0x1180]; struct E ae[3]; };
/** @sly2-annot addr=0x141E78
 * Purpose: Index (0..2) of the gadget slot holding id, else -1
 * Info: proposed name IGadgetSlotFromId (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_00141E78(struct X *p, int id)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (id == p->ae[i].id) return i;
    }
    return -1;
}
