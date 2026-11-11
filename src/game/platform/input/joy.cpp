// joy.cpp -- the controllers (JOY, sly2/joy.h): pad startup and the per-frame state machine (mode, actuators, pressure
// mode), reading buttons and sticks with their deflection, the button pressed/handled masks, rumble, and the menu
// selection repeat (GetJoySelectionStepX/GetJoySelectionStepY). FPU asm (sqrt.s).
// One JOY (g_joy, g_joy) for pad port 0. UpdateJoy runs each frame: it follows the pad's connection state,
// switches it to analog/DualShock mode and sets up its actuators and pressure mode, then reads it (ReadJoy). Game
// code reads buttons as masks: held, pressed this frame, and "handled" (consumed so a press acts once).
// Entry points: StartupJoy, UpdateJoy, GetJoyButtonsPressed / HandleJoyButtons / SetJoyBtnHandled / ClearJoyButtons,
// TriggerJoyRumbleRumk, GetJoySelectionStepX / GetJoySelectionStepY, AddGrfusr / RemoveGrfusr.
// TU 0x187030..0x188488 (19 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_joy
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_joy
extern int g_grfjoyt;     /* g_grfjoyt: declared before joy.h's g_joy, since .bss is emitted in first-declaration order */
extern int g_grfusr;     /* the users' grf */
#include "sly2/basic.h"
#include "sly2/joy.h"
#include "sly2/cm.h"
#include "sly2/slotheap.h"
#include "sly2/util.h"
#include "sly2/scrap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/game.h"
#include "sly2/sw.h"

// 0x188330 StaticInit_joy and GlobalConstructors_joy: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The four stick axes the deflection is measured against. */
struct VEC2 { float x; float y; };
/* The event timer at g_game as ReadJoy reads it. */

extern char D_002DDEC0[];  /* the cheat codes */
VEC2 D_002BE950[4] = {{1.0f, 0.0f}, {0.0f, 1.0f}, {0.7f, 0.7f}, {0.7f, -0.7f}};   /* the four stick axes (joy's .data) */
unsigned short D_002BE970[12] = {0x2000, 0x8000, 0x1000, 0x4000, 0x10, 0x20, 0x40, 0x80, 0x4, 0x8, 0x1, 0x2};   /* pad button bits (d-pad, then the shape buttons, then L/R), named by no code (joy's .data) */
extern "C" {
void StartupCodes(void *pcodes);
void scePadInit(int n);
void scePadPortOpen(int port, int slot, JOY *pjoy);
int scePadGetState_243388(int port, int slot);
int scePadInfoMode(int port, int slot, int term, int offs);
int scePadSetMainMode(int port, int slot, int offs, int lock);
int scePadInfoAct(int port, int slot, int term, int offs);
void *func_002333B4(void *pv, int b, int cb) __asm__("memset");
int scePadSetActAlign(int port, int slot, unsigned char *pb);
int scePadInfoPressMode(int port, int slot);
int scePadEnterPressMode(int port, int slot);
int scePadGetState(int port, int slot);
int scePadRead(int port, int slot, unsigned char *pb);
void MatchCheatCodePress(void *p, JOY *pjoy, int grfbtn);
int GetHeroPo(void *p);
void BroadcastGameEvent(GAME *p, int n, int a);
int ReadJoy(JOY *pjoy);
int GetJoyButtonsPressed(JOY *pjoy, int grf);
void SetJoyBtnHandled(JOY *pjoy, int grf);
int HandleJoyButtons(JOY *pjoy, int grf);
}
void UpdateGrfjoytFromGrfusr();
void InitJoy(JOY *joy, int port, int slot);
void SetJoyJoys(JOY *joy, JOYS state, JOYK kind);
void GetJoyXYDeflection(JOY *joy, uchar rawX, uchar rawY, float *outX, float *outY, float *outDeflection,
                        uchar *prevRawX, uchar *prevRawY, int *outActive, LM *ranges);
void InitRumble(RUMBLE *prumble, int nPort, int nSlot);
void UpdateRumble(RUMBLE *prumble);
void TriggerRumbleRumk(RUMBLE *prumble, RUMK rumk, float dt);
void CopyAb(void *pvDst, void *pvSrc, uint cb);

// Boot step: derives the input-type flags, initialises the pad library and g_joy on port 0 slot 0, and starts the
// button-code (cheat) recogniser D_002DDEC0 (StartupCodes).
void StartupJoy()
{
    UpdateGrfjoytFromGrfusr();
    scePadInit(0);
    InitJoy(&g_joy, 0, 0);
    StartupCodes(D_002DDEC0);
}

// Sets user-state bits in g_grfusr and re-derives the input-type flags.
void AddGrfusr(int mask)
{
    g_grfusr |= mask;
    UpdateGrfjoytFromGrfusr();
}

// Clears user-state bits in g_grfusr and re-derives the input-type flags.
void RemoveGrfusr(int mask)
{
    g_grfusr &= ~mask;
    UpdateGrfjoytFromGrfusr();
}

// Derives g_grfjoyt (which input kinds are accepted) from the user-state bits; when bit 2 is clear, also resets
// the latched-button mask.
void UpdateGrfjoytFromGrfusr()
{
    int v = g_grfusr;
    if (v & 0x10) g_grfjoyt = 5;
    else if (v & 0xC) g_grfjoyt = 0;
    else if (v & 1) g_grfjoyt = 5;
    else if (v & 2) g_grfjoyt = 4;
    else g_grfjoyt = 7;
    if ((g_grfjoyt & 2) == 0) (*(int *)&g_joy.grfbtnLatched) = 0xFFFF;
}

// Initialises a controller: searching state, port and slot, opens the pad port (the JOY is the library's DMA
// buffer), enables vibration and sets uDpad to 1.
void InitJoy(JOY *joy, int port, int slot)
{
    SetJoyJoys(joy, (JOYS)1, (JOYK)0);
    joy->port = port;
    joy->slot = slot;
    scePadPortOpen(port, slot, joy);
    joy->isRumbleEnabled = 1;
    joy->uDpad = 1.0f;
}

// Per-frame controller update: tracks the pad library state (disconnect after 2 s without progress), then steps
// the setup state machine (detect mode, switch to analog and lock it, query and enable the actuators, enable
// pressure sensitivity) and, once ready, reads the pad and updates its rumble.
void UpdateJoy(JOY *joy)
{

    int isReady;
    int x;
    int joys, joyk;
    int state;



    isReady = 0;

    if (joy->joys == 0)
        return;

    state = scePadGetState_243388(joy->port, joy->slot);
    if (state == 6 || state == 2)

        isReady = 1;


    if (joy->joys != 3 && joy->joyk != 0) {
        if (g_clock.realTime - joy->stateStartTime >= 2.0f)
            state = 0;
    }

    if (joy->joyk != 0 && state == 0)
        SetJoyJoys(joy, (JOYS)1, (JOYK)0);


    if (!isReady)
        return;

    x = 2;

    joys = joy->joys;
    joyk = joy->joyk;

    switch (joys) {
    case 1:
        switch (joyk) {
        case 0: {

            int m;
            int n = scePadInfoMode(joy->port, joy->slot, 1, 0);
            if (n == 0)
                break;
            m = scePadInfoMode(joy->port, joy->slot, 2, 0);
            if (m > 0)
                n = m;
            if (n == 4 || n == 7) {

                joyk = 1;
                joys = m ? 1 : 3;
            }
            break;
        }
        case 1:

            if (scePadSetMainMode(joy->port, joy->slot, 1, 3) == 1)
                joys = 2;

            break;
        case 2: {
            unsigned char ab[6];
            int i;

            x = scePadInfoAct(joy->port, joy->slot, -1, 0);
            if (x != 2 ||

                scePadInfoAct(joy->port, joy->slot, 0, 1) != 1 ||
                scePadInfoAct(joy->port, joy->slot, 0, 2) != 2 ||
                scePadInfoAct(joy->port, joy->slot, 0, 3) != 0 ||
                scePadInfoAct(joy->port, joy->slot, 1, 1) != 1 ||
                scePadInfoAct(joy->port, joy->slot, 1, 2) != 1 ||
                scePadInfoAct(joy->port, joy->slot, 1, 3) != 1) {
                joys = 3;
                break;
            }
            func_002333B4(ab, 0xFF, 6);
            for (i = 0; i < x; i++)
                ab[i] = i;
            if (scePadSetActAlign(joy->port, joy->slot, ab) == 1)
                joys = 2;
            break;
#undef x
        }
        case 3:
            if (scePadInfoPressMode(joy->port, joy->slot) != 1) {
                joys = 3;
                break;
            }
            if (scePadEnterPressMode(joy->port, joy->slot) == 1)
                joys = x;
            break;
        }
        break;
    case 2:
        switch (scePadGetState(joy->port, joy->slot)) {
        case 1:
            joys = 1;
            break;
        case 2:
            break;
        case 0:
            switch (joy->joyk) {
            case 1:
                joyk = 2;
                joys = 1;
                break;
            case 2:
                joyk = 3;
                joys = 1;
                break;
            case 3:
                joyk = 4;
                joys = 3;
                break;
            default:
                joyk = 0;
                break;
            }
            break;
        default:
            joyk = 0;
            break;
        }
        break;
    case 3:
        if (ReadJoy(joy)) {
            if (joy->isRumbleEnabled && joy->joyk >= 3)
                UpdateRumble(&joy->rumble);
        } else {
            joys = 1;
            joyk = 0;
        }
        break;
    }

    SetJoyJoys(joy, (JOYS)joys, (JOYK)joyk);
}

// Changes the controller state/kind: a DualShock (kind >= 3) gets its rumble initialised, kind-4 changes clear the
// analog block, entering or leaving the ready state resets the button masks; records the time of the change.
void SetJoyJoys(JOY *joy, JOYS state, JOYK kind)
{
    unsigned int i;
    if (state == joy->joys && kind == joy->joyk) return;
    if (kind == 0) {
        joy->term = 0;
        state = (JOYS)1;
    }
    if (joy->joyk < 3 && kind >= 3) InitRumble((RUMBLE *)&joy->rumble, joy->port, joy->slot);
    if (joy->joyk == 4 ? kind != 4 : kind == 4) func_002333B4(joy->mpbtnpb, 0, 0xC);
    if (joy->joys == 3 ? state != 3 : state == 3) {
        joy->grfbtnLatched = 0xFFFF;
        joy->lastReadTime = 0.0f;
        joy->hasRightStickMoved = 0;
        joy->hasLeftStickMoved = 0;
        joy->buttonsPressed = 0;
        joy->buttonsHeld = 0;
        func_002333B4(joy->mpbtnpb, 0, 0xC);
        joy->dirY = 0;
        joy->leftStickX = joy->leftStickY = joy->leftStickDeflection = 0.0f;
        joy->dxRight = joy->dyRight = joy->rightStickDeflection = 0.0f;
        joy->dxdtLeft = joy->dydtLeft = 0.0f;
        joy->dydtRight = 0.0f;
        joy->dxdtRight = 0.0f;
        joy->dirX = 0;
        for (i = 0; i < 4; i++) {
            joy->almDeflect2[i].minValue = -0.75f;
            joy->almDeflect[i].minValue = -0.75f;
            joy->almDeflect2[i].maxValue = 0.75f;
            joy->almDeflect[i].maxValue = 0.75f;
        }
    }
    joy->joys = state;
    joy->joyk = kind;
    joy->stateStartTime = g_clock.realTime;
}

// Single-precision square root (sqrt.s).
static inline float SqrtF(float g) { float r; __asm__ volatile("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }
// Converts one stick's raw bytes into x/y in -1..1 with a dead zone, its deflection, and whether it moved (compared
// with the previous bytes); the LM ranges shape the response.
void GetJoyXYDeflection(JOY *joy, uchar rawX, uchar rawY, float *outX, float *outY, float *outDeflection,
                        uchar *prevRawX, uchar *prevRawY, int *outActive, LM *ranges)
{
    float x, y, gMax, g, r;
    unsigned int i;
    if (!*outActive) {
        int fActive = 0;
        if (joy->lastReadTime != 0.0f &&
            (rawX != *prevRawX || rawY != *prevRawY || rawX < 0x60 || rawX > 0xA0 || rawY < 0x60 || rawY > 0xA0))
            fActive = 1;
        *outActive = fActive;
    }
    if (*outActive) {
        x = 2.0f * (float)rawX / 255.0f + -1.0f;
        y = 1.0f - 2.0f * (float)rawY / 255.0f;
    } else {
        x = y = 0.0f;
    }
    gMax = 0.0f;
    for (i = 0; i < 4; i++) {
        g = x * D_002BE950[i].x + y * D_002BE950[i].y;
        if (g >= 0.0f) {
            if (ranges[i].maxValue < g) ranges[i].maxValue = g;
            r = g / ranges[i].maxValue;
        } else {
            if (g < ranges[i].minValue) ranges[i].minValue = g;
            r = g / ranges[i].minValue;
        }
        if (gMax < r) gMax = r;
    }
    r = Clamp((gMax - 0.35f) * 2.0f, 0.0f, 1.0f);
    *outDeflection = r;
    g = SqrtF(x * x + y * y);
    {
        if (g > 0.0001f) {
            *outX = r * x / g;
            *outY = r * y / g;
        } else {
            *outX = *outY = 0.0f;
        }
    }
    *prevRawX = rawX;
    *prevRawY = rawY;
}

// Reads the pad (scePadRead): updates held/pressed/released masks, the stick and pressure values and their
// deflection, and broadcasts a game event on activity. Returns 0 when the pad's terminal id changed (a different controller),
// else 1.
// Also known as: FReadJoy (upstream readable / Sly 1 name; FReadJoy__FP3JOY).
extern "C" int ReadJoy(JOY *joy)
{
    unsigned char padData[0x40];
    int result = 1;
    unsigned int prevButtons;
    unsigned int buttons;
    float prevLeftX, prevLeftY, prevRightX, prevRightY;

    func_002333B4(padData, 0, 0x40);
    if (scePadRead(joy->port, joy->slot, padData) > 0) {
        buttons = ((padData[2] << 8) | padData[3]) ^ 0xFFFF;
        prevButtons = joy->buttonsHeld;
        joy->buttonsPressed = buttons & ~prevButtons & ~joy->grfbtnLatched;
        joy->grfbtnLatched = joy->grfbtnLatched & buttons;
        joy->buttonsHeld = buttons;
        if (joy->term != 0 && joy->term != padData[1])
            result = 0;
        joy->term = padData[1];
        if (joy->joyk == 4)
            CopyAb(joy->mpbtnpb, &padData[8], 0xC);
        prevLeftX = joy->leftStickX;
        prevLeftY = joy->leftStickY;
        prevRightX = joy->dxRight;
        prevRightY = joy->dyRight;
        if (joy->joyk >= 2) {
            GetJoyXYDeflection(joy, padData[6], padData[7], &joy->leftStickX, &joy->leftStickY, &joy->leftStickDeflection, &joy->bXLeft, &joy->bYLeft, &joy->hasLeftStickMoved, joy->almDeflect);
            GetJoyXYDeflection(joy, padData[4], padData[5], &joy->dxRight, &joy->dyRight, &joy->rightStickDeflection, &joy->bXRight, &joy->bYRight, &joy->hasRightStickMoved, joy->almDeflect2);
        } else {


            joy->leftStickX = joy->leftStickY = joy->leftStickDeflection = 0.0f;
            joy->dyRight = joy->rightStickDeflection = 0.0f;
            joy->dxRight = 0.0f;


        }
        if (g_cm.numPolicies == 0 || g_cm.policyStack[0].policy != &g_cm.part3a0) {   /* the d-pad steers like the left stick except under the default follow camera (inferred) */
            if (joy->buttonsHeld & 0xF000) {

                joy->leftStickX = joy->leftStickY = joy->leftStickDeflection = 0.0f;

                if (joy->buttonsHeld & 0x8000)
                    joy->leftStickX = -joy->uDpad;
                else if (joy->buttonsHeld & 0x2000)
                    joy->leftStickX = joy->uDpad;
                if (joy->buttonsHeld & 0x1000)
                    joy->leftStickY = joy->uDpad;
                else if (joy->buttonsHeld & 0x4000)
                    joy->leftStickY = -joy->uDpad;
                if (joy->leftStickX * joy->leftStickY != 0.0f) {

                    joy->leftStickX = joy->leftStickX * SqrtFloat(0.5f);
                    joy->leftStickY = joy->leftStickY * SqrtFloat(0.5f);

                }
                joy->leftStickDeflection = 1.0f;
            }
        }
        joy->dxdtLeft = (joy->leftStickX - prevLeftX) / g_clock.realDeltaTime;
        joy->dydtLeft = (joy->leftStickY - prevLeftY) / g_clock.realDeltaTime;
        joy->dxdtRight = (joy->dxRight - prevRightX) / g_clock.realDeltaTime;
        joy->dydtRight = (joy->dyRight - prevRightY) / g_clock.realDeltaTime;
        MatchCheatCodePress(D_002DDEC0, joy, prevButtons & 0xFFFF);
        joy->lastReadTime = g_clock.realTime;
        if (joy->buttonsHeld != 0 || joy->leftStickDeflection != 0.0f || joy->rightStickDeflection != 0.0f) {

            joy->tActive = g_clock.realTime;
            joy->dtIdle = 0;

            if (g_clock.realTime - g_game.tIdleEvent > 15.0f) {
                BroadcastGameEvent(&g_game, 0xE, GetHeroPo(&g_sw));
                g_game.tIdleEvent = g_clock.realTime;
            }
        } else {
            joy->dtIdle = (int)(g_clock.realTime - joy->tActive);
        }
    } else {
        if (g_clock.realTime - joy->lastReadTime >= 2.0f)
            result = 0;
    }
    return result;
}

// Returns the buttons in `mask` pressed this frame and marks them handled.
extern "C" int HandleJoyButtons(JOY *joy, int mask)
{
    int grfPressed = GetJoyButtonsPressed(joy, mask);
    SetJoyBtnHandled(joy, mask);
    return grfPressed;
}

// Returns the buttons in `mask` pressed this frame.
extern "C" int GetJoyButtonsPressed(JOY *joy, int mask)
{
    return joy->buttonsPressed & mask;
}

// Marks the buttons in `mask` handled: clears them from the pressed mask and remembers them as held-and-handled.
// Also known as: SetJoyPs2BtnHandled (upstream readable / Sly 1 name; SetJoyPs2BtnHandled__FP6JOYPS2Ui).
extern "C" void SetJoyBtnHandled(JOY *joy, int mask)
{
    joy->buttonsPressed &= ~mask;
    joy->grfbtnLatched |= joy->buttonsHeld & mask;
}

// Forgets the buttons in `mask`: clears them from the held and handled masks.
// Also known as: ClearJoyBtns (upstream readable / Sly 1 name; ClearJoyBtns__FP6JOYPS2Ui).
extern "C" void ClearJoyButtons(JOY *joy, int mask)
{
    int grfKeep = ~mask;
    joy->buttonsHeld &= grfKeep;
    joy->grfbtnLatched &= grfKeep;
}

// Plays a predefined rumble pattern on the controller if it is a ready DualShock and vibration is enabled.
void TriggerJoyRumbleRumk(JOY *joy, RUMK kind, float duration)
{
    if (joy->isRumbleEnabled != 0 && joy->joyk >= 3 && joy->joys == 3) {
        TriggerRumbleRumk((RUMBLE *)&joy->rumble, kind, duration);
    }
}

// 0x187E70: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// Resets the menu selection auto-repeat state.
void StartJoySelection(JOY *joy)
{
    joy->dirX = 0;
    joy->dirY = 0;
}

// Menu left/right step with auto-repeat: returns -1, 0 or +1 from the d-pad left/right or the left stick (magnitude
// >= 0.8), repeating while held.
extern "C" int GetJoySelectionStepX(JOY *joy)
{
    float g;
    if (!joy->dirX) {
        int grf = HandleJoyButtons(joy, 0xA000);
        if (grf & 0x8000) joy->dirX = -1;
        else if (grf & 0x2000) joy->dirX = 1;
        else if (joy->leftStickDeflection >= 0.8f && __builtin_fabsf(joy->leftStickX) > __builtin_fabsf(joy->leftStickY)) {
            if (joy->leftStickX < 0.0f) joy->dirX = -1;
            else joy->dirX = 1;
        }
        if (joy->dirX) {
            g = 0.3333f;
            joy->repeatIntervalX = g;
            joy->nextRepeatTimeX = g_clock.realTime + g;
        }
    } else {
        int fHeld = 0;
        if (joy->dirX < 0) {
            if (joy->buttonsHeld & 0x8000) fHeld = 1;
            else if (joy->leftStickDeflection >= 0.25f && __builtin_fabsf(joy->leftStickX) > __builtin_fabsf(joy->leftStickY) && joy->leftStickX < 0.0f)
                fHeld = 1;
        } else {
            if (joy->buttonsHeld & 0x2000) fHeld = 1;
            else if (joy->leftStickDeflection >= 0.25f && __builtin_fabsf(joy->leftStickX) > __builtin_fabsf(joy->leftStickY) && 0.0f < joy->leftStickX)
                fHeld = 1;
        }
        if (!fHeld) {
            joy->dirX = 0;
        } else if (joy->nextRepeatTimeX <= g_clock.realTime) {
            g = Clamp(2.0f * joy->repeatIntervalX / 3.0f, 0.1f, 0.3333f);
            joy->repeatIntervalX = g;
            joy->nextRepeatTimeX = g_clock.realTime + g;
        } else {
            return 0;
        }
    }
    return joy->dirX;
}

// Menu up/down step with auto-repeat: as GetJoySelectionStepX for the d-pad up/down or the stick.
extern "C" int GetJoySelectionStepY(JOY *joy)
{
    float g;
    if (!joy->dirY) {
        int grf = HandleJoyButtons(joy, 0x5000);
        if (grf & 0x1000) joy->dirY = -1;
        else if (grf & 0x4000) joy->dirY = 1;
        else if (joy->leftStickDeflection >= 0.8f && __builtin_fabsf(joy->leftStickY) > __builtin_fabsf(joy->leftStickX)) {
            if (0.0f < joy->leftStickY) joy->dirY = -1;
            else joy->dirY = 1;
        }
        if (joy->dirY) {
            g = 0.3333f;
            joy->repeatIntervalY = g;
            joy->nextRepeatTimeY = g_clock.realTime + g;
        }
    } else {
        int fHeld = 0;
        if (joy->dirY < 0) {
            if (joy->buttonsHeld & 0x1000) fHeld = 1;
            else if (joy->leftStickDeflection >= 0.25f && __builtin_fabsf(joy->leftStickY) > __builtin_fabsf(joy->leftStickX) && 0.0f < joy->leftStickY)
                fHeld = 1;
        } else {
            if (joy->buttonsHeld & 0x4000) fHeld = 1;
            else if (joy->leftStickDeflection >= 0.25f && __builtin_fabsf(joy->leftStickY) > __builtin_fabsf(joy->leftStickX) && joy->leftStickY < 0.0f)
                fHeld = 1;
        }
        if (!fHeld) {
            joy->dirY = 0;
        } else if (joy->nextRepeatTimeY <= g_clock.realTime) {
            g = Clamp(2.0f * joy->repeatIntervalY / 3.0f, 0.1f, 0.3333f);
            joy->repeatIntervalY = g;
            joy->nextRepeatTimeY = g_clock.realTime + g;
        } else {
            return 0;
        }
    }
    return joy->dirY;
}

// 0x188328: dead-code scrap, a nop unit (sly2/scrap.h)
SLY2_SCRAP("nop");

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
int g_grfjoyt;   /* g_grfjoyt */
int g_grfusr;   /* the users' grf */
JOY g_joy __attribute__((aligned(64)));   /* g_joy (64-aligned by layout: 0x2DFC00 after the words at 0x2DFBC0) */
JOY D_002E0D00;   /* the second JOY (joy.h), right after g_joy: read when g_grfjoyt bit 1 is clear */
