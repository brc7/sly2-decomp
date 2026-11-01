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

/** @sly2-annot addr=0x17B178
 * Purpose: BHVHELD Close: state -1 (restore mass), BHVANIM Close
 * Info: proposed name CloseBhvjuggled (T1); class BHVJUGGLED slot9; TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   SetAloInitialVelocity (names_v2:B(unverified)) | BHV class word normalised: CloseBhvheld ->
 *   CloseBhvjuggled | runtime2/4 confirmed the BHVJUGGLED role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B178(s32 arg0) {
    func_0017B1A8(arg0, -1);
    func_00174B28(arg0);
}
