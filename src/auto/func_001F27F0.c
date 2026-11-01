#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, void *);           /* extern */
void *PsoFindPhysRoot();                              /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1F27F0
 * Purpose: Ensure the SO has a contact-tree record (+0xEC): take the phys root's, creating it via vt53 and
 *   registering with g_sw (0x134320)
 * Info: proposed name EnsureSoTree (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F27F0(void *arg0) {
    void *temp_v0;
    void *temp_v1;

    if (M2C_FIELD(arg0, s32 *, 0xEC) == 0) {
        temp_v0 = PsoFindPhysRoot();
        if (M2C_FIELD(temp_v0, s32 *, 0xEC) == 0) {
            temp_v1 = M2C_FIELD(temp_v0, void **, 0xC);
            M2C_FIELD(temp_v0, s32 *, 0xEC) = M2C_FIELD(temp_v1, s32 (**)(void *), 0x1AC)(temp_v0 + M2C_FIELD(temp_v1, s16 *, 0x1A8));
            RecalcSwOxfFilterForObject(&D_003DD490, temp_v0);
        }
        M2C_FIELD(arg0, s32 *, 0xEC) = M2C_FIELD(temp_v0, s32 *, 0xEC);
    }
}
