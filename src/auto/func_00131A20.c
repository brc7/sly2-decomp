#include "m2c_prelude.h"
/** @sly2-annot addr=0x131A20
 * Purpose: +0x48 = g; effective speed +0x40 = speed*master*k
 * Info: proposed name SetAsegaMasterSpeed (T1); TU asega
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131A20(void *arg0, f32 fparg0) {
    M2C_FIELD(arg0, f32 *, 0x48) = fparg0;
    M2C_FIELD(arg0, f32 *, 0x40) = (f32) (M2C_FIELD(arg0, f32 *, 0x44) * fparg0 * M2C_FIELD(arg0, f32 *, 0x4C));
}
