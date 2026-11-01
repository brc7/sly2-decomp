#include "m2c_prelude.h"
M2C_UNK func_00138F08(M2C_UNK, M2C_UNK *);          /* extern */
extern M2C_UNK D_002DD5C0;
extern M2C_UNK D_002DD600;

/** @sly2-annot addr=0x138FC0
 * Purpose: Build the two blip GIF packets (BuildBlipAqwGifs(1,&D_002DD5C0), (2,&D_002DD600))
 * Info: proposed name StartupBlips (T1); TU blip
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_A1+verify_side
 * See: docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot,
 *   docs/engine/particles-effects.md#6-key-functions
 * @end-sly2-annot */
void func_00138FC0(void) {
    func_00138F08(1, &D_002DD5C0);
    func_00138F08(2, &D_002DD600);
}
