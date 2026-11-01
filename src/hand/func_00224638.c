struct S { char pad[0x150]; int a; char pad2[8]; int k; };
/** @sly2-annot addr=0x224638
 * Purpose: state table predicate (4 with flag; 5,7,9,11,13,26,37,41)
 * Info: proposed name FCid100StateBlocks (T2); class CID100; TU tu_223F70
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
int func_00224638(struct S *p)
{
    switch (p->k) {
    case 4: return p->a == 1;
    default: return 0;
    case 5: case 7: case 9: case 11: case 13: case 26: case 37: case 41: return 1;
    }
}
