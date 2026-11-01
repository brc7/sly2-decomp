#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK func_00129EC8(s32 *, s32);                  /* extern */
M2C_UNK func_0012A0B0(s32 *, M2C_UNK);              /* extern */
s32 func_001AE318(s32 *);                           /* extern */
extern M2C_UNK D_002A43B0;
extern M2C_UNK D_002BBB90;
extern void *D_002DD2AC;
extern void *D_002DD2CC;
extern s32 D_002DD598;
extern void *D_003E0E80;
extern M2C_UNK D_004520A8;
extern s32 D_004520B8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520E8;
extern s32 D_00452108;
extern s32 D_00452130;
extern M2C_UNK D_00452138;
extern M2C_UNK D_00452188;
extern M2C_UNK D_004521A0;
extern M2C_UNK D_004521B8;
extern M2C_UNK D_004521D0;
extern M2C_UNK D_004521E8;

/** @sly2-annot addr=0x1294C0
 * Purpose: Constructor: tag 0x21; calls the object's vtable slot 1 (Incref) if non-null
 * Info: proposed name CRef_ctor_BASIC (T1); class CRef; TU ref
 * Notes: names_v2 v1 proposed SetBool__4CRefi (0.002) | C++ signature hint (BASIC *) | COLLISION: CRef_ctor
 *   already used (same name proposed for 0x129248); renamed to CRef_ctor_BASIC
 * Confidence: high [T1 0.80] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
s32 CRef_ctor_BASIC(s32 arg0, s32 arg1) {
    void *temp_v0;

    if (arg1 != 0) {
        temp_v0 = M2C_FIELD(arg1, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32), 0xC)(arg1 + M2C_FIELD(temp_v0, s16 *, 8));
    }
    M2C_FIELD(arg0, s32 *, 4) = arg1;
    M2C_FIELD(arg0, s32 *, 0) = 0x21;
    return arg0;
}
