extern char D_002E6550[], D_003DD490[];
int func_001ACC80(void *p);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *strcpy(char *, const char *);
void strcpy1(char *a, const char *b);
/** @sly2-annot addr=0x1AD820
 * Purpose: Dynamic text provider (0x2F3) depending on dialog state
 * Info: proposed name TextMcDialog (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD820(char *buf)
{
    switch (func_001ACC80(D_002E6550)) {
    case 1:
        strcpy(buf, "&2S&.: ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x38));
        break;
    case 2:
        strcpy(buf, "&2S&. : ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x36));
        strcpy1(buf, "       ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x37));
        strcpy1(buf, " : &2T&.");
        break;
    }
}
