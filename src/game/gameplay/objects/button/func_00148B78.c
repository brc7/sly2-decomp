extern char D_002DEB30[];
void func_00161F10(void *a, void *b, int n, void *c, int d, int e);
/** @sly2-annot addr=0x148B78
 * Purpose: Register (oid, x) in a 4-entry list via 0x161F10 on D_002DEB30
 * Info: proposed name AddBtnEventListener (T2); TU button
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148B78(char *p, int d, int e)
{
    func_00161F10(D_002DEB30, p + 0x138, 4, p + 0x134, d, e);
}
