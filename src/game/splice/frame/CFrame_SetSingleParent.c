#include "m2c_prelude.h"
M2C_UNK func_00256EE0(void *);                      /* extern */

/** @sly2-annot addr=0x127B18
 * Purpose: cframe(+8)=1; apframe[0] (+0xC, refcounting pointer assign 0x256EE0) = parent
 * Info: proposed name CFrame_SetSingleParent (T1); class CFrame; TU frame
 * Notes: names_v2 v1 proposed AddParent__6CFrameP6CFrame (0.020)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r1_116918
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
void CFrame_SetSingleParent(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = 1;
    func_00256EE0(arg0 + 0xC);
}
