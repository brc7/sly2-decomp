#include "m2c_prelude.h"
/** @sly2-annot addr=0x128138
 * Purpose: Constructor of the global GC object (zeroes first field)
 * Info: proposed name CGc_ctor (T1); class CGc; TU gc
 * Notes: mangled dropped (base does not match final name): __3CGc
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#3-the-evaluator, docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#gc-0x128138-0x128b60
 * @end-sly2-annot */
s32 *func_00128138(s32 *arg0) {
    *arg0 = 0;
    return arg0;
}
