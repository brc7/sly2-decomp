#include "m2c_prelude.h"
M2C_UNK InitSo();                            /* extern */
M2C_UNK func_001F7918(s32, M2C_UNK, M2C_UNK);       /* extern */

/** @sly2-annot addr=0x1BAB50
 * Purpose: CID 98 vt7: InitSo, zap responses (SetSoZapResponse 0x3FFFFFF->8, 0x80001->0)
 * Info: proposed name InitCid98 (T2); class CID98 slot7; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BAB50(s32 arg0) {
    InitSo();
    func_001F7918(arg0, 0x03FFFFFF, 8);
    func_001F7918(arg0, 0x80001, 0);
}
