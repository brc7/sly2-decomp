#include "vifs.h"
#include "gifs.h"
struct FR {
    int cqwGifs;     /* 0x00 */
    QW *aqwGifs;     /* 0x04 */
    QW *aqwGifsBuf;  /* 0x08 */
    int cqwGifsBuf;  /* 0x0c */
    int cqwVifs;     /* 0x10 */
    QW *aqwVifs;     /* 0x14 */
    QW *aqwVifsBuf;  /* 0x18 */
    int cqwVifsBuf;  /* 0x1c */
    int cqw20;       /* 0x20 */
    QW *aqw24;       /* 0x24 */
    int n28;         /* 0x28 */
    int n2C;         /* 0x2c */
    int n30;         /* 0x30 */
    int adl[6];      /* 0x34 */
    int n4C;         /* 0x4c */
};
extern FR *D_002DEA4C;
extern FR D_002DEA50[2];
extern unsigned int D_002DEB10;
extern VIFS D_002DEA28;
extern GIFS D_002DEA00;
extern int D_003D3980;
extern int D_002DEB1C;
extern int D_002DDF54;
extern QW D_002DF740[];
extern "C" void SetPhase__F5PHASE(int phase);
extern "C" void func_00234A60(int sid);
extern "C" void ClearPendingFrame__FP3FRM(FR *pfr);
/** @sly2-annot addr=0x160828
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name OpenFrame__Fv (EXISTING); TU frm
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order, docs/engine/rendering.md#1-frame-structure,
 *   docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
void OpenFrame()
{
    if (!D_003D3980)
        SetPhase__F5PHASE(3);
    func_00234A60(D_002DEB1C);
    D_002DEA4C = &D_002DEA50[D_002DEB10++];
    if (D_002DEB10 >= 2)
        D_002DEB10 = 0;
    ClearPendingFrame__FP3FRM(D_002DEA4C);
    D_002DEA4C->n4C = D_002DDF54;
    D_002DEA4C->n30 = 1;
    D_002DEA4C->n2C = 0;
    D_002DEA4C->aqw24 = 0;
    D_002DEA00.AllocStatic(D_002DEA4C->cqwGifsBuf, D_002DEA4C->aqwGifsBuf);
    D_002DEA28.AllocStatic(D_002DEA4C->cqwVifsBuf, D_002DEA4C->aqwVifsBuf);
    D_002DEA00.AddDmaRefs(12, D_002DF740);
    D_002DEA00.AddDmaCnt();
}
