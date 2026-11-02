#include "m2c_prelude.h"
void *PvAllocSlotheapClear(M2C_UNK *);                     /* extern */
extern M2C_UNK D_003E1144;

/** @sly2-annot addr=0x1CF578
 * Purpose: Push {plo} subscription node (pool D_003E1144) onto rip->(+0xE8) list
 * Info: proposed name SubscribeRipObject (T1); TU rip
 * Notes: names_v2 v1 proposed InitDablet__FP6DABLETP6VECTORfP2SO (0.079)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001CF578(void *arg0, s32 arg1) {
    void *temp_v0;

    temp_v0 = PvAllocSlotheapClear(&D_003E1144);
    M2C_FIELD(temp_v0, s32 *, 0) = arg1;
    M2C_FIELD(temp_v0, void **, 8) = (void *) M2C_FIELD(arg0, void **, 0xE8);
    M2C_FIELD(arg0, void **, 0xE8) = temp_v0;
}
