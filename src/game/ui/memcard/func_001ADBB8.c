extern char D_003D9ED0[], D_003DD490[], D_002A6EB8[];
int IdPauseTopPage(void *p);
void *PchzFindSwLangstr__FP2SWi(void *p, int n);
void strcpy(void *s, void *q);
void strcpy1(void *s, void *q);
/** @sly2-annot addr=0x1ADBB8
 * Purpose: Dynamic text provider (0x5CE) with ' : &2T&.' format
 * Info: proposed name TextSaveTime (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ADBB8(void *s)
{
    switch (IdPauseTopPage(D_003D9ED0)) {
    case 0x19:
    case 0x1E:
    case 0x1F:
        strcpy(s, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x40));
        strcpy1(s, D_002A6EB8);
        break;
    }
}
