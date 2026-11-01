int func_0015E678(void *a, int b, int c, int d, int e, int f, int *pn);
/** @sly2-annot addr=0x15E958
 * Purpose: PloFindSwObject overload with the extra Sly 2 parameter: CploFindSwObjects(grffso|0x200, ..., 1,
 *   &plo)
 * Info: proposed name PloFindSwObject_15E958 (T1); TU find
 * Notes: names_v2 v1 proposed FUN_0015c188 (0.000) | COLLISION: PloFindSwObject already used (same name
 *   proposed for 0x15E938); renamed to PloFindSwObject_15E958
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/object-model.md#find-0x15e380-0x15ef00,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
int PloFindSwObject_15E958(void *a, int b, int c, int d, int e)
{
    int n;
    if (!func_0015E678(a, b | 0x200, c, d, e, 1, &n))
        return 0;
    return n;
}
