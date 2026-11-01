#include "m2c_prelude.h"
M2C_UNK CloneSo();                            /* extern */

/** @sly2-annot addr=0x1C3DF8
 * Purpose: PO::Clone (vt slot 41): base clone 0x1F6068 then pzi.paloCollect(+0x168)=this
 * Info: proposed name ClonePo (T1); class PO slot41; TU po
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void ClonePo(void *arg0) {
    CloneSo();
    M2C_FIELD(arg0, void **, 0x168) = arg0;
}
