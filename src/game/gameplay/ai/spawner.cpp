// spawner.cpp -- the guard spawner (CID119, cid 0x77, an LO of 0xB0 bytes). It keeps spawn selectors (an LO spot,
// a small spot, or a curve sampled along its length), guard templates and test offsets, and a fixed number of
// spawned-guard slots; every few seconds it picks a free slot and a spawn point the camera cannot see (screen
// rectangle, distance, line of sight from the camera) and clones a guard there. Entry points: the CID119 slots
// (Init, LoadSpawnerFromBrx, PostSpawnerLoad, UpdateSpawner, RemoveSpawnerHierarchy) and the Splice options
// AddSpawner*. VU0 and EE asm; GetSpawnPointConcealment, UpdateSpawner, UpdateSpawnerEntries and
// ChooseSpawnerSpawnPoint are hand-tuned (register pins, allocation-only operands).
// TU 0x173158..0x174898 (21 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_spawner
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_spawner
#include "sly2/slotheap.h"
#include "sly2/port.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/sw.h"
#include "sly2/lsg.h"   /* LSG: the line clip's span records */

// 0x174740 StaticInit_spawner and GlobalConstructors_spawner: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_BHVGUARD = 0x9C,
};

typedef int qwordV __attribute__((mode(TI)));
union U { qwordV q; float f[4]; };
struct M3 { qwordV row[3]; };
struct M4 { qwordV r[4]; };
struct SCOPE { int a[4]; };
struct AI { int cb; int align; };

/* The spawner is classes.h CID119 (SPAWNSELECTOR, SPAWNENTRY), its guards STEPGUARD. */
/* Spawn selector kinds (SPAWNSELECTOR::kind). */
enum { SELECTOR_SPOT = 0, SELECTOR_CURVE = 1, SELECTOR_SMALL_SPOT = 2 };
/* Most selectors, templates and test offsets a spawner holds. */
enum { MAX_SELECTORS = 128, MAX_TEMPLATES = 8, MAX_OFFSETS = 16 };
/* SPAWNENTRY::status: free (ready at readyTime), or waiting for the player to move away before re-arming. */
enum { ENTRY_FREE = 0, ENTRY_RESPAWN_DELAY = 1 };
/* UpdateSpawner's event record (the hero's last noise) and the message argument it sends. */
struct EVENT{float time;char pad4[8];LO*tag;char pad10[0x10];EVENT*next;char pad24[4];int kind;LO*previous;};
struct LINK{LO**pp;};
/* SnapToGround's line hit. */
union HITY { LSG lsg; struct { qwordV v; char pad[0x18]; float normalZ; char pad2[0x14]; float unknown40; }; };   /* hit point, normal at +0x20 */
struct VU_FLOAT { qwordV qw; };

extern qwordV D_002A2DD0;
extern qwordV g_normalZ;
/* spawner's .data (address order). */
/* Spawner tuning (data symbols, so they keep their address names):
   B8 test-radius scale;
   BC nearest camera depth;
   C0 / C4 screen rect (on screen / away from the edge);
   C8 occlusion test point raise;
   CC how long an alert event (mode 1) gates spawning, s;
   D0 spawn check interval, s;
   D4 how far the player must move before a full spawner spawns again; D8 delay after a spawn, s;
   DC re-arm distance of a respawn-delayed slot; E0 a slot's guard is replaceable after this many s;
   E4 nearest spawn distance; E8 curve sample spacing; F0 ground probe {bottom, top} offsets. */
float D_002BE5B8=0.8f,D_002BE5BC=500.0f,D_002BE5C0=1.7f,D_002BE5C4=1.25f,D_002BE5C8=100.0f;
float D_002BE5CC=10.0f,D_002BE5D0=1.0f,D_002BE5D4=300.0f,D_002BE5D8=2.5f;
float D_002BE5DC=3500.0f;
float D_002BE5E0=4.0f;
float D_002BE5E4=1500.0f;
float D_002BE5E8=200.0f;
float D_002BE5F0[2]={-350.0f,100.0f};
extern "C" {
extern const float D_002A53E0;   /* defined below where the ELF has it (.rodata order) */
extern LM g_lmZeroOne;
void InitLo(CID119 *p);
void LoadLoFromBrx(CID119 *p, void *pbis);
void PostLoLoad(CID119 *p);
void UpdateLo(CID119 *p);
void RemoveLoHierarchy(CID119 *p);
void SetLoBusyKind(CID119 *p, int n);
SCOPE *STACKSCOPE_ctor(SCOPE *pscope);
void STACKSCOPE_ctor_1AE4A0(SCOPE *pscope, int n) __asm__("_$_10STACKSCOPE");
void *AllocClearedFromStack(AI *pa);
void *AllocFromStack(AI *pa);
void *AllocCopyFromWorldHeap(AI *pa, void *pv);
void *AllocClearedFromWorldHeap(AI *pa);
void *FindSwObject__FP2SWi3OIDP2LO(void *g, int a, int b, LO *c);
void AddGuardScriptFollowPath(CID113 *o, int a, int b, int c);
int ShouldKeepSoNotSelfSlot62(void);
void IntersectSwBoundingBox(void *g, qwordV a, qwordV b, int (*fn)(void), int z, int *pcObject, SOPART ***papObject);
int FindFirstObjectHitByLine(int n, qwordV a, qwordV b, int c, void *d, void *phit);
qwordV ConvertCmWorldToScreen(CM*,qwordV);
LO *GetHeroPo(void *g);
qwordV GetLoWorldPos(void *p);
M4 *GetLoWorldMatrix(void *p);
EVENT *FindTargetRecordById(LO *object);
void SetSoAlpha(STEPGUARD *,float);
void FadeSoIn(STEPGUARD *,float);
void SetDetectorTargetStateById(DETECTOR *pdet,LO *object,int k);
void CollectVisibleMsGlobs(int *, void ***);
float GetShapeMaxDistance(LO *);
int FindRangeContaining(void *, float);
void EvaluateShapeAtDistance(LO *, qwordV *, qwordV *, float);
float atan2f(float, float);
M3 BuildAxisAngleRotationMatrix(float, qwordV);
STEPGUARD *AcquireProxyObject(void *p, int n);
void *FindStepBehavior(STEPGUARD *r, int n);
void SetBhvguardMode(void *o, BHVGUARDRECDATA *f);
void AddSpawnerOffset(CID119 *p, qwordV q);
int IsInScreenRect(qwordV v, float r);
int SnapToGround(qwordV *pv, float a, float b, int k, int (*fn)(void));
int GetSpawnPointConcealment(CID119 *p, qwordV pos, int count, void **objects, float distance);
void ReleaseSpawnerEntry(CID119 *p, SPAWNENTRY *pe, int k, int f);
qwordV GetSpawnerFocusPosition(CID119 *p);
void UpdateSpawnerEntries(CID119 *p, qwordV pos);
SPAWNENTRY *FindFreeSpawnerEntry(CID119 *p, LO *object);
SPAWNSELECTOR *ChooseSpawnerSpawnPoint(CID119 *spawner, qwordV pos, qwordV *outPosition, M3 *outRotation, int *outIsFlipped);
STEPGUARD *SpawnGuardFromSpawner(CID119 *spawner, SPAWNSELECTOR *selector, qwordV *position, M3 *rotation, int isFlipped);
}
float RandomFloatInRange(float gMin, float gMax);
int RandomIntInRange(int nMin, int nMax);
CID113 *CreateLo(int cid, void *psoParent, int oid, int isplice) __asm__("CreateLo__F3CIDP2SO3OIDi");

// 0x173158: Spawner (CID119) vtable slot 7 (Init): LO Init, then the defaults: spawn distance 5500, capacity/counts
// 1, respawn delays 30 and 50 s.
void CID119::Init()
{
    CID119 *spawner = this;
    InitLo(spawner);
    spawner->planarRange = 5500.0f;
    spawner->useEdgeTest = spawner->useOcclusionTest = spawner->useRangeTest = 1;
    spawner->respawnDelay = 30.0f;
    spawner->respawnDelay2 = 50.0f;
}

// Fills an allocation request (size, alignment).
static inline AI *SetAi(AI *request, int size, int align) { request->cb = size; request->align = align; return request; }
// 0x1731B8: Spawner vtable slot 14 (LoadFromBrx): preallocates the selector, template and offset arrays at their
// maximum sizes, loads the LO (Splice options fill them), then shrinks them.
extern "C" void LoadSpawnerFromBrx(CID119 *spawner, void *bis) {
    SCOPE scope;
    AI request;
    STACKSCOPE_ctor(&scope);
    spawner->selectors = (SPAWNSELECTOR *)AllocClearedFromStack(SetAi(&request, MAX_SELECTORS * sizeof(SPAWNSELECTOR), 0x10));
    spawner->offsets = (qword *)AllocFromStack(SetAi(&request, MAX_OFFSETS * sizeof(qwordV), 0x10));
    spawner->templates = (LO **)AllocFromStack(SetAi(&request, MAX_TEMPLATES * sizeof(LO *), 0x10));
    AddSpawnerOffset(spawner, D_002A2DD0);   // the zero offset: the spawn point itself is always tested
    LoadLoFromBrx(spawner, bis);
    spawner->selectors = (SPAWNSELECTOR *)AllocCopyFromWorldHeap(SetAi(&request, spawner->selectorCount * sizeof(SPAWNSELECTOR), 0x10), spawner->selectors);
    {
        void *offsets = AllocCopyFromWorldHeap(SetAi(&request, spawner->offsetCount << 4, 0x10), spawner->offsets);
        void *templates = spawner->templates;
        spawner->offsets = (qword *)offsets;
        spawner->templates = (LO **)AllocCopyFromWorldHeap(SetAi(&request, MAX_TEMPLATES * sizeof(LO *), 0x10), templates);
    }
    STACKSCOPE_ctor_1AE4A0(&scope, 2);
}

// 0x1732E0: Splice option: appends a spawn selector of kind 0 (an LO spot) by oid (at most 128).
extern "C" void AddSpawnerLoSpot(CID119 *spawner, int oid)
{
    int count = spawner->selectorCount;
    if (count < MAX_SELECTORS) {
        SPAWNSELECTOR *selector;
        spawner->selectorCount = count + 1;
        selector = &spawner->selectors[count];
        selector->oid = oid;
        selector->kind = SELECTOR_SPOT;
    }
}

// 0x173318: Splice option: appends a spawn selector of kind 1 (a curve, sampled along its length).
extern "C" void AddSpawnerCurve(CID119 *spawner, int oid)
{
    int count = spawner->selectorCount;
    if (count < MAX_SELECTORS) {
        SPAWNSELECTOR *selector;
        spawner->selectorCount = count + 1;
        selector = &spawner->selectors[count];
        selector->kind = SELECTOR_CURVE;
        selector->oid = oid;
    }
}

// 0x173358: Splice option: appends a spawn selector of kind 2 (an LO spot tested with the minimum radius).
extern "C" void AddSpawnerSmallLoSpot(CID119 *spawner, int oid)
{
    int count = spawner->selectorCount;
    if (count < MAX_SELECTORS) {
        SPAWNSELECTOR *selector;
        spawner->selectorCount = count + 1;
        selector = &spawner->selectors[count];
        selector->kind = SELECTOR_SMALL_SPOT;
        selector->oid = oid;
    }
}

// 0x173398: Splice option: appends a guard template object (at most 8).
extern "C" void AddSpawnerTemplate(CID119 *spawner, LO *guardTemplate)
{
    if (spawner->templateCount < MAX_TEMPLATES) {
        spawner->templates[spawner->templateCount++] = guardTemplate;
    }
}

// 0x1733C8: Splice option: appends a test offset (at most 16): points around a spawn point that must also be hidden
// from the camera.
// (Register-pinned, and the store spelled as byte arithmetic, for the match.)
extern "C" void AddSpawnerOffset(CID119 *spawner, qwordV offset)
{
    register int newCount __asm__("$3");
    register int count __asm__("$6");
    count = spawner->offsetCount;
    if (count < MAX_OFFSETS) {
        register qwordV *offsets __asm__("$4");
        register int byteOffset __asm__("$2");
        offsets = (qwordV *)spawner->offsets;
        byteOffset = count * 16;
        newCount = count + 1;
        spawner->offsetCount = newCount;
        *(qwordV *)(byteOffset + (sptr)offsets) = offset;
    }
}

// 0x1733F8: Spawner vtable slot 15 (PostLoad): resolves the selector oids; for curves creates a guard script (cid
// 0x71) with a goto; allocates the per-slot entries and registers for updates.
// Also known as: StartSpawner (upstream readable / Sly 1 name; StartSpawner__FP2SW).
extern "C" void PostSpawnerLoad(CID119 *spawner) {
    int i;
    AI al;
    uptr f;
    PostLoLoad(spawner);
    for (i = 0; i < spawner->selectorCount; i++) {
        SPAWNSELECTOR *selector = &spawner->selectors[i];
        switch (selector->kind) {
        case SELECTOR_CURVE:
            {
                // A curve spawn walks its guard along the curve: a script (cid 0x71) with one goto command.
                CID113 *script;
                selector->object = (LO *)FindSwObject__FP2SWi3OIDP2LO(&g_sw, 5, selector->oid, 0);
                script = CreateLo(0x71, 0, 0x2CB, -1);
                AddGuardScriptFollowPath(script, selector->oid, 1, -1);
                selector->scriptCommand = (SSGE *)script->commands.tail;   /* its one command */
                selector->script = script;
            }
            break;
        case SELECTOR_SPOT:
        case SELECTOR_SMALL_SPOT:
            selector->object = (LO *)FindSwObject__FP2SWi3OIDP2LO(&g_sw, 5, selector->oid, 0);
            break;
        }
    }
    al.cb = spawner->entryCapacity * sizeof(SPAWNENTRY);
    al.align = 0x10;
    spawner->entries = (SPAWNENTRY *)AllocClearedFromWorldHeap(&al);
    SetLoBusyKind(spawner, 1);
    f = (uptr)spawner->ploupd;
    if (!(f & 1)) ((LOUPD *)f)->grf |= 2;   // (bit 0 clear: the word is a pointer to the real flags)
    else spawner->inlineFlags = f | 2;
}

// VU0 helper: x component of a vector.
// VU0 helper: y component of a vector.
// VU0 helper: z component of a vector.
// 0x173588: True if a projected point is in front of the camera (z > 1e-4) and within +-`extent` in x and y.
extern "C" int IsInScreenRect(qwordV v, float extent) {
    int isInside = 0;
    if (QwordZ(v) > 0.0001f) {
        float x = QwordX(v);
        if (-extent < x && x < extent) {
            float y = QwordY(v);
            if (-extent < y && y < extent) isInside = 1;
        }
    }
    return isInside;
}

// VU0 helper: p + up * s.
static __inline__ qwordV VuMulAddScalar(qwordV p, qwordV up, float s) {
    U u;
    qwordV uq;   /* the asm writes a scalar, stored into the union after (see "asm output" in the merge guide) */
    qwordV r;
    uq = VuFromFloat<qwordV>(s);
    u.q = uq;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(p), "j"(up), "j"(u.q));
    return r;
}

// 0x173610: Casts a line along the up axis (from pos + `top` to pos + `bottom`) against the world's objects; on a
// hit with a walkable normal (z >= 0.7) moves the position to the hit and returns true.
// Also known as: FDropOnGround (upstream readable / Sly 1 name; FDropOnGround__FP6VECTORffiPv).
extern "C" int SnapToGround(qwordV *position, float top, float bottom, int acceptAnySlope, int (*filter)(void)) {
    SCOPE scope;
    int objectCount;
    SOPART **objects;   /* the parts in the box (IntersectSwBoundingBox) */
    qwordV start, end;
    STACKSCOPE_ctor(&scope);
    start = VuMulAddScalar(*position, g_normalZ, top);   // g_normalZ: the up axis
    end = VuMulAddScalar(*position, g_normalZ, bottom);
    if (!filter) filter = ShouldKeepSoNotSelfSlot62;
    {
        HITY hit;
        IntersectSwBoundingBox(&g_sw, start, end, filter, 0, &objectCount, &objects);
        if (!FindFirstObjectHitByLine(1, start, end, objectCount, objects, &hit)
            || hit.unknown40 == 0.0f || (!acceptAnySlope && hit.normalZ < 0.7f)) {
            STACKSCOPE_ctor_1AE4A0(&scope, 2);
            return 0;
        }
        *position = hit.v;
    }
    STACKSCOPE_ctor_1AE4A0(&scope, 2);
    return 1;
}

// Loads a quadword into a CPU register (register-pinned for the match).
static inline qwordV LoadCpu(const U&u){register qwordV q asm("$5");__asm__("lq %0,%1":"=r"(q):"m"(u.q));return q;}
// VU0 helper: loads a quadword into a VU register.
static inline qwordV Load(const U&u){register qwordV q asm("vf3");__asm__("lqc2 %0,%1":"=j"(q):"m"(u.q));return q;}
// VU0 helper: length of a vector (third instruction pattern for the match).
static inline float Len3(qwordV a, const U &u)
{
    qwordV b = Load(u);
    register qwordV d asm("vf1");
    float f;
    d = VuDot(a, b);
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "0"(d));
    f = VuToFloat(d);
    return f;
}
// VU0 helper: length of a vector (alternate instruction pattern for the match).
static inline float Len2(qwordV input)
{
    register qwordV a asm("vf2") = input, d asm("vf1");
    float f;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(a));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "0"(d));
    f = VuToFloat(d);
    return f;
}
// Loads the camera and position operands (register-pinned asm for the match).
static inline void ReadInputs(CM *source, const U &position, qwordV &cpu, qwordV &point, qwordV &vector)
{
    register CM *p asm("$2") = source;
    register qwordV r asm("$5"), v asm("vf1"), o asm("vf2");
    __asm__("lq %0,%3\n\tlqc2 %1,%4\n\tqmtc2.ni %0,%2" : "=r"(r), "=j"(v), "=j"(o) : "m"(p->matq.pos), "m"(position.q));
    cpu = r;
    point = v;
    vector = o;
}
// z component of a quadword vector.
static inline float Z(qwordV a)
{
    register int bits asm("$5");
    union {
        int n;
        float f;
    } value;
    __asm__("pextuw %0,$0,%1" : "=r"(bits) : "r"(a));
    value.n = bits;
    return value.f;
}
// True if a vector is longer than `limit`.
static inline int TooFar(qwordV a,float limit){return limit<Len2(a);}
// 0x173768: Camera concealment of a spawn point: 0 if it is visible (or too far / behind / off screen: unusable),
// else 1 at the screen edge, 2 out of the planar range, 3 occluded.
extern "C" int GetSpawnPointConcealment(CID119 *spawner, qwordV pos, int count, void **objects, float distance)
{
    // (Register pins and the U unions reproduce the original's register and stack use.)
    register int numObjects asm("$19") = count;
    register void **objectList asm("$18") = objects;
    U raised, origin, position, difference;
    register CM *camera asm("$2") = &g_cm;
    position.q = pos;
    qwordV cpu, point, vector;
    ReadInputs(camera, position, cpu, point, vector);
    qwordV delta = VuSub(point, vector);
    origin.q = cpu;
    difference.q = delta;
    distance *= D_002BE5B8;
    if (distance < Len3(difference.q, difference))
        return 0;   // too far from the camera
    qwordV screen = ConvertCmWorldToScreen(camera, LoadCpu(position));
    register float zLimit asm("$f1") = D_002BE5BC;
    if (Z(screen) < zLimit)
        return 0;   // behind or too close to the camera
    if (!IsInScreenRect(screen, D_002BE5C0))
        return 0;   // off screen
    if (spawner->useEdgeTest && !IsInScreenRect(screen, D_002BE5C4))
        return 1;   // at the screen edge
    if (spawner->useRangeTest && TooFar(difference.q, spawner->planarRange))
        return 2;   // out of the planar range
    if (spawner->useOcclusionTest) {
        {
            register qwordV copied asm("$5") = position.q;
            raised.q = copied;
        }
        raised.f[2] += D_002BE5C8;
        if (FindFirstObjectHitByLine(0, origin.q, raised.q, numObjects, objectList, 0))
            return 3;   // occluded
    }
    return 0;
}

// 0x173928: Ends a spawned-guard slot: kind 0 removes the guard (vtable slot 9, RemoveLo), kind 1 arms a respawn
// delay; decrements the live count.
// Also known as: ReleaseSlot (upstream readable / Sly 1 name; ReleaseSlot__FP7SPAWNERP4SLOTii).
extern "C" void ReleaseSpawnerEntry(CID119 *spawner, SPAWNENTRY *slot, int status, int keepSelectorReady) {
    slot->status = status;
    switch (status) {
    case ENTRY_FREE:
        {
            STEPGUARD *guard = slot->guard;
            slot->readyTime = g_clock.time;
            guard->Remove();
        }
        if (!(keepSelectorReady & 1)) slot->selector->busyUntil = g_clock.time;
        break;
    case ENTRY_RESPAWN_DELAY:
        slot->readyTime = g_clock.time + spawner->respawnDelay;
        slot->position = GetSpawnerFocusPosition(spawner);
        if (!(keepSelectorReady & 1)) slot->selector->busyUntil = g_clock.time + spawner->respawnDelay2;
        break;
    }
    slot->guard = 0;
    slot->selector->isBusy = 0;
    slot->selector = 0;
    spawner->entryCount--;
}

// 0x173A18: Returns the focus position the spawner works from: the current player's position, or the camera's.
// Also known as: PosCenter (upstream readable / Sly 1 name; PosCenter__Fv).
extern "C" qwordV GetSpawnerFocusPosition(CID119 *spawner)
{
    void *world = &g_sw;
    if (GetHeroPo(world))
        return GetLoWorldPos(GetHeroPo(world));
    return g_cm.matq.pos;   // the camera position
}

// g_clock must be a struct: a scalar float global is assumed not to alias the struct-field stores, which lets gcc CSE the clock reloads.
static inline float DistanceXY(qwordV a, qwordV b)
{
    qwordV d;
    float f;
    d = VuSub(a, b);
    __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y" : "+j"(d));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "0"(d));
    f = VuToFloat(d);
    return f;
}
// 0x173A68: Spawner vtable slot 16 (Update): periodically, when a slot is free (and gated by the player's last
// alert event), spawns a guard at a concealed spawn point, aims it at the player and sends it message 0x19.
// Also known as: UpdateSpawner (upstream readable / Sly 1 name; UpdateSpawner__FP2LO).
extern "C" void UpdateSpawner(CID119 *spawner)
{
    U point;
    M3 rotation;
    int isFlipped;
    STEPGUARD *guard;
    LINK link;
    qwordV pos;
    EVENT *event;
    SPAWNENTRY *entry;
    SPAWNSELECTOR *selector;
    LO *hero;
    UpdateLo(spawner);
    pos = GetSpawnerFocusPosition(spawner);
    UpdateSpawnerEntries(spawner, pos);
    if (g_clock.time < spawner->nextSpawnTime)
        return;
    spawner->nextSpawnTime = g_clock.time + D_002BE5D0;
    if (spawner->entryCount == spawner->entryCapacity) {
        // Full: only recycle a slot once the player has moved away from the last spawn.
        int isClose = DistanceXY(pos, spawner->lastSpawnPosition) < D_002BE5D4;   // (an int temporary: needed)
        if (isClose)
            return;
    }
    event = 0;
    hero = GetHeroPo(&g_sw);
    if (hero) {
        event = FindTargetRecordById(hero);
        if (event) {
            if (!event->kind)
                event = 0;
            else switch (spawner->mode) {
            case 0:   // never spawn while the player has an alert event
                return;
            case 1:   // only within D_002BE5CC seconds of it
                if (g_clock.time - event->time > D_002BE5CC)
                    return;
                break;
            case 2:
                break;
            }
        }
    }
    entry = FindFreeSpawnerEntry(spawner, event && event->kind == 1 ? event->previous : 0);
    if (!entry)
        return;
    selector = ChooseSpawnerSpawnPoint(spawner, pos, &point.q, &rotation, &isFlipped);
    if (!selector)
        return;
    spawner->lastSpawnPosition = pos;
    if (entry->guard)
        ReleaseSpawnerEntry(spawner, entry, ENTRY_FREE, 1);
    guard = SpawnGuardFromSpawner(spawner, selector, &point.q, &rotation, isFlipped);
    if (!guard)
        return;
    SetSoAlpha(guard, 0.01f);
    FadeSoIn(guard, 0.5f);
    if (event)
        SetDetectorTargetStateById(guard->pdetector, event->tag, 2);   // the event's source starts as "lost"
    entry->guard = guard;
    entry->activeTime = g_clock.time;
    entry->selector = selector;
    spawner->entryCount++;
    selector->isBusy = 1;
    link.pp = (LO **)&guard;
    spawner->nextSpawnTime = g_clock.time + D_002BE5D8;
    // vtable slot 3 (HandleSpliceEvent): message 0x19 "guard spawned" with the guard.
    spawner->HandleSpliceEvent(0x19, 0, 1, (void **)&link, 0);
}

// UpdateSpawnerEntries (Claude agent, 2026-10-25): MATCH. Updates the per-entry state of an array of 0x30-byte records (e->po object,
// e->k state, g4 max, g8 clock stamp). The VU helper layers (VsubXyzw, operator-, LengthXY, DistXYImpl, DistXY) pass VU_VECTORs
// by value; they add ~30 RTL insns to the loop (pass 1: 90, pass 2: 81 > threshold) so loop.c hoists the clock's %hi only in
// the second pass, giving the target's preheader order (1, hi BE5DC, giv 0, hi clock). Fewer layers (86) or more (94+) differ.
struct VU_VECTOR { qwordV q; VU_VECTOR() {} VU_VECTOR(qwordV a) { q = a; } };
// VU0 helper: vector difference.
static inline VU_VECTOR VsubXyzw(VU_VECTOR a, VU_VECTOR b) { qwordV d; d = VuSub(a.q, b.q); return VU_VECTOR(d); }
// VU0 helper: x*x + y*y of a vector.
static inline VU_VECTOR SumXY(VU_VECTOR a)
{
    qwordV d;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(a.q));
    return VU_VECTOR(d);
}
// VU0 helper: square root of a VU vector's x.
static inline VU_VECTOR SqrtV(VU_VECTOR a)
{
    qwordV d;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a.q));
    return VU_VECTOR(d);
}
// Return a - b.
static inline VU_VECTOR operator-(VU_VECTOR a, VU_VECTOR b) { return VsubXyzw(a, b); }
// VU0 helper: horizontal (xy) length as a VU vector.
static inline VU_VECTOR LengthXYV(VU_VECTOR a) { return SqrtV(SumXY(a)); }
// VU0 helper: horizontal (xy) length of a vector.
static inline float LengthXY(VU_VECTOR a) { return VuToFloat(LengthXYV(a).q); }
// VU0 helper: horizontal (xy) distance between two points.
static inline float DistXYImpl(VU_VECTOR a, VU_VECTOR b) { return LengthXY(a - b); }
// VU0 helper: horizontal (xy) distance.
static inline float DistXY(VU_VECTOR a, VU_VECTOR b) { return DistXYImpl(a, b); }
// 0x173D18: Releases slots whose guard died or was disabled and re-arms slots once the player is far enough away.
// Also known as: CheckSlots (upstream readable / Sly 1 name; CheckSlots__FP7SPAWNERG9VU_VECTOR).
extern "C" void UpdateSpawnerEntries(CID119 *spawner, qwordV pos) {
    int i;
    for (i = 0; i < spawner->entryCapacity; i++) {
        SPAWNENTRY *entry = &spawner->entries[i];
        STEPGUARD *guard = entry->guard;
        if (!guard) {
            if (entry->status == ENTRY_RESPAWN_DELAY) {
                int isFarEnough = D_002BE5DC < DistXY(VU_VECTOR(entry->position), VU_VECTOR(pos));
                if (isFarEnough) {
                    entry->status = ENTRY_FREE;
                    entry->readyTime = g_clock.time;
                }
            }
        } else if (guard->isKnockedOut) {
            ReleaseSpawnerEntry(spawner, entry, ENTRY_RESPAWN_DELAY, 0);
        } else if (((int)guard->isInWorld & 1) == 0 || ((int)entry->selector->object->isInWorld & 1) == 0) {
            // the guard or its spawn object left the world
            ReleaseSpawnerEntry(spawner, entry, ENTRY_FREE, 0);
        } else {
            entry->activeTime = (entry->activeTime > guard->model.unknown04) ? entry->activeTime : guard->model.unknown04;
        }
    }
}

// 0x173E78: Picks the slot to use: the oldest free one, or the oldest occupied one past the timeout.
// Also known as: PslotChoose (upstream readable / Sly 1 name; PslotChoose__FP7SPAWNERi).
extern "C" SPAWNENTRY *FindFreeSpawnerEntry(CID119 *spawner, LO *object) {
    float oldestActiveTime = g_clock.time - D_002BE5E0; // occupied slots must be older than this
    float oldestReadyTime = g_clock.time;
    SPAWNENTRY *best = 0;
    SPAWNENTRY *slot;
    int i;
    for (i = 0; i < spawner->entryCapacity; i++) {
        slot = &spawner->entries[i];
        if (slot->guard != 0) {
            if ((LO *)slot->guard == object) continue;   // never replace the excluded guard
            if (best && best->guard == 0) continue;       // a free slot beats any occupied one
            if (oldestActiveTime <= slot->activeTime) continue;
            oldestActiveTime = slot->activeTime;
        } else {
            if (oldestReadyTime <= slot->readyTime) continue;
            oldestReadyTime = slot->readyTime;
        }
        best = slot;
    }
    return best;
}

// VU0 helper: loads a float constant from a .data word into a VU register.
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    // volatile: the constant is rebuilt at each use (loop.c must not hoist it)
    __asm__ __volatile__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
// VU0 helper: vector difference qw0 - qw1.
static inline qwordV VuDifference(const qwordV &qw0, qwordV qw1)
{
    qwordV qw;
    qw = VuSub(qw0, qw1);
    return qw;
}
// VU0 helper: vector sum.
static inline qwordV VuSum(qwordV qw0, qwordV qw1)
{
    qwordV qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
// VU0 helper: x component of a quadword vector.
// VU0 helper: vector scaled by the x of a VU float.
static inline qwordV VuScaleByX(qwordV qwVec, const VU_FLOAT &g)
{
    qwordV qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
// VU0 helper: rotates a vector by a 3x3 matrix.
static inline qwordV VuRotateByMatrix(M3 *pmat, qword *pqwVec)
{
    qwordV qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(*pqwVec), "j"(pmat->row[0]), "j"(pmat->row[1]), "j"(pmat->row[2]));
    return qwResult;
}
// VU0 helper: horizontal (xy) length of a vector.
static inline float VectorLengthXY(qwordV qw)
{
    qwordV qwT;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qwT) : "j"(qw));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwT) : "j"(qwT));
    return QwordX(qwT);
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A53E0 __asm__("D_002A53E0") = 3.40282347e+38f; }
// 0x173F18: Picks the nearest usable spawn selector (LO spot or curve sample) where every test offset point is
// concealed from the camera; returns its position.
// Also known as: PsrcChoose (upstream readable / Sly 1 name; PsrcChoose__FP7SPAWNERG9VU_VECTORP6VECTORP7MATRIX3Pi).
extern "C" SPAWNSELECTOR *ChooseSpawnerSpawnPoint(CID119 *spawner, qwordV pos, qwordV *outPosition, M3 *outRotation, int *outIsFlipped)
{
    SCOPE frame;
    union { VECTOR4 v; qwordV q; } bestPosition;
    M3 bestRotation;
    int numObjects;
    void **objects;

    // The smallest template radius, used for curve samples and small spots.
    float minRadius = D_002A53E0;
    for (int i = 0; i < spawner->templateCount; i++)
    {
        float radius = spawner->templates[i]->busyRadius;
        minRadius = minRadius < radius ? minRadius : radius;
    }
    SPAWNSELECTOR *bestSelector;
    int bestIsFlipped = 0;
    STACKSCOPE_ctor(&frame);
    CollectVisibleMsGlobs(&numObjects, &objects);
    bestSelector = 0;
    float bestDistance = D_002A53E0;

    for (int i = 0; i < spawner->selectorCount; i++)
    {
        SPAWNSELECTOR *selector = &spawner->selectors[i];
        LO *object;
        if (selector->isBusy)
            continue;
        if (g_clock.time < selector->busyUntil)
            continue;
        object = selector->object;
        if (!object)
            continue;
        {
            int isInWorld = (int)object->isInWorld & 1;   // (the named temporary is needed for the match)
            if (!isInWorld)
                continue;
        }
        switch (selector->kind)
        {
        case SELECTOR_SPOT:
        case SELECTOR_SMALL_SPOT:
        {
            float radius;
            qwordV spotPosition;
            M3 rotation;
            switch (selector->kind)
            {
            case SELECTOR_SPOT:
                spotPosition = GetLoWorldPos(selector->object);
                rotation = *(M3 *)GetLoWorldMatrix(selector->object);
                radius = selector->object->busyRadius;
                break;
            case SELECTOR_SMALL_SPOT:
                radius = minRadius;
                spotPosition = GetLoWorldPos(selector->object);
                rotation = *(M3 *)GetLoWorldMatrix(selector->object);
                break;
            }
            float distance = VectorLengthXY(VuDifference(pos, spotPosition));
            if (distance < D_002BE5E4 || bestDistance < distance)
                continue;
            for (int j = 0; j < spawner->offsetCount; j++)
            {
                if (!GetSpawnPointConcealment(spawner, VuSum(spotPosition, VuRotateByMatrix(&rotation, &spawner->offsets[j])), numObjects, objects, radius))
                    goto nextSelector;   // some test point is visible: try the next selector
            }
            bestDistance = distance;
            bestSelector = selector;
            bestPosition.q = spotPosition;
            bestRotation = rotation;
            break;
        }
        case SELECTOR_CURVE:
        {
            // Samples the curve at random points about D_002BE5E8 apart.
            union { VECTOR4 v; qwordV q; } curvePosition, curveDirection;
            int numSamples = (int)(GetShapeMaxDistance(object) / D_002BE5E8);
            for (int k = 0; k < numSamples; k++)
            {
                float u = D_002BE5E8 * ((float)k + RandomFloatInRange(g_lmZeroOne.minValue, g_lmZeroOne.maxValue));
                if (FindRangeContaining(((SHAPE *)object)->pranges, u))
                    continue;
                EvaluateShapeAtDistance(object, &curvePosition.q, &curveDirection.q, u);
                float distance = VectorLengthXY(VuDifference(pos, curvePosition.q));
                if (distance < D_002BE5E4 || bestDistance < distance)
                    continue;
                // Face along the curve, toward the camera's x axis.
                int isFlipped = QwordX(VuDot<qwordV>(g_cm.matq.rot[0], curveDirection.q)) < 0.0f;
                if (!isFlipped)
                {
                    VU_FLOAT minusOne = VuFloatFromMemWord(0xBF800000);   // -1.0f
                    curveDirection.q = VuScaleByX(curveDirection.q, minusOne);
                }
                M3 rotation = BuildAxisAngleRotationMatrix(atan2f(curveDirection.v.y, curveDirection.v.x), g_normalZ);
                int j;
                for (j = 0; j < spawner->offsetCount; j++)
                {
                    if (!GetSpawnPointConcealment(spawner, VuSum(curvePosition.q, VuRotateByMatrix(&rotation, &spawner->offsets[j])), numObjects, objects, minRadius))
                        break;
                }
                if (j >= spawner->offsetCount)
                {
                    bestDistance = distance;
                    bestIsFlipped = isFlipped;
                    bestSelector = selector;
                    bestPosition.q = curvePosition.q;
                    bestRotation = rotation;
                }
            }
            break;
        }
        }
    nextSelector:;
    }
    *outPosition = bestPosition.q;
    *outRotation = bestRotation;
    *outIsFlipped = bestIsFlipped;
    STACKSCOPE_ctor_1AE4A0(&frame, 2);
    return bestSelector;
}

// 0x174490: Spawns a guard from a template at the chosen point (clone and activate) and optionally sets its root
// behaviour mode.
// Also known as: PloSpawn (upstream readable / Sly 1 name; PloSpawn__FP7SPAWNERP6SOURCEP6VECTORii).
extern "C" STEPGUARD *SpawnGuardFromSpawner(CID119 *spawner, SPAWNSELECTOR *selector, qwordV *position, M3 *rotation, int isFlipped) {
    STEPGUARD *guard = 0;
    int kind;
    switch (selector->kind) {
    case SELECTOR_SPOT:
        guard = AcquireProxyObject(selector->object, 0);
        break;
    case SELECTOR_CURVE:
    case SELECTOR_SMALL_SPOT:
      {
        int order[8];
        int i, j, l;
        SnapToGround(position, D_002BE5F0[1], D_002BE5F0[0], 1, 0);
        // Try the templates in a random order until one yields a free proxy.
        for (l = 0; l < spawner->templateCount; l++) order[l] = l;
        for (j = 0; j < spawner->templateCount; j++) {
            int other = RandomIntInRange(0, spawner->templateCount - 1);
            int swap = order[j];
            order[j] = *(other + order);   // (index-first addition order needed for the match)
            order[other] = swap;
        }
        for (i = 0; i < spawner->templateCount; i++) {
            LO *object = spawner->templates[order[i]];
            object->TranslateToPos(*position);
            object->RotateToMat((MATRIX3 *)rotation);   /* a 3x3 rotation (M3) */
            guard = AcquireProxyObject(object, 0);
            if (guard) break;
        }
      }
        kind = selector->kind;
        if (kind == SELECTOR_CURVE && guard) {
            // A curve spawn runs the selector's script, in the curve direction.
            void *bhvguard;
            selector->scriptCommand->n = isFlipped;   /* the follow-path command's direction */
            guard->patrolScript = selector->script;
            bhvguard = FindStepBehavior(guard, CID_BHVGUARD);
            BHVGUARDREC mode;
            mode.kind = -1;
            SetBhvguardMode(bhvguard, &mode);
            mode.kind = kind;
            SetBhvguardMode(bhvguard, &mode);
        }
        break;
    }
    if (guard) guard->removalMode = 1;
    return guard;
}

// 0x1746B8: Spawner vtable slot 12 (RemoveHierarchy): LO base, then releases every active slot.
// Also known as: OnRemoveSpawner (upstream readable / Sly 1 name; OnRemoveSpawner__FP2LO).
extern "C" void RemoveSpawnerHierarchy(CID119 *spawner) {
    int i;
    RemoveLoHierarchy(spawner);
    if (!spawner->entries) return;
    for (i = 0; i < spawner->entryCapacity; i++) {
        SPAWNENTRY *slot = &spawner->entries[i];
        if (slot->guard) ReleaseSpawnerEntry(spawner, slot, ENTRY_FREE, 0);
    }
}
