#include "m2c_prelude.h"
M2C_UNK func_0011CCC0(M2C_UNK *);                   /* extern */
extern M2C_UNK D_003E1AD8;

/** @sly2-annot addr=0x20C010
 * Purpose: alloc from slotheap D_003E1AD8
 * Info: proposed name PvAllocLeap (T2); TU tu_20B840
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020C010(void) {
    func_0011CCC0(&D_003E1AD8);
}
