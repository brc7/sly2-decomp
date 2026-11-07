/* include/sly2/gs.h -- the GS memory allocator (engine/graphics/backend/gs.cpp): a range of GS blocks handed out
 * first-fit from size-bucketed free runs, else from the top (IgsAllocGsb). Self-contained (no sly2/basic.h). */
#ifndef SLY2_GS_H
#define SLY2_GS_H

/* GSBB: a free run of GS blocks [start, end) */
struct GSBB { int start; int end; GSBB *next; };

/* GSB: the allocator (0x630; font.cpp keeps one on its stack) */
struct GSB
{
    int first;              /* +0x00 first GS block of the range */
    int top;                /* +0x04 next free block */
    int limit;              /* +0x08 end of the range */
    GSBB *freeRuns[8];      /* +0x0C free runs by size bucket */
    int numRuns;            /* +0x2C runs used */
    GSBB runs[128];         /* +0x30 */
};
typedef char assert_GSB_size[sizeof(GSB) == 0x630 ? 1 : -1];

#endif
