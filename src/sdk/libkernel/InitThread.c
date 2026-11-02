// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int currentCount; int maxCount; int initCount; int numWaitThreads; unsigned int attr; unsigned int option; } SemaParam;
typedef struct { int status; void *entry; void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr; unsigned int option; int waitType; int waitId; int wakeupCount; } ThreadParam;
typedef struct { int x0; int idx; } WQ;
extern char D_002AA7D8[];
extern char D_00448540[];
extern char D_002E5270[];
extern int D_00448940;
extern WQ D_00448948;
extern int D_002C49E8;
int func_00234A20(SemaParam *);
int func_00234A30(int);
int func_00234820(ThreadParam *);
int func_002355E0(int, void *);
int func_00234910(void);
int func_002348B0(int, int);
void topThread(void *);
int InitThread(void)
{
    ThreadParam tp;
    SemaParam sp;
    char *name;
    if (D_002C49E8 > 0)
        goto fail;
    {
        name = D_002AA7D8;
        sp.maxCount = 0xFF;
        sp.initCount = 0;
        sp.option = (unsigned int)name;
        D_00448940 = func_00234A20(&sp);
        if (D_00448940 < 0)
            goto fail;
        tp.entry = topThread;
        tp.stack = D_00448540;
        tp.stackSize = 0x400;
        tp.gpReg = D_002E5270;
        tp.option = (unsigned int)name;
        tp.initPriority = 0;
        D_002C49E8 = func_00234820(&tp);
        if (D_002C49E8 < 0) {
            func_00234A30(D_00448940);
        fail:
            return -1;
        }
        D_00448948.x0 = 0;
        D_00448948.idx = 0;
        func_002355E0(D_002C49E8, &D_00448948);
        func_002348B0(func_00234910(), 1);
    }
    return D_002C49E8;
}
