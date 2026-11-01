#include "m2c_prelude.h"
M2C_UNK func_001A3968(s32);                         /* extern */
M2C_UNK func_001A5C48();                            /* extern */

/** @sly2-annot addr=0x1A3400
 * Purpose: LIGHT vt15: PostLoLoad then RebuildLight
 * Info: proposed name PostLightLoad (T1); class LIGHT slot15; TU light
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3400(s32 arg0) {
    func_001A5C48();
    func_001A3968(arg0);
}
