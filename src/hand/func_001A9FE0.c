struct O { char pad[0xAC]; struct O *next; };
/** @sly2-annot addr=0x1A9FE0
 * Purpose: Splice list b in front of *pp (walk to tail, link)
 * Info: proposed name AppendXpList (T2); TU mark
 * Notes: names_v2 v1 proposed InitMemcard__FP7MEMCARD (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001A9FE0(void *unused, struct O *b, struct O **pp)
{
    if (b) {
        struct O *last = b;
        while (last->next) last = last->next;
        last->next = *pp;
        *pp = b;
    }
}
