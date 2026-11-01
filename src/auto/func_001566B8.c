#include "m2c_prelude.h"
/** @sly2-annot addr=0x1566B8
 * Purpose: cid 0x93 vt1: ++refcount (+0x10)
 * Info: proposed name IncrefCid147 (T2); class CID147 slot1; TU noise
 * Notes: verifier PLAUSIBLE: IncrefNoise -> IncrefCid93 | Cid93->Cid147 (CID 0x93=147, vtable)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r3b_rest+verdicts_tu_r3r5
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_001566B8(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
