#include "m2c_prelude.h"
s32 FSpooling__18CBinaryAsyncStream(void *);        /* extern */
M2C_UNK FinishSpooling__18CBinaryAsyncStream(void *); /* extern */
M2C_UNK sceCdSync(M2C_UNK);                         /* extern */
M2C_UNK sceClose(s32);                              /* extern */

/** @sly2-annot addr=0x132688
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#bas-0x132570-0x132b00
 * @end-sly2-annot */
void Close__18CBinaryAsyncStream(void *arg0) {
    s32 temp_a0;
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0);
    switch (temp_v1) {                              /* irregular */
    case 0:
        temp_a0 = M2C_FIELD(arg0, s32 *, 4);
        if (temp_a0 >= 0) {
            sceClose(temp_a0);
        }
        M2C_FIELD(arg0, s32 *, 4) = -1;
        break;
    case 1:
        if (FSpooling__18CBinaryAsyncStream(arg0) != 0) {
            sceCdSync(0);
            FinishSpooling__18CBinaryAsyncStream(arg0);
        }
        M2C_FIELD(arg0, s32 *, 8) = 0;
        break;
    }
    M2C_FIELD(arg0, s32 *, 0) = -1;
}
