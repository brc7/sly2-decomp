#include "m2c_prelude.h"
s128 func_001A6F00(s32, void *, s128);              /* extern */
M2C_UNK func_001C3DC8();                            /* extern */
M2C_UNK func_001F8300(void *);                      /* extern */
extern s128 D_002A2DD0;

/** @sly2-annot addr=0x2039C0
 * Purpose: STEP vt14: LoadPoFromBrx, cache local base pos (+0x240 = ConvertAloPos(parent,this,0)), 0x1F8300
 * Info: proposed name LoadStepFromBrx (T1); class STEP slot14; TU step
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+parse-verified
 * See: docs/engine/guards-ai.md#9-key-functions, docs/engine/player.md#11-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14
 * @end-sly2-annot */
void func_002039C0(void *arg0) {
    func_001C3DC8();
    M2C_FIELD(arg0, s128 *, 0x240) = func_001A6F00(M2C_FIELD(arg0, s32 *, 0x10), arg0, D_002A2DD0);
    func_001F8300(arg0);
}
