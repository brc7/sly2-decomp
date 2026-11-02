#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(void *, s32, s32);          /* extern */
M2C_UNK Decref__4CRef(void *);                      /* extern */
s128 *func_001AE318(s32 *);                         /* extern */
extern M2C_UNK D_002A5018;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002BDAB0;
extern M2C_UNK D_002BDAC0;
extern f32 D_002BDACC;
extern M2C_UNK D_002BDAD0;
extern M2C_UNK D_002BDAE0;
extern M2C_UNK D_002BDAF0;
extern M2C_UNK D_002BDB00;
extern M2C_UNK D_002BDB10;
extern M2C_UNK D_002BDB20;
extern M2C_UNK D_002BDB30;
extern M2C_UNK D_002BDB40;
extern f32 D_002BDB50;
extern f32 D_002BDB54;
extern f32 D_002BDB58;
extern f32 D_002BDB5C;
extern M2C_UNK D_002BDB60;
extern M2C_UNK D_002BDB68;
extern M2C_UNK D_002BDB70;
extern f32 D_002BDBB0;
extern s128 D_002BDBC0;
extern s128 D_002BDBD0;
extern M2C_UNK D_002BDBE0;
extern s128 D_002BDBF0;
extern M2C_UNK D_002DD590;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DDF40;
extern void *D_002DE570;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern void *D_003E0F9C;
extern s32 D_003E1480;

/** @sly2-annot addr=0x153EF8
 * Purpose: If the tracked object changed (a!=b) registers the new one as a detector target
 * Info: TU detector
 * Notes: names_v2 C proposed FReachedStepguardGoal__FP9STEPGUARD (0.200) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r3_1539A8
 * @end-sly2-annot */
void func_00153EF8(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 != arg2) {
        AddDetectorTarget(arg0, arg2);
    }
}
