// recon.cpp -- recon selection: Sly's photo/recon prompt (RECONSEL: candidate targets, screen projection, the
// selection state machine and its widgets) and the recon targets themselves (RECONTGT, CID 0x2E). TU 0x201D78..
// 0x203948 (26 functions, address order).
// Entry points: InitReconSel/PostReconSelLoad, UpdateReconSel (per frame), SetReconSelState, and the recon
// target class (InitReconTarget .. IsReconTargetSelectable).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_recon
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_recon
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/stackscope.h"
#include "sly2/cm.h"
#include "sly2/classes.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/sndmgr.h"
#include "sly2/sw.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"   /* JT::reconSel, sly2/recon.h RECONSEL */
// 0x2037F0 StaticInit_recon and GlobalConstructors_recon: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

union V { qword q; float f[4]; };
   /* sly2/basic.h's VECTOR has constructors (not in a union) */

/* Hand-made virtual calls: the vtable pointer of an object is at +0xC. */

/* basic.h's CID has no enumerators yet. */
#define CID_2E ((CID)0x2E)
#define CID_34 ((CID)0x34)
#define CID_35 ((CID)0x35)
#define CID_19 ((CID)0x19)


/* A recon target: classes.h CID46 (an XFM). */
typedef CID46 RECONTGT;

/* The hero mode the prompt belongs to (stepplayer.h JTMODE: its player, its kind). */
typedef JTMODE MODEL;


typedef STEPPLAYER PLAYER;

extern "C" {
extern PLAYER *D_002E1E40;

extern int D_002C3588[];
extern const float D_002A8550;   /* defined below where the ELF has it (.rodata order) */

int FindSwObjectsByClass(SW *psw, int grf, CID cid, LO *ploCtx, int c, RECONTGT **apl);
int IsPointInVol(LO *pvol, qword v);
qword GetLoWorldPos(void *p);
void FadeSoIn(LO *plo, float f);
void FadeSoOut(LO *plo, float f);
void PlayWidgetAnimWithArg(WIDGET *pw, int id, int v);
void IntersectSwBoundingBox(SW *, qword, qword, int (*)(void *, SOPART *), void *, int *, SOPART ***);
SOPART *FindFirstObjectHitByLine(int, qword, qword, int, SOPART **, LSG *);
int IsInCameraFrustum(CM *, qword, qword);
int IsWithinCameraRenderDistance(CM *, qword, float, float *);
void IncrementSwHandsOff(SW *, int);
void DecrementSwHandsOff(SW *, int);
void StopWidgetAnim(WIDGET *, int);
void PlayWidgetAnim(WIDGET *, int);
void SetWidgetExclusive(WIDGET *, int);
void SetClockRate(void *, int, float);
void SetSoundManagerFlags(void *, int, int);
void ResetCplookZoom(void *);
void *StartDialog(void *, int, int);
void StartFrameCapture(void *, int, int);
void BroadcastGameEvent(void *, int, void *);
void *CreateSfxSound(int);
void SetSndSourceLo(void *, void *);
void SetWidgetAnimPhase(WIDGET *, int, float);
void SetWidgetState(WIDGET *, int);
void SetWidgetRequest(WIDGET *, int);
int IsWidgetAnimPlaying(WIDGET *, int);
void EndJtMode(MODEL *);
qword ConvertCmWorldToScreen(CM *, qword);
int ConsumeButtonsPressedWithin__5POCTLUif(void *, int, float);
void InitXfm(RECONTGT *p);
void LoadXfmFromBrx(RECONTGT *p, void *pbis);
void PostLoLoad(RECONTGT *p);
void SubscribeLoObject(TASK *plink, RECONTGT *p);
void HandleBasicMessage(RECONTGT *p, int msg, void *pm);
PLAYER *GetHeroPo(SW *psw);
void RequestTaskState(TASK *plink, int n, RECONTGT *p);
void GatherJtModeHudTags(void *pv);
int IsTaskComplete(TASK *plink);

void ResetReconTargets(RECONSEL *p);
void NotifyReconTargets(RECONSEL *p);
void UpdateReconSelInVolume(RECONSEL *p);
void RefreshReconPendingTargets(RECONSEL *p);
void FadeReconTargetMarkers(RECONSEL *p);
void UpdateReconChecklist(RECONSEL *p);
void ClearReconTargetMarks(RECONSEL *p);
int IsReconSelAvailable(RECONSEL *p);
int FilterReconOccluder(void *, SOPART *);
void UpdateReconTargetVisibility(RECONSEL *p, RECONTGT *other);
void SelectReconTarget(RECONSEL *p);
void SetReconSelState(RECONSEL *p, int nextState);
void SetReconTargetDone(RECONTGT *p, int n);
int IsReconTargetDone(RECONTGT *p);
int IsReconTargetSelectable(RECONTGT *p);
}
int IsBasicDerivedFrom(BASIC *pbasic, CID cid);                    /* IsBasicDerivedFrom__FP5BASIC3CID */
LO *FindSwObject(SW *psw, int grf, OID oid, LO *ploCtx) __asm__("FindSwObject__FP2SWi3OIDP2LO");         /* FindSwObject__FP2SWi3OIDP2LO */
const char *FindSwLocalizedString(SW *psw, int oid);                    /* FindSwLocalizedString__FP2SWi */
int IsCameraPolicyActive(CPLCY *pcplcy);                                    /* IsCameraPolicyActive__FP5CPLCY */

#define IN_WORLD(o) ((int)(o)->isInWorld & 1)   /* LO::isInWorld (bit 60 of the word at +0x28 on the EE) */

// Initializes the recon (camera snapshot) selector: owner, aim origin at the screen centre (320, 246.4), state -1.
extern "C" void InitReconSel(RECONSEL *reconSel, MODEL *pmodel)
{
    VECTOR *pv = (VECTOR *)&reconSel->origin;
    float y;
    reconSel->model = pmodel;
    pv->x = 0;
    pv->y = 0;
    pv->z = 0;
    y = pv->y;
    pv->x = pv->x * 320.0f + 320.0f;
    pv->y = 246.40000915527344f;
    pv->z = y;
    reconSel->state = -1;
}

// Post-load: finds the selector's HUD widgets (0x362/0x365, Sly only) and object 0x602, and collects every recon target
// (CID 0x2E, at most 16).
extern "C" void PostReconSelLoad(RECONSEL *reconSel)
{
    if (IsBasicDerivedFrom((BASIC *)reconSel->model->player, CID_35)) {
        reconSel->primary = (WIDGET130 *)FindSwObject(&g_sw, 0x405, (OID)0x362, 0);
        reconSel->secondary = (WIDGET *)FindSwObject(&g_sw, 0x405, (OID)0x365, 0);
    }
    reconSel->resource = FindSwObject(&g_sw, 0x405, (OID)0x602, 0);
    reconSel->count = FindSwObjectsByClass(&g_sw, 0x405, CID_2E, 0, 16, reconSel->items);
    ResetReconTargets(reconSel);
}

// Removes each target's companion object (+0x100) from the world (LO::Remove). (inferred: its marker)
void ResetReconTargets(RECONSEL *reconSel)
{
    int i;
    for (i = 0; i < reconSel->count; i++) {
        LO *o = reconSel->items[i]->fadeObject;
        if (o) {
            ((LO *)o)->Remove();
        }
    }
}

// Calls virtual slot 8 on the companion of each visible target. (inferred)
void NotifyReconTargets(RECONSEL *reconSel)
{
    int i;
    for (i = 0; i < reconSel->count; i++) {
        RECONTGT *o = reconSel->items[i];
        if (IN_WORLD(o) && o->fadeObject) o->fadeObject->Add();
    }
}

// Sets the in-volume flag (+0xB8) when the aim origin is inside any volume of class 0x19.
void UpdateReconSelInVolume(RECONSEL *reconSel)
{
    LO *a[16];
    int n, i;
    reconSel->visible = 0;
    n = FindSwObjectsByClass(&g_sw, 0x505, CID_19, 0, 16, (RECONTGT **)a);
    for (i = 0; i < n; i++) {
        if (IsPointInVol(a[i], reconSel->position.q)) {
            reconSel->visible = 1;
            break;
        }
    }
}

// Rebuilds the pending list (at most 5): targets with a name id whose task state is 1 or 2.
void RefreshReconPendingTargets(RECONSEL *reconSel)
{
    int i;
    reconSel->cPending = 0;
    for (i = 0; i < reconSel->count; i++) {
        RECONTGT *o = reconSel->items[i];
        if (o->idPending != -1) {
            TASK *q = o->link;
            if (!q || q->state == 1 || q->state == 2) {
                if (reconSel->cPending < 5) reconSel->apending[reconSel->cPending++] = o;
            }
        }
    }
}

// Fades each target's marker in or out (0.25 s) by camera distance against its radius.
// Also known as: DistanceModels (upstream readable / Sly 1 name; DistanceModels__FP11PHOTOPROMPT).
void FadeReconTargetMarkers(RECONSEL *reconSel)
{
    int i;
    for (i = 0; i < reconSel->count; i++) {
        RECONTGT *o = reconSel->items[i];
        qword a, d;
        float f, r;
        if (!o->fadeObject)
            continue;
        a = GetLoWorldPos(o);
        __asm__("vsub.xyzw %0, %1, %2\n\tvmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a), "j"(g_cm.matq.pos));
        __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
        if (o->rFade < f && IN_WORLD(o->fadeObject) == 0)
            FadeSoIn(o->fadeObject, 0.25f);
        else if (f <= o->rFade && !!IN_WORLD(o->fadeObject))
            FadeSoOut(o->fadeObject, 0.25f);
    }
}

// Per pending target plays the checklist animation on its widget: 0x33A when done, 0x33B otherwise.
// Also known as: ListMarks (upstream readable / Sly 1 name; ListMarks__FP11PHOTOPROMPT).
void UpdateReconChecklist(RECONSEL *reconSel)
{
    int i;
    for (i = 0; i < (int)reconSel->cPending; i++) {
        int v;
        RECONTGT *a = reconSel->apending[i];
        v = D_002C3588[i];
        if (IsReconTargetDone(a))
            PlayWidgetAnimWithArg(reconSel->primary, 0x33A, v);
        else
            PlayWidgetAnimWithArg(reconSel->primary, 0x33B, v);
    }
}

// Clears each target's mark (+0x108).
void ClearReconTargetMarks(RECONSEL *reconSel) { int i; for (i = 0; i < reconSel->count; i++) reconSel->items[i]->state = 0; }

// True when the aim origin is in a recon volume or any target is visible.
int IsReconSelAvailable(RECONSEL *reconSel)
{
    int i;
    if (reconSel->visible) return 1;
    for (i = 0; i < reconSel->count; i++) {
        RECONTGT *o = reconSel->items[i];
        if (IN_WORLD(o) && (!o->fadeObject || IN_WORLD(o->fadeObject) == 0)) return 1;
    }
    return 0;
}

// Filter callback for the occlusion box query (the objects are SOPARTs: parts with camera nodes, not unknownFlag10A).
int FilterReconOccluder(void *a, SOPART *p) { int r = 0; if (!p->unknownFlag10A) r = p->bspc.numCameraNodes > 0; return r; }

/* UpdateReconTargetVisibility (Lilly / Claude agent, T1560): four-ray visibility of an actor from the selector's origin (p+0x20, the caller
 * SelectReconTarget raises it by 20). Collects the objects between the two points (IntersectSwBoundingBox with the FilterReconOccluder filter),
 * then casts rays from the origin to the actor's centre (pulled radius 0xE4 towards the viewer) and to that centre moved by
 * the radius along the camera's up row (g_cm.matq.rot[1]) and along the side direction up x view. Every ray that hits an object
 * counts: none -> state 1, all four -> state 2 (SelectReconTarget drops those), some -> state 3. No register pins.
 * What made it match: the actor position is a plain qword across the call (spilled to 0x70 by the compiler), the scale-by-radius
 * multiplies are in place on direction/up (so they wait for the cross product), the side vector's dot product names its
 * operand twice, and the case-4 constant is a variable set before the switch (the game has it in v1 in the beqz delay slot). */
struct QUERY {int count;SOPART **objects;};

// v / |v|: the dot product names v twice (the side vector's registers only match that way)
static inline qword Normalize(qword v)
{
    qword d,one,l,r;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v),"j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(l):"j"(d));
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(l),"j"(v));
    return r;
}
// v *= k.x in place
#define SCALE_IN_PLACE(v,k) __asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k))
// Four-ray occlusion test from the origin to a target: state 1 clear, 2 blocked, 3 partly visible.
void UpdateReconTargetVisibility(RECONSEL *reconSel, RECONTGT *other)
{
    int cHit = 0;
    unsigned i = 0;
    STACKSCOPE frame;
    V points[4], scale;
    QUERY query;
    float radius;
    V *point;
    qword origin = GetLoWorldPos(other);
    radius = other->sHead;
    IntersectSwBoundingBox(&g_sw, origin, reconSel->position.q, FilterReconOccluder, 0, &query.count,
                           &query.objects);
    {
        CMMATQ *pm = &g_cm.matq; qword up = pm->rot[1];
        qword direction = Normalize(VuSub(reconSel->position.q, origin));
        qword k, side;
        __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(radius));
        scale.q = k;
        side = Normalize(VuCross(up, direction));
        SCALE_IN_PLACE(direction, scale.q);
        origin = VuAdd(origin, direction);
        SCALE_IN_PLACE(up, scale.q);
        points[0].q = origin;
        points[1].q = VuAdd(origin, up);
        points[2].q = VuSub(origin, up);
        points[3].q = VuAdd(origin, VuScaleByX(side, scale.q));
    }
    point = points;
    for (; i < 4; ++i) {
        if (FindFirstObjectHitByLine(0, reconSel->position.q, point->q, query.count,
                                                           query.objects, 0))
            ++cHit;
        ++point;
    }
    {
        int cRay =
            4; // the game loads this 4 before the switch (v1, beqz delay slot); a case-label constant alone is loaded in v0 after it
        switch (cHit) {
        case 0: other->state = 1; break;
        case 4: other->state = 2; break;
        case 1:
        case 2:
        case 3: other->state = 3; break;
        }
        i = cRay;
    }
}

// Load the plane test inputs (eye, threshold, point) from the camera (register-pinned).
static inline void PlaneArguments(qword *eye,qword *threshold,qword *point,qword scalar,const qword *camera,const qword *position) {
    register qword view __asm__("vf1");
    register qword value __asm__("$6");
    register qword local __asm__("vf2");
    __asm__("lqc2 %0,%3\n\tpor %1,$zero,%5\n\tlqc2 %2,%4"
            : "=j"(view),"=&r"(value),"=j"(local) : "m"(*camera),"m"(*position),"r"(scalar));
    *eye=view;*threshold=value;*point=local;
}
// position - camera (register-pinned).
static inline qword PlaneDelta(qword position,qword camera) {
    register qword eye __asm__("vf1")=camera;
    register qword point __asm__("vf2")=position;
    __asm__("vsub.xyzw %0,%0,%1" : "+j"(point) : "j"(eye));return point;
}
// Return a - b, computed in b's register.
static inline qword SubSecond(qword a,qword b){__asm__("vsub.xyzw %0,%1,%2":"=j"(b):"j"(a),"0"(b));return b;}
// XY length of a as a float.
static inline float LengthXY(qword a)
{
    float g;
    __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y" : "=j"(a) : "0"(a));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(a) : "0"(a));
    __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(a));
    return g;
}
// Store a 128-bit GPR value to *p.
static inline void Store(qword*p,qword q){__asm__("sq %1,%0":"=m"(*p):"r"(q));}
// Load a float's bits into a GPR qword.
static inline qword ScalarBits(const float*p){qword q;__asm__("lw %0,%1":"=r"(q):"m"(*p));return q;}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A8550 __asm__("D_002A8550") = 3.40282347e+38f; }
// Chooses the closest unblocked target in the view frustum; on a change enters selector state 2.
void SelectReconTarget(RECONSEL *reconSel)
{
    V radius, screen, position;
    int i = 0;
    register RECONTGT *best __asm__("$20") = 0;
    LO *owner = reconSel->model->player;
    float minimum;
    RECONTGT **slot;
    ((JT *)owner)->GetCameraFocusPos((VECTOR4 *)&reconSel->position.q);
    reconSel->position.f[2] += 20.0f;
    minimum = D_002A8550;
    if (reconSel->count > 0) {
        slot = reconSel->items;
        do {
            RECONTGT *o = *slot;
            CM *camera;
            float distance;
            qword eye, threshold, point;
            if (!IsReconTargetSelectable(o))
                continue;
            Store(&position.q, GetLoWorldPos(o));
            camera = &g_cm;
            radius.q = ScalarBits(&o->sRadius);
            PlaneArguments(&eye, &threshold, &point, radius.q, &*(qword *)&camera->pos, &position.q);
            if (!IsInCameraFrustum(camera, PlaneDelta(point, eye), threshold))
                continue;
            if (!IsWithinCameraRenderDistance(camera, GetLoWorldPos(o), o->busyRadius, &screen.f[0]))
                continue;
            distance = LengthXY(SubSecond(position.q, *(qword *)&camera->pos));
            if (distance < minimum) {
                UpdateReconTargetVisibility(reconSel, o);
                if (o->state != 2) {
                    best = o;
                    minimum = distance;
                }
            }
        } while (++slot, ++i < reconSel->count);
    }
    if (best != reconSel->actor) {
        reconSel->actor = best;
        SetReconSelState(reconSel, 2);
    }
}

// The localized string of the pending-target label (+0xC0).
extern "C" const char *GetReconPendingText(RECONSEL *reconSel) { if (reconSel->pending == -1) return 0; return FindSwLocalizedString(&g_sw, reconSel->pending); }

// The localized string of the selected-target label (+0xBC).
extern "C" const char *GetReconSelectedText(RECONSEL *reconSel) { if (reconSel->selected == -1) return 0; return FindSwLocalizedString(&g_sw, reconSel->selected); }

// Clear bit 0 of a flag word (the pointer-or-inline tag).
static inline int Untag(int word)
{
    register int result __asm__("v0");
    __asm__("and %0,%1,%2" : "=r"(result) : "r"(word),"r"(~1));
    return result;
}
// 1 when bit 0 of word is clear (the word is a pointer to the flags).
static inline int IndirectWord(int word)
{
    register int result __asm__("v1");
    __asm__("xori %0,%1,1\n\tandi %0,%0,1" : "=r"(result) : "r"(word));
    return result;
}
// Restore the object's flags to mask, keeping bit 0 (writes through the pointer when bit 0 is clear).
static inline void Restore(LO *x,uint mask){uint f=x->unknown40;if((f^1)&1)*(uint *)f=mask|(*(uint *)f&1);else x->unknown40=mask|(f&1);}
// Selector state machine (-1..10); 8 = snapshot (clock and sound paused, player flags 0x102), 9 = photo taken
// (dialog, game event 10, target marked done).
// Also known as: SetPhotoPromptState (upstream readable / Sly 1 name; SetPhotoPromptState__FPvi).
void SetReconSelState(RECONSEL *reconSel, int nextState)
{
    if (reconSel->state == nextState)
        return;
    if (!reconSel->primary)
        return;
    if (!reconSel->secondary)
        return;
    if (((uint)(reconSel->state - 8) < 2) != ((uint)(nextState - 8) < 2)) {
        if ((uint)(nextState - 8) < 2)
            IncrementSwHandsOff(&g_sw, 0);
        else
            DecrementSwHandsOff(&g_sw, 0);
    }
    switch (reconSel->state) {
    case -1:
    case 0:
    case 2:
    case 3:
    case 10: break;
    case 1:
        FadeReconTargetMarkers(reconSel);
        RefreshReconPendingTargets(reconSel);
        PlayWidgetAnim(reconSel->primary, 0x35D);
        if ((int)reconSel->cPending > 0)
            PlayWidgetAnim(reconSel->primary, 0x35E);
        UpdateReconChecklist(reconSel);
        break;
    case 4: StopWidgetAnim(reconSel->secondary, 0x359); break;
    case 5:
    case 6: StopWidgetAnim(reconSel->secondary, 0x35A); break;
    case 7:
        StopWidgetAnim(reconSel->primary, 0x356);
        StopWidgetAnim(reconSel->secondary, 0x356);
        StopWidgetAnim(reconSel->secondary, 0x357);
        break;
    case 8:
        Restore(reconSel->model->player, reconSel->savedFlags);
        SetClockRate(&g_clock, 0, 1.0f);
        SetSoundManagerFlags(&g_sndmgr, 1, 1);
        StopWidgetAnim(reconSel->primary, 0x358);
        break;
    case 9:
        ResetCplookZoom(&g_cm.cplook);
        SetWidgetExclusive(reconSel->primary, -1);
        reconSel->primary->Show(0x310);
        if (reconSel->resource) {
            if (reconSel->actor->key != -1)
                StartDialog(&g_sndmgr, reconSel->actor->key, 0);
            StartFrameCapture(reconSel->resource, 0x608, reconSel->actor->key);
        }
        SetReconTargetDone(reconSel->actor, 1);
        BroadcastGameEvent(&g_game, 10, 0);
        reconSel->actor = 0;
        if (IsReconSelAvailable(reconSel)) {
            PlayWidgetAnim(reconSel->primary, 0x35D);
            if ((int)reconSel->cPending > 0)
                PlayWidgetAnim(reconSel->primary, 0x35E);
        }
        UpdateReconChecklist(reconSel);
        break;
    }
    int previous = reconSel->state;
    reconSel->state = nextState;
    switch (nextState) {
    case 0:
        reconSel->actor = 0;
        reconSel->selected = -1;
        reconSel->pending = -1;
        ClearReconTargetMarks(reconSel);
        reconSel->primary->Show(-1);
        ResetReconTargets(reconSel);
        UpdateReconSelInVolume(reconSel);
        BroadcastGameEvent(&g_game, 9, 0);
        break;
    case 1:
        StopWidgetAnim(reconSel->primary, 0x35D);
        reconSel->secondary->Show(0x339);
        break;
    case 5:
        PlayWidgetAnim(reconSel->secondary, 0x35A);
        SetWidgetAnimPhase(reconSel->secondary, 0x35A, 0.0f);
        break;
    case 6:
        PlayWidgetAnim(reconSel->secondary, 0x35A);
        SetWidgetAnimPhase(reconSel->secondary, 0x35A, 0.0f);
        break;
    case 7:
        SetSndSourceLo(CreateSfxSound(0x52), 0);
        reconSel->selected = reconSel->actor->id;
        PlayWidgetAnim(reconSel->primary, 0x356);
        PlayWidgetAnim(reconSel->secondary, 0x356);
        PlayWidgetAnim(reconSel->secondary, 0x357);
        break;
    case 8: {
        StopWidgetAnim(reconSel->primary, 0x35B);
        PlayWidgetAnim(reconSel->primary, 0x358);
#ifdef SLY2_EE_ASM
        register MODEL *model __asm__("a1");
        register LO *widget __asm__("v0");
        register int flags __asm__("a0");
        __asm__("lw %0,%1" : "=r"(model) : "m"(reconSel->model));
        __asm__("lw %0,0(%1)" : "=r"(widget) : "r"(model), "m"(model->player));
        __asm__("lw %0,64(%1)" : "=r"(flags) : "r"(widget), "m"(widget->unknown40));
        if (IndirectWord(flags))
            flags = *(uint *)flags;
        reconSel->savedFlags = Untag(flags);
        register LO *updated __asm__("a0");
        register uint raw __asm__("v1");
        __asm__("lw %0,0(%1)" : "=r"(updated) : "r"(model), "m"(model->player));
        __asm__("lw %0,64(%1)" : "=r"(raw) : "r"(updated), "m"(updated->unknown40));
        if ((raw ^ 1) & 1)
            *(uint *)raw |= 0x102;
        else {
            register uint inlineWord __asm__("v0") = raw | 0x102;
            __asm__("sw %1,%0" : "=m"(updated->unknown40) : "r"(inlineWord));
        }
#else
        {   /* the player's tagged flag word (LO::unknown40): save it untagged, then set 0x102 */
            LO *player = reconSel->model->player;
            sptr word = player->unknown40;
            if (word & 1) {
                reconSel->savedFlags = (uint)word & ~1u;
                player->unknown40 = (int)word | 0x102;
            } else {
                reconSel->savedFlags = *(uint *)word & ~1u;
                *(uint *)word |= 0x102;
            }
        }
#endif
        SetSoundManagerFlags(&g_sndmgr, 1, 0);
        SetClockRate(&g_clock, 0, 0.0f);
        break;
    }
    case 9:
        reconSel->secondary->Show(0x339);
        reconSel->primary->Show(0x35C);
        SetWidgetExclusive(reconSel->primary, 0);
        break;
    case 3:
    case 4:
        reconSel->secondary->followObject = (LO *)reconSel->actor;
        reconSel->secondary->Show(0x337);
        PlayWidgetAnim(reconSel->secondary, 0x359);
        SetWidgetAnimPhase(reconSel->secondary, 0x359, 0.0f);
        break;
    case 2: reconSel->secondary->Show(0x339); break;
    case 10:
        reconSel->actor = 0;
        reconSel->selected = -1;
        reconSel->pending = -1;
        reconSel->secondary->Hide();
        reconSel->primary->Hide();
        NotifyReconTargets(reconSel);
        BroadcastGameEvent(&g_game, 11, 0);
        break;
    case -1:
        if (previous != 10) {
            BroadcastGameEvent(&g_game, 11, 0);
            if (reconSel->primary->state) {
                SetWidgetState(reconSel->primary, 1);
                SetWidgetRequest(reconSel->primary, 1);
            }
            if (reconSel->secondary->state) {
                SetWidgetState(reconSel->secondary, 1);
                SetWidgetRequest(reconSel->secondary, 1);
            }
        }
        break;
    }
}

// UpdateReconSel (0x202D38, 524 instructions): per-frame update of the Sly-only interaction prompt (screen projection of the
// actor's head, proximity/size gain, state machine, input check). MATCH (Lilly/Claude T1613), no register pins.
// Case 9 flag is `!link || link->state == 1` (the game's `beqz link` jumps TO the `li a0,1`); the label before that set
// keeps jump.c from turning it into movn.
struct VU_FLOAT {qword qw;};
// Broadcast the float word n into a VU0 register.
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2 %1,%0":"=j"(v.qw):"rV"(n));return v;}
// camera up row; the pointer parameter gives the game's &cm+0x60 base register (lqc2 0x10(s1), then s1-0x60 for the calls)
static inline qword GetMatrixUp(const MATRIX4Q *m){return m->r[1].q;}
// screen position of a projected point: x and y from [-1,1] to pixels, z kept
static inline VU_FLOAT ScreenFromProjected(qword q)
{
    VU_FLOAT v;
    int t;
    float x=QwordX(q)*320.0f+320.0f;
    float y=QwordY(q)*-246.40000915527344f+246.40000915527344f;
    float z=QwordZ(q);
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(v.qw),"=&r"(t):"f"(x),"f"(y),"f"(z));
    return v;
}
// Clamp g to [minValue, maxValue].
static inline float ClampF(float g,float minValue,float maxValue){return StdMax(minValue,Min(g,maxValue));}
// Per frame: projects the target's head to the screen, scores proximity to the centre and size, drives the states and
// takes the photo on button 10 (250 when locked).
// Also known as: UpdatePhotoPrompt (upstream readable / Sly 1 name; UpdatePhotoPrompt__FPv).
extern "C" void UpdateReconSel(RECONSEL *reconSel)
{
    if (!reconSel->primary)
        return;
    if (!reconSel->secondary)
        return;
    if (reconSel->refreshPending) {
        reconSel->refreshPending = 0;
        EndJtMode(reconSel->model);
    }
    if (reconSel->choiceTime + 5.0f < g_clock.time)
        StopWidgetAnim(reconSel->primary, 0x35B);
    if (reconSel->state < 8)
        if (reconSel->state >= 2) {
            SelectReconTarget(reconSel);
            FadeReconTargetMarkers(reconSel);
            if (!IsReconSelAvailable(reconSel))
                SetReconSelState(reconSel, 1);
        }
    if (reconSel->actor) {
        qword world = GetLoWorldPos(reconSel->actor);
        VU_FLOAT offset = VuFloatFromMemWord(*(int *)&reconSel->actor->sHead);
        qword head = VuAdd(world, VuScaleByX(GetMatrixUp((MATRIX4Q *)&g_cm.mat), offset.qw));
        qword base = ConvertCmWorldToScreen(&g_cm, world);
        qword top = ConvertCmWorldToScreen(&g_cm, head);
        VU_FLOAT screen = ScreenFromProjected(base);
        float distance = QwordX(VuLengthXY(VuSub(screen.qw, reconSel->origin.q)));
        VU_FLOAT screenTop = ScreenFromProjected(top);
        float size = QwordX(VuLengthXY(VuSub(screen.qw, screenTop.qw)));
        float proximity = 1.0f - (distance - 128.0f) / 128.0f;
        size = size / 120.0f;
        reconSel->rising = 1;
        reconSel->proximity = ClampF(proximity, 0.0f, 1.0f);
        reconSel->sizeGain = size;
        if (size > 1.0f) {
            reconSel->rising = 0;
            reconSel->sizeGain = 2.0f - size;
        }
        reconSel->sizeGain = StdMax(reconSel->sizeGain, 0.0f);
    }
    switch (reconSel->state) {
    case -1: break;
    case 2:
        if (reconSel->actor && reconSel->actor->state != 2)
            SetReconSelState(reconSel, 4);
        break;
    case 4:
        if (!reconSel->actor)
            SetReconSelState(reconSel, 2);
        else if (reconSel->actor->state == 3)
            SetReconSelState(reconSel, 3);
        else if (reconSel->proximity >= 1.0f) {
            if (reconSel->rising)
                SetReconSelState(reconSel, 5);
            else
                SetReconSelState(reconSel, 6);
        }
        break;
    case 3:
        if (!reconSel->actor)
            SetReconSelState(reconSel, 2);
        else if (reconSel->actor->state != 3)
            SetReconSelState(reconSel, 4);
        break;
    case 5:
        if (!reconSel->actor)
            SetReconSelState(reconSel, 2);
        else if (reconSel->proximity < 1.0f)
            SetReconSelState(reconSel, 4);
        else if (reconSel->sizeGain >= 1.0f - reconSel->actor->threshold)
            SetReconSelState(reconSel, 7);
        else
            SetWidgetAnimPhase(reconSel->secondary, 0x35A, reconSel->sizeGain);
        break;
    case 6:
        if (!reconSel->actor)
            SetReconSelState(reconSel, 2);
        else if (reconSel->proximity < 1.0f)
            SetReconSelState(reconSel, 4);
        else if (reconSel->sizeGain >= 1.0f - reconSel->actor->threshold)
            SetReconSelState(reconSel, 7);
        else
            SetWidgetAnimPhase(reconSel->secondary, 0x35A, reconSel->sizeGain);
        break;
    case 7:
        if (reconSel->actor) {
            if (reconSel->proximity < 1.0f || reconSel->sizeGain < 1.0f - reconSel->actor->threshold) {
                if (reconSel->rising)
                    SetReconSelState(reconSel, 5);
                else
                    SetReconSelState(reconSel, 6);
            }
        } else
            SetReconSelState(reconSel, 2);
        break;
    case 8:
        if (!IsWidgetAnimPlaying(reconSel->primary, 0x358)) {
            int changed = 0;
            if (reconSel->actor && reconSel->actor->state == 1 && reconSel->proximity >= 1.0f &&
                reconSel->sizeGain >= 1.0f - reconSel->actor->threshold) {
                if (!reconSel->actor->mode)
                    SetReconSelState(reconSel, 9);
                else {
                    reconSel->pending = reconSel->actor->choice;
                    SetReconSelState(reconSel, 7);
                    changed = 1;
                }
            } else {
                if (reconSel->actor && reconSel->actor->state != 1) {
                    reconSel->pending = reconSel->primary->actorChoice;
                    SetReconSelState(reconSel, 3);
                } else if (reconSel->proximity >= 1.0f && reconSel->rising) {
                    reconSel->pending = reconSel->primary->risingChoice;
                    SetReconSelState(reconSel, 5);
                } else if (reconSel->proximity >= 1.0f && !reconSel->rising) {
                    reconSel->pending = reconSel->primary->fallingChoice;
                    SetReconSelState(reconSel, 6);
                } else if (reconSel->actor) {
                    reconSel->pending = reconSel->primary->nearChoice;
                    SetReconSelState(reconSel, 4);
                } else {
                    reconSel->pending = reconSel->primary->emptyChoice;
                    SetReconSelState(reconSel, 2);
                }
                changed = 1;
            }
            if (changed) {
                PlayWidgetAnim(reconSel->primary, 0x35B);
                reconSel->choiceTime = g_clock.time;
            }
        }
        break;
    case 9:
        if (!IsWidgetAnimPlaying(reconSel->primary, 0x35C)) {
            int ready = 1;
            SetReconSelState(reconSel, 2);
            for (int i = 0; i < reconSel->count; i++) {
                RECONTGT *actor = reconSel->items[i];
                int done = IsReconTargetDone(actor);
                // an actor whose link is missing or in state 1 must be done before the refresh
                int linkReady = !actor->link || actor->link->state == 1;
                if (!done && linkReady) {
                    ready = 0;
                    break;
                }
            }
            if (ready)
                reconSel->refreshPending = 1;
        }
        break;
    case 10:
        if (!IsWidgetAnimPlaying(reconSel->primary, 0x314))
            SetReconSelState(reconSel, -1);
        break;
    case 0:
        if (IsWidgetAnimPlaying(reconSel->primary, 0x314))
            SetReconSelState(reconSel, 1);
        break;
    case 1:
        FadeReconTargetMarkers(reconSel);
        if (IsReconSelAvailable(reconSel))
            SetReconSelState(reconSel, 2);
        break;
    }
    if (reconSel->model->kind == 1)
        return;
    switch (reconSel->state) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (!IsCameraPolicyActive(&g_cm.part3a0) && ConsumeButtonsPressedWithin__5POCTLUif(D_002E1E40->controller, 10, 0.0f))
            SetReconSelState(reconSel, 8);
        break;
    case 7:
        if (!IsCameraPolicyActive(&g_cm.part3a0) && ConsumeButtonsPressedWithin__5POCTLUif(D_002E1E40->controller, 250, 0.0f))
            SetReconSelState(reconSel, 8);
        break;
    }
}

// Recon target (CID 0x2E) Init: transform init, ids -1/-2.
// Also known as: InitPhotoTarget (upstream readable / Sly 1 name; InitPhotoTarget__FP2LO).
extern "C" void InitReconTarget(RECONTGT *target)
{
    InitXfm(target);
    target->id = -1; //P
    target->choice = -1; //P
    target->key = -1; //P
    target->idPending = -2; //P
}

// Recon target LoadFromBrx: defaults 75 / 0.25 / 5000, then the level's values. Verified by parsing all 43 levels.
extern "C" void LoadReconTargetFromBrx(RECONTGT *target, void *pbis)
{
    target->sHead = 0.0f;
    target->sRadius = 75.0f;
    target->threshold = 0.25f;
    target->rFade = 5000.0f;
    LoadXfmFromBrx(target, pbis);
    if (target->sHead == 0.0f) target->sHead = target->sRadius;
    if (target->idPending == -2) target->idPending = target->id;
}

// Recon target post-load: LO base and registration with its task (+0xEC).
void CID46::PostLoad()
{
    RECONTGT *p = (RECONTGT *)this;
    TASK *link;
    PostLoLoad(p);
    link = p->link;
    if (link != 0) {
        SubscribeLoObject(link, p);
    }
}

// Recon target message handler: on message 0x11 refreshes the player's recon list and checklist.
extern "C" void HandleReconTargetMessage(RECONTGT *target, int msg, void *pm)
{
    PLAYER *o;
    HandleBasicMessage(target, msg, pm);
    if (msg != 0x11) return;
    o = GetHeroPo(&g_sw);
    if (o && IsBasicDerivedFrom((BASIC *)o, CID_34) && o->jtmode.hud.reconSel) {
        RefreshReconPendingTargets(o->jtmode.hud.reconSel);
        UpdateReconChecklist(o->jtmode.hud.reconSel);
    }
}

// Marks the target photographed (in its task, or a local flag without one).
void SetReconTargetDone(RECONTGT *target, int n)
{
    if (target->link) {
        PLAYER *o;
        RequestTaskState(target->link, 2, target);
        o = GetHeroPo(&g_sw);
        if (o && IsBasicDerivedFrom((BASIC *)o, CID_34)) GatherJtModeHudTags(&o->jtmode.hud);
    } else target->fDone = n;
}

// Whether the target was photographed (task done, or the local flag).
int IsReconTargetDone(RECONTGT *target) { if (target->link) return IsTaskComplete(target->link); return target->fDone; }

// Whether the target is visible and its task is in state 1.
int IsReconTargetSelectable(RECONTGT *target)
{
    TASK *o;
    if (IN_WORLD(target) == 0) return 0;
    if (target->fadeObject && IN_WORLD(target->fadeObject)) return 0;
    o = target->link;
    if (o && o->state != 1) return 0;
    return 1;
}
