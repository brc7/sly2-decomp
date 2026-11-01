#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A5588;
extern f32 D_002A558C;
extern f32 D_002A5590;
extern f32 D_002A5594;
extern f32 D_002A5598;
extern M2C_UNK D_002A55A0;
extern f32 D_002A55FC;
extern f32 D_002A5600;
extern M2C_UNK D_002BE6B8;
extern f32 D_002BE6C0;
extern M2C_UNK D_002BE6D0;
extern M2C_UNK D_002BE6E8;
extern M2C_UNK D_002BE710;
extern M2C_UNK D_002BE720;
extern M2C_UNK D_002BE730;
extern M2C_UNK D_002BE740;
extern f32 D_002BE748;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEB30;
extern s64 D_002DFB70;
extern s32 D_002DFB78;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x179B60
 * Purpose: BHVPATROL Close: stop, lower flag 0x1E
 * Info: proposed name CloseBhvpatrol (T1); class BHVPATROL slot9; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00179B60(s32 arg0) {
    func_00179D30(arg0, -1);
    func_001EEF78(arg0);
    func_00209970(M2C_FIELD(arg0, s32 *, 0x10), 0x1E);
}
