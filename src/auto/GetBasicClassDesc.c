#include "m2c_prelude.h"
extern M2C_UNK D_002BAAE8;

/** @sly2-annot addr=0x113A10
 * Purpose: vtable slot 6 of BASIC (cid 0): returns &D_002BAAE8[0], the class descriptor {pcdParent, cid,
 *   cidEnd, cb, pfnNew}
 * Info: proposed name GetBasicClassDesc (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/engine/object-model.md#1-class-descriptors-and-vtables,
 *   docs/engine/object-model.md#devtools-findings-v3-2026-11-01, docs/guides/devtools.md#symbols_export
 * @end-sly2-annot */
M2C_UNK *GetBasicClassDesc(void) {
    return &D_002BAAE8;
}
