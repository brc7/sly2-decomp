#include "m2c_prelude.h"
M2C_UNK func_002325E0(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */

/** @sly2-annot addr=0x233018
 * Purpose: 989snd: AndWait IOP command 0x5B with no data; returns IOP movie-audio position (used by
 *   CQueueOutputIop_Update as pos*blocksize) [989snd]
 * Info: proposed name snd_Cmd5B (T2); TU sdk_libs
 * Notes: sdk_names 989snd-style hypothesis snd_GetMovieNAX; master convention snd_CmdNN used (v1 snd_Cmd5C
 *   precedent)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void func_00233018(void) {
    func_002325E0(0x5B, 0, 0);
}
