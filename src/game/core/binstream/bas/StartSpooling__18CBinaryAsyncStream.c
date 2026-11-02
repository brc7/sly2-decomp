#include "m2c_prelude.h"
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
extern M2C_UNK D_0027A10C;
extern M2C_UNK D_0027A1D8;
extern M2C_UNK D_002BAAE8;
extern f32 D_002DDED0;
extern M2C_UNK D_003DD490;
extern f32 *D_003E0E80;
extern M2C_UNK D_003E1144;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x132710
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/formats/disc-records.md#2-the-read-path,
 *   docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
void StartSpooling__18CBinaryAsyncStream(s32 arg0) {
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_a3;
    s32 temp_s1;
    s32 temp_v1;
    u32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, u32 *, 0x14);
    temp_s1 = (s32) (temp_v0 + (temp_v0 >> 0x1F)) >> 1;
    if ((FSpooling__18CBinaryAsyncStream(arg0) == 0) && (temp_a1 = M2C_FIELD(arg0, s32 *, 0x28), (temp_a1 != 0))) {
        temp_a2 = M2C_FIELD(arg0, s32 *, 0x18);
        temp_a3 = (temp_a1 < temp_s1) ? temp_a1 : temp_s1;
        temp_v1 = M2C_FIELD(arg0, s32 *, 0);
        temp_a2_2 = ((M2C_FIELD(arg0, s32 *, 0x20) ^ temp_a2) == 0) ? (temp_a2 + temp_s1) : temp_a2;
        M2C_FIELD(arg0, s32 *, 0xC) = temp_a3;
        M2C_FIELD(arg0, s32 *, 0x10) = temp_a2_2;
        switch (temp_v1) {                          /* irregular */
        case 0:
            sceRead(M2C_FIELD(arg0, s32 *, 4), temp_a2_2, temp_a3);
            return;
        case 1:
            ReadCdDirect(M2C_FIELD(arg0, s32 *, 8), (s32) ((u32) (temp_a3 + 0x7FF) >> 0xB), temp_a2_2, 1);
            break;
        }
    }
}
