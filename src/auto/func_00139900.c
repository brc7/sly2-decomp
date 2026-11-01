#include "m2c_prelude.h"
s32 func_00132B10(void *, M2C_UNK);                 /* extern */
M2C_UNK func_001333C0();                            /* extern */

/** @sly2-annot addr=0x139900
 * Purpose: BLIPG vt51: SubscribeLoObject, and if the target is an EMITTER-family (cid 0x1D) remember it
 *   (+0xE0) and copy its +0x38
 * Info: proposed name SubscribeBlipgObject (T1); class BLIPG slot51; TU blip
 * Notes: names_v2 C proposed UpdateBlipg__FP5BLIPGf (0.638)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_00139900(void *arg0, void *arg1) {
    func_001333C0();
    if (func_00132B10(arg1, 0x1D) != 0) {
        M2C_FIELD(arg0, void **, 0xE0) = arg1;
        M2C_FIELD(arg0, f32 *, 0x38) = (f32) M2C_FIELD(arg1, f32 *, 0x38);
    }
}
