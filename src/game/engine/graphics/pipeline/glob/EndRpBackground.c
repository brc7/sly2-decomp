extern int D_003DCFA0;
extern char D_002DF010[];
void EndRpGlob(void *p);
void AddVifDirectFlushed(void *p, int n, void *pv);
/** @sly2-annot addr=0x16E380
 * Purpose: Pass 3 end: EndRpGlob + Z-only clear (D_002DF010) when per-frame uploads were made (textures
 *   parked in Z-buffer memory)
 * Info: proposed name EndRpBackground (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void EndRpBackground(void *p)
{
    EndRpGlob(p);
    if (D_003DCFA0 > 0)
        AddVifDirectFlushed(p, 0x25, D_002DF010);
}
