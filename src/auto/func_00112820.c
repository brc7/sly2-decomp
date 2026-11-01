#include "m2c_prelude.h"
extern M2C_UNK D_002C94A0;
extern M2C_UNK D_002C94E0;
extern M2C_UNK D_002CA300;
extern M2C_UNK D_002CA490;
extern M2C_UNK D_002CAD78;
extern M2C_UNK D_002CC4C0;
extern M2C_UNK D_002CC548;
extern M2C_UNK D_002CCF10;
extern M2C_UNK D_002CD5B8;
extern M2C_UNK D_002CDA10;
extern M2C_UNK D_002CE2F8;
extern M2C_UNK D_002CF058;
extern M2C_UNK D_002CF1E8;
extern M2C_UNK D_002CF378;
extern M2C_UNK D_002CF7F8;
extern M2C_UNK D_002CF880;
extern M2C_UNK D_002CFA90;
extern M2C_UNK D_002D04A0;
extern M2C_UNK D_002D0520;
extern M2C_UNK D_002D05A0;
extern M2C_UNK D_002D0620;
extern M2C_UNK D_002D06A0;
extern M2C_UNK D_002D0728;
extern M2C_UNK D_002D07B0;
extern M2C_UNK D_002D0830;
extern M2C_UNK D_002D08B0;
extern M2C_UNK D_002D0930;
extern M2C_UNK D_002D09B0;
extern M2C_UNK D_002D0A30;
extern M2C_UNK D_002D0AB0;
extern M2C_UNK D_002D5120;
extern M2C_UNK D_002D7A40;
extern M2C_UNK D_002D7F40;
extern M2C_UNK D_002D8C48;
extern M2C_UNK D_002D8DD8;
extern M2C_UNK D_002D91E0;
extern M2C_UNK D_002D9240;
extern M2C_UNK D_002D92A0;
extern M2C_UNK D_002D9300;
extern M2C_UNK D_002D9360;
extern M2C_UNK D_002D93C0;
extern M2C_UNK D_002D9480;
extern M2C_UNK D_002D9768;
extern M2C_UNK D_002D97A8;
extern M2C_UNK D_002D9978;
extern M2C_UNK D_002DA3D8;
extern M2C_UNK D_002DA840;
extern M2C_UNK D_002DA9D0;
extern M2C_UNK D_002DB0D0;
extern M2C_UNK D_002DCC98;
extern M2C_UNK D_002DCF00;

static inline void *m2c_ctor_0(void *p)
{
    M2C_FIELD(p, M2C_UNK **, 0xC) = &D_002CA300;
    M2C_FIELD(p, s32 *, 0) = 0;
    M2C_FIELD(p, s32 *, 4) = 0;
    return p;
}

/** @sly2-annot addr=0x112820
 * Purpose: pfnNew factory for ASEGBL (cid 110): allocates 0xE0 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2CA300 at +0xC
 * Info: proposed name NewAsegbl (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *func_00112820(s32 arg0) {
    void *temp_v0;

    return m2c_ctor_0(func_001AE4F8(0xE0));
}
