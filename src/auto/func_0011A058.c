#include "m2c_prelude.h"
/** @sly2-annot addr=0x11A058
 * Purpose: Clears a singly-linked entry (next = NULL)
 * Info: proposed name ClearSle (T1); TU dl
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void func_0011A058(s32 *arg0) {
    *arg0 = 0;
}
