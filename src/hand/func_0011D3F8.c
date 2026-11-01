int func_0011D3C0(void);

/** @sly2-annot addr=0x11D3F8
 * Purpose: Uniform int in [nLow, nHi]
 * Info: proposed name NRandInRange (T1); TU util
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
int func_0011D3F8(int nLow, int nHi)
{
    int nRand = func_0011D3C0();
    int nBase = nLow - 1;
    return nLow + nRand % (nHi - nBase);
}
