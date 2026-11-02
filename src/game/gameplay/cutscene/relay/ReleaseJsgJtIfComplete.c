struct O { int n0; char pad[0xC]; int n10; };
struct S { char pad[0x40]; struct O *po; };
int FIsJsgCommandComplete__FP3JSGP8RELAYCMD(struct S *p, struct O *po);
void func_001E4EB8(struct S *p, int a, int b);
/** @sly2-annot addr=0x1E1D48
 * Purpose: If the command holding the character (prelaycmdJt +0x40) is complete (StepCmd1D only when its goal
 *   is -1) release it (SetJsgJt(p,0,0))
 * Info: proposed name ReleaseJsgJtIfComplete (T1); TU relay
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void ReleaseJsgJtIfComplete(struct S *p)
{
    if (p->po && FIsJsgCommandComplete__FP3JSGP8RELAYCMD(p, p->po)) {
        int f = 1;
        if (p->po->n0 == 0x1D) f = (p->po->n10 == -1);
        if (f) func_001E4EB8(p, 0, 0);
    }
}
