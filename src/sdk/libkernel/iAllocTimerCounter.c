// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder
typedef struct TC {
    struct TC *next; int pad4; volatile int id; int xC; int x10, x14; long x18; int x20, x24; void *x28;
} TC;
typedef struct { int x0, x4, x8; volatile int seq; int count; TC *free; } TS;
extern TS D_002C5A30;
int iAllocTimerCounter(void)
{
    TS *ts = &D_002C5A30;
    TC *p = ts->free;
    if (p != 0) {
    ts->count++;
 p->x28 = 0;
 p->xC = 0;
 ts->free = p->next;
    p->x18 = 0;
    ts->seq++;
    p->id = ((ts->seq << 1) & 0x3FE) | 1;
    return ((int)p << 4) | p->id;
    }
    return 0x80008005;
}
