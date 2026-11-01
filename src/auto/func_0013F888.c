#include "m2c_prelude.h"
extern M2C_UNK D_002CBE50;

/** @sly2-annot addr=0x13F888
 * Purpose: CLoadCollectorArray ctor: vptr(+0)=D_002CBE50, cMax=c=0, a=0
 * Info: proposed name CLoadCollectorArray_ctor (T1); class CLoadCollectorArray; TU brx
 * Notes: mangled dropped (base does not match final name): __19CLoadCollectorArray
 * Confidence: high [T1 0.88] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
void *func_0013F888(void *arg0) {
    M2C_FIELD(arg0, s32 *, 4) = 0;
    M2C_FIELD(arg0, M2C_UNK **, 0) = &D_002CBE50;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    return arg0;
}
