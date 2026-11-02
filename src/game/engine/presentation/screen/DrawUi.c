#include "m2c_prelude.h"
M2C_UNK Flush__4DMAS(M2C_UNK *);                   /* extern */
M2C_UNK func_001D9878(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002DEA00;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_003D9E80;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DA150;

/** @sly2-annot addr=0x1D9820
 * Purpose: Called from main: run 0x1D9878 on the three main HUD screens, then flush the two HUD DMA streams
 *   (D_002DEA00/D_002DEA28)
 * Info: proposed name DrawUi (EXISTING); TU screen
 * Notes: superseded r6_1D5150 proposal: UpdateScreen? (0.3) | agent/verifier proposes DrawScreens for this
 *   existing name (see existing_name_changes.csv) | RENAME proposed -> DrawUi (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6b_rest+arbitration
 * See: docs/engine/hud-ui.md#2-screens-and-menus-screencpp-0x1d93d8-menu-0x1ae928,
 *   docs/engine/hud-ui.md#5-key-functions, docs/engine/overview.md#2-the-frame-loop-exact-order
 * @end-sly2-annot */
void DrawUi(void) {
    func_001D9878(&D_003D9E80);
    func_001D9878(&D_003DA0F0);
    func_001D9878(&D_003DA150);
    Flush__4DMAS(&D_002DEA00);
    Flush__4DMAS(&D_002DEA28);
}
