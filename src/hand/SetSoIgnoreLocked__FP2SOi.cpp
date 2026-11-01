#include "sly2/so.h"
// SO+0xD0 bit 34 = GLOBSET+0x50 bit 34 (Lilly's port: globset.fUnk34).
void SetSoIgnoreLocked(SO *pso, int f) { pso->globset.fUnk34 = f; }
