#include "m2c_prelude.h"
M2C_UNK func_0017F090();                            /* extern */
M2C_UNK func_0017F3E0(void *, M2C_UNK, s32);        /* extern */
extern void *D_002DFB80;

/** @sly2-annot addr=0x17F6F8
 * Purpose: CID 0x48 vt7: InitCid47, 5 entries, state -1, register singleton D_002DFB80 = this, sound -1
 * Info: proposed name InitHackship (T1); class HACKSHIP slot7; TU tu_17F090
 * Notes: names_v2 v1 proposed UseStepguardAnimation__FP9STEPGUARD3SGS3OID (0.027) | Cid48->Cid72 (CID
 *   0x48=72, vtable) | class identity from whole_game WE names/object strings: InitCid72 -> InitHackship
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F6F8(void *arg0) {
    func_0017F090();
    func_0017F3E0(arg0, 5, arg0 + 0x1C8);
    M2C_FIELD(arg0, s32 *, 0x1C0) = -1;
    D_002DFB80 = arg0;
    M2C_FIELD(arg0, s32 *, 0x2A8) = -1;
}
