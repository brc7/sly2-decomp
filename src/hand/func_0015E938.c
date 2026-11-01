int func_0015E958(void *a, int b, int c, int d, int e);
/** @sly2-annot addr=0x15E938
 * Purpose: Returns the first LO matching grffso/oid relative to a context LO (or NULL)
 * Info: proposed name PloFindSwObject (T1); TU find
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/object-model.md#6-finding-objects, docs/engine/object-model.md#9-key-functions,
 *   docs/engine/overview.md#6-subsystem-map
 * @end-sly2-annot */
int func_0015E938(void *a, int b, int c, int d)
{
    return func_0015E958(a, b, c, d, 0);
}
