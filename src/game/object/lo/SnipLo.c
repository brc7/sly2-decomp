#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001A50E8 (initial). */

extern s128 D_002A2DA0;
extern s128 D_002A2DB0;
extern M2C_UNK D_002A3868;
extern M2C_UNK D_002A38A8;
extern f32 D_002A6A40;
extern M2C_UNK D_002BFC60;
extern f32 D_002BFC94;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEA28;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002E1E68;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4A0;
extern M2C_UNK D_003DD4BC;
extern void *D_003E04F8;
extern s32 D_003E139C;
extern s32 D_003E19E4;
extern M2C_UNK D_004435F0;

/** @sly2-annot addr=0x1A57A8
 * Purpose: Unlink plo from its parent child DL (fixing ploRemovedFirst) without hierarchy callbacks
 * Info: proposed name SnipLo (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void SnipLo(s32 arg0) {
    struct {
        M2C_UNK m0;
        s32 * m4;
    } fr;
    GetLoChildDl(M2C_FIELD(arg0, s32 *, 0x10), (void *)&fr, &fr.m4);
    if (*fr.m4 == arg0) {
        *fr.m4 = M2C_FIELD(arg0, s32 *, 0x24);
    }
    RemoveDlEntry__FP2DLPv(fr.m0, arg0);
}
