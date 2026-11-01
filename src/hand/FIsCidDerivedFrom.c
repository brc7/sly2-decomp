struct E { struct E *next; int x; int pad[3]; };
extern struct E D_002BAAE8[];
/** @sly2-annot addr=0x15E988
 * Purpose: Walks the class-descriptor parent chain from cid (D_002BAAE8 records) and returns 1 if it reaches
 *   cidAncestor
 * Info: proposed name FIsCidDerivedFrom (T1); TU find
 * Notes: names_v2 v1 proposed FUN_0015c1c0 (0.000)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
int FIsCidDerivedFrom(int i, int x)
{
    struct E *p;
    for (p = &D_002BAAE8[i]; p; p = p->next)
        if (p->x == x)
            return 1;
    return 0;
}
