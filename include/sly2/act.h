/* include/sly2/act.h -- the act and animation-curve classes (engine/anim/act.cpp's factories), with their virtual
 * layouts from the ELF vtables. Each slot is declared as a member whose asm label is the function the vtable holds
 * there (as sly2/classes.h does); the first declared virtual of each class is its key function, and the class's key
 * TU defines it as a member (act.cpp for the classes it emits; asega, blend, tail and ik hold the rest's slot
 * functions, see below). The classes are defined in act.cpp's emission order (g++ 2.95 emits a TU's vtables in the
 * reverse order of their classes' definitions): 0x2CA6A0..0x2CA9C8 descending, bases first.
 *
 * Names: ACT, ACTSEG, AC, ACG are Sly 1's (HYPOTHESIS for Sly 2: the family has the same shape: an act with a kind at
 * +0x0 and a vptr at +0x18, curves with a vptr at +0x0; GetActsegGoal is ACTSEG's slot 5). The others are DRAFT names
 * after the kind their factory switches on (InitActOfKind's act kind 0..9, as jt.cpp's ACT1/ACT5 views;
 * PacNewKind's curve kind 0..5, PacNewKind2's float-curve kind 0..2). Member names are drafts too (VSlot<n> where
 * nothing names the slot); signatures are those of the functions the key TUs define as members. The pure
 * slots name libgcc's __pure_virtual (0x23C820, in the SDK). */
#ifndef SLY2_ACT_H
#define SLY2_ACT_H
#include "sly2/basic.h"

/* AC (vtable 0x2CA9C8, 3 slots): an animation curve; vptr at +0x0. Allocated from the level heap (act.cpp's PacNewKind). */
struct AC
{
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadCurveFromBrxDefault");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("__pure_virtual");   /* slot 2 (pure) */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("__pure_virtual");   /* slot 3 (pure) */
};

/* AC0 (0x10; vtable 0x2CA9A0): constant vector curve */
struct AC0 : AC
{
    int an[3];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcConstFromBrx");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("PrepareConstCurveSegment");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateConstCurve");   /* slot 3 */
};

/* AC1 (0xC; vtable 0x2CA978): keyed vector curve */
struct AC1 : AC
{
    int an[2];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcVecAFromBrx");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("PrepareVecCurveASegment");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateVecCurveA");   /* slot 3 */
};

/* AC2 (0xC; vtable 0x2CA950): keyed vector curve */
struct AC2 : AC
{
    int an[2];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcVecBFromBrx");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("PrepareVecCurveBSegment");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateVecCurveB");   /* slot 3 */
};

/* AC3 (0x30; vtable 0x2CA928): per-component curve */
struct AC3 : AC
{
    int an[11];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcFloatAFromBrx");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("PrepareFloatCurveASegment");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateFloatCurveA");   /* slot 3 */
};

/* AC4 (0x30; vtable 0x2CA900): per-component curve */
struct AC4 : AC
{
    int an[11];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcFloatBFromBrx");   /* slot 1 */
    virtual void Prepare(void *pv) __asm__("PrepareFloatCurveBSegment");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateFloatCurveB");   /* slot 3 */
};

/* ACG (vtable 0x2CA8E0, 2 slots): a float curve; vptr at +0x0. */
struct ACG
{
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadFloatCurveFromBrxDefault");   /* slot 1 */
    virtual void Evaluate(float *pg, float *pdg, float g, float dtScale) __asm__("__pure_virtual");   /* slot 2 (pure) */
};

/* ACG1 (0xC; vtable 0x2CA8C0): keyed float curve */
struct ACG1 : ACG
{
    int an[2];
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcFloatKeysFromBrx");   /* slot 1 */
    virtual void Evaluate(float *pg, float *pdg, float g, float dtScale) __asm__("EvaluateKeyedFloatCurve");   /* slot 2 */
};

/* ACG0 (0x8; vtable 0x2CA8A0): constant float curve */
struct ACG0 : ACG
{
    float g;   /* the constant value */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadAcFloatConstFromBrx");   /* slot 1 */
    virtual void Evaluate(float *pg, float *pdg, float g, float dtScale) __asm__("EvaluateConstFloatCurve");   /* slot 2 */
};

/* ACT (vtable 0x2CA860, 6 slots): an act driving an object's transform; kind at +0x0, vptr at +0x18. Allocated
   from the act pool (g_sw.slotheapAct, act.cpp). */
struct ACT
{
    int ack;                /* +0x00 kind: InitActOfKind's switch */
    ALO *palo;              /* +0x04 the animated object */
    XF *pxf;                /* +0x08 the transform the act drives */
    int nPriority;          /* +0x0C priority among the acts on the same transform */
    ACT *pactNext;          /* +0x10 next act on the same transform (XF::firstActuator list) */
    int nRole;              /* +0x14 role of the transform the act drives (inferred, as XF::role) */
    virtual void Attach() __asm__("AttachActBase");   /* slot 1 */
    virtual void Detach() __asm__("DetachActBase");   /* slot 2 */
    virtual void SwitchTarget() __asm__("ActSlot3Default");   /* slot 3 */
    virtual void Update() __asm__("UpdateActDefault");   /* slot 4 */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetActGoalDefault");   /* slot 5 */
    virtual void Clone() __asm__("CloneAct");   /* slot 6 */
};

/* ACT1 (0x30; vtable 0x2CA820): a phase and a rest position (act.cpp's ACTPHASE view) */
struct ACT1 : ACT
{
    int nPhase;             /* +0x1C (2 at construction) */
    char pad20[0x30 - 0x20];
    virtual void Update() __asm__("UpdatePositionAct");   /* slot 4 */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetPhaseActGoal");   /* slot 5 */
};

/* ACT2 (0x80; vtable 0x2CA7E0): look-at act (UpdateActLookAt) */
struct ACT2 : ACT
{
    char pad1C[0x80 - 0x1C];
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetLookAtActGoal");   /* slot 5 */
    virtual void Detach() __asm__("DetachLookAtAct");   /* slot 2 */
    virtual void Update() __asm__("UpdateActLookAt");   /* slot 4 */
};

/* ACT4 (0x28; vtable 0x2CA7A0) */
struct ACT4 : ACT
{
    char pad1C[0x28 - 0x1C];
    virtual void Update() __asm__("UpdatePathAct");   /* slot 4 */
};

/* ACT5 (0x30; vtable 0x2CA760): an act over another act and an offset (jt.cpp's ACT5 view) */
struct ACT5 : ACT
{
    char pad1C[0x30 - 0x1C];
    virtual void Attach() __asm__("AttachOffsetAct");   /* slot 1 */
    virtual void Detach() __asm__("DetachOffsetAct");   /* slot 2 */
    virtual void SwitchTarget() __asm__("SwitchOffsetActTarget");   /* slot 3 */
    virtual void Update() __asm__("UpdateOffsetAct");   /* slot 4 */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetOffsetActGoal");   /* slot 5 */
};

/* ACT6 (0x60; vtable 0x2CA720): a matrix (identity at construction) */
struct ACT6 : ACT
{
    int n1C;
    float mat[4][4];        /* +0x20 the matrix applied */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetMatrixActGoal");   /* slot 5 */
    virtual void Update() __asm__("UpdateMatrixAct");   /* slot 4 */
};

/* ACT8 (0x40; vtable 0x2CA6E0): a scale and a rest position (act.cpp's ACTREST view) */
struct ACT8 : ACT
{
    char pad1C[0x40 - 0x1C];
    virtual void Update() __asm__("UpdateBezierAct");   /* slot 4 */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetBezierActGoal");   /* slot 5 */
};

/* ACT9 (0x30; vtable 0x2CA6A0) */
struct ACT9 : ACT
{
    char pad1C[0x30 - 0x1C];
    virtual void Attach() __asm__("AttachKind9Act");   /* slot 1 */
};

/* The classes whose slot functions other TUs define (their vtables lie in those TUs' part of the linkonce area;
   act.cpp's factories only construct them). */

/* ACTSEG (0x2C; vtable 0x2CAF08, asega.cpp's): an animation segment's channel act (kind 0), built by aseg.cpp's
   ApplyAsegCore (AllocAct(0x2C), vtable stored by hand) for each CHN of the segment and linked on its ASEGA. */
struct ACTSEG : ACT
{
    AC *pac;                /* +0x1C the channel's vector curve (a CHN without isScalar), or 0 */
    ACG *pacg;              /* +0x20 the channel's float curve (CHN::isScalar); Attach binds it and the ASEGA's time
                               into the reference XF (asega.cpp's "rotation curve"), or 0 */
    ASEGA *pasega;          /* +0x24 the segment application; DetachXfActById finds an ASEGA's acts by it */
    ACTSEG *pactsegNext;    /* +0x28 next channel act of the same ASEGA (ASEGA::pactsegFirst list) */
    virtual void Attach() __asm__("AttachActseg");   /* slot 1 */
    virtual void Detach() __asm__("DetachActseg");   /* slot 2 */
    virtual void Update() __asm__("UpdateActseg");   /* slot 4 */
    virtual void GetGoal(qword *ppos, VECTOR *pdvec) __asm__("GetActsegGoal");   /* slot 5 */
};
typedef char assert_ACTSEG_size[sizeof(ACTSEG) == 0x2C ? 1 : -1];
typedef char assert_ACTSEG_pac[SLY2_OFFSETOF(ACTSEG, pac) == 0x1C ? 1 : -1];

/* ACT3 (0x20; vtable 0x2DB338, tail.cpp's): kind 3 */
struct ACT3 : ACT
{
    int n1C;
    virtual void Attach() __asm__("AttachChainAct");   /* slot 1 */
    virtual void Clone() __asm__("CloneChainAct");   /* slot 6 */
};

/* ACT7 (0x20; vtable 0x2D2E58, ik.cpp's): kind 7 */
struct ACT7 : ACT
{
    int n1C;
    virtual void Attach() __asm__("AttachIkAct");   /* slot 1 */
    virtual void Clone() __asm__("CloneIkAct");   /* slot 6 */
};

/* AC5 (0x20; vtable 0x2CB5A8, emitted by blend.cpp, which declares AC/AC5/ACG/ACG2 itself with its own parameter
   types: its local typedefs clash with sly2/basic.h): blend-channel curve; a new slot 4 (inline, `return 5`) */
struct AC5 : AC
{
    int an[7];
    virtual void Prepare(void *pv) __asm__("BlendChannelPose");   /* slot 2 */
    virtual void Evaluate(void *pv, void *pdv, float g, float dtScale) __asm__("EvaluateBlendChnPosVel");   /* slot 3 */
    virtual int GetKind() __asm__("GetBlendCurveKind");   /* slot 4 */
};

/* ACG2 (0x14; vtable 0x2CB588, emitted by blend.cpp): blend-channel float curve */
struct ACG2 : ACG
{
    int an[4];
    virtual void Evaluate(float *pg, float *pdg, float g, float dtScale) __asm__("EvaluateBlendFloatCurve");   /* slot 2 */
};

#endif
