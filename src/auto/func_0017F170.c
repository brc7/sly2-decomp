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

/** @sly2-annot addr=0x17F170
 * Purpose: vt26: SO base then health x184 = max x180 (reset)
 * Info: proposed name HACK_vt26 (T2); class HACK slot26; TU tu_17F090
 * Notes: names_v2 C proposed InitVismap__FP6VISMAP (0.177) | CID47->CID71 (CID 0x47=71, vtable) | v3 class
 *   identity: CID71_vt26 -> HACK_vt26 | T2: placeholder token vt26 | T2 forced: v3: coined class word (HACK)
 *   without descriptor/string backing
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
s32 func_0017F170(s32 arg0, s32 arg1) {
    s32 temp_v0;

    func_001CB518(arg0, arg1);
    temp_v0 = M2C_FIELD(arg0, s32 *, 0x180);
    M2C_FIELD(arg0, s32 *, 0x184) = temp_v0;
    return temp_v0;
}
