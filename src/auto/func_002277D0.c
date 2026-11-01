#include "m2c_prelude.h"
s32 func_0013E368(M2C_UNK *, s32);                  /* extern */
M2C_UNK strcpy(s8 *, s32);                          /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x2277D0
 * Purpose: oid -1 -> empty string else strcpy(name of oid via 0x13E368(g_sw,oid))
 * Info: proposed name GetOidName (T2); TU tu_2275B8
 * Notes: agent qualifier: into buffer
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88,
 *   docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void func_002277D0(s32 *arg0, s8 *arg1) {
    s32 temp_v1;

    temp_v1 = *arg0;
    if (temp_v1 == -1) {
        *arg1 = 0;
        return;
    }
    strcpy(arg1, func_0013E368(&D_003DD490, temp_v1));
}
