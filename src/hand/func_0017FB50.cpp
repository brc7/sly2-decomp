#include "mat.h"
typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1C0]; int n1C0; char pad2[0x54]; float g218; int pad3; qword q220; };
extern "C" {
extern qword D_002A2DC0, D_002A2DD0;
extern char D_002BE800[], D_002BE810[];
MATRIX3 func_0011B500(qword q, float g);
void func_001F59A0(S *p, MATRIX3 *pm, void *pv);
void AccelSoTowardPosSpring(S *p, qword a, int b, qword c, void *pv);
void func_0017FB50(S *p);
}
/** @sly2-annot addr=0x17FB50
 * Purpose: vt70 override
 * Info: proposed name PresetHackshipAccel (T2); class HACKSHIP slot70; TU tu_17F090
 * Notes: CID48->CID72 (CID 0x48=72, vtable) | class identity from whole_game WE names/object strings:
 *   PresetCid72Accel -> PresetHackshipAccel
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/engine/minigames.md#22-input-and-ship-behaviour,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017FB50(S *p) {
    if (p->n1C0 == 3) return;
    MATRIX3 m; m = func_0011B500(D_002A2DC0, p->g218);
    func_001F59A0(p, &m, D_002BE800);
    AccelSoTowardPosSpring(p, D_002A2DD0, 0, p->q220, D_002BE810);
}
