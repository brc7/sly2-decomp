#include "m2c_prelude.h"
/** @sly2-annot addr=0x1A6DC0
 * Purpose: LO vt46: grfvis(byte x2D)=b
 * Info: proposed name SetLoVisMask (T1); class LO slot46; TU lo
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6DC0(void *arg0, s8 arg1) {
    M2C_FIELD(arg0, s8 *, 0x2D) = arg1;
}
