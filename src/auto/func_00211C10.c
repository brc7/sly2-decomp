#include "m2c_prelude.h"
M2C_UNK AddDmaCnt__4DMAS();                         /* extern */
M2C_UNK AddVifFlusha__4VIFS(s32);                   /* extern */
M2C_UNK AddVifMscal__4VIFSPv(s32, M2C_UNK *);       /* extern */
M2C_UNK EndDmaCnt__4DMAS(s32);                      /* extern */
extern M2C_UNK D_C0;

/** @sly2-annot addr=0x211C10
 * Purpose: Pass 5 end: MSCAL 0xC0, FLUSHA
 * Info: proposed name EndRpHud (T1); TU hud
 * Notes: superseded: FinishSpriteVu1 (r7c_hud)
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211C10(s32 arg0) {
    AddDmaCnt__4DMAS();
    AddVifMscal__4VIFSPv(arg0, &D_C0);
    AddVifFlusha__4VIFS(arg0);
    EndDmaCnt__4DMAS(arg0);
}
