#include "m2c_prelude.h"
s32 func_0011A380(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_003E1218;

/** @sly2-annot addr=0x131968
 * Purpose: True if the ASEGA is not on the live list D_003E1218
 * Info: proposed name FAsegaRetracted (T1); TU asega
 * Notes: names_v2 B proposed AdaptAsega__FP5ASEGA (0.856)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
s32 func_00131968(s32 arg0) {
    return func_0011A380(&D_003E1218, arg0) == 0;
}
