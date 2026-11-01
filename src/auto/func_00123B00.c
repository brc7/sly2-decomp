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

/** @sly2-annot addr=0x123B00
 * Purpose: CRef(BASIC*) of D_003E0FA0
 * Info: proposed name RefOpFindCurrentTask (T1); TU bif
 * Notes: v3: superseded RefOpFindPlayerObject1 | splice builtin 101 find-player-object-1~ ->
 *   find-current-task~ (0.80): Body CRef of g_sw+0x3B10 (current TASK, FIsCurrentTask 0x164018). Scripts:
 *   ((geto (find-player-object-1~) 'RequestTaskState:1104) 2 self), (eqv? (find-player-ob
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r1_116918+top50_round(splice builtin 101)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 func_00123B00(s32 arg0) {
    func_001294C0(arg0, D_003E0FA0);
    return arg0;
}
