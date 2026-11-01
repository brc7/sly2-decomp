struct TFT { char pad[0x20]; int cbits; char pad24[8]; unsigned char chConv; };
union UL { unsigned short us; short s; unsigned int u; int i; unsigned long ul; long l; struct { unsigned int lo, hi; } w; };
/** @sly2-annot addr=0x21DB38
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int FNegTftUl(TFT *ptft, UL *pul)
{
    int fNeg = 0;
    switch (ptft->cbits) {
    case 0:
        pul->w.hi = 0;
        break;
    case 1:
        pul->ul = pul->us;
        break;
    case 2:
        break;
    }
    if (ptft->chConv == 'd' || ptft->chConv == 'i') {
        switch (ptft->cbits) {
        case 0:
            if (pul->i < 0) {
                pul->i = -pul->i;
                fNeg = 1;
            }
            break;
        case 1:
            if (pul->s < 0) {
                pul->s = -pul->s;
                fNeg = 1;
            }
            break;
        case 2:
            if (pul->l < 0) {
                pul->l = -pul->l;
                fNeg = 1;
            }
            break;
        }
    }
    return fNeg;
}
