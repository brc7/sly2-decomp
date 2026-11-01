#include "m2c_prelude.h"
M2C_UNK BindLoOidTable(s32, M2C_UNK, M2C_UNK *);     /* extern */
M2C_UNK LoadSoFromBrx();                            /* extern */
extern M2C_UNK D_002C0258;

/** @sly2-annot addr=0x1B1E00
 * Purpose: MISSILE::LoadFromBrx (vt14): base load (0x1F31E0) + SnipLoObjects(this,1,s_asnipMissile
 *   D_002C0258)
 * Info: proposed name LoadMissileFromBrx (T1); class MISSILE slot14; TU missile
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r5_1AE130+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void LoadMissileFromBrx__FP7MISSILEP18CBinaryInputStream(s32 arg0) {
    LoadSoFromBrx();
    BindLoOidTable(arg0, 1, &D_002C0258);
}
