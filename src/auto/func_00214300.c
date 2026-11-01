#include "m2c_prelude.h"
M2C_UNK func_0011A220(M2C_UNK *, void *);           /* extern */
M2C_UNK func_0011A2E8(M2C_UNK *, void *);           /* extern */
s32 func_0011A380(M2C_UNK *, void *);               /* extern */
extern M2C_UNK D_003E1060;

/** @sly2-annot addr=0x214300
 * Purpose: flag +0xA8; value 0 = in exclusive list D_003E1060 (suppresses lower-priority widgets)
 * Info: proposed name SetWidgetExclusive (T2); TU hud
 * Notes: names_v2 v1 proposed UpdateAttract__FP7ATTRACT (0.024)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00214300(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0xA8);
    if (temp_v0 != arg1) {
        if ((temp_v0 == 0) && (func_0011A380(&D_003E1060, arg0) != 0)) {
            func_0011A2E8(&D_003E1060, arg0);
        }
        M2C_FIELD(arg0, s32 *, 0xA8) = arg1;
        if ((arg1 == 0) && (func_0011A380(&D_003E1060, arg0) == 0)) {
            func_0011A220(&D_003E1060, arg0);
        }
    }
}
