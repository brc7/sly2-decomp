struct E { int a; int b; };
extern unsigned int D_002E1E58;
extern struct E D_002BFBD0[];
/** @sly2-annot addr=0x1A2C48
 * Purpose: Out count and pointer to the language index list of the current region (index clamped to <7)
 * Info: proposed name GetRegionLanguages (T1); TU language
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#language-0x1a2bd0-0x1a3020
 * @end-sly2-annot */
void GetRegionLanguages(int *pa, int *pb)
{
    struct E *pe;
    if (D_002E1E58 >= 7) D_002E1E58 = 0;
    pe = &D_002BFBD0[D_002E1E58];
    if (pa) *pa = pe->a;
    if (pb) *pb = pe->b;
}
