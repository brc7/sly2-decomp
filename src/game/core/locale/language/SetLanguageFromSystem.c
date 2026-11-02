void GetRegionLanguages(int *pn, int **pa);
void SetLanguage(int i);
/** @sly2-annot addr=0x1A2DA0
 * Purpose: Map the PS2 system language (sceScf code: 0 ja,1 en,2 fr,3 es,4 de,5 it,6 nl,7 pt) to a game
 *   language index and select it if the region offers it, else region default
 * Info: proposed name SetLanguageFromSystem (T1); TU language
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#language-0x1a2bd0-0x1a3020
 * @end-sly2-annot */
void SetLanguageFromSystem(int key)
{
    int tbl[14] = { 1, 2, 5, 4, 3, 6, 7, -1, -1, -1, -1, 0, -1, -1 };
    int i, j;
    int n;
    int *a;
    for (i = 0; i < 14; i++) {
        if (key == tbl[i]) {
            GetRegionLanguages(&n, &a);
            for (j = 0; j < n; j++) {
                if (i == a[j]) {
                    SetLanguage(i);
                    return;
                }
            }
        }
    }
    SetLanguage(-1);
}
