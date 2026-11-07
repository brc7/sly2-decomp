/* include/sly2/game.h -- GAME, the game-progress record (g_game = D_002DEB30, 0x1C bytes): the idle / camera-turn
 * event timers, the last refused task state and gadget, and the oids of the current job, current task and pending
 * task (game.cpp keeps them in step with SW::currentJob / currentTask / pendingTask).
 *
 * Layout: the Lilly-port copies in tn.cpp and relay.cpp (identical), game.cpp's InitGame view (memset of 0x1C bytes,
 * then the three oids set to -1). game.cpp itself still reaches the oid words through the address names
 * D_002DEB40/44/48 (tu-merging.md "Data", phase 3) and passes g_game around as `char *`. The JOB and TASK objects are
 * sly2/task.h's; the save blob (GS) and the save manager are sly2/save.h's. */
#ifndef SLY2_GAME_H
#define SLY2_GAME_H
#include "sly2/basic.h"

struct GAME
{
      float tIdleEvent;     /* +0x00 when the last idle game event was sent */
      float tCmTurned;      /* +0x04 when the player last turned the camera (tn.cpp: game event 6 at most every 5 s) */
      int nTaskStateDenied; /* +0x08 the task state last refused (game.cpp InitGame's rejectedState) */
      int idGadgetLast;     /* +0x0C the gadget last used */
      int oidJobCur;        /* +0x10 oid of the current JOB (-1: none) */
      int oidTaskCur;       /* +0x14 oid of the current TASK (-1: none) */
      int oidTaskPending;   /* +0x18 oid of the pending TASK (-1: none) */
};
typedef char assert_GAME_size[sizeof(GAME) == 0x1C ? 1 : -1];   /* InitGame clears sizeof(GAME) bytes */
typedef char assert_GAME_oidJobCur[SLY2_OFFSETOF(GAME, oidJobCur) == 0x10 ? 1 : -1];   /* address name D_002DEB40 */

/* g_game: defined by game.cpp's .bss under its address name. */
extern GAME g_game __asm__("D_002DEB30");

#endif /* SLY2_GAME_H */
