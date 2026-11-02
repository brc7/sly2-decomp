#include "m2c_prelude.h"
M2C_UNK AddDmaCnt__4DMAS();                         /* extern */
M2C_UNK AddVifDirect__4VIFSiP2QWi(s32, s32, s32, M2C_UNK); /* extern */
M2C_UNK AddVifFlush__4VIFS(s32);                    /* extern */
M2C_UNK EndDmaCnt__4DMAS(s32);                      /* extern */

/** @sly2-annot addr=0x16E318
 * Purpose: CNT{FLUSH, DIRECT(cqw, static GIF packet), FLUSH}
 * Info: proposed name AddVifDirectFlushed (T1); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_0016E318(s32 arg0, s32 arg1, s32 arg2) {
    AddDmaCnt__4DMAS();
    AddVifFlush__4VIFS(arg0);
    AddVifDirect__4VIFSiP2QWi(arg0, arg1, arg2, 0);
    AddVifFlush__4VIFS(arg0);
    EndDmaCnt__4DMAS(arg0);
}
