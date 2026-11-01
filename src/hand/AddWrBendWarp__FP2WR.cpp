typedef int qword __attribute__((mode(TI)));
struct WRE { int wrek; int pad[3]; qword a; qword b; int pad2[2]; float c; char pad3[0x4C]; float d; char pad4[0x44]; };
struct WR { char pad[0x50]; float f50; int cwre; int c58; int pad5; WRE awre[8]; char pad6[0x6E0 - 0x60 - 0xD0 * 8]; int c6E0; };
extern qword D_002A2DB0, D_002A2DC0;
/** @sly2-annot addr=0x22C430
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void AddWrBendWarp(WR *pwr)
{
    WRE *pwre = &pwr->awre[pwr->cwre++];
    pwr->c6E0++;
    pwre->wrek = 1;
    pwre->a = D_002A2DB0;
    pwre->b = D_002A2DC0;
    pwre->c = 0.5f;
    pwre->d = 0.5f;
    pwr->c58++;
}
