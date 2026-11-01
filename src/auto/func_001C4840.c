#include "m2c_prelude.h"
/** @sly2-annot addr=0x1C4840
 * Purpose: Returns g_sw current PO (SW+0x3EFC)
 * Info: proposed name PpoHero (T1); TU po
 * Notes: verify r3r5: 0x1C4840 returns SW+0x3EFC (the hero), distinct from effective PpoCur 0x1C4A48;
 *   integrator name (was PpoCur) | runtime3: g_pjt is always Sly, not the current player (PpoHero confirmed)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r5_1AE130+runtime
 * See: docs/engine/player.md#1-class-chain, docs/engine/player.md#11-key-functions,
 *   docs/guides/devtools.md#code_explorer
 * @end-sly2-annot */
s32 func_001C4840(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x3EFC);
}
