#include "sly2/so.h"
// SO+0x148 bit 33 (64-bit unit at 0x148). Lilly's port calls this bit fHurtRespawns; the function name says "iceable".
void SetSoIceable(SO *pso, int f) { pso->fHurtRespawns = f; }
