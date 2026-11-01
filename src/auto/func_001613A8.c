#include "m2c_prelude.h"
M2C_UNK func_001619A0(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_001A2C20(M2C_UNK);                     /* extern */
M2C_UNK func_001A2DA0(s32);                         /* extern */
M2C_UNK func_001A4E08(M2C_UNK *);                   /* extern */
s32 func_0024A860();                                /* extern */
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E1E88;

/** @sly2-annot addr=0x1613A8
 * Purpose: Once-per-boot game startup from main: language-dependent init (sceScfGetLanguage), broadcast game
 *   event 0, init the game sequence queue D_002E1E88
 * Info: proposed name StartupGame (T1); TU game
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/game-progression.md#7-source-files, docs/engine/game-progression.md#8-key-functions,
 *   docs/engine/main-loop-startup.md#8-key-functions
 * @end-sly2-annot */
void func_001613A8(void) {
    func_001A2C20(0);
    func_001A2DA0(func_0024A860());
    func_001619A0(&D_002DEB30, 0, 0);
    func_001A4E08(&D_002E1E88);
}
