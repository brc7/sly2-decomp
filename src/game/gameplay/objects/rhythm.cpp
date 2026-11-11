// rhythm.cpp -- the rhythm (dance) minigame: the dancers (a step object playing the move clip for each key, D_002DE570's cues
// telling them when) and the rhythm manager (cue timing, button presses, the score window, the note track it draws with
// rich text, the score-driven SMA crowd). TU 0x1520C8..0x153850 (30 functions, address order), then the five inline
// CTextBox setters behind the linkonce copies 0x258438..0x258480 (used by DrawRhythmMgr). Entry points: the DANCER
// and CID121 (rhythm manager) vtables, the Splice setters (AddDancerMove, SetRhythmManagerPrompts...) and the manager's
// world callback BindRhythmManagerGrades.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_rhythm
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_rhythm
#include "sly2/port.h"   /* SLY2_EE_LAYOUT */
#include "sly2/slotheap.h"
#include "sly2/util.h"
#include "sly2/clock.h"
#include "sly2/classes.h"
#include "sly2/game.h"
#include "sly2/sndmgr.h"
#include "sly2/sw.h"
// 0x153850 StaticInit_rhythm and GlobalConstructors_rhythm: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

typedef int qwordV __attribute__((mode(TI)));
/* DMR / DMOVE (a dancer's moves) and the dancer's own fields: sly2/classes.h DANCER. */
enum { MOVE_CLIP_SUCCESS = 0, MOVE_CLIP_FAIL = 1, MOVE_CLIP_MISS = 2 };
enum { ANY_KEY = -1 };   /* PlayDancerMove: the move whose button the controller reports pressed */

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))

/* A note on the drawn track (0x30). */
struct ENT { float time; int buttonMask; char pad[8]; union { qwordV screenPos; VECTOR4 screenPosV; }; int isHit; char pad2[0x30 - 0x24]; };
/* A cue: the dancer, its button and the move that follows, when it is due (music time). */
struct CUE
{
    DANCER *dancer;
    int key;
    int nextKey;
    float time;
    float timeAdded;
    int hasPlayed;              /* demonstration state: the dancer showed the move */
    int hasPrompted;            /* play state: game event 0x34 sent */
    int wasPressed;             /* play state: the player pressed for it */
    int isExpired;              /* play state: the late window passed */
};
/* A crowd grade SMA driven by the score: object oid, grade oid, the bound SMA, current grade, score thresholds. */
struct SMAE { int oid; int gradeOid; void *sma; int gradeIndex; float thresholds[3]; };
/* SmoothValue parameters (SMP). */

/* Rhythm manager states (inferred). */
enum RHYTHM_STATE { RHYTHM_IDLE = 0, RHYTHM_DEMONSTRATE = 1, RHYTHM_PLAY = 2 };
enum { GAME_EVENT_RHYTHM_CUE = 0x34, GAME_EVENT_RHYTHM_PRESS = 0x35, GAME_EVENT_RHYTHM_SCORE = 0x36 };

/* The rhythm manager. */
struct RHYTHMMGR
{
    char pad0[0x40];
    unsigned int taggedFlags;   /* 0x040 */
    char pad44[0x50 - 0x44];
    int state;                  /* 0x050 RHYTHM_STATE */
    float stateTime;            /* 0x054 */
    float earlyWindow;          /* 0x058 seconds before a cue a press still counts */
    float lateWindow;           /* 0x05C seconds after */
    int numNotes;               /* 0x060 */
    int pad64[3];
    ENT notes[6];               /* 0x070 */
    int hintState;              /* 0x190 0 waiting, 1 flying to the next note */
    int hintIndex;              /* 0x194 */
    int pad198[2];
    union { qwordV hintPos; VECTOR4 hintPosV; };  /* 0x1A0 the hint marker */
    qwordV hintVelocity;        /* 0x1B0 */
    int numCues;                /* 0x1C0 */
    CUE cues[3];                /* 0x1C4 */
    int eventButtons;           /* 0x230 buttons reported with game events 0x34/0x35 */
    LM scoreRange;             /* 0x234 */
    int numScores;              /* 0x23C */
    float scores[16];           /* 0x240 last scores */
    int averageWindow;          /* 0x280 scores averaged */
    float averageScore;         /* 0x284 */
    float smoothedScore;        /* 0x288 */
    union { char smoothing[0x0C]; SMP smoothingParams; };   /* 0x28C */
    float earlyPressScore, wrongButtonScore, repeatPressScore, missScore;   /* 0x298 */
    union { unsigned int numGradesU; int numGrades; };    /* 0x2A8 */
    SMAE grades[8];             /* 0x2AC */
};
typedef char RHYTHMMGR_2AC[FIELD_OFFSET(RHYTHMMGR,grades)==0x2AC?1:-1];
typedef char RHYTHMMGR_1C4[FIELD_OFFSET(RHYTHMMGR,cues)==0x1C4?1:-1];

struct SW;
extern "C" {
extern RHYTHMMGR *D_002DE570;
extern char D_002DE578[];
extern const LONGU D_002A5010;
extern float D_002BDBE0[3];
extern LM g_lmZeroOne;
void RemoveSoHierarchy(DANCER *p);
void PostStepLoad(DANCER *o);
void UpdateStep(DANCER *p);
int IsAsegaRetracted(ASEGA *pa);
void SetXfRole(DANCER *p, void *pv, int n);
qwordV GetStepAsegaTranslation(DANCER *p, void *pa, float g);
void HandlePoMessage(DANCER *p, int msgk, struct MSGD *pmsg);
void GetSoCameraDefaults(DANCER *p, CPDEFI *pc);
int POCTL_ConsumeAnyPressed(void *p, int a);
int GetButtonsPressedWithin__5POCTLUif(void *, int, float);
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(void *paseg, void *palo, float a, float b, int n, ASEGA **pp);
void *FindSwObject__FP2SWi3OIDP2LO(SW *psw, int k, int oid, void *p);
void InitLo(RHYTHMMGR *p);
void SetLoBusyKind(RHYTHMMGR *p, int n);
void PostLoLoad(RHYTHMMGR *p);
void PostSwCallback(SW *psw, RHYTHMMGR *p, long k, int n);
void ReplaceSma(void *o2, void *o, int k, int n, void **ppo);
float RandomFloatInRange__Fff(float a, float b);
void UpdateLo(RHYTHMMGR *);
float GetMusicTime(void *);
void BroadcastGameEvent(void *, int, RHYTHMMGR *);
float SmoothValue__FfffP3SMPPf(void *, float, float, float, int);
int IsSmaRetracted(void *o);
void SetSmaGoal(void *o, int n);
void IncrementSwHandsOff(SW *psw, int n);
void DecrementSwHandsOff(SW *psw, int n);
void CopyAb__FPvT0Ui(void *dst, void *src, int cb);
void *memmove(void *dst, void *src, int cb);
int AreFloatsNear__Ffff(float, float, float);
float GetDifficulty(void *p);
float ClampToLimits__FP2LMf(LM *plm, float g);

void ApplyDancerAseg(DANCER *p, void *paseg, int n);
void PlayDancerMove(DANCER *p, int id, int idNew, int j);
void *FindDancerMoveObject(DANCER *p, int id);
void BindDancerMoves(DANCER *p);
int GetGradeIndexForOid(int k);
int GetGradeOidForIndex(int k);
void UpdateRhythmManagerGrades(RHYTHMMGR *p);
void SetRhythmMgrState(RHYTHMMGR *p, int s);
void JudgeRhythmPress(RHYTHMMGR *p, DANCER *po);
float GetRhythmTimingScore(RHYTHMMGR *p, CUE *q);
float AdjustRhythmScore(RHYTHMMGR *p, float g);
void AddRhythmScore(RHYTHMMGR *p, float g);
float GetAverageRhythmScore(RHYTHMMGR *p);
}

// DANCER (cid 0x44) vtable slot 12 (RemoveHierarchy): SO removal, then stops the dancer's segment.
extern "C" void RemoveDancerHierarchy(DANCER *dancer) {
    RemoveSoHierarchy(dancer);
    ApplyDancerAseg(dancer, 0, 0);
}

// DANCER vtable slot 15 (PostLoad): step post-load, resolves the move clips, then vtable slot 79 with 0. (inferred)
// Also known as: PostLoadDancer (upstream readable / Sly 1 name; PostLoadDancer__FP2LO).
void DANCER::PostLoad()
{
    DANCER *dancer = this;
    PostStepLoad(dancer);
    BindDancerMoves(dancer);
    SetCtlState(0);
}

// Vector scaled by a float.
static inline qwordV VecMul(qwordV v, float g)
{
    qwordV r;
    union { qwordV q; float f[4]; } s;
    qwordV ts;
    __asm__("qmtc2.ni %1, %0" : "=j"(ts) : "r"(g));
    s.q = ts;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    return r;
}
// DANCER vtable slot 16 (Update): step update; when the segment has finished the dancer goes back to idle. While the
// rhythm manager (D_002DE570) is idle the player dances freely (any pressed button plays its move); while it plays,
// presses are judged (JudgeRhythmPress). The velocity follows the playing segment's root motion. (inferred)
// Also known as: UpdateDancer (upstream readable / Sly 1 name; UpdateDancer__FP2LO).
extern "C" void UpdateDancer(DANCER *dancer)
{
    int managerState;
    UpdateStep(dancer);
    if (!dancer->segmentAnim || IsAsegaRetracted(dancer->segmentAnim)) {
        SetXfRole(dancer, dancer->positionXf, 1);
        SetXfRole(dancer, dancer->rotationXf, 1);
        dancer->isPerforming = 0;
        ApplyDancerAseg(dancer, dancer->idleSegment, 8);
    }
    managerState = D_002DE570 ? D_002DE570->state : RHYTHM_IDLE;
    switch (managerState) {
    case RHYTHM_IDLE:
        if (!dancer->isPerforming)
            PlayDancerMove(dancer, ANY_KEY, ANY_KEY, MOVE_CLIP_SUCCESS);
        break;
    case RHYTHM_PLAY:
        JudgeRhythmPress(D_002DE570, dancer);
        break;
    }
    if (dancer->segmentAnim)
        dancer->qwUnknown190 = VecMul(GetStepAsegaTranslation(dancer, dancer->segmentAnim, dancer->segmentAnim->time), -1.0f);
}

/* A segment event message (kind 9): the segment, its time and the event kind. */
struct SOM { char pad[0x18]; DANCER *target; };
struct MSGD { SOM *segment; float time; int eventKind; };
enum { MESSAGE_SEGMENT_EVENT = 9 };
enum { SEGMENT_EVENT_PERFORM_BEGIN = 0x27, SEGMENT_EVENT_PERFORM_END = 0x28, SEGMENT_EVENT_LAUNCH = 0x15E };   /* (inferred) */
// DANCER vtable slot 5 (HandleMessage): the PO handler, then segment events aimed at this dancer: 0x27/0x28 begin and
// end a performed move; 0x15E takes the segment's root velocity and hands it, rotated into world space, to vtable
// slot 35. (inferred)
// Also known as: OnDancerMessage (upstream readable / Sly 1 name; OnDancerMessage__FP2LO5MSGIDPv).
extern "C" void HandleDancerMessage(DANCER *dancer, int messageKind, MSGD *message)
{
    HandlePoMessage(dancer, messageKind, message);
    if (messageKind == MESSAGE_SEGMENT_EVENT) {
        DANCER *target = message->segment->target;
        if (target == dancer) {
            switch (message->eventKind) {
            case SEGMENT_EVENT_PERFORM_BEGIN:
                target->isPerforming = 1;
                break;
            case SEGMENT_EVENT_PERFORM_END:
                target->isPerforming = 0;
                break;
            case SEGMENT_EVENT_LAUNCH: {
                qwordV localVelocity = GetStepAsegaTranslation(target, message->segment, message->time);
#if defined(SLY2_EE_LAYOUT)
                VTE *vt = *(VTE **)((char *)target + 0xC);   /* loaded before the transform, as the original schedules it (a SetVelocity call reloads it after: 14 of 64 off) */
#endif
                qwordV *rows = (qwordV *)target->aqwTarget;
                qwordV worldVelocity;
                target->qwUnknown190 = VecMul(localVelocity, -1.0f);
                __asm__("vmulax.xyz ACC, %1, %2x\n\tvmadday.xyz ACC, %3, %2y\n\tvmaddz.xyz %0, %4, %2z" : "=j"(worldVelocity) : "j"(rows[0]), "j"(localVelocity), "j"(rows[1]), "j"(rows[2]));
#if defined(SLY2_EE_LAYOUT)
                ((void (*)(void *, qwordV))vt[35].pfn)((char *)target + vt[35].d, worldVelocity);   /* LO::SetVelocity */
#else
                target->SetVelocity(worldVelocity);
#endif
                break;
            }
            }
        }
    }
}

// DANCER vtable slot 50 (GetCpdefi): the SO camera definition with its home angle swung by -3*PI/4 (135 degrees).
extern "C" void GetDancerCpdefi(DANCER *dancer, CPDEFI *cameraDef) {
    GetSoCameraDefaults(dancer, cameraDef);
    cameraDef->homeHeading = cameraDef->homeHeading - 2.3561945f;   /* 3*PI/4 */
}

// Plays clip clipIndex of a move: the move for key, or (ANY_KEY) the first move whose button the controller reports
// pressed (consuming it). nextClipOid, when set, replaces the move's success clip first. (inferred)
// Also known as: PlayDancerMove (upstream readable / Sly 1 name; PlayDancerMove__FP2LOiii).
void PlayDancerMove(DANCER *dancer, int key, int nextClipOid, int clipIndex)
{
    int i;
    for (i = 0; i < dancer->numMoves; i++) {
        DMOVE *move = &dancer->moves[i];
        if ((key == ANY_KEY && POCTL_ConsumeAnyPressed(dancer->controller, move->key)) || move->key == key) {
            if (nextClipOid != -1) {
                void *clip = FindDancerMoveObject(dancer, nextClipOid);
                if (clip)
                    move->clips[MOVE_CLIP_SUCCESS].clip = clip;
            }
            SetXfRole(dancer, dancer->positionXf, 2);
            SetXfRole(dancer, dancer->rotationXf, 2);
            ApplyDancerAseg(dancer, move->clips[clipIndex].clip, move->clips[clipIndex].applyFlags);
            return;
        }
    }
}

// Plays a segment on the dancer from its start at speed 1 (apply flags | 1), keeping its ASEGA in segmentAnim;
// a null segment stops it.
// Also known as: PlayDancerAseg (upstream readable / Sly 1 name; PlayDancerAseg__FP2LOT0i).
void ApplyDancerAseg(DANCER *dancer, void *segment, int applyFlags)
{
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(segment, dancer, 0.0f, 1.0f, applyFlags | 1, &dancer->segmentAnim);
}

// Splice: adds a move (at most 8): its button, the success clip and its apply flags, the fail and miss clips.
extern "C" void AddDancerMove(DANCER *dancer, int key, int successOid, int successFlags, int failOid, int missOid)
{
    if (dancer->numMovesU < 8) {
        DMOVE *move = &dancer->moves[dancer->numMovesU++];
        move->key = key;
        move->clips[MOVE_CLIP_SUCCESS].oid = successOid;
        move->clips[MOVE_CLIP_SUCCESS].applyFlags = successFlags;
        move->clips[MOVE_CLIP_FAIL].oid = failOid;
        move->clips[MOVE_CLIP_MISS].oid = missOid;
        move->clips[MOVE_CLIP_FAIL].applyFlags = 0;
        move->clips[MOVE_CLIP_MISS].applyFlags = 0;
    }
}

// Finds a clip by oid (searched from the dancer) and removes it from the world (vtable slot 9), so it only plays as
// a move.
// Also known as: PloDanceAseg (upstream readable / Sly 1 name; PloDanceAseg__FP2LO3OIDiiiii).
void *FindDancerMoveObject(DANCER *dancer, int oid) {
    DANCER *clip = (DANCER *)FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, oid, dancer);
    if (clip != 0) {
        ((LO *)clip)->Remove();
    }
    return clip;
}

// Resolves the three clip oids of every move.
// Also known as: InitDancer (upstream readable / Sly 1 name; InitDancer__FP2LO).
void BindDancerMoves(DANCER *dancer)
{
    int i, j;
    for (i = 0; i < dancer->numMoves; i++) {
        DMOVE *move = &dancer->moves[i];
        for (j = 0; j < 3; j++) {
            if (move->clips[j].oid != -1)
                move->clips[j].clip = FindDancerMoveObject(dancer, move->clips[j].oid);
        }
    }
}

/* The crowd grade goals (SMA goal oids), best first (inferred). */
enum { OID_GRADE_GOOD = 0x626, OID_GRADE_FAIR = 0x627, OID_GRADE_POOR = 0x628, OID_GRADE_SMA = 0x625 };

// Grade goal oid -> grade index 0..2 (-1 for any other oid).
int GetGradeIndexForOid(int oid)
{
    switch (oid) {
    case OID_GRADE_GOOD: return 0;
    case OID_GRADE_FAIR: return 1;
    case OID_GRADE_POOR: return 2;
    }
    return -1;
}

// Grade index 0..2 -> grade goal oid (-1 otherwise).
int GetGradeOidForIndex(int gradeIndex)
{
    switch (gradeIndex) {
    case 0: return OID_GRADE_GOOD;
    case 1: return OID_GRADE_FAIR;
    case 2: return OID_GRADE_POOR;
    }
    return -1;
}

// Claude agent (lane D37): writing the range through a pointer to the 0x234 pair keeps cse from folding
// gMax - gMin, like the target (no asm, no volatile).
// The LO flag word: stored inline, or (low bit clear) a pointer to a shared one; sets bits wherever it lives.
static inline void SetTaggedFlags(unsigned int *taggedFlags, int bits)
{
    unsigned int flags = *taggedFlags;
    if ((flags ^ 1) & 1) *(int *)flags |= bits;
    else *taggedFlags = flags | bits;
}
// Rhythm manager (cid 0x79) vtable slot 7 (Init): LO init; becomes the singleton D_002DE570; score range [-1.5, 1],
// 0.4 s press windows, 8-score average, smoothing, the per-outcome scores; busy, LO flags 0xA.
// Also known as: InitDance (upstream readable / Sly 1 name; InitDance__FP2LO).
extern "C" void InitRhythmMgr(RHYTHMMGR *manager)
{
    InitLo(manager);
    D_002DE570 = manager;
    {
        LM *range = &manager->scoreRange;
        range->minValue = -1.5f;
        range->maxValue = 1.0f;
    }
    manager->earlyWindow = 0.4f;
    manager->lateWindow = 0.4f;
    manager->averageWindow = 8;
    {
        SMP *smoothing = &manager->smoothingParams;
        smoothing->fastSpeed = __builtin_fabsf(manager->scoreRange.maxValue - manager->scoreRange.minValue) * 0.33333334f;
        smoothing->fastDuration = 5.0f;
        smoothing->slowSpeed = 0;
    }
    manager->wrongButtonScore = -0.8f;
    manager->repeatPressScore = -0.2f;
    manager->earlyPressScore = -1.5f;
    manager->missScore = -1.5f;
    SetLoBusyKind(manager, 1);
    SetTaggedFlags(&manager->taggedFlags, 0xA);
}

// Rhythm manager vtable slot 15 (PostLoad): LO post-load, then queues BindRhythmManagerGrades (D_002A5010) as a world
// callback (it needs the other objects loaded).
void CID121::PostLoad()
{
    RHYTHMMGR *manager = (RHYTHMMGR *)this;
    PostLoLoad(manager);
    PostSwCallback(&g_sw, manager, D_002A5010.l, 0);
}

// World callback: binds each crowd grade object's SMA (its child 0x625) with its starting grade goal, and draws its
// score thresholds: the range split in thirds, each boundary jittered by half a third; the last is the minimum.
// Also known as: InitDanceCrowd (upstream readable / Sly 1 name; InitDanceCrowd__FP2LO).
extern "C" void BindRhythmManagerGrades(RHYTHMMGR *manager)
{
    int i, j;
    float third = __builtin_fabsf(manager->scoreRange.maxValue - manager->scoreRange.minValue) / 3.0f;
    float jitter = third * 0.5f;
    for (i = 0; i < manager->numGrades; i++) {
        SMAE *grade = &manager->grades[i];
        void *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, grade->oid, manager);
        void *sma;
        float boundary;
        if (!object)
            continue;
        sma = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 1, OID_GRADE_SMA, object);
        if (!sma)
            continue;
        ReplaceSma(sma, object, grade->gradeOid, 0, &grade->sma);
        grade->gradeIndex = GetGradeIndexForOid(grade->gradeOid);
        boundary = manager->scoreRange.maxValue;
        for (j = 0; j < 2; j++) {
            boundary -= third;
            grade->thresholds[j] = RandomFloatInRange__Fff(boundary - jitter, boundary + jitter);
        }
        grade->thresholds[2] = manager->scoreRange.minValue;
    }
}

// Rhythm manager vtable slot 16 (Update), on music time: demonstrating, each due cue's move plays once; playing, each
// due cue sends game event 0x34 (with its button) and, once its late window passes unanswered, plays the miss clip
// and scores a miss. Then the displayed score eases toward the average and the crowd grades follow it.
// Also known as: UpdateDance (upstream readable / Sly 1 name; UpdateDance__FP2LO).
extern "C" void UpdateRhythmMgr(RHYTHMMGR *manager)
{
    float musicTime;
    int i, j;
    CUE *cue;
    UpdateLo(manager);
    musicTime = GetMusicTime(&g_sndmgr);
    switch (manager->state) {
    case RHYTHM_DEMONSTRATE:
        for (j = 0; j < manager->numCues; j++) {
            cue = &manager->cues[j];
            if (cue->hasPlayed)
                continue;
            if (cue->time < musicTime) {
                PlayDancerMove(cue->dancer, cue->key, cue->nextKey, MOVE_CLIP_SUCCESS);
                cue->hasPlayed = 1;
                break;
            }
        }
        break;
    case RHYTHM_PLAY:
        for (i = 0; i < manager->numCues; i++) {
            cue = &manager->cues[i];
            if (cue->isExpired)
                continue;
            if (!cue->hasPrompted && cue->time < musicTime) {
                manager->eventButtons = cue->key;
                BroadcastGameEvent(&g_game, GAME_EVENT_RHYTHM_CUE, manager);
                cue->hasPrompted = 1;
            }
            if (cue->time + manager->lateWindow < musicTime) {
                cue->isExpired = 1;
                if (!cue->wasPressed) {
                    PlayDancerMove(cue->dancer, cue->key, cue->nextKey, MOVE_CLIP_MISS);
                    AddRhythmScore(manager, manager->missScore);
                }
                break;
            }
        }
        break;
    }
    {
        float dt = g_clock.deltaTime;
        manager->smoothedScore = SmoothValue__FfffP3SMPPf(manager->smoothing, manager->smoothedScore, manager->averageScore, dt, 0);
    }
    UpdateRhythmManagerGrades(manager);
}

// Each bound crowd grade: a retracted SMA is released; otherwise its goal becomes the best grade whose threshold the
// smoothed score reaches.
// Also known as: UpdateDanceCrowd (upstream readable / Sly 1 name; UpdateDanceCrowd__FP5DANCE).
void UpdateRhythmManagerGrades(RHYTHMMGR *manager)
{
    int i, j;
    for (i = 0; i < manager->numGrades; i++) {
        SMAE *grade = &manager->grades[i];
        if (!grade->sma)
            continue;
        if (IsSmaRetracted(grade->sma)) {
            ReplaceSma(0, 0, -1, 0, &grade->sma);
            grade->gradeIndex = -1;
        } else {
            for (j = 0; j < 3; j++) {
                if (grade->thresholds[j] <= manager->smoothedScore) {
                    SetSmaGoal(grade->sma, GetGradeOidForIndex(j));
                    grade->gradeIndex = j;
                    break;
                }
            }
        }
    }
}

// DrawRhythmMgr (Claude agent, lane D17): MATCH. The last diff was the daddu a0/sb order in the key loop: in sched1 the f12 = scale arg move
// (reg weight 0) took the t=1 alu slot ahead of a0 = &fontBig (weight +1), so a0 got a later luid than the a[0] store. Pinning scale to $f12 removes that move.
struct FONT { char ab[0x84]; float dy; char pad[0xA0 - 0x88]; };
struct RGBAV { unsigned int word; };
struct RGBAB { unsigned char c[4]; };
// Identity accessor (keeps the original address arithmetic).
template <class T> static inline T *Addr(T *p) { return p; }
/* The text box (0x1C); its setters are inline members defined at the end of this file (linkonce copies). */
struct TB {
    float x, y, dx, dy;
    RGBAV color;
    int jh;
    int jv;
    void SetPos(float x, float y) __asm__("CTextBox_SetPos");
    void SetSize(float dx, float dy) __asm__("CTextBox_SetSize");
    void SetColor(RGBAV *prgba) __asm__("CTextBox_SetTextColor");
    void SetHorizontalJust(int j) __asm__("CTextBox_SetHorizontalJust");
    void SetVerticalJust(int j) __asm__("CTextBox_SetVerticalJust");
    // Uninitialised text box.
    TB() {}
    // Set the text box position, size, color word and color.
    void Init(float x, float y, float dx, float dy, unsigned int word, RGBAV *prgba) {
        SetPos(x, y);
        SetSize(dx, dy);
        RGBAV *p = Addr(prgba);
        p->word = word;
        SetColor(p);
        SetHorizontalJust(0);
        SetVerticalJust(0);
    }
};
struct SZ { char a[6]; };
struct KEYV { int id; unsigned char ch; float dx, dy; };
extern "C" {
/* rhythm's .data (address order): text scales, the four button keys {mask, glyph, dx, dy}, the cue lead time, two
 * vectors (VQ: initializable, read as qwords) and the score polynomial (declared above). */
union VQ { float af[4]; qwordV q; };
float D_002BDB50 = 0.55f, D_002BDB54 = 0.35f, D_002BDB58 = 1.2f, D_002BDB5C = 0.6f;
/* The two prompt glyph strings are writable arrays in .data (not literals: the ELF has them here). */
char D_002BDB60[] = "&2?&.";
char D_002BDB68[] = "&2m&.";
KEYV D_002BDB70[4] = {{0x10, 'T', -0.5f, -3.0f}, {0x20, 'O', 0.5f, -2.0f}, {0x40, 'X', -0.5f, -1.0f}, {0x80, 'S', -1.5f, -2.0f}};
float D_002BDBB0 = 0.4f;
VQ D_002BDBC0 = {{100.0f, -1000.0f, 0.0f, 0.0f}}, D_002BDBD0 = {{100.0f, 1000.0f, 0.0f, 0.0f}};
float D_002BDBE0[3] = {1.2f, -0.6f, 0.0f};
}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): the pointer-to-member record precedes the
// big-glyph string in the ELF; definition order is emission order.
namespace RO_ORDER {
void p_func_00152810() __asm__("BindRhythmManagerGrades");
extern const unsigned int r_D_002A5010[2] __asm__("D_002A5010") = {0xFFFF0000, (unsigned int)p_func_00152810};
}
extern "C" {
extern const SZ D_002A5018 = {"&2T&."};
void RenderLoSelf(RHYTHMMGR *p);
FONT *CRichText_ctor(FONT *pf, const char *sz, float scale);
float CRichText_GetWidth(FONT *pf);
void DrawRichText2(FONT *pf, TB *ptb, int grf);
void EvaluateBezierPos(float dt, float t, float dtScale, qwordV p0, qwordV v0, qwordV p1, qwordV v1, qwordV *ppos, qwordV *pvel, qwordV *pacc);
}
// The larger of two floats.
// Rhythm manager render: while demonstrating or playing, draws the note track: each note not yet due (or every note
// while demonstrating, or a hit/expired one) as the four button glyphs in a diamond with the note's button large; a
// pending note as the '?' glyph; then the hint marker ('m' glyph), which flies along a Bezier to each note as it
// comes due. Text goes through rich-text fonts ("&2<glyph>&." strings) and one text box.
// Also known as: DrawDancePrompts (upstream readable / Sly 1 name; DrawDancePrompts__FP7FONTBRX).
extern "C" void DrawRhythmMgr(RHYTHMMGR *manager)
{
    RenderLoSelf(manager);
    float t = GetMusicTime(&g_sndmgr);
    if (manager->state < 3) if (manager->state > 0) if (manager->numNotes != 0) {   /* demonstrating or playing */
        FONT fontA;     /* the pending-note glyph '?' */
        CRichText_ctor(&fontA, D_002BDB60, D_002BDB58);
        FONT fontB;     /* the hint marker glyph 'm' */
        CRichText_ctor(&fontB, D_002BDB68, D_002BDB5C);
        TB tb;
        union { RGBAV rgba; SZ s; } u;
        tb.Init(0.0f, 0.0f, 640.0f, 448.0f * 1.1f, 0xFF808080, &u.rgba);
        {
        u.s = D_002A5018;
        FONT fontBig;
        CRichText_ctor(&fontBig, u.s.a, D_002BDB50);
        float dxBig = CRichText_GetWidth(&fontBig);
        float dyBig = fontBig.dy;
        FONT fontSmall;
        CRichText_ctor(&fontSmall, u.s.a, D_002BDB54);
        float dxSmall = CRichText_GetWidth(&fontSmall);
        float dySmall = fontSmall.dy;
        float dxA = CRichText_GetWidth(&fontA);
        float dyA = fontA.dy;
        float dxB = CRichText_GetWidth(&fontB);
        float dyB = fontB.dy;
        int i;
        for (i = 0; i < manager->numNotes; i++) {
            ENT *pe = &manager->notes[i];
            if (manager->state == 1 || pe->isHit || pe->time + manager->lateWindow < t) {
                unsigned int j;
                for (j = 0; j < 4; j++) {
                    KEYV *pk = &D_002BDB70[j];
                    float x = pe->screenPosV.x + pk->dx * dxSmall;
                    float y = pe->screenPosV.y + pk->dy * dySmall;
                    unsigned char ch = 'o';
                    register float scale asm("$f12") = D_002BDB54; /* pin: no separate f12 arg move in sched1 */
                    if (pk->id == pe->buttonMask) {
                        x -= (dxBig - dxSmall) * 0.5f;
                        y -= (dyBig - dySmall) * 0.5f;
                        ch = pk->ch;
                        scale = D_002BDB50;
                    }
                    u.s.a[2] = ch;
                    u.s.a[0] = '&';
                    u.s.a[1] = '2';
                    u.s.a[3] = '&';
                    u.s.a[4] = '.';
                    u.s.a[5] = 0;
                    CRichText_ctor(&fontBig, u.s.a, scale);
                    tb.x = x;
                    tb.y = y;
                    DrawRichText2(&fontBig, &tb, 0xC);
                }
            } else {
                tb.x = pe->screenPosV.x - dxA * 0.5f;
                tb.y = pe->screenPosV.y - dyA;
                DrawRichText2(&fontA, &tb, 0xC);
            }
        }
        if (manager->hintIndex < manager->numNotes) {
            ENT *pe = &manager->notes[manager->hintIndex];
            switch (manager->hintState) {
            case 0:
                if (pe->time - t < D_002BDBB0) {
                    manager->hintVelocity = D_002BDBC0.q;
                    manager->hintState = 1;
                }
                break;
            case 1:
                if (pe->time < t) {
                    manager->hintState = 0;
                    manager->hintIndex = manager->hintIndex + 1;
                }
                break;
            }
            if (manager->hintState == 1) {
                EvaluateBezierPos(StdMax(pe->time - t, g_clock.deltaTime), g_clock.deltaTime, 1.0f, manager->hintPos, manager->hintVelocity, pe->screenPos, D_002BDBD0.q, &manager->hintPos, &manager->hintVelocity, 0);
            }
        }
        tb.x = manager->hintPosV.x - dxB * 0.5f;
        tb.y = manager->hintPosV.y - dySmall * 3.0f - dyB - 7.0f;
        DrawRichText2(&fontB, &tb, 0xC);
        }
    }
}

// Changes the manager state (stamping the time and dropping the cues); demonstrating holds the world hands-off
// (input locked, kind 3) for its whole duration.
// Also known as: SetDanceState (upstream readable / Sly 1 name; SetDanceState__FP5DANCEi).
void SetRhythmMgrState(RHYTHMMGR *manager, int newState) {
    int oldState = manager->state;
    if (oldState != newState) {
        if (oldState == RHYTHM_DEMONSTRATE) {
            DecrementSwHandsOff(&g_sw, 3);
        }
        manager->state = newState;
        manager->stateTime = g_clock.time;
        manager->numCues = 0;
        if (newState == RHYTHM_DEMONSTRATE) {
            IncrementSwHandsOff(&g_sw, 3);
        }
    }
}

// Packs x, y, z into a vector.
static inline void SetVector(VECTOR4 *pv, float x, float y, float z)
{
    pv->x = x;
    pv->y = y;
    pv->z = z;
}
// Splice: loads the note track (at most 6 notes of 0x30 bytes) and lays it out on screen: x from 100 to 540 pixels
// by note time, y 468.8; the hint marker starts at x 10. Then enters state.
// Also known as: SetDancePrompts (upstream readable / Sly 1 name; SetDancePrompts__FPvT0iT0).
extern "C" void SetRhythmManagerPrompts(RHYTHMMGR *manager, int state, int numNotes, void *notes)
{
    manager->numNotes = numNotes;
    if (numNotes) {
        float firstTime, pixelsPerSecond;
        int i;
        CopyAb__FPvT0Ui(manager->notes, notes, numNotes * sizeof(ENT));
        firstTime = manager->notes[0].time;
        pixelsPerSecond = 440.0f / (manager->notes[manager->numNotes - 1].time - firstTime);
        SetVector(&manager->hintPosV, 100.0f, 468.80002f, 0.0f);   /* (the track origin, reused as a temporary) */
        for (i = 0; i < manager->numNotes; i++) {
            ENT *note = &manager->notes[i];
            note->screenPosV = manager->hintPosV;
            note->isHit = 0;
            note->screenPosV.x += pixelsPerSecond * (note->time - firstTime);
        }
        SetVector(&manager->hintPosV, 10.0f, 468.80002f, 0.0f);
        manager->hintState = 0;
        manager->hintIndex = 0;
    }
    SetRhythmMgrState(manager, state);
}

// Splice: queues a cue for a dancer (its button, the next move's clip, the due time); with three queued the oldest
// is dropped.
// Also known as: AddDanceBeat (upstream readable / Sly 1 name; AddDanceBeat__FP2LOiiif).
extern "C" void PushRhythmManagerCue(RHYTHMMGR *manager, DANCER *dancer, int key, int nextKey, float time)
{
    CUE *cue;
    if (manager->numCues == 3) {
        memmove(&manager->cues[0], &manager->cues[1], (manager->numCues - 1) * sizeof(CUE));
        cue = &manager->cues[manager->numCues - 1];
    } else {
        cue = &manager->cues[manager->numCues++];
    }
    cue->dancer = dancer;
    cue->key = key;
    cue->nextKey = nextKey;
    cue->time = time;
    cue->timeAdded = GetMusicTime(&g_sndmgr);
    cue->hasPlayed = 0;
    cue->hasPrompted = 0;
    cue->wasPressed = 0;
    cue->isExpired = 0;
}

// Judges a dancer's button press (any face button this frame) against its first live cue and sends game event
// 0x35: too early, or a second press, plays the fail clip with that penalty; the right button plays the move and
// scores the timing (and marks the note hit); a wrong button plays the fail clip with the wrong-button penalty.
// Also known as: CheckDancerBeat (upstream readable / Sly 1 name; CheckDancerBeat__FP2LOP5DANCE).
void JudgeRhythmPress(RHYTHMMGR *manager, DANCER *dancer)
{
    void *controller = dancer->controller;
    int i, j;
    CUE *cue;
    manager->eventButtons = GetButtonsPressedWithin__5POCTLUif(controller, 0xF0, 0.0f);   /* the four face buttons */
    if (!manager->eventButtons)
        return;
    for (i = 0; i < manager->numCues; i++) {
        cue = &manager->cues[i];
        if (cue->dancer != dancer)
            continue;
        if (cue->isExpired)
            continue;
        BroadcastGameEvent(&g_game, GAME_EVENT_RHYTHM_PRESS, manager);
        if (GetMusicTime(&g_sndmgr) < cue->time - manager->earlyWindow) {
            PlayDancerMove(dancer, ANY_KEY, ANY_KEY, MOVE_CLIP_FAIL);
            AddRhythmScore(manager, manager->earlyPressScore);
            return;
        }
        if (cue->wasPressed) {
            PlayDancerMove(dancer, ANY_KEY, ANY_KEY, MOVE_CLIP_FAIL);
            AddRhythmScore(manager, manager->repeatPressScore);
            return;
        }
        cue->wasPressed = 1;
        if (POCTL_ConsumeAnyPressed(controller, cue->key)) {
            PlayDancerMove(dancer, cue->key, cue->nextKey, MOVE_CLIP_SUCCESS);
            AddRhythmScore(manager, AdjustRhythmScore(manager, GetRhythmTimingScore(manager, cue)));
            for (j = 0; j < manager->numNotes; j++) {
                ENT *note = &manager->notes[j];
                if (AreFloatsNear__Ffff(note->time, cue->time, 0.1f)) {
                    note->isHit = 1;
                    break;
                }
            }
        } else {
            PlayDancerMove(dancer, ANY_KEY, ANY_KEY, MOVE_CLIP_FAIL);
            AddRhythmScore(manager, manager->wrongButtonScore);
        }
        return;
    }
}

// Linear interpolation from a (t = 0) to b (t = 1).
static inline float GLerp(float a, float b, float t) { return (1.0f - t) * a + t * b; }
// Timing score of a press for a cue: 1 - |now - due| / window (the early or late window by side), mapped linearly onto
// the score range g_lmZeroOne (exactly on time gives its maximum).
// Also known as: GBeatScore (upstream readable / Sly 1 name; GBeatScore__FP5DANCEP4BEAT).
float GetRhythmTimingScore(RHYTHMMGR *manager, CUE *cue)
{
    float musicTime = GetMusicTime(&g_sndmgr);
    float lateness;
    if (musicTime < cue->time) {
        lateness = (cue->time - musicTime) / manager->earlyWindow;
    } else {
        lateness = (musicTime - cue->time) / manager->lateWindow;
    }
    return GLerp(g_lmZeroOne.minValue, g_lmZeroOne.maxValue, 1.0f - lateness);
}

// Returns its float argument (a hook between timing and scoring; inferred).
float AdjustRhythmScore(RHYTHMMGR *manager, float score) {
    return score;
}

// Records a score in the 16-score history (clamped to the score range); penalties scale with the difficulty
// (D_002BDBE0 polynomial, clamped); updates the average and sends game event 0x36.
// Also known as: AddDanceScore (upstream readable / Sly 1 name; AddDanceScore__FP2LOiif).
void AddRhythmScore(RHYTHMMGR *manager, float score)
{
    float *slot;
    if (score < 0.0f) {
        float difficulty = GetDifficulty(D_002DE578);
        score *= Clamp(D_002BDBE0[0] + difficulty * (D_002BDBE0[1] + difficulty * D_002BDBE0[2]), g_lmZeroOne.minValue, g_lmZeroOne.maxValue);
    }
    if (manager->numScores == 16) {
        memmove(&manager->scores[0], &manager->scores[1], 15 * sizeof(float));
        slot = &manager->scores[manager->numScores - 1];
    } else {
        slot = &manager->scores[manager->numScores++];
    }
    *slot = ClampToLimits__FP2LMf(&manager->scoreRange, score);
    manager->averageScore = GetAverageRhythmScore(manager);
    BroadcastGameEvent(&g_game, GAME_EVENT_RHYTHM_SCORE, manager);
}

// The larger of two ints.
// Average of the last averageWindow scores (missing ones count as 0), clamped to the score range.
// Also known as: GScoreMean (upstream readable / Sly 1 name; GScoreMean__FP5DANCE).
float GetAverageRhythmScore(RHYTHMMGR *manager)
{
    float sum;
    int i, oldest;
    if (manager->averageWindow <= 0)
        return 0.0f;
    sum = 0.0f;
    oldest = StdMaxInt(manager->numScores - manager->averageWindow, 0);
    for (i = manager->numScores - 1; i >= oldest; i--)
        sum += manager->scores[i];
    return ClampToLimits__FP2LMf(&manager->scoreRange, sum / manager->averageWindow);
}

// Splice: clears the score history, the average and the displayed score.
extern "C" void ResetRhythmScore(RHYTHMMGR *manager)
{
    manager->numScores = 0;
    manager->averageScore = 0.0f;
    manager->smoothedScore = 0.0f;
}

// Splice: adds a crowd grade object with its starting grade goal (the bound limit is 128, though the manager has room
// for 8).
extern "C" void AddRhythmManagerGrade(RHYTHMMGR *manager, int oid, int gradeOid) {
    unsigned int count = manager->numGradesU;
    if (count < 0x80U) {
        SMAE *grade;
        manager->numGradesU = count + 1;
        grade = &manager->grades[count];
        grade->oid = oid;
        grade->gradeOid = gradeOid;
        grade->gradeIndex = -1;
    }
}

/* The CTextBox setters (0x258438..): inline members defined after their callers, so g++ calls them and emits the
   out-of-line copies (linkonce), as the original did. Symbols: the asm labels on the declarations (screen, mpeg and
   render call them by those names; inferred: Sly 1's CTextBox::SetPos/SetSize/SetTextColor/SetHorizontalJust/
   SetVerticalJust, whose mangled names would need their parameter types). */
// Sets the text box position (out-of-line linkonce copy).
inline void TB::SetPos(float x, float y) { this->x = x; this->y = y; }

// Sets the text box size.
inline void TB::SetSize(float x, float y) { this->dx = x; this->dy = y; }

// Sets the text color (copied as four bytes).
inline void TB::SetColor(RGBAV *c) { *(RGBAB *)&this->color = *(RGBAB *)c; }

// Sets the horizontal justification.
inline void TB::SetHorizontalJust(int j) {
    jh = j;
}

// Sets the vertical justification.
inline void TB::SetVerticalJust(int j) {
    jv = j;
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
RHYTHMMGR *D_002DE570;
