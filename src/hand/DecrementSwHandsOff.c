struct S { char pad[0x3D78]; int n; };
struct G;
extern struct G D_002DFC00;
void RemoveGrfusr__Fi(int n);
void func_00187E08(struct G *pg, int n);
void ReleaseSwLetterbox(struct S *p);
void func_0020F1A8(struct S *p);
/** @sly2-annot addr=0x20F030
 * Purpose: --SW+0x3D78 (last -> RemoveGrfusr(2), SetJoyBtnHandled); grf&1 ReleaseSwLetterbox, grf&2 --0x3D84
 * Info: proposed name DecrementSwHandsOff (T1); TU sw
 * Notes: names_v2 v1 proposed FUN_001417f0 (0.000) | runtime6: lock counters return to 0 after HandsOff(0,3)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime
 * See: docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void DecrementSwHandsOff(struct S *p, int grf)
{
    if (--p->n == 0) {
        RemoveGrfusr__Fi(2);
        func_00187E08(&D_002DFC00, 8);
    }
    if (grf & 1) ReleaseSwLetterbox(p);
    if (grf & 2) func_0020F1A8(p);
}
