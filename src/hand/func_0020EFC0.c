struct S { char pad[0x3D78]; int c; };
void func_00187078(int n);
void func_0020F128(struct S *p);
void func_0020F198(struct S *p);
/** @sly2-annot addr=0x20EFC0
 * Purpose: ++SW+0x3D78 (first -> AddGrfusr(2)); grf&1 AcquireSwLetterbox, grf&2 ++count 0x3D84
 * Info: proposed name IncrementSwHandsOff (T1); TU sw
 * Notes: runtime5b/runtime6: hands-off counter SW+0x3D78 = 1 in pause, cutscene and job banner
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources: r7_1F8DD8+verify_side+runtime
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_0020EFC0(struct S *p, int fl)
{
    if (++p->c == 1) func_00187078(2);
    if (fl & 1) func_0020F128(p);
    if (fl & 2) func_0020F198(p);
}
