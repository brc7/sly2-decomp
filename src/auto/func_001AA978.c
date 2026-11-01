#include "m2c_prelude.h"
M2C_UNK InitSave__FP4SAVEii(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK InitGame(M2C_UNK *);                   /* extern */
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E65E0;
extern s32 D_004425D8;

/** @sly2-annot addr=0x1AA978
 * Purpose: Startup-table entry: reset D_002DEB30 (0x1613F8), clear D_004425D8, InitSave(&g_save,0,0)
 * Info: proposed name StartupMemcard (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/overview.md#1-boot, docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/save-memcard.md#4-source-files
 * @end-sly2-annot */
void func_001AA978(void) {
    InitGame(&D_002DEB30);
    D_004425D8 = -1;
    InitSave__FP4SAVEii(&D_002E65E0, 0, 0);
}
