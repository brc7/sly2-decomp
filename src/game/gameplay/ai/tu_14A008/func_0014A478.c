typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *p);
int func_00173610(qword *pq, int a, int b, float f, float g);
/** @sly2-annot addr=0x14A478
 * Purpose: true if guard not in air and no ground found within 100/-250
 * Info: proposed name FBhv182OffGround (T2); TU tu_14A008
 * Notes: verifier better_name (CORRECT row): Returns 0 if owner+0x208 set, else
 *   !FSnapToGround(PosLoWorld(owner),1,0,100,-250); sole caller UpdateBhv182. Drop the '?'. '+0x208 = in air'
 *   not independently c
 * Confidence: low (draft) [T2 0.85] - evidence: semantic review; sources: r2b_rest+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
int func_0014A478(char *p)
{
    char *q = *(char **)(p + 0x10);
    qword t;
    if (*(int *)(q + 0x208)) return 0;
    t = func_001A62E8(q);
    return func_00173610(&t, 1, 0, 100.0f, -250.0f) == 0;
}
