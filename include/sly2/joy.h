/* include/sly2/joy.h -- JOY (controller state). g_joy = D_002DFC00 (pad 0); a second JOY at D_002E0D00 (= g_joy + 0x1100)
 * is used when g_grfjoyt bit 1 is clear (pine_input README). Layout from src/game/gameplay/cutscene/relay/func_001E3890.cpp (Lilly port), with the
 * menu-repeat fields at +0x1024 named from src/game/platform/input/joy/DxSelectionJoy.c (matched; reads dirX, dirY, tNextX/Y, tRepX/Y there and
 * gX/gY/gMag = dxLeft/dyLeft/uDeflectLeft at +0x103C..). Tags: [R] runtime / [M] offset used by matched code / [S] static. */
#ifndef SLY2_JOY_H
#define SLY2_JOY_H
#include "sly2/basic.h"

struct JOYRUMBLE    /* = RUMBLE in func_001E3890.cpp, 0x1C bytes */
{
      int nPort;
      int nSlot;
      RUMS rums;
      RUMPAT *prumpat;
      int irumins;
      float dtRumble;
      float dtRumins;
};

struct JOY
{
      byte abPadDma[0x1000];   /* +0x0000 [R] libpad DMA buffer (scePadPortOpen); IOP padman writes it every vblank */
      int nPort;            /* +0x1000 [S] */
      int nSlot;            /* +0x1004 [S] */
      int term;             /* +0x1008 [S] */
      JOYS joys;            /* +0x100C [R] pad state (3 in gameplay; second JOY: 0); 0 => nothing writes the JOY (FReadJoy only runs when non-zero; pine_input) */
      JOYK joyk;            /* +0x1010 [S] controller kind */
      float tJoys;          /* +0x1014 [S] */
      float tRead;          /* +0x1018 [R] clock.tReal of the last successful read: one frame (0.017 s) behind tReal in 72/73 states */
      float tActive;        /* +0x101C [S] */
      int dtIdle;           /* +0x1020 [S] */
      int dirX;             /* +0x1024 [M] menu repeat direction -1/0/1 (DxSelectionJoy) */
      int dirY;             /* +0x1028 [M] */
      float tNextX;         /* +0x102C [M] next repeat time (clock.tReal based) */
      float tRepX;          /* +0x1030 [M] repeat interval (0.3333 first) */
      float tNextY;         /* +0x1034 [M] */
      float tRepY;          /* +0x1038 [M] */
      float dxLeft;         /* +0x103C [R][M] left stick x (-1..1) */
      float dyLeft;         /* +0x1040 [R][M] left stick y */
      float uDeflectLeft;   /* +0x1044 [M] left stick deflection 0..1 (>= 0.8 counts as a menu move) */
      byte bXLeft;          /* +0x1048 [S] raw byte */
      byte bYLeft;
      ushort unk_0x104a;
      int fStickMovedLeft;  /* +0x104C [S] */
      LM almDeflect[4];     /* +0x1050 [S] */
      float dxRight;        /* +0x1070 [R] right stick x */
      float dyRight;        /* +0x1074 [R] right stick y */
      float sRight;         /* +0x1078 [S] */
      byte bXRight;
      byte bYRight;
      ushort unk_0x107e;
      int fStickMovedRight; /* +0x1080 [S] */
      LM almDeflect2[4];    /* +0x1084 [S] */
      float dxdtLeft;       /* +0x10A4 [S] */
      float dydtLeft;
      float dxdtRight;
      float dydtRight;
      uint grfbtn;          /* +0x10B4 [R][M] buttons held (copied to POCTL+0x30 with the mask at +0x10BC; DxSelectionJoy grfHeld) */
      uint grfbtnPressed;   /* +0x10B8 [R] new presses this frame (cheat-code reader uses it) */
      uint grfbtnLatched;   /* +0x10BC [R] handled / masked buttons */
      byte mpbtnpb[12];     /* +0x10C0 [S] pressure bytes */
      JOYRUMBLE rumble;     /* +0x10CC [S] */
      int fRumbleEnabled;   /* +0x10E8 [S] */
      float uDpad;          /* +0x10EC [S] */
      undefined4 unk_0x10f0[4];   /* +0x10F0 [?] up to the next JOY at +0x1100 */
};
typedef char assert_JOY_size[sizeof(JOY) == 0x1100 ? 1 : -1];   /* second JOY at g_joy+0x1100 (R: two instances) */
typedef char assert_JOY_joys[SLY2_OFFSETOF(JOY, joys) == 0x100C ? 1 : -1];   /* CONFIRMED (pine_input: 0x2E0C0C) */
typedef char assert_JOY_tRead[SLY2_OFFSETOF(JOY, tRead) == 0x1018 ? 1 : -1];   /* CONFIRMED (pine_input) */
typedef char assert_JOY_dirX[SLY2_OFFSETOF(JOY, dirX) == 0x1024 ? 1 : -1];   /* CONFIRMED match DxSelectionJoy */
typedef char assert_JOY_dxLeft[SLY2_OFFSETOF(JOY, dxLeft) == 0x103C ? 1 : -1];   /* CONFIRMED (pine_input + match) */
typedef char assert_JOY_dxRight[SLY2_OFFSETOF(JOY, dxRight) == 0x1070 ? 1 : -1];   /* CONFIRMED (pine_input) */
typedef char assert_JOY_grfbtn[SLY2_OFFSETOF(JOY, grfbtn) == 0x10B4 ? 1 : -1];   /* CONFIRMED (pine_input + match) */
typedef char assert_JOY_grfbtnLatched[SLY2_OFFSETOF(JOY, grfbtnLatched) == 0x10BC ? 1 : -1];   /* CONFIRMED (pine_input) */

#endif /* SLY2_JOY_H */
