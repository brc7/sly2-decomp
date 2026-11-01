#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(void *, s32, s32);          /* extern */
M2C_UNK func_00129EC8(void *);                      /* extern */
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

/** @sly2-annot addr=0x152350
 * Purpose: cid 0x44 vt50 GetCpdefi: base 0x1F58F0 then radHome(+0x30) -= 3*PI/4 (camera home swung 135
 *   degrees)
 * Info: proposed name GetDancerCpdefi (T1); class DANCER slot50; TU rhythm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152350(s32 arg0, s32 arg1) {
    func_001F58F0(arg0, arg1);
    M2C_FIELD(arg1, f32 *, 0x30) = (f32) (M2C_FIELD(arg1, f32 *, 0x30) - 2.3561945f);
}
