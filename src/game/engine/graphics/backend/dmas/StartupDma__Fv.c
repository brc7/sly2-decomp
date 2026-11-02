#include "m2c_prelude.h"
s32 sceDmaGetChan(M2C_UNK);                         /* extern */
M2C_UNK sceDmaReset(M2C_UNK);                       /* extern */
M2C_UNK sceGsResetPath();                           /* extern */
extern s32 D_002DE590;
extern s32 D_002DE594;
extern s32 D_002DE598;
extern s32 D_002DE59C;
extern s32 D_002DE5A0;

/** @sly2-annot addr=0x157200
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/overview.md#1-boot,
 *   docs/engine/rendering.md#12-source-files
 * @end-sly2-annot */
void StartupDma__Fv(void) {
    sceDmaReset(1);
    sceGsResetPath();
    D_002DE590 = sceDmaGetChan(0);
    D_002DE594 = sceDmaGetChan(1);
    D_002DE598 = sceDmaGetChan(2);
    D_002DE59C = sceDmaGetChan(8);
    D_002DE5A0 = sceDmaGetChan(9);
}
