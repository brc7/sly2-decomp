#include "m2c_prelude.h"
s32 *func_001FD028(M2C_UNK *, s32);                 /* extern */
extern M2C_UNK D_003DD040;

/** @sly2-annot addr=0x1FBC20
 * Purpose: vt8: kind of this sfx from SNDMGR table (2 = looping) or -1
 * Info: proposed name GetBanksndKind (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
s32 func_001FBC20(void *arg0) {
    s32 *temp_v0;
    s32 var_v1;

    temp_v0 = func_001FD028(&D_003DD040, M2C_FIELD(arg0, s32 *, 0x60));
    var_v1 = -1;
    if (temp_v0 != NULL) {
        var_v1 = *temp_v0;
    }
    return var_v1;
}
