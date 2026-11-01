struct TFT { int fLeft; int x4; int x8; int xC; int fZero; int fPrec0; int cchWidth; int cchPrec; int cbits; char pad24[8]; unsigned char chConv; };
class OSTRM {
public:
    int CbWrite(void *pv, int cb);
};
extern "C" int func_0021ED28(char *pch);
extern "C" char D_002A9370[];
static inline void WritePad(OSTRM *postrm, char ch, int c)
{
    char b = ch;
    while (c-- > 0)
        if (postrm->CbWrite(&b, 1) != 1)
            break;
}
/** @sly2-annot addr=0x21D9F0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
void WriteTftPchz(TFT *ptft, OSTRM *postrm, char *pchz)
{
    int cch;
    int cchLeft;
    int cchRight;
    char ch;
    if (!pchz)
        pchz = D_002A9370;
    cch = func_0021ED28(pchz);
    cchRight = 0;
    cchLeft = 0;
    if (ptft->cchPrec && ptft->cchPrec < cch)
        cch = ptft->cchPrec;
    else if (ptft->fPrec0)
        cch = 0;
    if (cch < ptft->cchWidth) {
        ch = ptft->fZero ? '0' : ' ';
        int cchPad = ptft->cchWidth - cch;
        if (ptft->fLeft)
            cchRight = cchPad;
        else
            cchLeft = cchPad;
    }
    WritePad(postrm, ch, cchLeft);
    postrm->CbWrite(pchz, cch);
    WritePad(postrm, ' ', cchRight);
}
