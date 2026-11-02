#include "qw.h"
struct CRITSECT { int a; };
struct GSDB { int a; };
extern CRITSECT D_002DFB40;
extern int D_002DEDB0;
extern GSDB D_002DEBD0, D_002DECC0;
extern int D_002DE598;
extern "C" {
void func_0021F060(CRITSECT *p);
void func_0021F0C0(CRITSECT *p);
void func_00234CA0(int n);
void func_002406C0(GSDB *p);
void func_00241728(int ch, QW *aqw);
void func_002417F0(int ch, int a, int b);
}
/** @sly2-annot addr=0x170278
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name BlastAqwGifsBothFrames__FP2QW (EXISTING); TU gs
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void BlastAqwGifsBothFrames(QW *aqw)
{
    func_0021F060(&D_002DFB40);
    func_00234CA0(0);
    for (int i = 0; i < 2; i++)
    {
        func_002406C0(((i + D_002DEDB0) & 1) ? &D_002DEBD0 : &D_002DECC0);
        func_00241728(D_002DE598, aqw);
    }
    func_002417F0(D_002DE598, 0, 0);
    func_0021F0C0(&D_002DFB40);
}
