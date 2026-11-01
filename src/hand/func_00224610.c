struct S { char pad[0x15C]; int x; };
/** @sly2-annot addr=0x224610
 * Purpose: True if the gadget id (+0x15C) is 7 (Trigger Bomb) or 11 (Health Extractor)
 * Info: proposed name FCid100ThrowableGadget (T2); class CID100; TU tu_223F70
 * Notes: verifier PLAUSIBLE: FCid100Held? (state 7 or 11) -> FCid100State7Or11 | v3: superseded
 *   FCid100State7Or11 | runtime5a #16 CORRECTION: reads +0x15C = gadget id (runtime bomb +0x15C = 7), not the
 *   state +0x150; v2 FCid100State7Or11 | T2: placeholder token Cid100
 * Confidence: low (draft) [T2 0.70] - evidence: runtime (save-state RAM); sources:
 *   r8_21BB88+verdicts_tu_r7r8+runtime5a
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/player.md#7-bentley-bentleycpp-0x134648,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
int func_00224610(struct S *p)
{
    int x = p->x; if (x != 7 && x != 11) return 0; return 1;
}
