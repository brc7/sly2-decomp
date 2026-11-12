/* include/sly2/blip.h -- the blip particle batch (engine/fx/blip.cpp): the record SW::slotheapBlip hands out, kept
 * on its group's list and, once removed, on frm.cpp's pending list until the frame that drew it is sent. Here (not in
 * blip.cpp) so frm.cpp can take the list link's offset from it. The group is classes.h BLIPG (its age table's
 * element constructor reproduces the class constructor's loop). */
#ifndef SLY2_BLIP_H
#define SLY2_BLIP_H
#include "sly2/basic.h"

/* One particle entry of a BLIPBUF: position (a), velocity (b), extra (c: c.z angular rate, c.w angle); b.w is the
   death time. */
union BLIPVEC { qword q; struct { float x, y, z, w; } v; };
/* A particle buffer: three parallel arrays of 0x2B entries. */
struct BLIPBUF { BLIPVEC a[0x2B]; BLIPVEC b[0x2B]; BLIPVEC c[0x2B]; char pad[0x840 - 0x810]; };
typedef char assert_BLIPBUF_size[sizeof(BLIPBUF) == 0x840 ? 1 : -1];
/* Bezier tangents of a particle on a path (BLIPG motion kind 2), from SW::slotheapBlipSpline. */
struct BLIPBEZ { qword v0; qword v10; };

/* BLIPBATCH: a particle batch: two particle buffers (abuf[i] current), the live count, the spline block, its group
   and its link on the group's list (then on frm.cpp's pending list). */
struct BLIPBATCH
{
    BLIPBUF abuf[2];
    int i;                  /* +0x1080 current buffer */
    int c;                  /* +0x1084 live particles */
    BLIPBEZ *ae;            /* +0x1088 one per particle (motion kind 2), else 0 */
    struct BLIPG *pblipg;   /* +0x108C its group */
    BLIPBATCH *pblipNext;   /* +0x1090 the list link (the group's dl, frm.cpp's D_002DEB00) */
    BLIPBATCH *pblipPrev;
};
typedef char assert_BLIPBATCH_pblipNext[SLY2_OFFSETOF(BLIPBATCH, pblipNext) == 0x1090 ? 1 : -1];

#endif
