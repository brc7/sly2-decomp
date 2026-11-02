#include "m2c_prelude.h"
extern s32 D_002DE254;

/** @sly2-annot addr=0x14FD50
 * Purpose: True if pcplcy is g_cm's current top policy (D_002DE254 = g_cm.acpr[0].pcplcy)
 * Info: proposed name FActiveCplcy (T1); TU cplcy
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
s32 FActiveCplcy__FP5CPLCY(s32 arg0) {
    return D_002DE254 == arg0;
}
