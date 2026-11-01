#include "m2c_prelude.h"
/** @sly2-annot addr=0x11A050
 * Purpose: Singly linked list init: stores ibNext at +8
 * Info: proposed name InitSl (T1); TU dl
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/core-library.md#2-containers, docs/engine/core-library.md#6-source-files,
 *   docs/reference/functions/core-library.md#dl-0x11a050-0x11a550
 * @end-sly2-annot */
void func_0011A050(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 8) = arg1;
}
