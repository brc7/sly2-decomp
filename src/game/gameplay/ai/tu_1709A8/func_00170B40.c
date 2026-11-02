#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s128 *, s128 *, s8, s128);  /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, s32 *);             /* extern */
M2C_UNK ReadVector__18CBinaryInputStreamP6VECTOR(s32, s32 *);                  /* extern */
M2C_UNK ReadMatrix4__18CBinaryInputStreamP7MATRIX4(s32, M2C_UNK *);              /* extern */
M2C_UNK ReadCb12__18CBinaryInputStreamPv(s32, M2C_UNK *);              /* extern */
M2C_UNK ReadCb8__18CBinaryInputStreamPv(s32, M2C_UNK *);              /* extern */
M2C_UNK AllocSw__4DMASi(M2C_UNK *, s32);              /* extern */
M2C_UNK DetachCopySw__4DMASPiPP2QWT2(M2C_UNK *, s32 *, s32 *, s32 *); /* extern */
s32 *func_001AE318(s32 *, s8, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK __4VIFS(M2C_UNK *, s64, s64, s64);    /* extern */
M2C_UNK AddVifUnpack__4VIFS3UPKiP18CBinaryInputStreami(M2C_UNK *, s32, s32, s32);    /* extern */
M2C_UNK sceGsResetGraph(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK sceGsPutDispEnv(void *, s32 *, s32 *, s32 *); /* extern */
M2C_UNK sceGsPutDrawEnv(M2C_UNK *);                   /* extern */
M2C_UNK sceGsSetDefDBuff(s32 *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK sceGsSwapDBuff(s32 *, s32);                  /* extern */
M2C_UNK sceDmaSend(s32, s32);                    /* extern */
M2C_UNK sceDmaSync(s32, M2C_UNK, M2C_UNK);       /* extern */
extern M2C_UNK D_0025B880;
extern M2C_UNK D_0025C1B0;
extern M2C_UNK D_0025C1C0;
extern M2C_UNK D_0025E040;
extern M2C_UNK D_0025E680;
extern M2C_UNK D_00262320;
extern M2C_UNK D_002A2D30;
extern s128 D_002A2DD0;
extern f32 D_002A52A0;
extern M2C_UNK D_002BE540;
extern M2C_UNK D_002BE558;
extern s32 D_002BE580;
extern s32 D_002BE590;
extern s32 D_002BE594;
extern M2C_UNK D_002DDF40;
extern s32 D_002DE598;
extern M2C_UNK D_002DEA28;
extern s32 D_002DEA4C;
extern M2C_UNK D_002DEA50;
extern s32 D_002DEB50;
extern s32 D_002DEB54;
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
extern s32 D_003E11EC;
extern s32 D_003E12C8;
extern s32 D_003E12D0;
extern s64 D_003E1330;
extern M2C_UNK D_10;
extern M2C_UNK D_1910;
extern M2C_UNK D_1A68;
extern M2C_UNK D_1AA0;
extern M2C_UNK D_1C48;
extern M2C_UNK D_2E20;
extern M2C_UNK D_2E68;
extern M2C_UNK D_2EB0;
extern M2C_UNK D_2EE8;
extern M2C_UNK D_2EF8;
extern M2C_UNK D_2F80;
extern M2C_UNK D_2FC8;
extern M2C_UNK D_3358;
extern M2C_UNK D_3628;
extern M2C_UNK D_37A8;
extern M2C_UNK D_3AB0;
extern M2C_UNK D_738;
extern M2C_UNK D_7F0;

/*
Decompilation failure in function RenderSoGlobset:

Found jr instruction at chunk.s line 733, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

/** @sly2-annot addr=0x170B40
 * Purpose: Splice: append command kind 0 (set lookup context oid)
 * Info: proposed name AddCid113CmdContext (T2); class CID113; TU tu_1709A8
 * Notes: names_v2 C proposed SetCmMat__FP2CMP7MATRIX3 (0.200) | Cid71->Cid113 (CID 0x71=113,
 *   agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170B40(s32 arg0, s32 arg1) {
    func_00170E00(arg0, 0, arg1);
}
