void LoadVolFromBrx(void *p);
void LoadBtn(void *p, void *q);
/** @sly2-annot addr=0x149358
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name LoadVolbtnFromBrx (T1); class VOLBTN slot14; TU button
 * Notes: names_v2 C hint not used: LoadVolbtnFromBrx__FP6VOLBTNP18CBinaryInputStream | CID24 vtable slot 14;
 *   body = LoadVolFromBrx only (r0: CID24 likely VOLBTN; names_v2 C hint LoadVolbtnFromBrx) | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels | v3 class identity:
 *   LoadCid24FromBrx -> LoadVolbtnFromBrx
 * Confidence: name mid [T1 0.75]; purpose not reviewed - evidence: static naming pass
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void LoadVolbtnFromBrx(char *p)
{
    LoadVolFromBrx(p);
    LoadBtn(p + 0x130, p);
    if (*(int *)(p + 0x2C8)) *(int *)(p + 0x1D4) = 1;
}
