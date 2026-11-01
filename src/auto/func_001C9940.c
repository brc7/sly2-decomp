#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C9940
 * Purpose: (isrc<<16)|(icopy&0xFFFF)
 * Info: proposed name IdProxy (T1); TU proxy
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/object-model.md#8-source-files,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
s32 func_001C9940(s32 arg0, s32 arg1) {
    return (arg0 << 0x10) | (arg1 & 0xFFFF);
}
