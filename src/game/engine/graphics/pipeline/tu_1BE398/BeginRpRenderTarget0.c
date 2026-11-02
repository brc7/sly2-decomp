#include "m2c_prelude.h"
M2C_UNK BeginRpGlob(s32, s32, s32);               /* extern */
M2C_UNK func_001BE6F0(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E1A78;

/** @sly2-annot addr=0x1BE990
 * Purpose: Pass 8 begin (kind 15): set up render-to-texture slot 0 (target packet + slot camera) then
 *   BeginRpGlob
 * Info: proposed name BeginRpRenderTarget0 (T1); TU tu_1BE398
 * Notes: superseded: DrawItemRenderSlot0 (r5b_rest)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void BeginRpRenderTarget0(s32 arg0, s32 arg1, s32 arg2) {
    func_001BE6F0(&D_003E1A78, arg0);
    BeginRpGlob(arg0, arg1, arg2);
}
