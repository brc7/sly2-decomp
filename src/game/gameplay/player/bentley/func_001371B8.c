#include "m2c_prelude.h"
/** @sly2-annot addr=0x1371B8
 * Purpose: {owner, -1} init
 * Info: proposed name InitJt1FF0Owner (T2); TU bentley
 * Notes: superseded: InitJtHelper1371B8 (verdicts_followups) | verifier better_name (PLAUSIBLE row): Body
 *   {*p=owner, p[1]=-1}; only caller InitJt passes (pjt+0x1FF0, pjt). Address-suffixed placeholder with '?';
 *   no class identity. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001371B8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
}
