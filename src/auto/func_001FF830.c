#include "m2c_prelude.h"
M2C_UNK func_00117968(s32, s32, s32);               /* extern */
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK func_0012A0B0(s32 *, M2C_UNK);              /* extern */
extern M2C_UNK D_00279AD0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002A8288;
extern M2C_UNK D_002A82A8;
extern s32 D_002A8350;
extern M2C_UNK D_002A83B8;
extern M2C_UNK D_002A83C0;
extern M2C_UNK D_002BBB60;
extern s32 D_002BD4C0;
extern M2C_UNK D_002C3C40;
extern f32 D_002C3C58;
extern M2C_UNK D_002CA510;
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern s32 D_002DD5BC;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDF10;
extern s32 D_002E1E40;
extern s32 D_002E1E54;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern s32 D_003E1C34;
extern M2C_UNK D_004435F0;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x1FF830
 * Purpose: Clear root frame (0x256EE0(p,0)) and ShutdownSplice (0x12AE28)
 * Info: proposed name DeleteSwSplice (T2); TU splicemap
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
void func_001FF830(s32 arg0, s32 arg1) {
    func_00256EE0(arg0, 0);
    ShutdownSplice();
}
