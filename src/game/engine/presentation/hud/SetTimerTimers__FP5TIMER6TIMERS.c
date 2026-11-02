#include "m2c_prelude.h"
extern f32 D_002DDF0C;

/** @sly2-annot addr=0x212A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name SetWidgetRequest (EXISTING); TU hud
 * Notes: RENAME proposed -> SetWidgetRequest (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.75]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void SetTimerTimers__FP5TIMER6TIMERS(void *arg0, s32 arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x64) != arg1) {
        M2C_FIELD(arg0, s32 *, 0x64) = arg1;
        M2C_FIELD(arg0, f32 *, 0x68) = (f32) D_002DDF0C;
    }
}
