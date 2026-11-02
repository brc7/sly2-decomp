#include "m2c_prelude.h"
extern M2C_UNK D_002BAD2C;

/** @sly2-annot addr=0x258F30
 * Purpose: duplicate EMITTER class-descriptor getter (vtable slot 6 of the second, never-stored EMITTER vtable):
 *   returns the same record as GetEmitterClassDesc (0x113BE0)
 * Info: was proposed as EMITTER_PvOptionDefaults (wrong); _linkonce suffix keeps the symbol distinct from
 *   GetEmitterClassDesc; TU game_linkonce
 * Confidence: high - evidence: static analysis (same data record as GetEmitterClassDesc); sources:
 *   class_reference hidden-types
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
M2C_UNK *GetEmitterClassDesc_linkonce(void) {
    return &D_002BAD2C;
}
