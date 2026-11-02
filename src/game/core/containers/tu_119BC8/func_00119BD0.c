#include "m2c_prelude.h"
M2C_UNK func_002564E0();                            /* extern */
M2C_UNK func_00256528(s32, s32, void *);            /* extern */
extern M2C_UNK func_00119BC8;

/** @sly2-annot addr=0x119BD0
 * Purpose: vtable 0x2CA5E0 slot 1: base init (0x2564E0) then sets up a keyed container at +0x34 with key fn
 *   0x119BC8
 * Info: proposed name CUnk2CA5E0_Init (T2); class CUnk2CA5E0; TU tu_119BC8
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#tu_119bc8-0x119bc8-0x119da0
 * @end-sly2-annot */
void func_00119BD0(s32 arg0) {
    func_002564E0();
    func_00256528(arg0 + 0x34, arg0, &func_00119BC8);
}
