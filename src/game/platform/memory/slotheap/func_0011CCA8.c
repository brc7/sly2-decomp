#include "m2c_prelude.h"
/** @sly2-annot addr=0x11CCA8
 * Purpose: Initialises a fixed-size slot pool {cb, c, nKind/align, pvFree=NULL}; storage allocated lazily on
 *   first alloc
 * Info: proposed name InitSlotheap (T1); TU slotheap
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/core-library.md#1-memory, docs/engine/core-library.md#6-source-files,
 *   docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60
 * @end-sly2-annot */
void *func_0011CCA8(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = arg2;
    M2C_FIELD(arg0, s32 *, 8) = arg3;
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    return arg0;
}
