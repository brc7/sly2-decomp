#include "m2c_prelude.h"
M2C_UNK SignalSema(s32);                            /* extern */
M2C_UNK WaitSema(s32);                              /* extern */
extern s32 D_002DD314;
extern s32 D_003D3A40;
extern s32 D_003D3A44;

/** @sly2-annot addr=0x1C9098
 * Purpose: signal frame-end sema, wait for loading thread to stop, clear D_002DD314
 * Info: proposed name EndLoadingScreen (T1); TU prog
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void EndLoadingScreen(void) {
    SignalSema(D_003D3A40);
    WaitSema(D_003D3A44);
    D_002DD314 = 0;
}
