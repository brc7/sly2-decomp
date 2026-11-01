#include "m2c_prelude.h"
s32 PloNew__F3CIDP2SO3OIDi(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_003DD48C;

/** @sly2-annot addr=0x205E70
 * Purpose: Create the global guard-manager singleton (cid 0x7D, oid 0x656) into D_003DD48C once
 * Info: proposed name EnsureStepguardManager (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void EnsureStepguardManager(void) {
    if (D_003DD48C == 0) {
        D_003DD48C = PloNew__F3CIDP2SO3OIDi(0x7D, 0, 0x656, -1);
    }
}
