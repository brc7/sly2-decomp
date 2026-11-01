int func_0015E678(void *a, int b, int c, int d, int e, int f, int g);
/** @sly2-annot addr=0x15E650
 * Purpose: Finds up to cploMax LOs with an oid relative to a context object (6-arg form; forwards with extra
 *   arg 0)
 * Info: proposed name CploFindSwObjects_15E650 (T1); TU find
 * Notes: COLLISION: CploFindSwObjects already used (same name proposed for 0x15E678); renamed to
 *   CploFindSwObjects_15E650
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
int func_0015E650(void *a, int b, int c, int d, int f, int g)
{
    return func_0015E678(a, b, c, d, 0, f, g);
}
