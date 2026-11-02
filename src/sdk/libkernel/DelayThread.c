// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct { int currentCount; int maxCount; int initCount; int numWaitThreads; unsigned int attr; unsigned int option; } SemaParam;
extern char D_002AA7F0[];
int func_00234A20(SemaParam *);
int func_00234A30(int);
int func_00234A60(int);
long func_0023A598(int, int);
int func_0023A6C8(long, void *, int);
int _DelayThreadHandler(int, unsigned short, void *);
int DelayThread(int usec)
{
    SemaParam sp;
    int sema;
    int al;
    int st;
    __asm__ volatile("mfc0 %0, $12" : "=r"(st));
    if ((st & 0x10000) == 0)
        return 0x80008008;
    sp.maxCount = 1;
    sp.option = (unsigned int)D_002AA7F0;
    sp.initCount = 0;
    sema = func_00234A20(&sp);
    if (sema < 0)
        return 0x80008003;
    al = func_0023A6C8(func_0023A598(0, usec), _DelayThreadHandler, sema);
    if (al >= 0) {
        func_00234A60(sema);
        func_00234A30(sema);
        return 0;
    }
    func_00234A30(sema);
    return al;
}
