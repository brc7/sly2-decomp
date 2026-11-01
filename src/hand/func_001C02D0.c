struct T { char pad[0xE8]; int ne8; char pad2[8]; int nf4; };
int func_001BFF28(void *p, int a, int b, int c, int d);
void func_001E6180(struct T *pt);
/** @sly2-annot addr=0x1C02D0
 * Purpose: CposFindPath on pathzone+0xE0 (max 16 points) into a path/curve object and rebuild it (0x1E6180);
 *   false if <2 points
 * Info: proposed name FFindPathzonePathCurve (T2); TU path
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920,
 *   docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
int func_001C02D0(char *p, int a, int b, struct T *pt)
{
    int n = func_001BFF28(p + 0xE0, a, b, 0x10, pt->nf4);
    if (n < 2) return 0;
    pt->ne8 = n;
    func_001E6180(pt);
    return 1;
}
