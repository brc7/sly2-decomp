/* include/sly2/joy.h -- JOY (controller state). g_joy = g_joy (pad 0); a second JOY at D_002E0D00 (= g_joy + 0x1100)
 * is used when g_grfjoyt bit 1 is clear (pine_input README). Layout from src/game/gameplay/cutscene/relay/IsJsgCommandComplete__FP3JSGP8RELAYCMD.cpp (Lilly port), with the
 * menu-repeat fields at +0x1024 named from src/game/platform/input/joy/GetJoySelectionStepX.c (matched; reads dirX, dirY, nextRepeatTimeX/Y, repeatIntervalX/Y there and
 * gX/gY/gMag = leftStickX/leftStickY/leftStickDeflection at +0x103C..). Tags: [R] runtime / [M] offset used by matched code / [S] static. */
#ifndef SLY2_JOY_H
#define SLY2_JOY_H
#include "sly2/basic.h"

/* RUMINS: one instruction of a rumble pattern: small motor on/off, large motor power, and how long it lasts. */
struct RUMINS
{
      byte highSpeedMotorOn;    /* small (high speed) motor on/off */
      byte lowSpeedMotorPower;  /* large (low speed) motor power, 0..255 */
      float duration;           /* +0x4 seconds */
};

/* RUMPAT: a rumble pattern: a looped list of instructions (rumble.cpp's s_mprumkrumpat holds the predefined ones, by RUMK). */
struct RUMPAT
{
      int instructionCount;
      RUMINS instructions[32];
};
typedef char assert_RUMPAT_size[sizeof(RUMPAT) == 0x104 ? 1 : -1];

/* RUMBLE: the rumble (vibration) state of a controller (JOY::rumble): the pattern playing and its timers; rumble.cpp
   steps it with real (unscaled) time. Moved from rumble.cpp. */
struct RUMBLE
{
      int port;                   /* +0x0 controller port */
      int slot;                   /* +0x4 multitap slot */
      RUMS state;                 /* +0x8 */
      RUMPAT *pattern;            /* +0xC the pattern playing */
      int instructionIndex;       /* +0x10 current instruction of the pattern */
      float timeLeft;             /* +0x14 seconds of the pattern left to play */
      float instructionTimeLeft;  /* +0x18 seconds left on the current instruction */
};
typedef char assert_RUMBLE_size[sizeof(RUMBLE) == 0x1C ? 1 : -1];

/* JOY: one controller (DualShock 2): the libpad DMA buffer, decoded sticks and buttons, menu auto-repeat and rumble.
   */
struct JOY
{
      byte padDmaBuffer[0x1000]; /* +0x0000 [R] libpad DMA buffer (scePadPortOpen); IOP padman writes it every vblank */
      int port;             /* +0x1000 [S] controller port */
      int slot;             /* +0x1004 [S] multitap slot */
      int term;             /* +0x1008 [S] */
      JOYS joys;            /* +0x100C [R] pad state (3 in gameplay; second JOY: 0); 0 => nothing writes the JOY (FReadJoy only runs when non-zero; pine_input) */
      JOYK joyk;            /* +0x1010 [S] controller kind */
      float stateStartTime; /* +0x1014 [S] time joys was entered */
      float lastReadTime;   /* +0x1018 [R] clock.tReal of the last successful read: one frame (0.017 s) behind tReal in 72/73 states */
      float tActive;        /* +0x101C [S] time of the last input (inferred) */
      int dtIdle;           /* +0x1020 [S] idle time (inferred) */
      int dirX;             /* +0x1024 [M] menu repeat direction -1/0/1 (GetJoySelectionStepX) */
      int dirY;             /* +0x1028 [M] menu repeat direction, y */
      float nextRepeatTimeX; /* +0x102C [M] next repeat time (clock.tReal based) */
      float repeatIntervalX; /* +0x1030 [M] repeat interval (0.3333 first) */
      float nextRepeatTimeY; /* +0x1034 [M] next repeat time, y */
      float repeatIntervalY; /* +0x1038 [M] repeat interval, y */
      float leftStickX;     /* +0x103C [R][M] left stick x (-1..1) */
      float leftStickY;     /* +0x1040 [R][M] left stick y */
      float leftStickDeflection; /* +0x1044 [M] left stick deflection 0..1 (>= 0.8 counts as a menu move) */
      byte bXLeft;          /* +0x1048 [S] raw byte */
      byte bYLeft;   /* raw left stick y byte */
      ushort unknown104A;
      int hasLeftStickMoved; /* +0x104C [S] the left stick left its dead zone */
      LM almDeflect[4];     /* +0x1050 [S] left stick calibration ranges (inferred) */
      float dxRight;        /* +0x1070 [R] right stick x */
      float dyRight;        /* +0x1074 [R] right stick y */
      float rightStickDeflection; /* +0x1078 [S] right stick deflection, 0..1 */
      byte bXRight;   /* raw right stick x byte */
      byte bYRight;   /* raw right stick y byte */
      ushort unknown107E;
      int hasRightStickMoved; /* +0x1080 [S] the right stick left its dead zone */
      LM almDeflect2[4];    /* +0x1084 [S] right stick calibration ranges (inferred) */
      float dxdtLeft;       /* +0x10A4 [S] left stick x speed */
      float dydtLeft;   /* left stick y speed */
      float dxdtRight;   /* right stick x speed */
      float dydtRight;   /* right stick y speed */
      uint buttonsHeld;     /* +0x10B4 [R][M] buttons held (copied to POCTL+0x30 with the mask at +0x10BC; GetJoySelectionStepX grfHeld) */
      uint buttonsPressed;  /* +0x10B8 [R] new presses this frame (cheat-code reader uses it) */
      uint grfbtnLatched;   /* +0x10BC [R] handled / masked buttons */
      byte mpbtnpb[12];     /* +0x10C0 [S] pressure bytes */
      RUMBLE rumble;         /* +0x10CC [S] rumble state */
      int isRumbleEnabled;  /* +0x10E8 [S] vibration is enabled */
      float uDpad;          /* +0x10EC [S] d-pad as an analog value (inferred) */
      undefined4 unknown10F0[4];  /* +0x10F0 [?] up to the next JOY at +0x1100 */
};
typedef char assert_JOY_size[sizeof(JOY) == 0x1100 ? 1 : -1];   /* second JOY at g_joy+0x1100 (R: two instances) */
typedef char assert_JOY_joys[SLY2_OFFSETOF(JOY, joys) == 0x100C ? 1 : -1];   /* CONFIRMED (pine_input: 0x2E0C0C) */
typedef char assert_JOY_lastReadTime[SLY2_OFFSETOF(JOY, lastReadTime) == 0x1018 ? 1 : -1]; /* CONFIRMED (pine_input) */
typedef char assert_JOY_dirX[SLY2_OFFSETOF(JOY, dirX) == 0x1024 ? 1 : -1];   /* CONFIRMED match GetJoySelectionStepX */
typedef char assert_JOY_leftStickX[SLY2_OFFSETOF(JOY, leftStickX) == 0x103C ? 1 : -1]; /* CONFIRMED (pine_input + match) */
typedef char assert_JOY_dxRight[SLY2_OFFSETOF(JOY, dxRight) == 0x1070 ? 1 : -1];   /* CONFIRMED (pine_input) */
typedef char assert_JOY_buttonsHeld[SLY2_OFFSETOF(JOY, buttonsHeld) == 0x10B4 ? 1 : -1]; /* CONFIRMED (pine_input + match) */
typedef char assert_JOY_grfbtnLatched[SLY2_OFFSETOF(JOY, grfbtnLatched) == 0x10BC ? 1 : -1];   /* CONFIRMED (pine_input) */
typedef char assert_JOY_dtIdle[SLY2_OFFSETOF(JOY, dtIdle) == 0x1020 ? 1 : -1];   /* match (game.cpp reads +0x1020) */
typedef char assert_JOY_rumble[SLY2_OFFSETOF(JOY, rumble) == 0x10CC ? 1 : -1];   /* match (sw.cpp passes +0x10CC) */

/* g_joy: the first of the two JOYs (joy.cpp defines it under its address name). */
extern JOY g_joy __asm__("g_joy");

#endif /* SLY2_JOY_H */
