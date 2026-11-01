struct E { int a, b, c; };
/** @sly2-annot addr=0x15E3C0
 * Purpose: Returns a bucket of a second oid-hashed DL table in the SW (SW+0x1838, same hash)
 * Info: proposed name PdlFromSwPxrOid (T1); TU find
 * Notes: verifier PLAUSIBLE: -> PdlFromSwPxrOid | names_v2 C proposed PdlFromSwOid__FP2SW3OID (0.438)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
struct E *func_0015E3C0(char *p, int i)
{
    return &((struct E *)(p + 0x1838))[(i * 0x95675) & 0x1FF];
}
