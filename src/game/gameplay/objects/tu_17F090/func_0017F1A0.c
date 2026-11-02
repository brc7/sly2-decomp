#include "m2c_prelude.h"
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern f32 D_002A5654;
extern f32 D_002A5658;
extern f32 D_002A565C;
extern f32 D_002BE7B8;
extern f32 D_002BE7BC;
extern f32 D_002BE7C0;
extern f32 D_002BE7C4;
extern s128 D_002BE7D0;
extern M2C_UNK D_002BE800;
extern f32 D_002BE880;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DEB30;
extern s32 D_002DFB80;
extern s32 D_002E1E00;
extern M2C_UNK D_003DD020;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x17F1A0
 * Purpose: vt55: SO base then clear adjust fields x94/x98 (same as pipe twin ResetJtPipeAdjust)
 * Info: proposed name AdjustHackXp (T2); class HACK slot55; TU tu_17F090
 * Notes: names_v2 C proposed InitMurray__FP6MURRAY (0.200) | CID47->CID71 (CID 0x47=71, vtable) | v3 class
 *   identity: AdjustCid71Xp -> AdjustHackXp | T2 forced: v3: coined class word (Hack) without
 *   descriptor/string backing
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/engine/minigames.md#21-classes-hack-tu-0x17f090-0x182ab8-ccode-names-cparse-we-names-hack-word-coined-t2,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F1A0(s32 arg0, s32 arg1) {
    func_001F4FB0();
    M2C_FIELD(arg1, s32 *, 0x94) = 0;
    M2C_FIELD(arg1, s32 *, 0x98) = 0;
}
