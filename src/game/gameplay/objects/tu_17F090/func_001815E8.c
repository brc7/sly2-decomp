#include "m2c_prelude.h"
M2C_UNK BroadcastGameEvent(M2C_UNK *, M2C_UNK, void *);  /* extern */
M2C_UNK func_0017F4C0(void *, void *);              /* extern */
M2C_UNK func_0017FD08(void *, M2C_UNK);             /* extern */
M2C_UNK func_00181728();                            /* extern */
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;

/** @sly2-annot addr=0x1815E8
 * Purpose: State switch; drives the CID 0x48 singleton (SetCid48State) and game events
 * Info: proposed name SetHacklockState (T2); class HACKLOCK; TU tu_17F090
 * Notes: Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class identity from whole_game WE names/object strings:
 *   SetCid75State -> SetHacklockState
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001815E8(void *arg0, s32 arg1) {
    s32 temp_v1;
    void *var_a1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x1C0);
    if (temp_v1 != arg1) {
        if ((temp_v1 == 1) && (M2C_FIELD(arg0, void **, 0x1C8) != NULL)) {
            func_00181728();
        }
        M2C_FIELD(arg0, s32 *, 0x1C0) = arg1;
        M2C_FIELD(arg0, f32 *, 0x1C4) = (f32) D_002DDED0;
        switch (arg1) {                             /* irregular */
        case 1:
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x1C8), s32 *, 0x294) = (s32) M2C_FIELD(arg0, s32 *, 0x1CC);
            func_0017FD08(M2C_FIELD(arg0, void **, 0x1C8), 3);
            break;
        case 2:
            BroadcastGameEvent(&D_002DEB30, 0x39, arg0);
            break;
        }
        if (arg1 != -1) {
            var_a1 = arg0 + ((arg1 * 0x10) + 0x1D0);
        } else {
            var_a1 = NULL;
        }
        func_0017F4C0(arg0, var_a1);
    }
}
