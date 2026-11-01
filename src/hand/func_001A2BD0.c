extern int D_002E1E54;
void func_001A2C48(int *pa, int **ppb);
/** @sly2-annot addr=0x1A2BD0
 * Purpose: Set current language D_002E1E54 = n, or for -1 the first language of the current region
 * Info: proposed name SetLanguage (T1); TU language
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen, docs/engine/hud-ui.md#4-source-files,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void func_001A2BD0(int n)
{
    if (n == -1) {
        int a;
        int *pb;
        func_001A2C48(&a, &pb);
        D_002E1E54 = *pb;
    } else {
        D_002E1E54 = n;
    }
}
