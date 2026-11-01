#include "m2c_prelude.h"
M2C_UNK func_001A3020(M2C_UNK *);                   /* extern */
M2C_UNK func_001ACAE8(M2C_UNK *);                   /* extern */
M2C_UNK func_001AE928(M2C_UNK *);                   /* extern */
M2C_UNK func_001D9968(M2C_UNK *);                   /* extern */
M2C_UNK func_001DA2E0(M2C_UNK *);                   /* extern */
M2C_UNK func_001DA770(M2C_UNK *);                   /* extern */
M2C_UNK func_001DCCA0(M2C_UNK *);                   /* extern */
M2C_UNK func_001DD690(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002E1E60;
extern M2C_UNK D_002E6550;
extern M2C_UNK D_002E65A0;
extern M2C_UNK D_003D9E80;
extern M2C_UNK D_003D9ED0;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DA0F0;
extern M2C_UNK D_003DA150;
extern M2C_UNK D_003DA3C0;
extern M2C_UNK D_003DA4F0;
extern M2C_UNK D_003DA530;

/** @sly2-annot addr=0x1D9520
 * Purpose: Call PostLoad (slot 12) on every global HUD screen object (D_003D9E80, D_003DA0F0, D_003DA150,
 *   D_003DA0C0, ...)
 * Info: proposed name PostScreensLoad (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9520(void) {
    func_001D9968(&D_003D9E80);
    func_001AE928(&D_003D9ED0);
    func_001A3020(&D_002E1E60);
    func_001DA770(&D_003DA0F0);
    func_001DA770(&D_003DA150);
    func_001DA2E0(&D_003DA0C0);
    func_001DCCA0(&D_003DA3C0);
    func_001ACAE8(&D_002E6550);
    func_001DA770(&D_002E65A0);
    func_001DD690(&D_003DA4F0);
    func_001DA770(&D_003DA530);
}
