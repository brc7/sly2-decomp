#include "m2c_prelude.h"
M2C_UNK func_001291A8(s32, s32 *);                  /* extern */
M2C_UNK func_00129EC8(s32 *);                       /* extern */
M2C_UNK SetTag__4CRef4TAGK(s32 *, M2C_UNK);              /* extern */
extern M2C_UNK D_002A3F20;
extern M2C_UNK D_002A3F90;
extern M2C_UNK D_002A3FA0;
extern M2C_UNK D_002A3FE8;
extern M2C_UNK D_002A4058;
extern M2C_UNK D_002A4098;
extern M2C_UNK D_002A40C8;
extern M2C_UNK D_002C9A80;
extern M2C_UNK D_002C9C20;
extern M2C_UNK D_002CA098;
extern M2C_UNK D_002CB5D8;
extern M2C_UNK D_002CC7B0;
extern M2C_UNK D_002CDBA0;
extern M2C_UNK D_002CE4D0;
extern M2C_UNK D_002CE828;
extern M2C_UNK D_002CEB78;
extern M2C_UNK D_002D1B20;
extern M2C_UNK D_002D1B60;
extern M2C_UNK D_002D1D00;
extern M2C_UNK D_002D1EA8;
extern M2C_UNK D_002D1EE8;
extern M2C_UNK D_002D2518;
extern M2C_UNK D_002D2558;
extern M2C_UNK D_002D26F8;
extern M2C_UNK D_002D2738;
extern M2C_UNK D_002D2938;
extern M2C_UNK D_002D2978;
extern M2C_UNK D_002D2B18;
extern M2C_UNK D_002D2CB8;
extern M2C_UNK D_002D33A0;
extern M2C_UNK D_002D3540;
extern M2C_UNK D_002D4DE0;
extern M2C_UNK D_002D52B0;
extern M2C_UNK D_002D56B8;
extern M2C_UNK D_002D56F8;
extern M2C_UNK D_002D5DC8;
extern M2C_UNK D_002D6E08;
extern M2C_UNK D_002D6FA8;
extern M2C_UNK D_002D6FE8;
extern M2C_UNK D_002D9040;
extern M2C_UNK D_002DB378;
extern M2C_UNK D_002DB518;
extern M2C_UNK D_002DB6B8;
extern M2C_UNK D_002DC070;
extern M2C_UNK D_002DC210;
extern M2C_UNK D_002DC3B0;
extern M2C_UNK D_002DC6F0;
extern M2C_UNK D_002DD090;

static inline void *m2c_ctor_0(void *p)
{
    M2C_FIELD(p, M2C_UNK **, 0xC) = &D_002CDBA0;
    M2C_FIELD(p, s32 *, 0) = 0;
    M2C_FIELD(p, s32 *, 4) = 0;
    return p;
}

/** @sly2-annot addr=0x110E18
 * Purpose: pfnNew factory for DETECTOR (cid 16): allocates 0x180 bytes (operator new 0x1AE4F8), inline ctor
 *   stores vtable 0x2CDBA0 at +0xC
 * Info: proposed name NewDetector (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *NewDetector(s32 arg0) {
    void *temp_v0;

    return m2c_ctor_0(func_001AE4F8(0x180));
}
