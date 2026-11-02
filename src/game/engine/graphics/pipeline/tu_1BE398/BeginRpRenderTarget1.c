#include "m2c_prelude.h"
M2C_UNK BeginRpGlob(s32, s32, s32);               /* extern */
M2C_UNK func_001BE6F0(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E1AA0;

/** @sly2-annot addr=0x1BE9E8
 * Purpose: Pass 9 begin (kind 18): same for slot 1
 * Info: proposed name BeginRpRenderTarget1 (T1); TU tu_1BE398
 * Notes: superseded: DrawItemRenderSlot1 (r5b_rest)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void BeginRpRenderTarget1(s32 arg0, s32 arg1, s32 arg2) {
    func_001BE6F0(&D_003E1AA0, arg0);
    BeginRpGlob(arg0, arg1, arg2);
}
