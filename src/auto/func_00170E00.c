#include "m2c_prelude.h"
M2C_UNK func_0011A070(s32, void *);                 /* extern */
void *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_003E1548;

/** @sly2-annot addr=0x170E00
 * Purpose: Allocate a script command node from pool D_003E1548 (PvAllocSlotheapClear), set kind (+0) and arg
 *   (+4), append to the CID113 script list (+0x50), return it
 * Info: proposed name PcmdAddCid113 (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex)) | v3: superseded PcmdAppendCid113 | top50_round
 *   (0.55): Body. All 16 callers are AddCid113Cmd* builders (Context, Teleport, Message, Wait, Goto, LookAt,
 *   Anim, BusyOn/Off, EnsureRelayAlarm). Sly-style analogue of PrelaycmdAddRelay (relay TU). Class still a
 *   placeholder (CID113 | T2: placeholder token Cid113
 * Confidence: low (draft) [T2 0.55] - evidence: semantic review; sources: r3b_rest+top50_round
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void *func_00170E00(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_v0;

    temp_v0 = PvAllocSlotheapClear(&D_003E1548);
    M2C_FIELD(temp_v0, s32 *, 0) = arg1;
    M2C_FIELD(temp_v0, s32 *, 4) = arg2;
    func_0011A070(arg0 + 0x50, temp_v0);
    return temp_v0;
}
