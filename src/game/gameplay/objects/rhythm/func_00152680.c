/** @sly2-annot addr=0x152680
 * Purpose: Map 0/1/2 -> oid 0x626/0x627/0x628
 * Info: proposed name OidFromIGrade (T2); TU rhythm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
int func_00152680(int k)
{
    switch (k) {
    case 0: return 0x626;
    case 1: return 0x627;
    case 2: return 0x628;
    }
    return -1;
}
