typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x4020]; int a; int b; };
void func_00156698(int n, qword q, int c);
/** @sly2-annot addr=0x156B98
 * Purpose: Emit a noise from one of the two SW noise templates (k=0 -> SW+0x4020, 1 -> +0x4024) at pos
 * Info: proposed name EmitSwNoise (T1); TU noise
 * Notes: top50_round (0.60): Body; Splice thunk RefThunk_EmitSwNoise 0x10BFF8 exposes it; guard/player
 *   message handlers call it
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r3b_rest+top50_round
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050,
 *   docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void EmitSwNoise(struct S *p, int k, qword q, int c)
{
    int n;
    switch (k) {
    case 0: n = p->a; break;
    case 1: n = p->b; break;
    }
    func_00156698(n, q, c);
}
