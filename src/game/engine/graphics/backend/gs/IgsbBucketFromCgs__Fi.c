/** @sly2-annot addr=0x170448
 * Purpose: Size-class index (0..7, -1 for 0) of a free GS-memory run of n units:
 *   1,2,3-4,5-8,9-16,17-32,33-64,>64
 * Info: proposed name IgsbBucketFromCgs (T1); TU gs
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
int IgsbBucketFromCgs__Fi(int n)
{
    if (n >= 9) {
        if (n >= 0x21) {
            if (n < 0x41) return 6;
            return 7;
        }
        if (n < 0x11) return 4;
        return 5;
    }
    if (n >= 3) {
        if (n < 5) return 2;
        return 3;
    }
    if (n == 0) return -1;
    return n >= 2;
}
