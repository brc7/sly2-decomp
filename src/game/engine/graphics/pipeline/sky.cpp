// sky.cpp -- SKY (cid 0x61), the sky dome: a solid object re-centred on the camera every frame so it always surrounds
// the view (it keeps its own height unless followCameraZ is set). Entry points: SKY::PostLoad (post-load), UpdateSky.
// TU 0x1EF850..0x1EFA90 (4 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_sky
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_sky
#include "sly2/slotheap.h"

// 0x1EF938 StaticInit_sky and GlobalConstructors_sky: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#include "sly2/cm.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
union V { qword q; float f[4]; };
/* SKY's own field (classes.h SKY has only padding past SO). */


extern "C" {
void PostSoLoad(SKY *pso);
void SetLoBusyKind(SKY *plo, int busyk);
void UpdateSo(SKY *pso);
qword GetLoWorldPos(SKY *pso);
}

// Returns the z component (word 2) of a quadword vector.

// Post-load hook (vtable slot 15): SO post-load, busy level 1, sets flag bit 1 in the +0x40 flags (through the pointer
// when it holds one) and clears SO flag bit 35, which keeps the sky out of the collision sphere tree (inferred).
// Also known as: PostLoadSky (upstream readable / Sly 1 name; PostLoadSky__FP3SKY).
void SKY::PostLoad()
{
    SKY *sky = this;
    sptr flagsWord;
    PostSoLoad(sky);
    SetLoBusyKind(sky, 1);
    flagsWord = (sptr)sky->ploupd;   /* a flags word, or (low bit clear) a pointer to one */
    if ((flagsWord ^ 1) & 1) ((LOUPD *)flagsWord)->grf |= 2;
    else sky->unknown40 = flagsWord | 2;
    sky->isRenderable = 0;
}

// Per-frame update (vtable slot 16): SO update, then moves the sky to the camera position (vtable slot 29, translate),
// keeping its own height unless followCameraZ is set.
// Also known as: UpdateSky (upstream readable / Sly 1 name; UpdateSky__FP3SKY).
extern "C" void UpdateSky(SKY *sky)
{
    union V target;
    UpdateSo(sky);
    CMMATQ *pmatq = &g_cm.matq;   /* through a pointer: read at base &g_cm.mat, offset 0x30 (g_cm.matq.pos is base &g_cm, offset 0x90) */
    target.q = pmatq->pos;
    if (!sky->followCameraZ) target.f[2] = QwordZ(GetLoWorldPos(sky));
    sky->TranslateToPos(target.q);
}
