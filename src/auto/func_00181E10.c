#include "m2c_prelude.h"
M2C_UNK func_00181EB0(s32, void *, M2C_UNK);        /* extern */
extern f32 D_002BE884;
extern f32 D_002DDED0;

/** @sly2-annot addr=0x181E10
 * Purpose: Iterate segments and advance their states
 * Info: proposed name UpdateHackwallSegments (T2); class HACKWALL; TU tu_17F090
 * Notes: Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   UpdateCid76Segments -> UpdateHackwallSegments
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181E10(s32 arg0, void *arg1) {
    s32 temp_s1;

    do {
        temp_s1 = M2C_FIELD(arg1, s32 *, 0x24);
        if ((temp_s1 == 1) && ((D_002DDED0 - M2C_FIELD(arg1, f32 *, 0x28)) > D_002BE884)) {
            func_00181EB0(arg0, arg1, 0);
        }
    } while (M2C_FIELD(arg1, s32 *, 0x24) != temp_s1);
}
