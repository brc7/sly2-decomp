#include "m2c_prelude.h"
M2C_UNK func_001A3968(s32);                         /* extern */
M2C_UNK func_002312A0();                            /* extern */

/** @sly2-annot addr=0x1A3550
 * Purpose: LIGHT vt29: XFM TranslateToPos then RebuildLight
 * Info: proposed name TranslateLightToPos (T1); class LIGHT slot29; TU light
 * Notes: names_v2 C proposed OnLightAdd__FP5LIGHT (0.638)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3550(s32 arg0) {
    func_002312A0();
    func_001A3968(arg0);
}
