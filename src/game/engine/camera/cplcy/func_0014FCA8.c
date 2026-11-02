#include "m2c_prelude.h"
extern M2C_UNK D_002CD570;

/** @sly2-annot addr=0x14FCA8
 * Purpose: CPLCY base constructor: vptr(+0xC) = D_002CD570
 * Info: proposed name CPLCY_ctor (T1); class CPLCY; TU cplcy
 * Notes: names_v2 v1 proposed SetCmSniperFocus__FP2CMP3ALOff (0.004) | mangled dropped (base does not match
 *   final name): __5CPLCY
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#4-source-files, docs/engine/camera.md#5-key-functions,
 *   docs/engine/guards-ai.md#8-source-files
 * @end-sly2-annot */
void *func_0014FCA8(void *arg0) {
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CD570;
    return arg0;
}
