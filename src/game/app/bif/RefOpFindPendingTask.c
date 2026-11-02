#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, M2C_UNK *);              /* extern */
M2C_UNK Decref__4CRef(M2C_UNK *);                   /* extern */
M2C_UNK SetTag__4CRef4TAGK(M2C_UNK *, M2C_UNK);          /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F9C;
extern s32 D_003E0FA0;
extern s32 D_003E0FA4;
extern M2C_UNK D_004520A8;

/** @sly2-annot addr=0x123B30
 * Purpose: CRef(BASIC*) of D_003E0FA4
 * Info: proposed name RefOpFindPendingTask (T1); TU bif
 * Notes: v3: superseded RefOpFindPlayerObject2 | splice builtin 102 find-player-object-2~ ->
 *   find-pending-task~ (0.65): Body CRef of g_sw+0x3B14 (pending task, RestartJob 0x162DF8 prefers it).
 *   Scripts compare it with a task and call RequestTaskState on it
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 102)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
s32 RefOpFindPendingTask(s32 arg0) {
    CRef_ctor_BASIC(arg0, D_003E0FA4);
    return arg0;
}
