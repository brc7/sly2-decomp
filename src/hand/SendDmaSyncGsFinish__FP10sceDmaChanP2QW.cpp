struct CRITSECT;
struct sceDmaChan;
union QW;
extern CRITSECT D_002DFB40;
extern int D_002DFB54;
extern int D_002DFB50;
void EnterCritSect(CRITSECT *pcs);
void LeaveCritSect(CRITSECT *pcs);
extern "C" int func_00240B68();
extern "C" void func_00240AA0(int n);
extern "C" void func_00234CA0(int n);
extern "C" void func_00241728(sceDmaChan *pch, QW *pqw);
extern "C" void func_00234A60(int sid);
/** @sly2-annot addr=0x16F8B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void SendDmaSyncGsFinish(sceDmaChan *pch, QW *pqw)
{
    EnterCritSect(&D_002DFB40);
    D_002DFB54 = 1;
    *(volatile long *)0x12001000 |= 2;
    func_00240AA0(func_00240B68() & ~0x200);
    func_00234CA0(0);
    func_00241728(pch, pqw);
    func_00234A60(D_002DFB50);
    LeaveCritSect(&D_002DFB40);
}
