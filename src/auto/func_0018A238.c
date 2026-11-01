#include "m2c_prelude.h"
M2C_UNK func_001425B0(void *);                      /* extern */
M2C_UNK func_00190D10();                            /* extern */

/** @sly2-annot addr=0x18A238
 * Purpose: JT vt19: if jts==3 render extra (0x190D10) then STEPPLAYER render 0x1425B0
 * Info: proposed name RenderJtSelf (T1); class JT slot19; TU jt
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018A238(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x18C8) == 3) {
        func_00190D10();
    }
    func_001425B0(arg0);
}
