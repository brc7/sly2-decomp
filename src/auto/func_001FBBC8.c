#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002D9BE0;
extern M2C_UNK D_002D9C70;
extern M2C_UNK D_002D9D00;
extern s32 D_002DD5BC;
extern M2C_UNK D_002DDFA0;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD08C;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1FBBC8
 * Purpose: BANKSND ctor: SND ctor, set start flag (vt D_002D9C70 store dropped by m2c)
 * Info: proposed name BANKSND_ctor (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
s32 func_001FBBC8(s32 arg0, s32 arg1) {
    func_001FAFD0(arg0);
    M2C_FIELD(arg0, s32 *, 0x60) = arg1;
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002D9C70;
    func_001FB658(arg0, 1, 1);
    return arg0;
}
