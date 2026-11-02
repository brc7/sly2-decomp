#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(void *, void *, s32);       /* extern */
M2C_UNK ListXYZF__4GIFSiiii(s32, s32, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK AllocStack__4DMASi(M2C_UNK *, s32);              /* extern */
M2C_UNK func_001573C0(M2C_UNK *, s32, M2C_UNK *, s32); /* extern */
M2C_UNK func_00166920(M2C_UNK *);                   /* extern */
M2C_UNK func_00226C10(M2C_UNK *, s64, s64, s64);    /* extern */
M2C_UNK func_00226FD0(M2C_UNK *, s32, s32, s32);    /* extern */
M2C_UNK func_00240460(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_00240600(void *, s32 *, s32 *, s32 *); /* extern */
s32 func_002406C0(M2C_UNK *);                       /* extern */
M2C_UNK func_002407A8(s32 *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_00240A40(s32 *, s32);                  /* extern */
M2C_UNK func_00241728(s32, s32);                    /* extern */
M2C_UNK func_002417F0(s32, M2C_UNK, M2C_UNK);       /* extern */
extern M2C_UNK D_0025C1C0;
extern M2C_UNK D_0025E040;
extern M2C_UNK D_0025E680;
extern M2C_UNK D_00262320;
extern s128 D_002A2DD0;
extern M2C_UNK D_002BE540;
extern M2C_UNK D_002BE558;
extern s32 D_002BE590;
extern s32 D_002BE594;
extern M2C_UNK D_002DDF40;
extern s32 D_002DE598;
extern s32 D_002DEA4C;
extern M2C_UNK D_002DEA50;
extern s32 D_002DEB50;
extern M2C_UNK D_002DEB60;
extern M2C_UNK D_002DEB70;
extern s32 D_002DEB74;
extern s32 D_002DEB80;
extern M2C_UNK D_002DEBD0;
extern u64 D_002DEBE0;
extern M2C_UNK D_002DECC0;
extern u64 D_002DECD0;
extern s32 D_002DEDB0;
extern s32 D_002DEDB4;
extern s32 D_002DEDB8;
extern s32 D_002DEDBC;
extern M2C_UNK D_002DEDC0;
extern M2C_UNK D_002DF010;
extern M2C_UNK D_002DF260;
extern M2C_UNK D_002DF4B0;
extern M2C_UNK D_002DF700;
extern M2C_UNK D_002DF720;
extern M2C_UNK D_002DF740;
extern M2C_UNK D_002DF800;
extern M2C_UNK D_002DF880;
extern M2C_UNK D_002DF940;
extern M2C_UNK D_002DF9C0;
extern M2C_UNK D_002DFA80;
extern M2C_UNK D_002DFB00;
extern M2C_UNK D_002DFB20;
extern M2C_UNK D_002DFB40;
extern s32 D_002DFB4C;
extern s32 D_002DFB50;
extern s32 D_002DFB54;
extern s32 D_002DFB58;
extern s32 D_002DFB5C;
extern s32 D_002DFB60;
extern s32 D_003DA790;
extern M2C_UNK D_003DA798;
extern s32 D_003DCF98;
extern s32 D_003DCF9C;
extern s32 D_003DCFA0;
extern s32 D_003DCFA8;
extern M2C_UNK D_003DD490;
extern s32 D_003E12C8;
extern s64 D_003E1330;
extern M2C_UNK D_1AA0;
extern M2C_UNK D_1C48;
extern M2C_UNK D_3AB0;
extern M2C_UNK D_738;
extern M2C_UNK D_7F0;

/** @sly2-annot addr=0x170C50
 * Purpose: Append command kind 6 (move to target oid, with two args)
 * Info: proposed name AddCid113CmdGoto (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170C50(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    temp_v0 = func_00170E00(arg0, 6, arg1);
    M2C_FIELD(temp_v0, s32 *, 8) = arg2;
    M2C_FIELD(temp_v0, s32 *, 0xC) = arg3;
}
