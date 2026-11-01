#include "m2c_prelude.h"
void **PvAllocSlotheapClear(M2C_UNK *);                    /* extern */
extern M2C_UNK D_003E0E1C;

/** @sly2-annot addr=0x1A53A0
 * Purpose: If plo->ploroot(x14)==0 alloc a LOROOT from pool g_sw+0x398C (D_003E0E1C) and set root->plo=plo
 * Info: proposed name EnsureLoroot (T1); TU lo
 * Notes: names_v2 v1 proposed SetLightFallOff__FP5LIGHTP2LM (0.012)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void EnsureLoroot(void *arg0) {
    void **temp_v0;

    if (M2C_FIELD(arg0, void ***, 0x14) == NULL) {
        temp_v0 = PvAllocSlotheapClear(&D_003E0E1C);
        M2C_FIELD(arg0, void ***, 0x14) = temp_v0;
        *temp_v0 = arg0;
    }
}
