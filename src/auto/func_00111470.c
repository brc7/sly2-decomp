#include "m2c_prelude.h"
extern M2C_UNK D_002C9810;
extern M2C_UNK D_002CA9F0;
extern M2C_UNK D_002CABD8;
extern M2C_UNK D_002CAF88;
extern M2C_UNK D_002CB230;
extern M2C_UNK D_002CB780;
extern M2C_UNK D_002CB9F8;
extern M2C_UNK D_002CBBA0;
extern M2C_UNK D_002CC950;
extern M2C_UNK D_002CD748;
extern M2C_UNK D_002CE118;
extern M2C_UNK D_002D0B30;
extern M2C_UNK D_002D0DD8;
extern M2C_UNK D_002D1080;
extern M2C_UNK D_002D1328;
extern M2C_UNK D_002D15D0;
extern M2C_UNK D_002D1878;
extern M2C_UNK D_002D1EA8;
extern M2C_UNK D_002D2090;
extern M2C_UNK D_002D20D0;
extern M2C_UNK D_002D2338;
extern M2C_UNK D_002D2378;
extern M2C_UNK D_002D3200;
extern M2C_UNK D_002D3838;
extern M2C_UNK D_002D3AA0;
extern M2C_UNK D_002D3D20;
extern M2C_UNK D_002D3FA0;
extern M2C_UNK D_002D4260;
extern M2C_UNK D_002D4B78;
extern M2C_UNK D_002D4F80;
extern M2C_UNK D_002D5450;
extern M2C_UNK D_002D5898;
extern M2C_UNK D_002D5B30;
extern M2C_UNK D_002D5F68;
extern M2C_UNK D_002D61D8;
extern M2C_UNK D_002D6448;
extern M2C_UNK D_002D66B8;
extern M2C_UNK D_002D6928;
extern M2C_UNK D_002D77D8;
extern M2C_UNK D_002D7CA8;
extern M2C_UNK D_002D80D0;
extern M2C_UNK D_002D8738;
extern M2C_UNK D_002D9500;
extern M2C_UNK D_002D9978;
extern M2C_UNK D_002D9D90;
extern M2C_UNK D_002D9DD0;
extern M2C_UNK D_002D9F70;
extern M2C_UNK D_002DA110;
extern M2C_UNK D_002DB8A0;
extern M2C_UNK D_002DBB10;
extern M2C_UNK D_002DBD80;
extern M2C_UNK D_002DC550;
extern M2C_UNK D_002DC890;
extern M2C_UNK D_002DCA30;

static inline void *m2c_ctor_0(void *p)
{
    M2C_FIELD(p, M2C_UNK **, 0xC) = &D_002CABD8;
    M2C_FIELD(p, s32 *, 0) = 0;
    M2C_FIELD(p, s32 *, 4) = 0;
    return p;
}

/** @sly2-annot addr=0x111470
 * Purpose: pfnNew factory for CID43 (cid 43): allocates 0x100 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2CABD8 at +0xC
 * Info: proposed name NewCid43 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *func_00111470(s32 arg0) {
    void *temp_v0;

    return m2c_ctor_0(func_001AE4F8(0x100));
}
