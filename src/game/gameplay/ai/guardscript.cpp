// guardscript.cpp -- guard scripts. The script object (CID113, cid 0x71, an LO) holds a list of commands (SSGE: set the
// context object, set walk/pause times, teleport, message, wait, go to / follow a path, look at, animation, labels
// and jumps, busy on/off, relay alarm) appended by its Splice options; the script-running guard behaviour (BHV169,
// BHV169, guard flag 0x21) steps through them, spawning child behaviours for the commands that take time. Entry
// points: AddGuardScript* (Splice), StartGuardScript, SetBhvScriptScript, UpdateBhvScript, ExecuteBhvScriptCommand.
// TU 0x1709A8..0x171F58 (42 functions, address order).
// Types: SSGE (with a kind-selected argument union), CID113, BHV169 and BHVGUARD are TU-private views written for the
// 26 m2c bodies and a few small functions; the other functions keep the per-function views they were matched with,
// each in a namespace named after it (as vec.cpp and mark.cpp do).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_guardscript
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_guardscript
#include "sly2/basic.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/classes.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"

// 0x171DF8 StaticInit_guardscript and GlobalConstructors_guardscript: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_BHV187 = 0xBB,
    CID_BHV188 = 0xBC,
    CID_SHAPE = 0xA,
    CID_PIPE = 0xB,
    CID_RAIL = 0xD,
    CID_BHVGUARD = 0x9C,
    CID_BHVANIM = 0x9D,
    CID_BHVIDLE = 0x9E,
    CID_BHVLEAP = 0xAC,
    CID_BHVRAIL = 0xB8,
    CID_BHVPIPE = 0xB9,
    CID_BHVGOTO = 0xBA,
    CID_BHVPATROLPATH = 0xC0,
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_SCRIPT = 0x21,
};

/* Guard script command kinds (SSGE::kind). */
enum {
    SSGK_SET_CONTEXT = 0,       /* look further oids up relative to this object */
    SSGK_SET_WALK_TIMES = 1,    /* guard +0x2CC */
    SSGK_SET_PAUSE_TIMES = 2,   /* guard +0x2D4 */
    SSGK_TELEPORT = 3,
    SSGK_MESSAGE = 4,
    SSGK_WAIT = 5,
    SSGK_FOLLOW_PATH = 6,       /* patrol path, rail or pipe */
    SSGK_GOTO = 7,
    SSGK_ANIM = 8,
    SSGK_LABEL = 9,
    SSGK_JUMP = 10,
    SSGK_BUSY_ON = 11,
    SSGK_BUSY_OFF = 12,
    SSGK_LEAP = 13,             /* BHVLEAP (cid 0xAC) */
    SSGK_PLAY_BINDING = 14,     /* BHVANIM on the behaviour's own binding */
    SSGK_RELAY_ALARM = 15,
};
/* The guard script object is CID113 (classes.h; its commands from +0x50), the commands SSGE and the script-running
   behaviour BHV169 (sly2/guard.h, classes.h). */
extern "C" {
void InitLo(CID113 *script);
void PostLoLoad(CID113 *script);
void InitSl(DL *pdl, int ibDle);       /* InitDl */
void AppendSlEntry(DL *pdl, void *pv);        /* AppendDlEntry */
void PrepareGuardScriptTargets(CID113 *script);
SSGE *AppendGuardScriptCommand(CID113 *script, int kind, int id) __asm__("AddGuardScriptCommand");
void StartGuardScript(LO *object, CID113 *script);
void InitBhv(BHV169 *behavior);
void CloseBhv(BHV169 *behavior);
void UpdateBhv(BHV169 *behavior);
void ClearBhvBusy(BHV169 *behavior);
void PopBhv(void *behavior);
void AddStepguardSgf(STEPGUARD *guard, int sgf);
void RemoveStepguardSgf(STEPGUARD *guard, int sgf);
void SetBhvScriptScript(BHV169 *behavior, CID113 *script, int allowPartialRoute, SSGRESUME *resume);
int IsBhvScriptCommandBusy(BHV169 *behavior);
SSGE *FindGuardScriptLabel(CID113 *script, int id);
void *FindStepBehavior(LO *object, int cid);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001709A8 {
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
// 0x1709A8: Classifies a script target for moving along it: 0 for a rail-like object (cid 0xD), 1 for a pipe-like
// object (cid 0xB), else -1 (inferred nouns).
extern "C" int GetShapeMoveKind(void *p)
{
    if (IsBasicDerivedFrom__FP5BASIC3CID(p, CID_RAIL))
        return 0;
    if (IsBasicDerivedFrom__FP5BASIC3CID(p, CID_PIPE))
        return 1;
    return -1;
}
} // namespace M_func_001709A8

// 0x1709F8: Guard script (cid 0x71) vtable slot 7 (Init): LO Init, then an empty command list.
extern "C" void InitGuardScript(CID113 *script)
{
    InitLo(script);
    InitSl(&script->commands, 0x18);
}

// 0x170A28: Guard script vtable slot 15 (PostLoad): LO post-load, then prepares its targets.
void CID113::PostLoad()
{
    CID113 *script = this;
    PostLoLoad(script);
    PrepareGuardScriptTargets(script);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00170A58 {
LO *FindSwObject__FP2SWi3OIDP2LO(SW *sw, int findKind, int oid, LO *context) __asm__("FindSwObject__FP2SWi3OIDP2LO");
int GetShapeMoveKind(LO *object) __asm__("GetShapeMoveKind");
void EnsureShapeVolumeSegments(LO *shape) __asm__("EnsureShapeVolumeSegments");
// 0x170A58: Walks the command list resolving targets: set-context commands (kind 0) change the lookup context;
// move-along-shape commands (kind 6) prepare their target shape.
// Also known as: PostLoadCmdlist (upstream readable / Sly 1 name; PostLoadCmdlist__FP7CMDLIST).
void PrepareGuardScriptTargets(LO *context) __asm__("PrepareGuardScriptTargets"); void PrepareGuardScriptTargets(LO *context)
{
    SSGE *command;
    for (command = (SSGE *)((CID113 *)context)->commands.head; command; command = command->next) {   /* context starts as the script itself */
        switch (command->kind) {
        case SSGK_FOLLOW_PATH: {
            LO *shape = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, command->id, context);
            if (shape && GetShapeMoveKind(shape) == -1)
                EnsureShapeVolumeSegments(shape);
            break;
        }
        case SSGK_SET_CONTEXT:
            context = FindSwObject__FP2SWi3OIDP2LO(&g_sw, context ? 4 : 5, command->id, context);
            break;
        }
    }
}
} // namespace M_func_00170A58

// 0x170B40: Splice option: appends command kind 0, set the lookup context object (oid).
extern "C" void AddGuardScriptSetContext(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_SET_CONTEXT, id);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00170B60 {
SSGE *AddGuardScriptCommand(void *script, int kind, int id) __asm__("AddGuardScriptCommand");
// 0x170B60: Splice option: appends command kind 1, set the guard's walk-time range (copied to guard +0x2CC).
extern "C" void AddGuardScriptSetWalkTimes(void *script, LM *range) { AddGuardScriptCommand(script, SSGK_SET_WALK_TIMES, -1)->range = *range; }
} // namespace M_func_00170B60

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00170BA0 {
SSGE *AddGuardScriptCommand(void *script, int kind, int id) __asm__("AddGuardScriptCommand");
// 0x170BA0: Splice option: appends command kind 2, set the guard's pause-time range (guard +0x2D4).
extern "C" void AddGuardScriptSetPauseTimes(void *script, LM *range) { AddGuardScriptCommand(script, SSGK_SET_PAUSE_TIMES, -1)->range = *range; }
} // namespace M_func_00170BA0

// 0x170BE0: Splice option: appends command kind 3, teleport the guard to a target (oid).
extern "C" void AddGuardScriptTeleport(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_TELEPORT, id);
}

// 0x170C00: Splice option: appends command kind 4, send the guard message 4 with an argument.
extern "C" void AddGuardScriptMessage(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_MESSAGE, id);
}

// 0x170C20: Splice option: appends command kind 5, wait/idle for a time.
extern "C" void AddGuardScriptWait(CID113 *script, float g)
{
    AppendGuardScriptCommand(script, SSGK_WAIT, -1)->g = g;
}

// 0x170C50: Appends command kind 6, follow a path target (oid): BHVPATROLPATH on a patrol path with `pathArg` and an
// optional second target, or BHVRAIL/BHVPIPE when the guard is attached to a rail or pipe.
extern "C" void AddGuardScriptFollowPath(CID113 *script, int id, int pathArg, int secondOid)
{
    SSGE *command;
    command = AppendGuardScriptCommand(script, SSGK_FOLLOW_PATH, id);
    command->n = pathArg;
    command->secondOid = secondOid;
}

// 0x170C90: Appends command kind 7, go to a target (oid; a point when -1) with BHVGOTO, optionally looking at
// `lookAtOid` (-1 for none); `gotoParam` is copied to BHVGOTO +0xB0 (meaning unknown).
extern "C" void AddGuardScriptGoto(CID113 *script, int id, int lookAtOid, float gotoParam)
{
    SSGE *command;
    command = AppendGuardScriptCommand(script, SSGK_GOTO, id);
    command->gotoParam = gotoParam;
    command->lookAtOid = lookAtOid;
}

// 0x170CD0: Splice option: appends command kind 8, play the animation of a target (oid).
extern "C" void AddGuardScriptAnim(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_ANIM, id);
}

// 0x170CF0: Splice option: appends command kind 9, a label.
extern "C" void AddGuardScriptLabel(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_LABEL, id);
}

// 0x170D10: Splice option: appends command kind 10, jump to a label.
extern "C" void AddGuardScriptJump(CID113 *script, int id)
{
    AppendGuardScriptCommand(script, SSGK_JUMP, id);
}

// 0x170D30: Splice option: appends command kind 11, behaviour busy on (also suspends target selection and
// nearest-command resume).
extern "C" void AddGuardScriptBusyOn(CID113 *script, int n1, int n2)
{
    AppendGuardScriptCommand(script, SSGK_BUSY_ON, -1);
}

// 0x170D50: Splice option: appends command kind 12, behaviour busy off.
extern "C" void AddGuardScriptBusyOff(CID113 *script, int n1, int n2)
{
    AppendGuardScriptCommand(script, SSGK_BUSY_OFF, -1);
}

// 0x170D70: Splice option: appends command kind 13, leap at a target (oid) with an int argument (BHVLEAP, cid 0xAC).
extern "C" void AddGuardScriptLeap(CID113 *script, int id, int n)
{
    AppendGuardScriptCommand(script, SSGK_LEAP, id)->n = n;
}

// 0x170DA0: Splice option: appends command kind 14, play the behaviour's own animation binding (BHVANIM, cid 0x9D)
// with an int argument and a sensor block.
extern "C" void AddGuardScriptPlayBinding(CID113 *script, int id, int n)
{
    AppendGuardScriptCommand(script, SSGK_PLAY_BINDING, id)->n = n;
}

// 0x170DD0: Splice option: appends command kind 15, ensure the relay alarm of a target.
extern "C" void AddGuardScriptEnsureRelayAlarm(CID113 *script, int id, int n)
{
    AppendGuardScriptCommand(script, SSGK_RELAY_ALARM, id)->n = n;
}

// 0x170E00: Allocates a command node from the script pool, sets its kind and argument and appends it to the
// script's list; returns it.
/* Appends a command of the given kind (and target id, -1 for none) to the script; returns it. */
// Also known as: PcmdAppend (upstream readable / Sly 1 name; PcmdAppend__FP7CMDLISTi3OID).
extern "C" SSGE *AddGuardScriptCommand(CID113 *script, int kind, int id)
{
    SSGE *command;
    command = (SSGE *)AllocZeroedFromSlotheap(&g_sw.slotheapSsge);
    command->kind = kind;
    command->id = id;
    AppendSlEntry(&script->commands, command);
    return command;
}

// 0x170E68: Starts this script on a guard, if one is given.
extern "C" void StartGuardScriptOnGuard(CID113 *script, LO *object)
{
    if (object != 0) {
        StartGuardScript(object, script);
    }
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A52B0 __asm__("D_002A52B0") = 3.40282347e+38f; }
// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00170E90 {
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
extern const float D_002A52B0 __asm__("D_002A52B0");   /* defined below where the ELF has it (.rodata order) */
LO*FindSwObject__FP2SWi3OIDP2LO(void*,int,int,LO*) __asm__("FindSwObject__FP2SWi3OIDP2LO");
void GetGuardScriptTargetPose(LO*,qword,LO*,qword*,float*) __asm__("GetGuardScriptTargetPose");
void FindShapeClosestPoint(LO*,qword,void*,qword*,qword*,float*,float*) __asm__("FindShapeClosestPoint");
void FindShapeClosestPointFromDistance(LO*,qword,void*,qword*,qword*,float*,float*,float) __asm__("FindShapeClosestPointFromDistance");
// VU0 helper: vector difference.
// VU0 helper: length of a vector.
extern "C" { static inline float Length(qword v)
{
    qword d, one;
    float f;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "=j"(d), "=&j"(one)
            : "0"(v));
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(d) : "0"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d));
    return f;
} }
// Choose the closest eligible point while walking the selector chain.
// 0x170E90: Picks the command (kind 3/7 point or kind 6 shape) whose point is closest to a position, skipping
// commands inside busy-on/off brackets; used to resume a script near the guard.
// Also known as: RefindSavedScript (upstream readable / Sly 1 name;
//   RefindSavedScript__FP7CMDLISTG9VU_VECTORP11SAVEDSCRIPT).
extern "C" void ChooseGuardScriptNearestCommand(LO *root, qword pos, SSGRESUME *out)
{
    SSGRESUME choice;
    U hit;
    float value;
    LO *current = root;
    SSGE *node;
    int stopped = 0;
    float best = D_002A52B0;
    for (node = (SSGE *)((CID113 *)current)->commands.head; node; node = node->next) {
        switch (node->kind) {
        case SSGK_BUSY_ON:
            stopped = 1;
            break;
        case SSGK_BUSY_OFF:
            stopped = 0;
            break;
        case SSGK_TELEPORT:
        case SSGK_GOTO:
            if (stopped)
                break;
            if (node->id != -1) {
                LO *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, node->id, current);
                GetGuardScriptTargetPose(root, pos, object, &hit.q, 0);
            } else if (node->ppos)
                hit.q = *node->ppos;
            {
                float d = Length(VuSub(pos, hit.q));
                if (d < best) {
                    best = d;
                    choice.command = node;
                    choice.context = current;
                }
            }
            break;
        case SSGK_FOLLOW_PATH:
            if (stopped)
                break;
            {
                LO *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, node->id, current);
                if (out && out->command == node)
                    FindShapeClosestPointFromDistance(object, pos, 0, &hit.q, 0, 0, &value, out->pathParam);
                else
                    FindShapeClosestPoint(object, pos, 0, &hit.q, 0, 0, &value);
                {
                    float d = Length(VuSub(pos, hit.q));
                    if (d < best) {
                        best = d;
                        choice.command = node;
                        choice.pathParam = value;
                        choice.context = current;
                    }
                }
            }
            break;
        case SSGK_SET_CONTEXT: {
            int kind = 5;
            if (current)
                kind = 4;
            current = FindSwObject__FP2SWi3OIDP2LO(&g_sw, kind, node->id, current);
            break;
        }
        case SSGK_SET_WALK_TIMES:
        case SSGK_SET_PAUSE_TIMES:
        case SSGK_MESSAGE:
        case SSGK_WAIT:
        case SSGK_ANIM:
        case SSGK_LABEL:
        case SSGK_JUMP:
            break;
        }
    }
    if (out)
        *out = choice;
}
} // namespace M_func_00170E90

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001710F8 {
typedef int qword __attribute__((mode(TI)));
struct VEC { float x, y, z, w; };
int IsBasicDerivedFrom__FP5BASIC3CID(LO *object, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void FindShapeClosestPoint(LO *shape, qword pos, int a, qword *point, struct VEC *tangent, int b, int c) __asm__("FindShapeClosestPoint");
float atan2f(float y, float x) __asm__("atan2f");
qword GetLoWorldPos(LO *object) __asm__("GetLoWorldPos");
// 0x1710F8: Position and heading of a target: the closest point and tangent on a SHAPE (cid 0xA), else the object's
// position and yaw.
// Also known as: GetCmdAim (upstream readable / Sly 1 name; GetCmdAim__FP5GUARDG9VU_VECTORP2LOP6VECTORPf).
extern "C" void GetGuardScriptTargetPose(void *script, qword pos, LO *target, qword *posOut, float *headingOut)
{
    union { qword q; } u;
#define q u.q
    struct VEC tangent;
    float heading;
    if (target) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_SHAPE)) {
            FindShapeClosestPoint(target, pos, 0, &q, &tangent, 0, 0);
            heading = atan2f(tangent.y, tangent.x);
        } else {
            float *forwardY, *forwardX;
            q = GetLoWorldPos(target);
            forwardY = (float *)target->GetWorldMatrix();
            forwardX = (float *)target->GetWorldMatrix();
            heading = atan2f(forwardY[1], forwardX[0]);
        }
    } else {
        q = pos;
        heading = 0.0f;
    }
    if (posOut)
        *posOut = q;
    if (headingOut)
        *headingOut = heading;
}
} // namespace M_func_001710F8
#undef q

// 0x1711F0: Finds the label command (kind 9) with the given id.
/* The label command (kind 9) with the given id. The unused array is part of the original frame. */
extern "C" SSGE *FindGuardScriptLabel(CID113 *script, int id)
{
    int unused[4];
    SSGE *command;
    for (command = (SSGE *)script->commands.head; command; command = command->next)
        if (command->kind == SSGK_LABEL && command->id == id)
            break;
    return command;
}

// 0x171228: Script behaviour (BHV169) vtable slot 7 (OnPush): base, then raises guard flag 0x21.
void BHV169::OnPush()
{
    BHV169 *behavior = this;
    InitBhv(behavior);
    AddStepguardSgf(((STEPGUARD *)behavior->owner), SGF_SCRIPT);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171258 {
void FramePtr_Assign(CRefPtr<CFrame> *pref, CFrame *frame) __asm__("__as__t7CRefPtr1Z6CFrameP6CFrame");
void AssignSidebagRef(CRefPtr<CSidebag> *pref, CSidebag *sidebag) __asm__("AssignSidebagRef");
}
// 0x171258: Switches the script a script behaviour runs: stops the old one (splice event 3), starts the new one
// (event 2) at its first command or at a saved resume point.
// Also known as: SetTarget (upstream readable / Sly 1 name; SetTarget__9JOBSCRIPTP7CMDLISTiP11SAVEDSCRIPT).
extern "C" void SetBhvScriptScript(BHV169 *behavior, CID113 *script, int allowPartialRoute, SSGRESUME *resume)
{
    using namespace M_func_00171258;
    if (script == behavior->script)
        return;
    if (behavior->script) {
        PopBhv(&behavior->pbhvNext);
        behavior->HandleSpliceEvent(3, 0, 0, 0, 0);
        FramePtr_Assign(&behavior->scriptFrame, 0);
        AssignSidebagRef(&behavior->scriptVariables, 0);
        behavior->context = 0;
        behavior->currentCommand = 0;
        behavior->isCommandBusy = 0;
        behavior->allowPartialRoute = 0;
    }
    behavior->script = script;
    if (!script)
        return;
    FramePtr_Assign(&behavior->scriptFrame, script->scriptFrame.m_p);
    behavior->HandleSpliceEvent(2, 0, 0, 0, 0);
    behavior->allowPartialRoute = allowPartialRoute;
    if (!resume || !resume->command) {
        CID113 *s = behavior->script;
        SSGE *first = (SSGE *)s->commands.head;
        behavior->context = s;
        behavior->currentCommand = first;
    } else {
        behavior->currentCommand = resume->command;
        behavior->resume = resume;
        behavior->context = resume->context;
    }
    if (behavior->currentCommand)   /* ExecuteBhvScriptCommand (vtable slot 15) */
        behavior->ExecuteScriptCommand(behavior->currentCommand);
    behavior->resume = 0;
}

// 0x171398: Script behaviour vtable slot 9 (Close): base, clears busy, drops the script and lowers flag 0x21.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvScript(BHV169 *behavior)
{
    CloseBhv(behavior);
    ClearBhvBusy(behavior);
    SetBhvScriptScript(behavior, 0, 0, 0);
    RemoveStepguardSgf(((STEPGUARD *)behavior->owner), SGF_SCRIPT);
}

// 0x1713E8: Script behaviour vtable slot 10 (Update): executes commands until one is still busy or the script ends.
/* Runs the script: while a command is current and done (IsBhvScriptCommandBusy), steps to the next one and starts it
   (behaviour slot 15). Returns the current command (0 at the end). */
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" SSGE *UpdateBhvScript(BHV169 *behavior)
{
    SSGE *next;
    SSGE *command;
    UpdateBhv(behavior);
    command = behavior->currentCommand;
    if (command != 0) {
        for (;;) {
            command = (SSGE *)IsBhvScriptCommandBusy(behavior);
            if (command != 0)
                break;   // still running
            command = behavior->currentCommand;
            next = command->next;
            behavior->currentCommand = next;
            if (next == 0)
                break;   // the script ended (returns the last command)
            behavior->ExecuteScriptCommand(next);   // StartCommand
            command = behavior->currentCommand;
            if (command == 0)
                break;
        }
    }
    return command;
}

// 0x171458: True when the script has no current command (finished).
extern "C" int IsBhvScriptDone(BHV169 *behavior)
{
    return behavior->currentCommand == 0;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171468 {
int IsBhvpatrolpathDone(void *bhv) __asm__("IsBhvpatrolpathDone");
int HasBhvrailArrived(void *bhv) __asm__("HasBhvrailArrived");
int HasBhvpipeArrived(void *bhv) __asm__("HasBhvpipeArrived");
int HasBhvgotoArrived(void *bhv) __asm__("HasBhvgotoArrived");
int IsBhvanimDone(void *bhv) __asm__("IsBhvanimDone");
void SetStepguardMoverRadTarget(GUARDMOVER *mover, float heading) __asm__("SetStepguardMoverRadTarget");
// 0x171468: True while the running command's child behaviour (goto, look, wait, kinds 13/14) is still active; pops
// it when done.
// Also known as: FRunning (upstream readable / Sly 1 name).
extern "C" int IsBhvScriptCommandBusy(BHV169 *behavior) {
    if (!behavior->isCommandBusy) return 0;
    switch (behavior->currentCommand->kind) {
    case SSGK_FOLLOW_PATH:
        switch (((STEPGUARD *)behavior->owner)->attachKind) {
        case -1:
            if (IsBhvpatrolpathDone(behavior->pbhvNext)) behavior->isCommandBusy = 0;
            break;
        case 0:
            if (HasBhvrailArrived(behavior->pbhvNext)) behavior->isCommandBusy = 0;
            break;
        case 1:
            if (HasBhvpipeArrived(behavior->pbhvNext)) behavior->isCommandBusy = 0;
            break;
        }
        break;
    case SSGK_GOTO:
        if (HasBhvgotoArrived(behavior->pbhvNext)) {
            behavior->isCommandBusy = 0;
            SetStepguardMoverRadTarget((&((STEPGUARD *)behavior->owner)->mover), behavior->heading);
        }
        break;
    case SSGK_WAIT:
        if (behavior->currentCommand->g < g_clock.time - behavior->commandStartTime) behavior->isCommandBusy = 0;
        break;
    case SSGK_LEAP: case SSGK_PLAY_BINDING:
        if (IsBhvanimDone(behavior->pbhvNext)) behavior->isCommandBusy = 0;
        break;
    }
    if (!behavior->isCommandBusy) PopBhv(&behavior->pbhvNext);
    return behavior->isCommandBusy;
}
} // namespace M_func_00171468

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001715B8 {
void *FindSwObject__FP2SWi3OIDP2LO(SW *sw, int findKind, int oid, LO *context) __asm__("FindSwObject__FP2SWi3OIDP2LO");
// 0x1715B8: Finds a world object by oid relative to the script's current context object.
// Also known as: PloNear (upstream readable / Sly 1 name; PloNear__9JOBSCRIPT3OID).
extern "C" void *FindBhvScriptTarget(BHV169 *behavior, int oid)
{
    LO *context = behavior->context;
    int findKind = context ? 4 : 5;
    return FindSwObject__FP2SWi3OIDP2LO(&g_sw, findKind, oid, context);
}
} // namespace M_func_001715B8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_001715F0 {
void *FindBhvScriptTarget(BHV169 *behavior, int oid) __asm__("FindBhvScriptTarget");
void GetBhvScriptTargetPose(BHV169 *behavior, LO *target, qword *posOut, float *headingOut) __asm__("GetBhvScriptTargetPose");
void TeleportBhvScriptGuard(BHV169 *behavior, qword pos, float heading) __asm__("TeleportBhvScriptGuard");
BHV *PushStepBehavior(STEPGUARD *guard, int cid) __asm__("PushStepBehavior");
void EnsureShapeVolumeSegments(void *shape) __asm__("EnsureShapeVolumeSegments");
void SetBhvrailGoal(BHV *bhv, int goal) __asm__("SetBhvrailGoal");
void SetBhvpipeGoal(BHV *bhv, int goal) __asm__("SetBhvpipeGoal");
int IsBasicDerivedFrom__FP5BASIC3CID(void *object, int cid) __asm__("IsBasicDerivedFrom__FP5BASIC3CID");
void SetBhvgotoGoal(BHVGOTO *bhv, PATHLINKEND *goal) __asm__("SetBhvgotoGoal");
void ApplyAsegCore(void *aseg, int guard, int b, void *asega, float start, float speed) __asm__("ApplyAsegCore");
void SetBhvBusy(BHV169 *behavior) __asm__("SetBhvBusy");
void ReleaseAnimBinding(ANIMBINDING *binding, STEPGUARD *guard) __asm__("ReleaseAnimBinding");
void ResolveAnimBinding(ANIMBINDING *binding, STEPGUARD *guard) __asm__("ResolveAnimBinding");
void func_001E0020(void *sgg, int alarm) __asm__("EnsureSggAlarm__FP3SGGP5ALARM");
void ClearPointerPair(void *goal) __asm__("ClearPointerPair");

}

// 0x1715F0: Script behaviour vtable slot 15: executes one script command (kinds 0-15): set context, set times,
// teleport, message, spawn child behaviours, label/jump, busy on/off, relay alarm.
// Also known as: Execute (upstream readable / Sly 1 name; Execute__9JOBSCRIPTP3CMD).
void BHV169::ExecuteScriptCommand(SSGE *command)
{
    using namespace M_func_001715F0;
    BHV169 *behavior = this;
    union { qword q; } pose;
    float heading;
    void *args;
    int asega;
    int attachKind;
    switch (command->kind) {
    case SSGK_SET_CONTEXT:
        behavior->context = (LO *)FindBhvScriptTarget(behavior, command->id);
        break;
    case SSGK_SET_WALK_TIMES:
        ((STEPGUARD *)behavior->owner)->walkTimes = command->range;
        break;
    case SSGK_SET_PAUSE_TIMES:
        ((STEPGUARD *)behavior->owner)->pauseTimes = command->range;
        break;
    case SSGK_TELEPORT: {
        GetBhvScriptTargetPose(behavior, (LO *)FindBhvScriptTarget(behavior, command->id), &pose.q, &heading);
        TeleportBhvScriptGuard(behavior, pose.q, heading);
        break;
    }
    case SSGK_MESSAGE:
        args = &command->id;
        behavior->HandleSpliceEvent(4, 0, 1, &args, 0);
        break;
    case SSGK_WAIT: {
        STEPGUARD *guard = (STEPGUARD *)behavior->owner;
        switch (guard->attachKind) {
        case 2:
            PushStepBehavior(guard, CID_BHV187)->Start();
            break;
        case 3:
            PushStepBehavior(guard, CID_BHV188)->Start();
            break;
        case -1:
            PushStepBehavior(guard, CID_BHVIDLE)->Start();
            break;
        }
        behavior->isCommandBusy = 1;
        break;
    }
    case SSGK_FOLLOW_PATH: {
        void *target = FindBhvScriptTarget(behavior, behavior->currentCommand->id);
        STEPGUARD *guard;
        if (!target) break;
        guard = (STEPGUARD *)behavior->owner;
        attachKind = guard->attachKind;
        switch (attachKind) {
        case -1: {
            BHVPATROLPATH *child = (BHVPATROLPATH *)PushStepBehavior(guard, CID_BHVPATROLPATH);
            child->shape = (SHAPE *)target;
            child->isForward = command->n;
            child->isDirect = behavior->allowPartialRoute;
            if (!behavior->allowPartialRoute) EnsureShapeVolumeSegments(target);
            if (behavior->resume) child->pathParam = behavior->resume->pathParam;   /* resume where it was */
            if (command->secondOid != -1) child->lookAtTarget = (LO *)FindBhvScriptTarget(behavior, command->secondOid);
            child->Start();
            break;
        }
        case 0: {
            BHV *child = PushStepBehavior(guard, CID_BHVRAIL);
            SetBhvrailGoal(child, (int)behavior->currentCommand->ppos);
            child->Start();
            break;
        }
        case 1: {
            BHV *child = PushStepBehavior(guard, CID_BHVPIPE);
            SetBhvpipeGoal(child, (int)behavior->currentCommand->ppos);
            child->Start();
            break;
        }
        }
        behavior->isCommandBusy = 1;
        break;
    }
    case SSGK_GOTO: {
        SSGE *point;
        STEPGUARD *guard;
        PATHLINKEND goal;
        point = behavior->currentCommand;
        if (point->id == -1) {
            goal.pos = *point->ppos;
            behavior->heading = point->heading;
        } else {
            void *target = FindBhvScriptTarget(behavior, point->id);
            if (target) {
                GetBhvScriptTargetPose(behavior, (LO *)target, 0, &behavior->heading);
                if (IsBasicDerivedFrom__FP5BASIC3CID(target, CID_SHAPE)) goal.shape = (SHAPE *)target;
                else goal.object = (LO *)target;
            }
        }
        guard = (STEPGUARD *)behavior->owner;
        attachKind = guard->attachKind;
        if (attachKind == -1) {
            BHVGOTO *child = (BHVGOTO *)PushStepBehavior(guard, CID_BHVGOTO);
            SetBhvgotoGoal(child, (PATHLINKEND *)&goal);
            if (command->lookAtOid != -1) child->lookAtTarget = (LO *)FindBhvScriptTarget(behavior, command->lookAtOid);
            child->gotoParam = command->gotoParam;
            child->isDirect = behavior->allowPartialRoute;
            child->Start();
        }
        behavior->isCommandBusy = 1;
        break;
    }
    case SSGK_ANIM: {
        ApplyAsegCore(FindBhvScriptTarget(behavior, command->id), 0, 0, &asega, 0.0f, 1.0f);
        break;
    }
    case SSGK_LABEL:
        break;
    case SSGK_JUMP: {
        SSGE *label = FindGuardScriptLabel(behavior->script, command->id);
        if (label) behavior->currentCommand = label;
        break;
    }
    case SSGK_BUSY_ON:
        SetBhvBusy(behavior);
        break;
    case SSGK_BUSY_OFF:
        ClearBhvBusy(behavior);
        break;
    case SSGK_LEAP: {
        void *target = FindBhvScriptTarget(behavior, behavior->currentCommand->id);
        BHVLEAP *child = (BHVLEAP *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHVLEAP);
        child->target = (LO *)target;
        child->extra = 1;
        child->isReversed = command->n;
        child->Start();
        behavior->isCommandBusy = 1;
        break;
    }
    case SSGK_PLAY_BINDING:
        ReleaseAnimBinding(&behavior->animBinding, (STEPGUARD *)behavior->owner);
        behavior->animBinding.oid = (OID)command->id;
        behavior->animBinding.flags = command->n;
        ResolveAnimBinding(&behavior->animBinding, (STEPGUARD *)behavior->owner);
        if (((STEPGUARD *)behavior->owner)->attachKind == -1) {
            BHVANIM *child = (BHVANIM *)PushStepBehavior((STEPGUARD *)behavior->owner, CID_BHVANIM);
            child->binding = &behavior->animBinding;
            child->Start();
            behavior->isCommandBusy = 1;
        }
        break;
    case SSGK_RELAY_ALARM:
        func_001E0020(FindBhvScriptTarget(behavior, command->id), command->n);
        break;
    }
    behavior->commandStartTime = g_clock.time;
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171AF8 {
qword GetLoWorldPos(LO *p) __asm__("GetLoWorldPos");
void GetGuardScriptTargetPose(CID113 *script, qword pos, LO *target, qword *posOut, float *headingOut) __asm__("GetGuardScriptTargetPose");
}
// 0x171AF8: Target pose (GetGuardScriptTargetPose) taken relative to the guard's position.
extern "C" void GetBhvScriptTargetPose(BHV169 *behavior, LO *target, qword *posOut, float *headingOut)
{
    using namespace M_func_00171AF8;
    GetGuardScriptTargetPose(behavior->script, GetLoWorldPos(behavior->owner), target, posOut, headingOut);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171B60 {
extern qword D_002A2DD0 __asm__("D_002A2DD0");
void DetachStepguard(STEPGUARD *guard) __asm__("DetachStepguard");
void SetStepTargetHeading(STEPGUARD *guard, float heading) __asm__("SetStepTargetHeading");
}
// 0x171B60: Places the guard at a position and heading and zeroes its velocities.
// Also known as: PlaceGuard (upstream readable / Sly 1 name; PlaceGuard__FP5GUARDG9VU_VECTORf).
extern "C" void TeleportBhvScriptGuard(BHV169 *behavior, qword pos, float heading)
{
    using namespace M_func_00171B60;
    STEPGUARD *guard = (STEPGUARD *)behavior->owner;
    DetachStepguard(guard);
    guard->TranslateToPos(pos);
    SetStepTargetHeading(guard, heading);
    guard->RotateToMat(&guard->matTarget);
    guard->SetVelocity(D_002A2DD0);
    guard->SetAngularVelocity(D_002A2DD0);
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171C18 {
void *func_002333B4(void *p, int c, int n) __asm__("memset");
}
// 0x171C18: Saves the current command and context (and remaining wait) so the script can resume later.
// Also known as: Save (upstream readable / Sly 1 name; Save__9JOBSCRIPTP11SAVEDSCRIPT).
extern "C" void SaveBhvScriptResumeState(BHV169 *behavior, SSGRESUME *resume)
{
    using namespace M_func_00171C18;
    switch (((STEPGUARD *)behavior->owner)->attachKind) {
    case -1:
        resume->command = behavior->currentCommand;
        resume->context = behavior->context;
        if (resume->command && resume->command->kind == SSGK_FOLLOW_PATH)
            resume->pathParam = ((BHVPATROLPATH *)behavior->pbhvNext)->pathParam;
        break;
    case 0: case 1:   /* on a rail or pipe: no resume point */
        func_002333B4(resume, 0, sizeof(*resume));
        break;
    }
}

// 0x171C98: Pops any running child behaviour and jumps the script to a label.
/* Jumps to the label command id of the running script (popping a running sub-behaviour first). */
// Also known as: SeekMarker (upstream readable / Sly 1 name; SeekMarker__9JOBSCRIPT3OID).
extern "C" void JumpBhvScriptToLabel(BHV169 *behavior, int id)
{
    if (behavior->script != 0) {
        if (behavior->isCommandBusy != 0) {
            PopBhv(&behavior->pbhvNext);
            behavior->isCommandBusy = 0;
        }
        behavior->currentCommand = FindGuardScriptLabel(behavior->script, id);
    }
}

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171CF8 {
void *FindStepBehavior(void *guard, int cid) __asm__("FindStepBehavior");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void SetBhvguardMode(void *bhv, BHVGUARDRECDATA *mode) __asm__("SetBhvguardMode");
// 0x171CF8: Puts the guard's root behaviour (BHVGUARD) in mode 5 running the given script.
// Also known as: TellGuardCommands (upstream readable / Sly 1 name; TellGuardCommands__FP2LOi).
void StartGuardScript(LO *guard, CID113 *script) __asm__("StartGuardScript"); void StartGuardScript(LO *guard, CID113 *script)
{
    BHVGUARDRECDATA mode;
    void *bhv = FindStepBehavior(guard, CID_BHVGUARD);
    if (bhv) {
        func_002333B4(&mode, 0, sizeof(mode));
        mode.arg.object = script;
        mode.kind = 5;
        SetBhvguardMode(bhv, &mode);
    }
}
} // namespace M_func_00171CF8

// Keeps its per-function views in a namespace of its own (see the file header).
namespace M_func_00171D58 {
void *FindStepBehavior(void *guard, int cid) __asm__("FindStepBehavior");
void *func_002333B4(void *p, int c, int n) __asm__("memset");
void SetBhvguardMode(void *bhv, BHVGUARDRECDATA *mode) __asm__("SetBhvguardMode");
// 0x171D58: Puts the guard's root behaviour back in mode 1 (its normal routine, inferred).
extern "C" void SetGuardRoutineMode(void *guard)
{
    BHVGUARDRECDATA mode;
    void *bhv = FindStepBehavior(guard, CID_BHVGUARD);
    if (bhv) {
        func_002333B4(&mode, 0, sizeof(mode));
        mode.kind = 1;
        SetBhvguardMode(bhv, &mode);
    }
}
} // namespace M_func_00171D58

// 0x171DA8: True if the guard's root behaviour is in mode 5 running this script.
/* Whether plo's guard behaviour (CID 0x9C) is in mode 5 on script command id. */
// Also known as: FGuardRunsCommands (upstream readable / Sly 1 name; FGuardRunsCommands__FP2LOT0).
extern "C" int IsGuardRunningScript(LO *object, int id)
{
    BHV156 *bhv;
    int running;
    bhv = (BHV156 *)FindStepBehavior(object, CID_BHVGUARD);
    running = 0;
    if ((bhv != 0) && (bhv->mode == 5)) {
        running = (int)bhv->modeObject == id;
    }
    return running;
}

