// thread.cpp -- EE thread and semaphore wrappers: critical sections and the render thread startup.
// The game runs two EE threads: the main (game logic) thread and a render thread (FrameRenderLoop) started at boot;
// CRITSECT is the recursive lock they share data under. Entry points: StartupThread (boot), SemaCreate,
// InitCritSect / EnterCritSect / LeaveCritSect. TU 0x21EFD0..0x21F2E0 (7 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_thread
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_thread
#include "sly2/slotheap.h"
#include "sly2/scrap.h"

// 0x21F188 StaticInit_thread and GlobalConstructors_thread: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Sony kernel parameter blocks (libkernel's SemaParam / ThreadParam). */
struct SemaParam { int count; int maxCount; int initCount; int numWaitThreads; unsigned int attr; unsigned int option; };
struct ThreadParam
{
    int status; void *entry; void *stack; int stackSize; void *gpReg;
    int initPriority; int currentPriority; unsigned attr; unsigned option; int waitType; int waitId; int wakeupCount;
};

/* A recursive critical section: entry count, owning thread id (-1 = none), semaphore id. */
struct CRITSECT
{
    int enterCount;     /* +0x0 */
    int ownerThread;     /* +0x4 */
    int semaphore;       /* +0x8 */
};

extern int D_004425D0[2];
extern char D_004425F0[];
extern char D_002E5270[] __asm__("_gp");   /* _gp (linker-defined), not an object */

/* libkernel names in the comments are HYPOTHESIS (from the arguments and the neighbouring SignalSema). */
extern "C" {
void *MemsetNoBuiltin(void *p, int c, int n) __asm__("memset");     /* memset */
int CreateSema(SemaParam *semaParam) __asm__("CreateSema");               /* CreateSema */
int GetThreadId(void);                        /* GetThreadId */
int WaitSema(int semaphore) __asm__("WaitSema");                    /* WaitSema */
int SignalSema(int sema);
void ChangeThreadPriority(int tid, int prio);          /* ChangeThreadPriority */
int CreateThread(ThreadParam *threadParam);             /* CreateThread */
}
void FrameRenderLoop(void *);

// Creates a kernel semaphore with the given initial and maximum counts; returns its id.
int SemaCreate(int initCount, int maxCount)
{
    SemaParam semaParam;
    MemsetNoBuiltin(&semaParam, 0, sizeof(semaParam));
    semaParam.initCount = initCount;
    semaParam.maxCount = maxCount;
    return CreateSema(&semaParam);
}

// 0x21F020: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x50");

// Initialises a recursive critical section: no owner, backed by a binary semaphore.
void InitCritSect(CRITSECT *critSect)
{
    critSect->ownerThread = -1;
    critSect->semaphore = SemaCreate(1, 1);
}

// Enters a recursive critical section: the owning thread just bumps the count, any other thread waits on the
// semaphore and then takes ownership.
void EnterCritSect(CRITSECT *critSect)
{
    int threadId = GetThreadId();
    if (threadId != critSect->ownerThread)
    {
        WaitSema(critSect->semaphore);
        critSect->ownerThread = threadId;
        critSect->enterCount = 1;
    }
    else
    {
        critSect->enterCount++;
    }
}

// Leaves a recursive critical section; the last leave clears the owner and signals the semaphore.
void LeaveCritSect(CRITSECT *critSect)
{
    if (--critSect->enterCount == 0)
    {
        critSect->ownerThread = -1;
        SignalSema(critSect->semaphore);
    }
}

// 0x21F0F8: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x30");

// Boot step: records the main thread id (D_004425D0), raises the main thread to priority 5 and creates the render
// thread (FrameRenderLoop, 0x1000-byte stack D_004425F0, priority 2: a lower number is a higher EE priority, so
// it preempts the main thread); its id goes to D_004425D4. The thread is only created here, not started.
void StartupThread()
{
    ThreadParam threadParam;
    int *threadIds = D_004425D0;
    *threadIds = GetThreadId();
    ChangeThreadPriority(*threadIds, 5);
    MemsetNoBuiltin(&threadParam, 0, sizeof(threadParam));
    threadParam.stackSize = 0x1000;
    threadParam.stack = D_004425F0;
    threadParam.initPriority = 2;
    threadParam.gpReg = D_002E5270;
    threadParam.entry = (void *)FrameRenderLoop;
    threadIds[1] = CreateThread(&threadParam);
}

// ---- thread's .bss (tu-merging.md "Data": .bss gaps): 0x4425D0..0x4435F0, after hud's .bss in TU order. The
// thread-id table (8 words up to the stack: main (prog's D_004425D0), render (frm's D_004425D4), memcard's
// D_004425D8, mpeg's D_004425DC, 4 words no code names) and the render thread's 0x1000-byte stack (StartupThread's
// stackSize). The table's words that other TUs name are spelled as separate words (an int is 4-aligned; a later
// views pass can make them interiors of one array), the rest as one array (8-aligned at 0x4425E0). Asm-labelled so
// the declarations above, which the functions see, stay as they are; emitted in this order (first declaration).
namespace BSS_thread {
int b_004425D0 __asm__("D_004425D0");
int b_004425D4 __asm__("D_004425D4");
int b_004425D8 __asm__("D_004425D8");
int b_004425DC __asm__("D_004425DC");
int b_004425E0[4] __asm__("D_004425E0");
char b_004425F0[0x1000] __asm__("D_004425F0");
}
