int func_00137A30(void *p, int a, int b, void *q, int c, int d, int e, int *pn);
/** @sly2-annot addr=0x18F908
 * Purpose: Create the 3-entry dynamic swing ASEG blend (0x137A30 with cbBl 0xC, cbl 3) for the BL array and
 *   return it via out pointer
 * Info: proposed name PostJtLoadSwing (T1); TU hang
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#10-source-files, docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
void func_0018F908(void *p, void *q, int *pn)
{
    int n;
    func_00137A30(p, 0xC, 3, q, 0, 0, 0, &n);
    if (pn) *pn = n;
}
