struct X { char pad[0x128]; int k; };
struct G { char pad[0x4774]; struct X *px; };
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *pg, int n);
void strcpy(void *p, void *q);
/** @sly2-annot addr=0x15DA88
 * Purpose: Text callback: string 0x77 or 0x78 depending on the parity of the widget's page
 * Info: proposed name SetEpisodeMenuPageText (T2); TU tu_15CD68
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void func_0015DA88(void *p, int f)
{
    struct X *px = D_003DD490.px;
    int k;
    if (!px)
        return;
    if (f)
        k = px->k - 1;
    else
        k = px->k;
    switch (k) {
    case 1: case 3: case 5: case 7: case 9: case 11: case 13: case 15:
        strcpy(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x77));
        break;
    case 2: case 4: case 6: case 8: case 10: case 12: case 14: case 16:
        strcpy(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x78));
        break;
    }
}
