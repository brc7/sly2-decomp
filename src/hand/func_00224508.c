struct Q { char pad[0x10]; struct Q *next; };
extern int D_002C42E8[4];
int FIsBasicDerivedFrom__FP5BASIC3CID(struct Q *q, int a);
/** @sly2-annot addr=0x224508
 * Purpose: returns 0 if any node in list has one of 4 flags D_002C42E8
 * Info: proposed name FCid100ChainFree (T2); class CID100; TU tu_223F70
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
int func_00224508(void *p, struct Q *q)
{
    unsigned i;
    for (; q; q = q->next) {
        for (i = 0; i < 4; i++)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(q, D_002C42E8[i])) return 0;
    }
    return 1;
}
