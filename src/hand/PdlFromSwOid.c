struct E { int a, b, c; };
/** @sly2-annot addr=0x15E380
 * Purpose: Returns the SW hash bucket DL for an oid: SW+0x38 table, index (oid*0x95675)&0x1FF
 * Info: proposed name PdlFromSwOid (T1); TU find
 * Notes: runtime: oid hash SW+0x38, bucket (oid*0x95675)&0x1FF confirmed (8146 objects)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r3_1539A8+runtime
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/object-model.md#8-source-files,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
struct E *PdlFromSwOid(char *p, int i)
{
    return &((struct E *)(p + 0x38))[(i * 0x95675) & 0x1FF];
}
