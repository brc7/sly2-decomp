typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *a, void *b);
/** @sly2-annot addr=0x184208
 * Purpose: Out PosLoWorld of the owner
 * Info: proposed name GetCid35GrabPos (T2); class CID35; TU grabspot
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00184208(void *a, void *b, qword *p)
{
    if (p) *p = func_001A62E8(a, b);
}
