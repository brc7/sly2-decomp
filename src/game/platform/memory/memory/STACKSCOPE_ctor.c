#include "m2c_prelude.h"
M2C_UNK InitStackImpl__Fv();                        /* extern */

/** @sly2-annot addr=0x1AE478
 * Purpose: RAII scope ctor: InitStackImpl; returns this
 * Info: proposed name STACKSCOPE_ctor (T1); class STACKSCOPE; TU memory
 * Notes: agent qualifier: (ctor) | mangled dropped (base does not match final name): __10STACKSCOPE
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
s32 STACKSCOPE_ctor(s32 arg0) {
    InitStackImpl__Fv();
    return arg0;
}
