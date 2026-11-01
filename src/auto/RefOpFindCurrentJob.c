#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, M2C_UNK *);              /* extern */
M2C_UNK func_00129EC8(M2C_UNK *);                   /* extern */
M2C_UNK func_0012A0B0(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x123AD0
 * Purpose: CRef(BASIC*) of g_sw slot D_003E0F9C
 * Info: proposed name RefOpFindCurrentJob (T1); TU bif
 * Notes: v3: superseded RefOpFindPlayerObject0 | splice builtin 100 find-player-object-0~ ->
 *   find-current-job~ (0.80): Body CRef(BASIC*) of g_sw+0x3B0C, documented current JOB slot (FIsCurrentJob
 *   0x1633E8 tests the same slot; NULL in free roam at runtime). Scripts call job metho
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r1_116918+top50_round(splice builtin 100)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpFindCurrentJob(s32 arg0) {
    CRef_ctor_BASIC(arg0, D_003E0F9C);
    return arg0;
}
