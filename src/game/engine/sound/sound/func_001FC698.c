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

/** @sly2-annot addr=0x1FC698
 * Purpose: vt10: RPC 'SUKR'(0x524B5553) cmd 2 for stream id
 * Info: proposed name StopStreamsnd (T2); class STREAMSND; TU sound
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FC698(s32 arg0) {
    func_00233088(0x524B5553, 2, M2C_FIELD(arg0, s32 *, 0x40), 0, M2C_ERROR(/* Read from unset register $a4 */), M2C_ERROR(/* Read from unset register $a5 */), M2C_ERROR(/* Read from unset register $a6 */));
}
