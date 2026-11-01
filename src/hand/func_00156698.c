void func_00156548(int a, int b, int c, int *pn);
/** @sly2-annot addr=0x156698
 * Purpose: NewNoise(template, pos, arg, &pnoise) and return the new noise
 * Info: proposed name PnoiseNew (T2); TU noise
 * Notes: v3: superseded PnoiseEmit | top50_round (0.55): Body. Wrapper returning the created stimulus;
 *   callers EmitSwNoise, DetonateBomb, EmitStepguardStimulus, RefThunkCid117APPLY. Sly-style Pxxx
 *   constructor-returning name | v2 draft was PnoiseEmit
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r3b_rest+top50_round
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
int func_00156698(int a, int b, int c)
{
    int n;
    func_00156548(a, b, c, &n);
    return n;
}
