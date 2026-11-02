#include "m2c_prelude.h"
M2C_UNK StartupCodes(M2C_UNK *);                   /* extern */
M2C_UNK UpdateGrfjoytFromGrfusr__Fv();                            /* extern */
M2C_UNK InitJoy__FP3JOYii(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK scePadInit(M2C_UNK);                        /* extern */
extern M2C_UNK D_002DDEC0;
extern M2C_UNK D_002DFC00;

/** @sly2-annot addr=0x187030
 * Purpose: UpdateGrfjoytFromGrfusr, scePadInit(0), InitJoy(&g_joy D_002DFC00, 0, 0), then init D_002DDEC0
 *   (0x14C2D8)
 * Info: proposed name StartupJoy (T1); TU joy
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/animation.md#6-source-files,
 *   docs/engine/main-loop-startup.md#3-pad-input-joycpp-0x187030-0x188488,
 *   docs/engine/main-loop-startup.md#7-source-files
 * @end-sly2-annot */
void StartupJoy__Fv(void) {
    UpdateGrfjoytFromGrfusr__Fv();
    scePadInit(0);
    InitJoy__FP3JOYii(&D_002DFC00, 0, 0);
    StartupCodes(&D_002DDEC0);
}
