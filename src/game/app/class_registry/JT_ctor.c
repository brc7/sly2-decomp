#include "m2c_prelude.h"
M2C_UNK __4CRef(void *);                            /* extern */
extern M2C_UNK D_002D2E98;
extern M2C_UNK D_002D9978;

/** @sly2-annot addr=0x2563E8
 * Purpose: inline constructor (vtable 0x2D2E98) [game linkonce (inline/template)]
 * Info: proposed name JT_ctor (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void *JT_ctor(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 *, 4) = 0;
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002D9978;
    __4CRef(arg0 + 0x80);
    M2C_FIELD(arg0, s32 *, 0x174) = 0;
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002D2E98;
    M2C_FIELD(arg0, s32 *, 0x2044) = 0;
    M2C_FIELD(arg0, s32 *, 0x2048) = 0;
    return arg0;
}
