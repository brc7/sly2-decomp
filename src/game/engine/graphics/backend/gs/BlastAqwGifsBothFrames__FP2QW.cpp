#include "qw.h"
struct CRITSECT { int a; };
struct GSDB { int a; };
extern CRITSECT D_002DFB40;
extern int D_002DEDB0;
extern GSDB D_002DEBD0, D_002DECC0;
extern int D_002DE598;
extern "C" {
void EnterCritSect__FP8CRITSECT(CRITSECT *p);
void LeaveCritSect__FP8CRITSECT(CRITSECT *p);
void func_00234CA0(int n);
void sceGsPutDrawEnv(GSDB *p);
void sceDmaSend(int ch, QW *aqw);
void sceDmaSync(int ch, int a, int b);
}
/** @sly2-annot addr=0x170278
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name BlastAqwGifsBothFrames__FP2QW (EXISTING); TU gs
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void BlastAqwGifsBothFrames(QW *aqw)
{
    EnterCritSect__FP8CRITSECT(&D_002DFB40);
    func_00234CA0(0);
    for (int i = 0; i < 2; i++)
    {
        sceGsPutDrawEnv(((i + D_002DEDB0) & 1) ? &D_002DEBD0 : &D_002DECC0);
        sceDmaSend(D_002DE598, aqw);
    }
    sceDmaSync(D_002DE598, 0, 0);
    LeaveCritSect__FP8CRITSECT(&D_002DFB40);
}
