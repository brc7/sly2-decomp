#include "m2c_prelude.h"
M2C_UNK func_00129EC8(void *);                      /* extern */
extern M2C_UNK D_002BDDF0;
extern f32 D_002BDE00;
extern M2C_UNK D_002BDE10;
extern s32 D_002BDE58;
extern M2C_UNK D_002BDE60;
extern M2C_UNK D_002BDE70;
extern M2C_UNK D_002CE4A0;
extern M2C_UNK D_002CE4B8;
extern M2C_UNK D_002DE720;
extern M2C_UNK D_002DE750;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_00452148;

/** @sly2-annot addr=0x258B38
 * Purpose: ensure EMITO kind 2, return &pemitb->emito (EMITTER copy) [game linkonce (inline/template)]
 * Info: proposed name PemitoEmitterEnsureKind2 (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
s32 func_00258B38(s32 arg0, s32 arg1) {
    if (arg1 == 1) {
        func_00258A00(arg0, 2);
    }
    return M2C_FIELD(arg0, s32 *, 0xE4) + 0x10;
}
