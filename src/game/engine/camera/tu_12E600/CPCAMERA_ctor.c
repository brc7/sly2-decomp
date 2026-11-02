#include "m2c_prelude.h"
M2C_UNK CPLCY_ctor();                            /* extern */
extern M2C_UNK D_002CAB90;

/** @sly2-annot addr=0x12E760
 * Purpose: Constructs the CPLCY-derived camera policy embedded in this class (__5CPLCY 0x14FCA8 then vptr)
 * Info: proposed name CPCAMERA_ctor (T1); class CPCAMERA; TU tu_12E600
 * Notes: verifier PLAUSIBLE: CplcyCid44::CplcyCid44 -> CPCAMERA::CPCAMERA | verifier: purpose partly wrong:
 *   CPLCY ctor then vtable 0x2CAB90. The CM ctor 0x14D3D8 calls it for CM+0x4A8 = Lilly 'CPCAMERA part4a8'
 *   (CPCAMERA : CPLCY {CID44 *pcamera}). It is embedded in CM
 * Confidence: mid (draft) [T1 0.60] - evidence: Sly 1 / Lilly reference; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/camera.md#3-camera-policies-cplcycpp-0x14fca8-0x1520c8-plus-tncpp,
 *   docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void *CPCAMERA_ctor(void *arg0) {
    CPLCY_ctor();
    M2C_FIELD(arg0, M2C_UNK **, 0xC) = &D_002CAB90;
    return arg0;
}
