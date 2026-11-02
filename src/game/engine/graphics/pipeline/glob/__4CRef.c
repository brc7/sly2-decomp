#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s128 *, s128 *, s8, s128);  /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, s32 *);             /* extern */
M2C_UNK Skip__18CBinaryInputStreami(s32, s32);                    /* extern */
M2C_UNK ReadVector__18CBinaryInputStreamP6VECTOR(s32, s32 *);                  /* extern */
M2C_UNK ReadMatrix4__18CBinaryInputStreamP7MATRIX4(s32, M2C_UNK *);              /* extern */
M2C_UNK ReadCb12__18CBinaryInputStreamPv(s32, M2C_UNK *);              /* extern */
M2C_UNK ReadCb8__18CBinaryInputStreamPv(s32, M2C_UNK *);              /* extern */
M2C_UNK func_001291A8(void **, void **);            /* extern */
M2C_UNK Decref__4CRef(void **);                     /* extern */
M2C_UNK AllocSw__4DMASi(M2C_UNK *, M2C_UNK);          /* extern */
M2C_UNK DetachCopySw__4DMASPiPP2QWT2(M2C_UNK *, s32 *, s32 *, s32 *); /* extern */
s32 *func_001AE318(s32 *, s32, s32 *, M2C_UNK);     /* extern */
M2C_UNK __4VIFS(M2C_UNK *);                   /* extern */
M2C_UNK AddVifUnpack__4VIFS3UPKiP18CBinaryInputStreami(M2C_UNK *, s32, s32, s32);    /* extern */
extern M2C_UNK D_0025E090;
extern M2C_UNK D_0025E668;
extern M2C_UNK D_002A2D30;
extern M2C_UNK D_002A3828;
extern M2C_UNK D_002A5238;
extern f32 D_002A5270;
extern f32 D_002A52A0;
extern M2C_UNK D_002BE540;
extern M2C_UNK D_002BE558;
extern s32 D_002BE580;
extern M2C_UNK D_002CF508;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEA28;
extern s32 D_002DEB28;
extern M2C_UNK D_002DEB30;
extern s32 D_002DEB4C;
extern s32 D_002DEB54;
extern M2C_UNK D_002DEB60;
extern M2C_UNK D_002E65E0;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA530;
extern s32 D_003DA790;
extern M2C_UNK D_003DD490;
extern s32 D_003E0F8C;
extern s32 D_003E11EC;
extern s32 D_003E12D0;
extern M2C_UNK D_10;
extern M2C_UNK D_1910;
extern M2C_UNK D_1A68;
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

/** @sly2-annot addr=0x167310
 * Purpose: CRef default constructor emitted (weak) at the start of the glob TU
 * Info: proposed name __4CRef (EXISTING); class CRef; TU glob
 * Notes: agent/verifier proposes CRef_ctor for this existing name (see existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r3_1539A8
 * See: docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
s32 __4CRef(s32 arg0) {
    M2C_FIELD(arg0, s32 *, 0x4C) = -1;
    return arg0;
}
