struct P { int a, b; };
struct L { unsigned int c; struct P ap[8]; };
/** @sly2-annot addr=0x148620
 * Purpose: Append {oid, x} to an ASH list (max 8)
 * Info: proposed name FAddAshOid (T1); TU button
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/gameplay-objects.md#source-files,
 *   docs/engine/object-model.md#8-source-files
 * @end-sly2-annot */
int FAddAshOid(struct L *l, int a, int b)
{
    if (l->c >= 8) return 0;
    l->ap[l->c].a = a;
    l->ap[l->c].b = b;
    l->c++;
    return 1;
}
