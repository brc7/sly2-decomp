#include "m2c_prelude.h"
M2C_UNK FreeStackImpl__Fv();                        /* extern */
M2C_UNK __builtin_delete(s32);                      /* extern */

/** @sly2-annot addr=0x1AE4A0
 * Purpose: RAII scope dtor: FreeStackImpl; deletes if (flags&1)
 * Info: proposed name STACKSCOPE_ctor_1AE4A0 (T1); class STACKSCOPE; TU memory
 * Notes: agent qualifier: (dtor) | mangled dropped: _$_10STACKSCOPE | COLLISION: STACKSCOPE_ctor already used
 *   (same name proposed for 0x1AE478); renamed to STACKSCOPE_ctor_1AE4A0
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void func_001AE4A0(s32 arg0, s32 arg1) {
    FreeStackImpl__Fv();
    if (arg1 & 1) {
        __builtin_delete(arg0);
    }
}
