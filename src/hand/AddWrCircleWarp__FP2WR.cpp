typedef int qword __attribute__((mode(TI)));
struct WRE { int wrek; int i; int pad[2]; qword a; qword b; char pad2[0x30]; int k; char pad4[0x6C]; };
struct WR { char pad[0x50]; float f50; int cwre; int c58; int i5C; WRE awre[8]; char pad6[0x6E0 - 0x60 - 0xD0 * 8]; int c6E0; };
extern qword D_002A2DA0;
/** @sly2-annot addr=0x22C3B0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void AddWrCircleWarp(WR *pwr)
{
    WRE *pwre = &pwr->awre[pwr->cwre++];
    pwr->c6E0++;
    pwre->wrek = 0;
    pwre->a = D_002A2DA0;
    if (pwr->i5C >= 0) {
        pwre->i = pwr->i5C;
        pwr->i5C = -1;
        pwre->k = 2;
    } else {
        pwre->i = pwr->c58++;
        pwr->i5C = pwre->i;
        pwre->k = 0;
    }
}
