/** @sly2-annot addr=0x152628
 * Purpose: Map oid 0x626/0x627/0x628 -> 0/1/2 (else -1)
 * Info: proposed name IGradeFromOid (T2); TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
int func_00152628(int k)
{
    switch (k) {
    case 0x626: return 0;
    case 0x627: return 1;
    case 0x628: return 2;
    }
    return -1;
}
