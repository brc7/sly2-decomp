// memcard.cpp -- memory card saves (SAVE, the global save manager D_002E65E0, with its MEMCARD worker thread):
// startup, card directory info, the card jobs (format, check, read / write / delete...), the save state machine,
// the icon, and the memory card dialog widgets. TU 0x1AA978..0x1AE150 (81 functions with the linkonce copy at
// 0x258248, address order).
//
// Types: TU-private views for the m2c bodies (SAVE with its three 0x5464-byte slots and its MEMCARD at +0xFD5C,
// SAVEINFO (a slot's 0x5C-byte card directory record), the dialog widgets). CompareSaves keeps its literal Sly 1
// symbol (CompareSaves__FP4SAVET0) as a C-linkage definition: it compares two SAVEINFO records, not SAVEs.
// The hand-written per-function files keep their views in namespaces named after each function (as jt.cpp does).
// The two dialogs D_002E6550 / D_002E65A0 are defined here with inline constructors (CMemcardDialog_ctor,
// CMcStatus_ctor) and destructors (DestroyBasic, in-charge flag 2): g++ generates the 93-instruction
// construct-and-destruct StaticInit_memcard and the _GLOBAL_$D$ stub func_001AE130 (cm.cpp's g_cm pattern).
// Entry points: StartupMemcard (boot), UpdateSave__FP4SAVE (per frame), AutosaveGame, the dialog
// widgets' Update (slot 7) and the save-page menu actions.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_memcard
// SLY2_ALIAS: _GLOBAL_$I$*=func_001AE110
// SLY2_ALIAS: _GLOBAL_$D$*=func_001AE130

#include "sly2/basic.h"
#include "sly2/scrap.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/screen.h"
extern "C" char base_D_003D9ED0[] __asm__("D_003D9ED0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */

/* The two dialog widgets (views of their bytes; constructed and destroyed by C-named functions). */
struct MCDLGG;
struct MCDLG;
extern "C" {
MCDLG *CMemcardDialog_ctor(MCDLG *dialog);
void *CMcStatus_ctor(void *pv);        /* CMcStatus constructor */
void DestroyBasic(MCDLGG *pv, int grfDelete);
}
struct MCDLGG
{
    char ab[0x40];
    int state;              /* +0x40 the dialog state (SetMcDialogState), also the langstr id of its message (func_001AD8F8 prints it); was D_002E6590 */
    int phase;              /* +0x44 the dialog's phase (SetMcDialogPhase; 1, 3 tested); was D_002E6594 */
    char ab48[8];
    MCDLGG() { CMemcardDialog_ctor((MCDLG *)this); }
    ~MCDLGG() { DestroyBasic(this, 2); }
};
struct MCSTATUSG
{
    char ab[0x40];
    MCSTATUSG() { CMcStatus_ctor(this); }
    ~MCSTATUSG() { DestroyBasic((MCDLGG *)this, 2); }
};
/* memcard's .bss, in the ELF's order: the thread stack, the sceMcGetDir table, the directory path, the dialog (0x50),
   the status widget (0x40), the save manager. g++ 2.95 emits an object with a constructor at its definition and
   every other uninitialized object at the end of the file, so the three plain buffers get an empty inline
   constructor (no code) to keep them ahead of the dialogs. Asm labels: the view declarations need not agree. */
namespace BSS_memcard {
template <int N> struct BUF { char ab[N]; BUF() {} };
BUF<0x4000> d_002E2080 __asm__("D_002E2080") __attribute__((aligned(64)));   /* the memcard thread's stack (stackSize 0x4000; 64-aligned by layout: 0x2E2080 after main's words at 0x2E2050..0x2E205C) */
BUF<0x480> d_002E6080 __asm__("D_002E6080");    /* sceMcGetDir table (ENTs), cleared as 0x480 bytes */
BUF<0x50> d_002E6500 __asm__("D_002E6500");     /* directory path */
}
MCDLGG D_002E6550;      /* the memory card dialog (CMemcardDialog) */
MCSTATUSG D_002E65A0;   /* the card status widget (CMcStatus) */
/* The save manager (SAVE, 0xFDA0 bytes up to memory.cpp's .bss at 0x2F6380; its MEMCARD worker D_002F633C at
   +0xFD5C). */
namespace BSS_memcard { BUF<0xFDA0> d_002E65E0 __asm__("D_002E65E0"); }

// 0x1ADF98 StaticInit_memcard (the two widgets, then the six slot pools; their destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The memory card worker: semaphore, thread id (-1: none), priority. */
struct MEMCARD
{
    int sema;
    int tid;                /* +0x04 */
    int job[2];             /* +0x08 the queued job: a pointer to a SAVE member function (CreateMemcardThread) */
    int jobResult;          /* +0x10 the job's result code (0x20 idle, 0x21 while running; McSyncResult codes) */
};
/* A card directory record as the game keeps it (0x5C bytes): name, size, attribute flags and the creation /
   modification dates. The flag word at +0x48 is read and written whole; the creation day and month bytes overlap
   its upper half. */
struct SAVEINFO
{
    char achName[0x44];
    int cb;                 /* +0x44 */
    union {
        int grf;            /* +0x48 */
        struct { uchar pad48[2]; uchar bDayC, bMonthC; };
    };
    ushort wYearC;          /* +0x4C */
    uchar bHourC, bMinC, bSecC;
    uchar pad51;
    uchar bDayM, bMonthM;   /* +0x52 */
    ushort wYearM;          /* +0x54 */
    uchar bHourM, bMinM, bSecM;
    uchar pad59[3];
};
typedef char assert_SAVEINFO_size[sizeof(SAVEINFO) == 0x5C ? 1 : -1];
/* The card's own directory entry (sceMcTblGetDir). */
struct MCTBLDIR
{
    uchar resvC, secC, minC, hourC, dayC, monthC;
    ushort yearC;
    uchar resvM, secM, minM, hourM, dayM, monthM;
    ushort yearM;
    uint cb;                /* +0x10 */
    ushort attr;            /* +0x14 */
    ushort resv16;
    uint resv18[2];
    char achName[32];       /* +0x20 */
};
/* The save manager: card port/slot, state, three slots, the current slot and the worker. */
struct SAVE
{
    int port;
    int slot;               /* +0x04 */
    int nState;             /* +0x08 */
    int nStateNext;         /* +0x0C */
    float tStateChanged;    /* +0x10 */
    float tNextPoll;        /* +0x14 */
    int cardType;           /* +0x18 sceMcGetInfo type (2: PS2 card) */
    int freeClusters;       /* +0x1C */
    int isFormatted;        /* +0x20 */
    char aslot[3][0x5464];  /* +0x24 */
    int currentSlot;        /* +0xFD50 the slot in use (0: none) */
    void *newestSlot;       /* +0xFD54 the most recently modified slot (SetSaveState) */
    int selectedSlot;          /* +0xFD58 the slot picked on the save page (SelectSaveSlotToSave) */
    MEMCARD memcard;        /* +0xFD5C */
};
/* A dialog widget: vtable +0xC, id +0x20, its child +0x24, mode and when it was set, state. */
/* The dialog's child widget: its display state (2: fully shown), when that was reached, the state it is heading for. */
struct MCCHILD { char pad0[0x58]; int displayState; float tDisplayState; char pad60[4]; int targetDisplayState; char pad68[0xB0 - 0x68]; int unknownB0; };
struct MCDLG
{
    char pad0[0xC];
    void *vtable;              /* +0x0C */
    char pad10[0x20 - 0x10];
    int id;                 /* +0x20 */
    MCCHILD *pchild;        /* +0x24 */
    int nMode;              /* +0x28 */
    float tMode;            /* +0x2C */
    char pad30[0x40 - 0x30];
    int nState;             /* +0x40 */
};
/* A file key (linkonce InitFileKey). */
struct FILEKEY { int an[2]; int n8; char bC; };

enum
{
    SAVE_MEMCARD_OFFSET = 0xFD5C,   /* SAVE::memcard */
    SAVE_SLOT_SIZE = 0x5464,        /* one slot in SAVE::aslot */
    SAVE_SLOT_FILE_SIZE = 0x5408,   /* the part of a slot written to the card (its SAVEINFO follows) */
};

extern SW D_003DD490;
extern "C" {
extern char D_002DEB30[];       /* the game */
extern SAVE D_002E65E0;   /* the save manager */
extern int D_004425D8;
extern char D_003D9ED0[];       /* the pause menu */
extern MEMCARD D_002F633C __asm__("D_002E65E0+0xFD5C");      /* the save manager's worker (D_002E65E0 + 0xFD5C) */
extern CLOCK D_002DDED0;
extern void *D_002D3750[] __asm__("_vt$14CMemcardDialog");      /* CMemcardDialog vtable (emitted here) */
/* CMemcardDialog (DRAFT name, from its functions' annotations): the memory-card dialog screen, vtable 0x2D3750, emitted
   here. Defined before CMcStatus (below), whose vtable comes out before it. MCDLG is the view the bodies read. */
struct CMemcardDialog : SCREEN
{
    virtual void PostLoad() __asm__("CMemcardDialog_PostLoad");   /* slot 12; key function */
    virtual void Update() __asm__("CMemcardDialog_Update");   /* slot 7 */
    virtual void Reset() __asm__("CMemcardDialog_Slot13");   /* slot 13 */
    virtual void SetMode(int nMode) __asm__("CMemcardDialog_SetMode");   /* slot 14 */
};
extern char D_002A38E8[];   /* the icon file */
void InitGame(void *pgame);
void *memset(void *pv, int b, unsigned int cb);
char *strcpy(char *pchDst, const char *pchSrc);
void SignalSema(int sema);
void WaitSema(int sema);
void func_002348B0(int tid, int prio);
int McSyncResult(MEMCARD *worker, int nResult, int nJob, int n);
int sceMcFormat(int port, int slot);
int sceMcMkdir(int port, int slot, const char *pchz);
int sceMcOpen(int port, int slot, const char *pchz, int mode);
int sceMcClose(int fd);
int sceMcDelete(int port, int slot, const char *pchz);
void SetSaveState(SAVE *saveManager, int nState);
int ResetSaveSlot(SAVE *saveManager, void *pslot);
void SetMcDialogPhase(void *dialog, int n);
int McJobGetInfo(SAVE *saveManager);
int WriteSaveSlotFile(SAVE *saveManager, int islot);
void SetMcDialogState(void *dialog, int nState);
void HideMcDialogWidget(void *dialog);
void load_file_from_disc(int n0, int n1, char *pchzFile, char *pchzCard);
void ResetGsIfSchemaChanged(int *pn);
MCDLG *ConstructScreen(MCDLG *dialog);   /* the widget base constructor (returns the widget) */
int ResetScreenMode(MCDLG *dialog);
void PostLoadScreen(MCDLG *dialog);
void ResetScreen(MCDLG *dialog);
void UpdateTimedScreen(MCDLG *dialog);
int GetPauseMenuTopPageId(void *pmenu);
}
void InitSave(SAVE *saveManager, int n0, int n1);
int SemaCreate(int cInit, int cMax);
int IsMemcardThreadValid(MEMCARD *worker);
void SetMemcardThreadHighPrio(MEMCARD *worker);
char *FindSwLocalizedString(SW *world, int id);

/* libc functions the per-function files declared: at file scope, so that g++ treats them as its builtins
   (declared inside a namespace they are ordinary functions: a strcpy of a constant or fabsf becomes a call). */
extern "C" {
int sprintf(char *, const char *, ...);
char *strcpy(char *, const char *);
}
/** @sly2-annot addr=0x1AA978
 * Purpose: Startup-table entry: reset D_002DEB30 (0x1613F8), clear D_004425D8, InitSave(&g_save,0,0)
 * Info: proposed name StartupMemcard (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/overview.md#1-boot, docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/save-memcard.md#4-source-files
 * @end-sly2-annot */
// Startup-table entry: resets the game-progress object, clears the save request flag and initializes the save
// manager g_save.
void StartupMemcard()
{
    InitGame(D_002DEB30);
    D_004425D8 = -1;
    InitSave(&D_002E65E0, 0, 0);
}

/** @sly2-annot addr=0x1AA9B8
 * Purpose: memset + fill a save-directory info block from sceMcGetDir results
 * Info: proposed name FillMcDirInfo (EXISTING); TU memcard
 * Notes: agent/verifier proposes FillMcDirInfo for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> FillMcDirInfo (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Converts a card directory entry (sceMcGetDir) into the game's SAVEINFO record (attribute bits renumbered).
/* Converts a card directory entry into the game's record (attribute bits renumbered). */
extern "C" void FillMcDirInfo(MCTBLDIR *pdir, SAVEINFO *psi)
{
    memset(psi, 0, 0x5C);
    strcpy(psi->achName, pdir->achName);
    psi->cb = pdir->cb;
    if (pdir->attr & 0x1) {
        psi->grf |= 0x1;
    }
    if (pdir->attr & 0x2) {
        psi->grf |= 0x2;
    }
    if (pdir->attr & 0x4) {
        psi->grf |= 0x4;
    }
    if (pdir->attr & 0x8) {
        psi->grf |= 0x8;
    }
    if (pdir->attr & 0x20) {
        psi->grf |= 0x10;
    }
    if (pdir->attr & 0x80) {
        psi->grf |= 0x20;
    }
    if (pdir->attr & 0x800) {
        psi->grf |= 0x40;
    }
    if (pdir->attr & 0x1000) {
        psi->grf |= 0x80;
    }
    psi->bDayC = pdir->monthC;
    psi->bMonthC = pdir->dayC;
    psi->wYearC = pdir->yearC;
    psi->bHourC = pdir->hourC;
    psi->bMinC = pdir->minC;
    psi->bSecC = pdir->secC;
    psi->bDayM = pdir->monthM;
    psi->bMonthM = pdir->dayM;
    psi->wYearM = pdir->yearM;
    psi->bHourM = pdir->hourM;
    psi->bMinM = pdir->minM;
    psi->bSecM = pdir->secM;
}

/* Internal card results (McResultFromSyncState / McResultFromSyncCommand). */
enum
{
    MC_RESULT_DONE = 0x1D,      /* sceMcSync: the command finished */
    MC_RESULT_BUSY = 0x1E,      /* sceMcSync: still running */
    MC_RESULT_ERROR = 0x1F,     /* sceMcSync failed / unknown command result */
    MC_RESULT_NEG1 = 0x20,      /* the command returned -1 */
    MC_RESULT_OK = 0x21,        /* the command returned 0 */
    MC_RESULT_FAILED = 0x22,    /* the command returned 1: mapped to a message by GetMcErrorMessageId */
};

namespace M_FUN_0018b910__Fi {
/** @sly2-annot addr=0x1AAB48
 * Purpose: sceMcSync state -> internal result: 0 -> 0x1D (done), >0 -> 0x1E, <0 -> 0x1F
 * Info: proposed name McResFromSyncState (EXISTING); TU memcard
 * Notes: agent/verifier proposes McResFromSyncState for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> McResFromSyncState
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Maps a sceMcSync state to the internal result: 0 -> 0x1D (done), positive -> 0x1E (busy), negative -> 0x1F (error).
extern "C" int McResultFromSyncState(int n)
{
    if (n != 0) return n <= 0 ? MC_RESULT_ERROR : MC_RESULT_BUSY;
    return MC_RESULT_DONE;
}
} // namespace M_FUN_0018b910__Fi

namespace M_FUN_0018b930__Fi {
/** @sly2-annot addr=0x1AAB68
 * Purpose: sceMcSync cmd result -> 0x21 (0), 0x22 (1), 0x20 (-1), else 0x1F
 * Info: proposed name McResFromSyncCmd (EXISTING); TU memcard
 * Notes: agent/verifier proposes McResFromSyncCmd for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> McResFromSyncCmd
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Maps a sceMcSync command result to the internal result: 0 -> 0x21, 1 -> 0x22, -1 -> 0x20, else 0x1F.
extern "C" int McResultFromSyncCommand(int n)
{
    switch (n) {
    case 0: return MC_RESULT_OK;
    case 1: return MC_RESULT_FAILED;
    case -1: return MC_RESULT_NEG1;
    }
    return MC_RESULT_ERROR;
}
} // namespace M_FUN_0018b930__Fi

namespace M_MsgFromMcError { extern "C" {
/* GetMcErrorMessageId (0x1AABC0): maps (kind 1..21, error code) to a message id.  Error codes are mostly small negatives
   (-1..-8, -403); code 0 is success and gives message 0, unknown codes fall back to 11 (code < -9) or 31.
   Kinds 7..9 and anything outside 1..21 give 31 (jump-table default).

   Compile note (lane 10): kinds 16/17 write `result = 0` first and then branch to the shared `done:` return at the
   end of the function.  Those conditional branches to the function's return label are what leave the label in
   front of the last `jr ra` while dbr runs, so the final "result = 31; return" keeps an unfilled delay slot like
   the original.  Kind 1's `result = 9` is plain.  No register pin or asm is needed. */
static inline int Fallback(int err) { int msg = 11; if (err >= -9) msg = 31; return msg; }

/** @sly2-annot addr=0x1AABC0
 * Purpose: (kind 1..21, sceMc error) -> message id for the UI
 * Info: proposed name GetMcErrorMessageId (EXISTING); TU memcard
 * Notes: agent/verifier proposes GetMcErrorMessageId for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> GetMcErrorMessageId (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Returns the UI message id for a card error (operation kind 1..21 and the sceMc error code).
int GetMcErrorMessageId(int kind, int err)
{
    int result;
    switch (kind) {
    case 1:
        if (err == -2) goto ret10;
        if (err < -1) { if (err == -5) goto ret11; goto fallback; }
        if (err != -1) goto zeroOrFallback;
        result = 9;
        return result;
ret10:  return 10;
ret11:  return 11;
fallback:
        return Fallback(err);
    case 2:
        switch (err) {
        case -3: ret1: return 1;
        case -4: return 13;
        case -5: return 6;
        case -7: return 25;
        case -2: goto ret12;
        }
        goto ordinary;
errOther16:
        if (err != -4) return Fallback(err);
ret16:  return 16;
    case 4:
        if (err == -4) goto ret16;
        goto checkOrdinary;
    case 5:
        switch (err) { case -4: return 16; case -5: return 17; case -2: goto ret12; }
        goto ordinary;
    case 6:
        switch (err) { case -3: return 1; case -4: return 16; case -5: return 18; case -8: return 26; case -2: goto ret12; }
        goto ordinary;
    case 3:
    case 10:
        if (err == -2) goto ret12;
        if (err < -1) goto errOther16;
zeroOrFallback:
        if (err != 0) goto fallback;
zero:   return 0;
    case 11:
        if (err == -3) goto ret1;
        if (err < -2) { if (err == -4) goto ret24; goto fallback; }
        if (err == -2) goto ret12;
        goto zeroOrFallback;
ret24:  return 24;
    case 12:
        if (err == -2) goto ret12;
        if (err >= -1) goto zeroOrFallback;
        if (err != -4) return Fallback(err);
ret21:  return 21;
    case 13:
        if (err == -4) goto ret21;
        goto checkOrdinary;
ret12:  return 12;
    case 14:
        if (err == -2) goto ret12;
        if (err >= -1) goto zeroOrFallback;
errOther22:
        if (err != -4) return Fallback(err);
        return 22;
    case 15:
        switch (err) { case -4: return 14; case -5: return 19; case -6: return 23; case -2: goto ret12; case 0: goto zero; }
        return Fallback(err);
    case 16:
        result = 0;
        if (err == 0) goto done;
        { int msg = 4; if (err >= 0) msg = 31; return msg; }
    case 17:
        result = 0;
        if (err == 0) goto done;
        { int msg = 5; if (err >= 0) msg = 31; return msg; }
    case 18:
checkOrdinary:
        if (err == -2) goto ret12;
ordinary:
        if (err >= 0) goto zero;
        return Fallback(err);
    case 19:
        if (err == -2) goto ret12;
        if (err < -1) goto errOther22;
        goto zeroOrFallback;
    case 20:
        switch (err) { case 0: goto zero; case -403: return 35; }
        return 31;
    case 21:
        if (err >= 0) goto zero;
        if ((unsigned)(err + 100) < 100) return 31;
        { int msg = 30; if (err >= -99) msg = 31; return msg; }
    }
    result = 31;
done:
    return result;
}
} } // namespace M_MsgFromMcError

/** @sly2-annot addr=0x1AAED8
 * Purpose: MEMCARD init: SemaCreate(1,1), thread id -1, job result 0x20
 * Info: proposed name InitMemcard (T1); TU memcard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Initializes the memory card worker: semaphore (1, 1), no thread yet, job result 0x20 (idle).
void InitMemcard(MEMCARD *worker)
{
    worker->sema = SemaCreate(1, 1);
    worker->tid = -1;
    worker->jobResult = MC_RESULT_NEG1;
}

namespace M_MemcardThreadMain { extern "C" {
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int sema; int tid; struct PMF pmf; int jobResult; };   /* MEMCARD */
extern char D_002E65E0[];
extern int D_004425D8[];
void func_00234A60(int) __asm__("WaitSema");
void func_00234A40(int) __asm__("SignalSema");
void ExitDeleteThread(void);
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
/** @sly2-annot addr=0x1AAF18
 * Purpose: Memcard worker thread body: WaitSema, call the queued SAVE member-function job on g_save, clear
 *   job, signal, ExitThread
 * Info: proposed name MemcardThreadMain (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/main-loop-startup.md#6-threads,
 *   docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions
 * @end-sly2-annot */
// Body of the memory card worker thread: waits on the semaphore, runs the queued SAVE member-function job on g_save,
// clears it, signals and exits.
void MemcardThreadMain(struct S *worker)
{
    func_00234A60(worker->sema);
    worker->jobResult = worker->pmf.pfn(D_002E65E0 + worker->pmf.delta);
    func_002333B4(&worker->pmf, 0, sizeof(worker->pmf));
    worker->tid = -1;
    D_004425D8[0] = -1;
    func_00234A40(worker->sema);
    ExitDeleteThread();
}
} } // namespace M_MemcardThreadMain
extern SAVE D_002E65E0;

namespace M_CreateMemcardThread { extern "C" {
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int sema; int tid; struct PMF pmf; int jobResult; };   /* MEMCARD */
struct ThreadParam { int attr; void *entry; void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr2; int option; int waitType; int waitId; int wakeupCount; };
extern char D_002E5270[] __asm__("_gp");   /* _gp (linker-defined), not an object */
extern char D_002E2080[];
extern int D_004425D8[];
void MemcardThreadMain(struct S *worker);
void SetMemcardThreadHighPrio__FP7MEMCARD(struct S *worker);
void *func_002333B4(void *p, int c, unsigned int n) __asm__("memset");
int func_00234820(struct ThreadParam *tp);
void func_002355E0(int id, void *arg) __asm__("StartThread_2355E0");
/** @sly2-annot addr=0x1AAF90
 * Purpose: Wait for previous job, store pointer-to-member job {0xFFFF0000,pfn}, _iDisableIntc+_iEnableDmac
 * Info: proposed name CreateMemcardThread (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Queues a job (a pointer to a SAVE member function) for the worker thread and starts the thread, after waiting for
// the previous job.
void CreateMemcardThread(struct S *worker, struct PMF pmf)
{
    struct ThreadParam tp;
    SetMemcardThreadHighPrio__FP7MEMCARD(worker);
    worker->pmf = pmf;
    worker->jobResult = MC_RESULT_OK;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.entry = MemcardThreadMain;
    tp.stack = D_002E2080;
    tp.gpReg = D_002E5270;
    tp.stackSize = 0x4000;
    tp.initPriority = 3;
    worker->tid = func_00234820(&tp);
    D_004425D8[0] = worker->tid;
    func_002355E0(worker->tid, worker);
}
} } // namespace M_CreateMemcardThread

/** @sly2-annot addr=0x1AB040
 * Purpose: If thread valid iReleaseAlarm(id,1); then wait/signal sema (block until job finishes)
 * Info: proposed name SetMemcardThreadHighPrio (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources:
 *   r4_17F090+verdicts_A2+names_v2-signature(verified)
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Raises the worker thread's priority when it is running, then blocks until its job has finished.
void SetMemcardThreadHighPrio(MEMCARD *worker)
{
    if (IsMemcardThreadValid(worker) != 0) {
        func_002348B0(worker->tid, 1);
    }
    WaitSema(worker->sema);
    SignalSema(worker->sema);
}

/** @sly2-annot addr=0x1AB088
 * Purpose: Thread id (+4) != -1
 * Info: proposed name IsMemcardThreadValid__FP7MEMCARD (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// True when the worker thread exists (thread id != -1).
int IsMemcardThreadValid(MEMCARD *worker)
{
    return ~worker->tid != 0;
}

namespace M_McSyncResult { extern "C" {
int McResultFromSyncState(void *p) __asm__("McResultFromSyncState");
int sceMcSync(int mode, int *cmd, int *result);
int McResultFromSyncCommand(int n) __asm__("McResultFromSyncCommand");
int GetMcErrorMessageId(void *p, int n);
/** @sly2-annot addr=0x1AB098
 * Purpose: Poll sceMcSync(0,&cmd,&res) and map to result; on error map through GetMcErrorMessageId
 * Info: proposed name McSyncResult (EXISTING); TU memcard
 * Notes: agent/verifier proposes McSyncResult for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McSyncResult (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Polls sceMcSync and maps the outcome to an internal result; errors are mapped to a UI message through
// GetMcErrorMessageId.
int McSyncResult(void *worker, void *syncState, void *operationKind, int *errorOut)
{
    int syncOut[2];     /* sceMcSync's command id and result */
    int result = McResultFromSyncState(syncState);
    if (result != MC_RESULT_DONE)
        return result;
    result = McResultFromSyncCommand(sceMcSync(0, &syncOut[0], &syncOut[1]));
    if (errorOut)
        *errorOut = syncOut[1];
    if (result == MC_RESULT_FAILED)
        result = GetMcErrorMessageId(operationKind, syncOut[1]);
    return result;
}
} } // namespace M_McSyncResult

namespace M_InitSave__FP4SAVEii {
struct SAVE {
    int port;
    int slot;
    int state;
    int nextState;
    float tStateChanged;
    float tNextPoll;
    int cardType;
    int freeClusters;
    int isFormatted;
};
extern "C" void sceMcInit(void);
extern "C" void InitMemcard__FP7MEMCARD(void *);
/** @sly2-annot addr=0x1AB128
 * Purpose: sceMcInit, InitMemcard(+0xFD5C), zero SAVE header
 * Info: proposed name InitSave__FP4SAVEii (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Initializes the save manager: sceMcInit, the worker (MEMCARD) and the SAVE header.
extern "C" void InitSave__FP4SAVEii(SAVE *saveManager, int port, int slot)
{
    sceMcInit();
    InitMemcard__FP7MEMCARD((char *)saveManager + SAVE_MEMCARD_OFFSET);
    saveManager->port = port;
    saveManager->slot = slot;
    saveManager->state = -1;
    saveManager->nextState = 0;
    saveManager->tStateChanged = saveManager->tNextPoll = 0.0f;
    saveManager->cardType = 0;
    saveManager->freeClusters = 0;
    saveManager->isFormatted = 0;
}
} // namespace M_InitSave__FP4SAVEii

// The memcard thread runs a pointer-to-member job (CreateMemcardThread). UpdateSave and SetSaveState spell the
// same constant (&McJobGetInfo), which g++ emits once, as the ELF has it at 0x2A6BA0 (with &McJobGetDir after it).
struct MCJOB { int GetInfo() __asm__("McJobGetInfo"); int GetDir() __asm__("McJobGetDir"); };
void CreateMemcardThreadJob(void *, int (MCJOB::*)()) __asm__("CreateMemcardThread");

namespace M_UpdateSave__FP4SAVE { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct H { char pad[0x44]; int phase; };   /* the memcard dialog */
struct S { int port, slot; int state; int nextState; float tStateChanged; float tNextPoll; char pad[0xFD5C - 0x18]; char memcard[1]; };
extern char D_003DD490[], D_002F66B0[], D_003D9ED0[];
extern int D_002DEB4C;
extern struct H D_002E6550;
int IsSwTransitionBusy(void *g);
int IsMemcardThreadValid__FP7MEMCARD(void *p);
void CreateMemcardThread(void *t, long pmf);
void ReloadSaveSlots(struct S *saveManager);
void SetSaveState(struct S *s, int);
int IsMpegActive(void *p);
int IsPauseMenuOnLoadPageB(void *p);
void SetMcDialogState(struct H *h, int n);
/** @sly2-annot addr=0x1AB1A0
 * Purpose: Per-frame save state machine: skip during transitions/when thread busy; poll card (job McJobGetInfo)
 *   or advance state (0x1AB418); autosave trigger with 0.3s debounce
 * Info: proposed name UpdateSave (T1); TU memcard
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Per-frame save state machine: skipped during transitions or while the worker is busy; polls the card (McJobGetInfo)
// or advances the state, and triggers the autosave with a 0.3 s debounce.
void UpdateSave__FP4SAVE(struct S *saveManager)
{
    if (IsSwTransitionBusy(D_003DD490))
        return;
    if (D_002DEB4C)
        return;
    if (D_002E6550.phase == 3)
        return;
    if (IsMemcardThreadValid__FP7MEMCARD(saveManager->memcard))
        return;
    if (saveManager->state == saveManager->nextState) {
        if (saveManager->tNextPoll <= g_clock.realTime)
            CreateMemcardThreadJob(saveManager->memcard, &MCJOB::GetInfo);
        else
            ReloadSaveSlots(saveManager);
    } else {
        SetSaveState(saveManager, saveManager->nextState);
    }
    if (IsMemcardThreadValid__FP7MEMCARD(saveManager->memcard))
        return;
    if (IsMpegActive(D_002F66B0))
        return;
    if (D_002E6550.phase != 1)
        return;
    if (0.3f <= g_clock.realTime - saveManager->tStateChanged) {
    if (IsPauseMenuOnLoadPageB(D_003D9ED0)) {
        if (saveManager->state != 6)
            SetMcDialogState(&D_002E6550, 0x2E);
        return;
    }
    switch (saveManager->state) {
    case 1:
        SetMcDialogState(&D_002E6550, 0x11);
        break;
    case 2:
        SetMcDialogState(&D_002E6550, 0x12);
        break;
    case 3:
        SetMcDialogState(&D_002E6550, 0x15);
        break;
    case 4:
        SetMcDialogState(&D_002E6550, 0x18);
        break;
    }
    }
}
} } // namespace M_UpdateSave__FP4SAVE

/** @sly2-annot addr=0x1AB398
 * Purpose: existing
 * Info: proposed name CompareSaves__FP4SAVET0 (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Orders two directory records by modification date (year, day/month bytes as stored, hour, minute, second).
/* Orders two records by modification date (year, day/month bytes as stored, hour, minute, second). */
extern "C" int CompareSaves__FP4SAVET0(SAVEINFO *psi0, SAVEINFO *psi1)
{
    int d = psi0->wYearM - psi1->wYearM;
    if (d == 0) {
        d = psi0->bDayM - psi1->bDayM;
        if (d == 0) {
            d = psi0->bMonthM - psi1->bMonthM;
            if (d == 0) {
                d = psi0->bHourM - psi1->bHourM;
                if (d == 0) {
                    d = psi0->bMinM - psi1->bMinM;
                    if (d == 0) {
                        return psi0->bSecM - psi1->bSecM;
                    }
                }
            }
        }
    }
    return d;
}

namespace M_SetSaveState {
struct CRef;
extern "C" void Decref__4CRef(CRef *);
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef(const CRef &) __asm__("__4CRefRC4CRef");
    ~CRef() { Decref__4CRef(this); }
};
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct SLOT { char pad[0x5408]; char info[0x5C]; };   /* SAVEINFO at +0x5408 */
struct THR;
struct MC {
    int port; int slot; int state; int nextState; float tStateChanged; char pad[0x24 - 0x14];
    SLOT aslot[3];
    int currentSlot; SLOT *newestSlot; int selectedSlot; THR *thr[1];
};
struct G { char pad[0x3B1C]; int saveVarIsUsed; };   /* g_sw: id of the slot-in-use save variable */
struct CLOCK { char pad[0x3C]; float realTime; };
struct ST { char pad[0x44]; int phase; };   /* the memcard dialog */
extern "C" {
extern G D_003DD490;
extern CLOCK D_002DDED0;
extern ST D_002E6550;
extern char D_002E65A0[];
void SetMcDialogPhase(ST *, int);
int IsMcStatusVisible(void *);
void SetMcDialogState(ST *, int);
void HideMcDialogWidget(void *);
int CompareSaves__FP4SAVET0(char *, char *);
void ResetSaveGs(SLOT *, int);
void SetSaveState(MC *saveManager, int state);
}
CRef Eval(int, SLOT *) __asm__("GetGlobalSaveVar");
static inline float GetF(int k, SLOT *p) { return Eval(k, p).u.g; }

/** @sly2-annot addr=0x1AB418
 * Purpose: SAVE state change (x8) with per-state actions on the 3 slots (Splice CRef evals), UI state
 *   updates, ResetGs on load
 * Info: proposed name SetSaveState (T1); TU memcard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Changes the save state, with per-state actions on the three slots (Splice evaluations), UI updates and a GS reset
// on load.
void SetSaveState(MC *saveManager, int state)
{
    int i;
    if (state == saveManager->state)
        return;
    if (saveManager->state == 6) {
        saveManager->newestSlot = 0;
        if (saveManager->currentSlot) {
            ST *dialog = &D_002E6550;
            if (dialog->phase == 1) {
                SetMcDialogPhase(dialog, 3);
                if (IsMcStatusVisible(D_002E65A0)) {
                    SetMcDialogState(dialog, 0x23);
                    HideMcDialogWidget(D_002E65A0);
                } else {
                    SetMcDialogState(dialog, 0x24);
                }
            }
        }
    }
    switch (state) {
    case 0:
    case 1:
        CreateMemcardThreadJob(&saveManager->thr, &MCJOB::GetInfo);
        break;
    case 3:
        CreateMemcardThreadJob(&saveManager->thr, &MCJOB::GetDir);
        break;
    case 6:
        saveManager->newestSlot = &saveManager->aslot[0];
        for (i = 0; i < 3; i++) {
            SLOT *pslot = &saveManager->aslot[i];
            float isUsed = GetF(D_003DD490.saveVarIsUsed, pslot);
            if (isUsed != 0.0f) {
                if (CompareSaves__FP4SAVET0(saveManager->newestSlot->info, pslot->info) < 0)
                    saveManager->newestSlot = pslot;
            } else {
                ResetSaveGs(pslot, 0);
            }
        }
        break;
    }
    saveManager->state = state;
    saveManager->tStateChanged = D_002DDED0.realTime;
}
} // namespace M_SetSaveState

/** @sly2-annot addr=0x1AB630
 * Purpose: If state 5 and (*(int *)&g_sw.savevars): wait for thread, re-init all 3 slots (stride 0x5464, 0x1AC570), go to
 *   state 6
 * Info: proposed name ReloadSaveSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Once the worker is idle in state 5 with save variables present, re-initializes the three slots and goes to state 6.
/* Reloads the three slots once the worker is idle (state 5 and (*(int *)&g_sw.savevars) set). */
extern "C" void ReloadSaveSlots(SAVE *saveManager)
{
    int i;
    char *pslot;
    if (saveManager->nState == 5 && (*(int *)&g_sw.savevars) != 0) {
        pslot = saveManager->aslot[0];
        i = 2;
        SetMemcardThreadHighPrio(&saveManager->memcard);
        do {
            ResetSaveSlot(saveManager, pslot);
            i--;
            pslot += 0x5464;
        } while (i >= 0);
        saveManager->nStateNext = 6;
        SetSaveState(saveManager, 6);
    }
}

/** @sly2-annot addr=0x1AB6C0
 * Purpose: Thread job: sceMcFormat(port,slot); result -> SAVE next state 2/3 and UI state
 * Info: proposed name McJobFormat (T1); TU memcard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: formats the card (sceMcFormat) and sets the next save state and UI state from the result.
extern "C" int McJobFormat(SAVE *saveManager)
{
    int nResult = McSyncResult(&saveManager->memcard, sceMcFormat(saveManager->port, saveManager->slot), 0x10, 0);
    if (nResult != 0) {
        saveManager->nStateNext = 2;
        SetMcDialogPhase(&D_002E6550, 3);
    } else {
        saveManager->nStateNext = 3;
    }
    return nResult;
}

// 0x1AB748: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x20");

namespace M_FOpenSaveIconFile {
struct S { int sectorKey; int sizeKey; char pad[0x2C]; int sectorXor; int unknown38; int sizeXor; };   /* a disc file entry: obfuscated sector / size */
struct K;
extern "C" void InitFileKey(K *k);
extern "C" int GetFileKeySize(K *k);
struct K { int sector; int size; int mode; char pad[0x104];
  K() { mode = 2; InitFileKey(this); }
  bool FSet(S *entry)
  {
      if (!entry) return false;
      size = entry->sizeKey ^ entry->sizeXor;
      sector = (entry->sectorKey ^ entry->sectorXor) + 0xCA0;
      mode = 2;
      return size != 0;
  }
};
/** @sly2-annot addr=0x1AB750
 * Purpose: Build a disc FLOC (lba=(w0^w13)+0xCA0,size=w1^w15) for a file entry and open it via 0x14B670
 * Info: proposed name GetDiscFileEntrySize (EXISTING); TU memcard
 * Notes: agent/verifier proposes GetDiscFileEntrySize for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> GetDiscFileEntrySize
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Opens the save icon file on the disc (a FLOC built from the file entry); returns false when the entry is not set.
extern "C" { int GetDiscFileEntrySize(S *entry)
{
    K key;
    bool isSet = key.FSet(entry);
    if (isSet) return GetFileKeySize(&key);
    else return 0;
} }
} // namespace M_FOpenSaveIconFile

namespace M_CreateSaveDir { extern "C" {
void *McMkdir(void *saveManager, char *dir);
void *McWriteFile(void *saveManager, char *path, char *data, int size);
void *WriteIconSys(void *saveManager, char *dir, char *title);
void *load_sly_icon_from_disc(void *saveManager, char *dir);
/** @sly2-annot addr=0x1AB7D0
 * Purpose: mkdir /BASCUS-97316YAOTWTD!, write data file, write icon.sys (0x1AC008), copy icon from disc
 *   (load_sly_icon_from_disc)
 * Info: proposed name CreateSaveDir (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Creates the save directory (/BASCUS-97316YAOTWTD!) with the data file, icon.sys and the icon copied from the disc.
void *CreateSaveDir(void *saveManager)
{
    void *error = McMkdir(saveManager, "/BASCUS-97316YAOTWTD!");
    if (error)
        return error;
    error = McWriteFile(saveManager, "/BASCUS-97316YAOTWTD!/BASCUS-97316YAOTWTD!", "Sly 2", 5);
    if (error)
        return error;
    error = WriteIconSys(saveManager, "/BASCUS-97316YAOTWTD!", "Sly 2");
    if (error)
        return error;
    return load_sly_icon_from_disc(saveManager, "/BASCUS-97316YAOTWTD!");
}
} } // namespace M_CreateSaveDir

namespace M_McJobCreateSave { extern "C" {
struct B { char data[0x5464]; };   /* a save slot */
struct S { int port; int slot; int state; int nextState; int tStateChanged, tNextPoll, cardType; int freeClusters; int isFormatted; struct B slots[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char memcard[1]; };
extern char D_002A38E8[], D_002E6550[];
int sceMcGetEntSpace(int a, int b, char *c);
int McSyncResult(void *p, int a, int b, int *c);
int GetDiscFileEntrySize(void *p);
int CreateSaveDir(struct S *s);
int WriteSaveSlotFileReset(struct S *s, struct B *b);
void BuildSaveFilePath(struct S *s, struct B *b, char *path);
void McDelete(struct S *s, char *path);
void SetMcDialogPhase(void *p, int n);
/** @sly2-annot addr=0x1AB868
 * Purpose: Thread job: check space (sceMcGetEntSpace), delete stale, CreateSaveDir, write slot files
 * Info: proposed name McJobCreateSave (T1); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c250 (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: checks the free space, deletes a stale save, creates the directory and writes the slot files.
int McJobCreateSave(struct S *saveManager)
{
    char path[0x50];
    int entrySpace;
    int clustersNeeded;
    int r;
    int i;
    void *worker = saveManager->memcard;
    clustersNeeded = 1;
    r = McSyncResult(worker, sceMcGetEntSpace(saveManager->port, saveManager->slot, "/"), 0x12, &entrySpace);
    if (r == 0) {
        if (!entrySpace)
            clustersNeeded = 2;
        clustersNeeded += 5;
        clustersNeeded += (GetDiscFileEntrySize(D_002A38E8) + 0x3FF) / 0x400;
        clustersNeeded += 0x42;
        if (saveManager->freeClusters < clustersNeeded) {
            r = 1;
        } else {
            r = CreateSaveDir(saveManager);
            if (r == 0) {
                for (i = 0; i < 3; i++) {
                    r = WriteSaveSlotFileReset(saveManager, &saveManager->slots[i]);
                    if (r)
                        goto cleanup;
                }
                if (saveManager->nextState < 5)
                    saveManager->nextState = 5;
                goto done;
            }
cleanup:
            for (i = 0; i < 3; i++) {
                BuildSaveFilePath(saveManager, &saveManager->slots[i], path);
                McDelete(saveManager, path);
            }
            McDelete(saveManager, "/BASCUS-97316YAOTWTD!/icon.sys");
            McDelete(saveManager, "/BASCUS-97316YAOTWTD!/memcard_icon.ico");
            McDelete(saveManager, "/BASCUS-97316YAOTWTD!/BASCUS-97316YAOTWTD!");
            McDelete(saveManager, "/BASCUS-97316YAOTWTD!");
        }
    }
    if (r)
        SetMcDialogPhase(D_002E6550, 3);
done:
    return r;
}
} } // namespace M_McJobCreateSave

namespace M_McJobGetDir { extern "C" {
struct B { char data[0x5464]; };
struct S { int port; int slot; int state; int nextState; int pad[5]; struct B slots[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char memcard[1]; };
int sceMcGetDir(int a, int b, char *c, int d, int e, int f);
int McSyncResult(void *p, int a, int b, int *c);
int McGetInfoSync(struct S *s, int *out);
int ReadSaveSlotHeaders(struct S *s, struct B *b);
/** @sly2-annot addr=0x1ABA10
 * Purpose: Thread job: sceMcGetInfo + sceMcGetDir of save dir, read slot headers (0x1AC4C8)
 * Info: proposed name McJobGetDir (EXISTING); TU memcard
 * Notes: agent/verifier proposes McJobGetDir for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McJobGetDir (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: reads the card info and the save directory, then each slot's header.
int McJobGetDir(struct S *saveManager)
{
    void *worker = saveManager->memcard;
    int numEntries_freeSpace[2];   /* [0]: directory entries found; [1]: free space (McGetInfoSync) */
    int r;
    int i;
    r = McSyncResult(worker, sceMcGetDir(saveManager->port, saveManager->slot, "/BASCUS-97316YAOTWTD!", 0, -1, 0), 0xD, &numEntries_freeSpace[0]);
    if (numEntries_freeSpace[0] <= 0) {
        McGetInfoSync(saveManager, &numEntries_freeSpace[1]);
        if (numEntries_freeSpace[1] <= 0x18FFF)
            saveManager->nextState = 4;
        return 0xE;
    }
    for (i = 0; r == 0 && i < 3; i++)
        ReadSaveSlotHeaders(saveManager, &saveManager->slots[i]);
    if (r == 0 && saveManager->nextState < 5)
        saveManager->nextState = 5;
    return r;
}
} } // namespace M_McJobGetDir

namespace M_BuildSaveFilePath { extern "C" {
struct SAVE { char ab[0x5464]; };
struct M { char pad[0x24]; struct SAVE slots[4]; };
int sprintf(char *buf, char *fmt, ...);
/** @sly2-annot addr=0x1ABB10
 * Purpose: sprintf('/%s/%s%d', 'BASCUS-97316YAOTWTD!', 'save', i)
 * Info: proposed name BuildSaveFilePath (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Builds a slot's file path: "/BASCUS-97316YAOTWTD!/save<i>".
void BuildSaveFilePath(struct M *saveManager, struct SAVE *slot, char *buf)
{
    sprintf(buf, "/%s/%s%d", "BASCUS-97316YAOTWTD!", "save", slot - saveManager->slots + 1);
}
} } // namespace M_BuildSaveFilePath

namespace M_McJobSaveSlot { extern "C" {
struct S { char pad[0xFD50]; void *currentSlot; };
extern char D_002E6550[];
void *McJobGetInfo(struct S *s);
void *CreateSaveDir(struct S *s);
void *WriteSaveSlotFile(struct S *s, void *x);
void SetMcDialogPhase(void *p, int n);
/** @sly2-annot addr=0x1ABB60
 * Purpose: Thread job: card info check, CreateSaveDir, write one slot file (0x1AC420)
 * Info: proposed name McJobSaveSlot (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: checks the card, creates the save directory when needed and writes one slot file.
void *McJobSaveSlot(struct S *saveManager)
{
    void *error = McJobGetInfo(saveManager);
    if (!error) error = CreateSaveDir(saveManager);
    if (!error) error = WriteSaveSlotFile(saveManager, saveManager->currentSlot);
    if (error) SetMcDialogPhase(D_002E6550, 3);
    return error;
}
} } // namespace M_McJobSaveSlot

/** @sly2-annot addr=0x1ABBE0
 * Purpose: Thread job: card info check, set UI/SAVE state
 * Info: proposed name McJobCheckCard (T2); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c770 (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job of the autosave: checks the card and rewrites the current slot; on failure shows the dialog's error
// state (0x23) and hides the status widget.
/* Thread job: check the card, then write the current slot. */
extern "C" int McJobAutosave(SAVE *saveManager)
{
    int nResult = McJobGetInfo(saveManager);
    if (nResult != 0 || (nResult = WriteSaveSlotFile(saveManager, saveManager->currentSlot)) != 0) {
        SetMcDialogPhase(&D_002E6550, 3);
        SetMcDialogState(&D_002E6550, 0x23);
        HideMcDialogWidget(&D_002E65A0);
    }
    return nResult;
}

namespace M_func_001ABC68 { extern "C" {
struct S { char pad[0xFD58]; void *selectedSlot; };   /* the slot picked on the save page */
extern char D_002E6550[];
void *McJobGetInfo(struct S *s);
void *CreateSaveDir(struct S *s);
void *WriteSaveSlotFileReset(struct S *s, void *x);
void SetMcDialogPhase(void *p, int n);
/** @sly2-annot addr=0x1ABC68
 * Purpose: Thread job: card check, CreateSaveDir, write via 0x1AC468 (with ResetGs)
 * Info: proposed name McJobWriteSave (T2); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c7f8__FP4SAVE (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: like McJobSaveSlot, but the slot is written with a GS reset first (WriteSaveSlotFileReset).
void *McJobSaveSlotResetGs(struct S *saveManager)
{
    void *error = McJobGetInfo(saveManager);
    if (!error) error = CreateSaveDir(saveManager);
    if (!error) error = WriteSaveSlotFileReset(saveManager, saveManager->selectedSlot);
    if (error) SetMcDialogPhase(D_002E6550, 3);
    return error;
}
} } // namespace M_func_001ABC68

namespace M_func_001ABCE8 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct K { int a, b; };
extern const struct LONGU D_002A6C88;
struct S { char pad[0xFD50]; int currentSlot; int pad2[2]; char memcard[4]; };
void ResetRumbleSaveVar(struct S *p);
void BuildMemcardGsMask(void *p, int n);
void CreateMemcardThread(void *p, long k);
/** @sly2-annot addr=0x1ABCE8
 * Purpose: Build GS mask (BuildMemcardGsMask) and CreateMemcardThread for a save
 * Info: proposed name StartSaveJobA (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Starts the autosave: resets the rumble save variable, stores the save variables into the current slot and queues
// McJobAutosave.
void StartAutosaveJob(struct S *saveManager)
{
    ResetRumbleSaveVar(saveManager);
    BuildMemcardGsMask((*(char (*)[])&g_sw.savevars), saveManager->currentSlot);
    CreateMemcardThread(saveManager->memcard, D_002A6C88.l);
}
} } // namespace M_func_001ABCE8

namespace M_func_001ABD40 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct K { int a, b; };
extern const struct LONGU D_002A6C90;
struct S { char pad[0xFD50]; int currentSlot; int pad2[2]; char memcard[4]; };
void ResetRumbleSaveVar(struct S *p);
void BuildMemcardGsMask(void *p, int n);
void CreateMemcardThread(void *p, long k);
/** @sly2-annot addr=0x1ABD40
 * Purpose: Same as 0x1ABCE8 with other job
 * Info: proposed name StartSaveJobB (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Starts a save to the current slot: same preparation as the autosave, then queues McJobSaveSlot.
void StartSaveSlotJob(struct S *saveManager)
{
    ResetRumbleSaveVar(saveManager);
    BuildMemcardGsMask((*(char (*)[])&g_sw.savevars), saveManager->currentSlot);
    CreateMemcardThread(saveManager->memcard, D_002A6C90.l);
}
} } // namespace M_func_001ABD40

namespace M_func_001ABD98 { extern "C" {
struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0x3B54]; int saveVarAutosaveEnabled; };   /* g_sw: id of the autosave-enabled save variable (inferred) */
struct H { char pad[0x4C]; int followUpAction; };   /* the memcard dialog */
struct S { char pad[0xFD50]; void *currentSlot; };
extern struct G D_003DD490;
extern struct H D_002E6550;
extern int D_002DEB4C;
extern char D_003D4A60[], D_002E65A0[];
int IsSwTransitionBusy(struct G *g);
int GetCurrentWorld(void);
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void StartAutosaveJob(struct S *s);
void ShowMcDialogWidget(void *p);
/** @sly2-annot addr=0x1ABD98
 * Purpose: If a slot is current and not in transition: eval Splice flag; if card missing show UI state 0x22
 *   else start save job
 * Info: proposed name AutoSaveGame (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Autosaves when a slot is current and no transition is running (Splice save flag unless forced): shows the "no card"
// dialog (state 0x22) when the card is missing, else starts the autosave job.
void AutosaveGame(struct S *saveManager, int force)
{
    struct G *world;
    struct CRef ref;
    int isEnabled;
    if (!saveManager->currentSlot)
        return;
    world = &D_003DD490;
    if (IsSwTransitionBusy(world))
        return;
    if (D_002DEB4C)
        return;
    if (!GetCurrentWorld() && !force)
        return;
    GetGlobalSaveVar(&ref, world->saveVarAutosaveEnabled, D_003D4A60);
    isEnabled = ref.m_tag;
    Decref__4CRef(&ref);
    if (!isEnabled) {
        D_002E6550.followUpAction = 3;
        SetMcDialogState(&D_002E6550, 0x22);
    } else {
        StartAutosaveJob(saveManager);
        ShowMcDialogWidget(D_002E65A0);
    }
}
} } // namespace M_func_001ABD98

namespace M_ReadMcDir { extern "C" {
typedef struct { char a[0x40]; } ENT;   /* sceMcTblGetDir */
typedef struct { char a[0x5C]; } OUT;   /* SAVEINFO */
extern char D_002E6500[];
extern ENT D_002E6080[];
unsigned int strlen(const char *);
char *strcpy(char *, const char *);
void *memset_(void *, int, unsigned int) __asm__("memset");
int sceMcGetDir(int, int, char *, int, int, ENT *);
int McSyncResult(void *, int, int, int *);
void FillMcDirInfo(ENT *, OUT *);

/** @sly2-annot addr=0x1ABE78
 * Purpose: sceMcGetDir into table, then FillMcDirInfo
 * Info: proposed name ReadMcDir (EXISTING); TU memcard
 * Notes: agent/verifier proposes ReadMcDir for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> ReadMcDir
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Reads a card directory listing (sceMcGetDir) into an array of records (FillMcDirInfo).
int ReadMcDir(int *saveManager, char *path, int maxEntries, OUT *records, int *numRecords)
{
    int continueListing;   /* sceMcGetDir mode: 0 new listing, 1 continue the previous one */
    int r;
    int i;
    if (maxEntries > 0x12)
        return 0x25;
    if (path) {
        if (strlen(path) > 0x40)
            return 0xD;
        strcpy(D_002E6500, path);
        continueListing = 0;
    } else {
        continueListing = 1;
        path = D_002E6500;
    }
    memset_(D_002E6080, 0, 0x480);
    r = McSyncResult((char *)saveManager + SAVE_MEMCARD_OFFSET, sceMcGetDir(saveManager[0], saveManager[1], path, continueListing, maxEntries, D_002E6080), 0xD, numRecords);
    if (r)
        return r;
    for (i = 0; i < *numRecords; i++)
        FillMcDirInfo(&D_002E6080[i], &records[i]);
    return 0;
}
} } // namespace M_ReadMcDir

namespace M_ReadMcDirWrapper { extern "C" {
int ReadMcDir(int saveManager, int path, int maxEntries, int records, int *numRecords);
/** @sly2-annot addr=0x1ABFC0
 * Purpose: wrapper of 0x1ABE78
 * Info: proposed name ReadMcDirWrapper (EXISTING); TU memcard
 * Notes: agent/verifier proposes ReadMcDirWrapper for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> ReadMcDirWrapper
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Wrapper of ReadMcDir.
int ReadMcDirWrapper(int saveManager, int path, int record)
{
    int numRecords;
    int r = ReadMcDir(saveManager, path, 1, record, &numRecords);
    if (r) return r;
    return numRecords == 1 ? 0 : 0xE;
}
} } // namespace M_ReadMcDirWrapper

// 0x1ABFF8..0x1AC004: 2 dead-code scraps (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x10");
SLY2_SCRAP("addiu $sp, $sp, 0x10");

// .rodata order (tu-merging.md "Phase 5: data order"): the pointer-to-member records are defined where the ELF has
// them among the functions' literals, under asm labels. 0x2A6C80 "/%s/%s" is a literal no code in the ELF
// references (a use the original compiled away); defined here under its address name to keep the layout.
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_func_001ABBE0() __asm__("McJobAutosave");
void p_McJobSaveSlot() __asm__("McJobSaveSlot");
extern const char r_D_002A6C80[] __asm__("D_002A6C80") = "/%s/%s";
extern const unsigned int r_D_002A6C88[2] __asm__("D_002A6C88") = {0xFFFF0000, (unsigned int)p_func_001ABBE0};
extern const unsigned int r_D_002A6C90[2] __asm__("D_002A6C90") = {0xFFFF0000, (unsigned int)p_McJobSaveSlot};
}
namespace M_WriteIconSys { extern "C" {
typedef struct {
    char Head[4];
    unsigned short Reserv1;
    unsigned short OffsLF;
    unsigned int Reserv2;
    unsigned int TransRate;
    int BgColor[4][4];
    float LightDir[3][4];
    float LightColor[3][4];
    float Ambient[4];
    unsigned char TitleName[68];
    unsigned char FnameView[64];
    unsigned char FnameCopy[64];
    unsigned char FnameDel[64];
    unsigned char Reserve3[512];
} ICONSYS;
void *memset_(void *, int, unsigned int) __asm__("memset");
void ConvertStringToSjis(char *, unsigned char *);
int McWriteFile(void *saveManager, char *path, void *data, int size);

/** @sly2-annot addr=0x1AC008
 * Purpose: Build PS2 icon.sys (memset, title 'Sly 2', sprintf) and write it
 * Info: proposed name WriteIconSys (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteIconSys for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> WriteIconSys (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Builds the PS2 icon.sys for the save (title "Sly 2") and writes it into the save directory.
int WriteIconSys(void *saveManager, char *dir, char *title)
{
    ICONSYS icon;
    char path[0x50];
    memset_(&icon, 0, sizeof(icon));
    strcpy(icon.Head, "PS2D");
    icon.OffsLF = 0x20;
    icon.TransRate = 0x60;
    icon.BgColor[0][0] = 0;
    icon.BgColor[0][1] = 0x87;
    icon.BgColor[0][2] = 0xFF;
    icon.BgColor[1][0] = 0;
    icon.BgColor[1][1] = 0x29;
    icon.BgColor[1][2] = 0x6F;
    icon.BgColor[2][0] = 0;
    icon.BgColor[2][1] = 0x29;
    icon.BgColor[2][2] = 0x6F;
    icon.BgColor[3][0] = 0;
    icon.BgColor[3][1] = 0x87;
    icon.BgColor[3][2] = 0xFF;
    icon.LightDir[0][0] = 0.0f;
    icon.LightDir[0][1] = 0.5f;
    icon.LightDir[0][2] = -0.5f;
    icon.LightDir[1][0] = 0.5f;
    icon.LightDir[1][1] = -0.4f;
    icon.LightDir[1][2] = 0.1f;
    icon.LightDir[2][0] = -0.5f;
    icon.LightDir[2][1] = 0.5f;
    icon.LightDir[2][2] = 0.5f;
    icon.LightColor[0][0] = 0.14f;
    icon.LightColor[0][1] = 0.03f;
    icon.LightColor[0][2] = 0.48f;
    icon.LightColor[1][0] = 0.33f;
    icon.LightColor[1][1] = 0.0f;
    icon.LightColor[1][2] = 0.33f;
    icon.LightColor[2][0] = 0.14f;
    icon.LightColor[2][1] = 0.14f;
    icon.LightColor[2][2] = 0.38f;
    icon.Ambient[0] = 0.5f;
    icon.Ambient[1] = 0.5f;
    icon.Ambient[2] = 0.5f;
    ConvertStringToSjis(title, icon.TitleName);
    strcpy((char *)icon.FnameView, "memcard_icon.ico");
    strcpy((char *)icon.FnameCopy, "memcard_icon.ico");
    strcpy((char *)icon.FnameDel, "memcard_icon.ico");
    sprintf(path, "%s/%s", dir, "icon.sys");
    return McWriteFile(saveManager, path, &icon, sizeof(icon));
}
} } // namespace M_WriteIconSys

/** @sly2-annot addr=0x1AC238
 * Purpose: existing: load memcard_icon.ico from disc and write to card
 * Info: proposed name load_sly_icon_from_disc (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.70] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/formats/disc-records.md#3-static-records-in-the-elf,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Loads the save icon (memcard_icon.ico) from the disc and writes it to the card.
extern "C" void load_sly_icon_from_disc(int n0, int n1)
{
    load_file_from_disc(n0, n1, D_002A38E8, (char *)"memcard_icon.ico");
}

namespace M_load_file_from_disc {
typedef unsigned int uint;
extern "C" void STACKSCOPE_ctor_1AE4A0(STACKSCOPE*,int) __asm__("_$_10STACKSCOPE");
struct REQUEST {uint bytes,alignment;};
struct X;
extern "C" void InitFileKey(X *px);
struct X { uint sector; uint size; int mode; char pad[0x110 - 0xC]; X() { mode = 2; InitFileKey(this); } };   /* a disc file location (FILEKEY) */
struct Y;extern "C" void DestroyCStreamBis(Y*,int) __asm__("_$_10CStreamBis");
struct Y {int flags;unsigned char*cursor;int left,consumed;unsigned char*raw;int rawBytes,decompFlags,pending,word20;unsigned char*window;int word28,word2C;void*vt;int capacity;unsigned char*buffer;int handle3C,word40,word44;uint word48;int word4C,word50,word54,word58,word5C,word60;char tail64[0xC];Y(int cb,void*pv,int flags) __asm__("CStreamBis_ctor");~Y(){DestroyCStreamBis(this,2);}};
struct K {uint sectorKey,sizeKey;char pad[0x34-8];uint sectorXor;int pad2;uint sizeXor;};   /* a disc file entry: obfuscated sector / size */
extern "C" {
int sprintf(char *buf, const char *fmt, ...);
int McOpen(void *saveManager, char *path, int mode, int *fd);
void *AllocFromStack(REQUEST *pmsg);
int OpenStreamFromFileKey(Y *py, X *px);
int McWrite(void *saveManager, int fd, char *buf, int size, int *written);
int CbRead__18CBinaryInputStreamiPv(Y *py, int cb, char *buf);
void CStreamBis_Close(Y *py);
int McClose(void *saveManager, int fd);
int load_file_from_disc(void *saveManager, const char *dir, K *entry, const char *name);
}
static inline int SetKey(X *location, K *entry)
{
    if (!entry)
        return 0;
    uint size;
    uint sectorXor = entry->sectorXor;
    uint sizeXor = entry->sizeXor;
    location->mode = 2;
    location->sector = (entry->sectorKey ^ sectorXor) + 0xCA0;
    size = entry->sizeKey ^ sizeXor;
    location->size = size;
    return size != 0;
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char X_sector_Offset[FIELD_OFFSET(X,sector)==0?1:-1];
typedef char X_size_Offset[FIELD_OFFSET(X,size)==4?1:-1];
typedef char X_mode_Offset[FIELD_OFFSET(X,mode)==8?1:-1];
typedef char K_sectorKey_Offset[FIELD_OFFSET(K,sectorKey)==0?1:-1];
typedef char K_sizeKey_Offset[FIELD_OFFSET(K,sizeKey)==4?1:-1];
typedef char K_sectorXor_Offset[FIELD_OFFSET(K,sectorXor)==52?1:-1];
typedef char K_sizeXor_Offset[FIELD_OFFSET(K,sizeXor)==60?1:-1];
typedef char Y_flags_Offset[FIELD_OFFSET(Y,flags)==0?1:-1];
typedef char Y_cursor_Offset[FIELD_OFFSET(Y,cursor)==4?1:-1];
typedef char Y_left_Offset[FIELD_OFFSET(Y,left)==8?1:-1];
typedef char Y_consumed_Offset[FIELD_OFFSET(Y,consumed)==12?1:-1];
typedef char Y_raw_Offset[FIELD_OFFSET(Y,raw)==16?1:-1];
typedef char Y_rawBytes_Offset[FIELD_OFFSET(Y,rawBytes)==20?1:-1];
typedef char Y_decompFlags_Offset[FIELD_OFFSET(Y,decompFlags)==24?1:-1];
typedef char Y_pending_Offset[FIELD_OFFSET(Y,pending)==28?1:-1];
typedef char Y_word20_Offset[FIELD_OFFSET(Y,word20)==32?1:-1];
typedef char Y_window_Offset[FIELD_OFFSET(Y,window)==36?1:-1];
typedef char Y_word28_Offset[FIELD_OFFSET(Y,word28)==40?1:-1];
typedef char Y_word2C_Offset[FIELD_OFFSET(Y,word2C)==44?1:-1];
typedef char Y_vt_Offset[FIELD_OFFSET(Y,vt)==48?1:-1];
typedef char Y_capacity_Offset[FIELD_OFFSET(Y,capacity)==52?1:-1];
typedef char Y_buffer_Offset[FIELD_OFFSET(Y,buffer)==56?1:-1];
typedef char Y_handle3C_Offset[FIELD_OFFSET(Y,handle3C)==60?1:-1];
typedef char Y_word40_Offset[FIELD_OFFSET(Y,word40)==64?1:-1];
typedef char Y_word44_Offset[FIELD_OFFSET(Y,word44)==68?1:-1];
typedef char Y_word48_Offset[FIELD_OFFSET(Y,word48)==72?1:-1];
typedef char Y_word4C_Offset[FIELD_OFFSET(Y,word4C)==76?1:-1];
typedef char Y_word50_Offset[FIELD_OFFSET(Y,word50)==80?1:-1];
typedef char Y_word54_Offset[FIELD_OFFSET(Y,word54)==84?1:-1];
typedef char Y_word58_Offset[FIELD_OFFSET(Y,word58)==88?1:-1];
typedef char Y_word5C_Offset[FIELD_OFFSET(Y,word5C)==92?1:-1];
typedef char Y_word60_Offset[FIELD_OFFSET(Y,word60)==96?1:-1];
typedef char Y_tail64_Offset[FIELD_OFFSET(Y,tail64)==100?1:-1];
typedef char Size_0[sizeof(STACKSCOPE)==1?1:-1];
typedef char Size_1[sizeof(REQUEST)==8?1:-1];
typedef char Size_2[sizeof(X)==272?1:-1];
typedef char Size_3[sizeof(K)==64?1:-1];
typedef char Size_4[sizeof(Y)==112?1:-1];
typedef char Size_5[sizeof(uint)==4?1:-1];
typedef char Size_6[sizeof(int)==4?1:-1];
typedef char Size_7[sizeof(void*)==4?1:-1];
/** @sly2-annot addr=0x1AC260
 * Purpose: Open disc file '%s/%s' via BIS, read into alloc, write to card with sceMcOpen/Write/Close
 * Info: proposed name load_file_from_disc (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Reads the disc file "<dir>/<name>" into memory and writes it to the card (sceMcOpen/Write/Close).
int load_file_from_disc(void *saveManager, const char *dir, K *entry, const char *fileName)
{
    STACKSCOPE frame;
    char path[0x50];
    int ret;
    sprintf(path, "%s/%s", dir, fileName);
    int fd = -1;
    ret = McOpen(saveManager, path, 0x202, &fd);
    if (ret == 0) {
        X location;
        REQUEST request;
        REQUEST *prequest = &request;
        prequest->bytes = 0x403F;
        prequest->alignment = 0x10;
        void *streamBuffer = AllocFromStack(prequest);
        Y stream(0x403F, streamBuffer, 0);
        if (!SetKey(&location, entry) || !OpenStreamFromFileKey(&stream, &location))
            ret = 0x27;
        else {
            char buf[0x1000];
            int numRead;
            int numWritten;
            while ((numRead = CbRead__18CBinaryInputStreamiPv(&stream, 0x1000, buf)) != 0) {
                ret = McWrite(saveManager, fd, buf, numRead, &numWritten);
                if (ret)
                    goto done;
                if (numWritten != numRead) {
                    ret = 7;
                    goto done;
                }
            }
            ret = 0;
        }
    done:
        CStreamBis_Close(&stream);
        int closeResult = McClose(saveManager, fd);
        if (!ret)
            ret = closeResult;
    }
    return ret;
}
} // namespace M_load_file_from_disc
#undef FIELD_OFFSET

namespace M_WriteSaveSlotFile { extern "C" {
struct B { int a[20]; };   /* the file path buffer (0x50 bytes) */
void BuildSaveFilePath(void *saveManager, void *slot, struct B *path);
void McWriteFile(void *saveManager, struct B *path, void *data, int size);
/** @sly2-annot addr=0x1AC420
 * Purpose: BuildSaveFilePath + McWriteFile
 * Info: proposed name WriteSaveSlotFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteSaveSlotFile for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> WriteSaveSlotFile
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.40] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Writes one slot's file (BuildSaveFilePath + McWriteFile).
void WriteSaveSlotFile(void *saveManager, void *slot)
{
    struct B path;
    BuildSaveFilePath(saveManager, slot, &path);
    McWriteFile(saveManager, &path, slot, SAVE_SLOT_FILE_SIZE);
}
} } // namespace M_WriteSaveSlotFile

namespace M_WriteSaveSlotFileReset { extern "C" {
struct B { char pad[0x5408]; char infoName[0x44]; int infoSize; int infoAttributes; };   /* a slot; its SAVEINFO at +0x5408 */
void ResetSaveGs(struct B *p, int n);
void BuildSaveFilePath(void *saveManager, void *slot, char *path);
int McWriteFile(void *saveManager, char *path, void *data, int size);
/** @sly2-annot addr=0x1AC468
 * Purpose: ResetGs then BuildSaveFilePath + McWriteFile
 * Info: proposed name WriteSaveSlotFileReset (EXISTING); TU memcard
 * Notes: agent/verifier proposes WriteSaveSlotFileReset for this existing name (see
 *   existing_name_changes.csv) | arbitration: draft replacement (not re-derived) | RENAME proposed ->
 *   WriteSaveSlotFileReset (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Resets the GS state, then writes one slot's file.
int WriteSaveSlotFileReset(void *saveManager, struct B *slot)
{
    char path[0x50];
    ResetSaveGs(slot, 0);
    BuildSaveFilePath(saveManager, slot, path);
    return McWriteFile(saveManager, path, slot, SAVE_SLOT_FILE_SIZE);
}
} } // namespace M_WriteSaveSlotFileReset

namespace M_func_001AC4C8 { extern "C" {
struct B { char pad[0x5408]; char infoName[0x44]; int infoSize; int infoAttributes; };   /* a slot; its SAVEINFO at +0x5408 */
void memset_(void *p, int c, int n) __asm__("memset");
void BuildSaveFilePath(void *saveManager, void *slot, char *path);
int ReadMcDirWrapper(void *saveManager, char *path, void *record);
int McReadFile(void *saveManager, char *path, void *data, int size);
/** @sly2-annot addr=0x1AC4C8
 * Purpose: For each slot: BuildSaveFilePath, read dir entry, McReadFile
 * Info: proposed name ReadSaveSlotHeaders (T2); TU memcard
 * Notes: names_v2 v1 proposed McWriteFile (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Reads each slot's directory entry and file header from the card.
int ReadSaveSlotHeaders(void *saveManager, struct B *slot)
{
    char path[0x50];
    int r;
    memset_(slot, 0, SAVE_SLOT_SIZE);
    BuildSaveFilePath(saveManager, slot, path);
    r = ReadMcDirWrapper(saveManager, path, slot->infoName);
    if (r)
        return r;
    if ((slot->infoAttributes & 0xD4) != 4)     /* a plain readable file (FillMcDirInfo bits) */
        return 0x14;
    if (slot->infoSize != SAVE_SLOT_FILE_SIZE)
        return 0x1C;
    return McReadFile(saveManager, path, slot, SAVE_SLOT_FILE_SIZE);
}
} } // namespace M_func_001AC4C8

/** @sly2-annot addr=0x1AC570
 * Purpose: Thin wrapper over ResetGsIfSchemaChanged for a slot
 * Info: proposed name ResetSaveSlot (T2); TU memcard
 * Notes: names_v2 C proposed Clear__13CFileLocation (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Resets a slot when its schema changed (ResetGsIfSchemaChanged).
extern "C" int ResetSaveSlot(SAVE *saveManager, void *pslot)
{
    ResetGsIfSchemaChanged(&(*(int *)&g_sw.savevars));
    return 0;
}

namespace M_McJobGetInfo { extern "C" {
struct S { int port; int slot; int state; int nextState; int tStateChanged; float tNextPoll; int cardType; int freeClusters; int isFormatted; char pad2[0xFD5C - 0x24]; char memcard[1]; };
int sceMcGetInfo(int port, int slot, int *type, int *free, int *format);
int McSyncResult(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1AC598
 * Purpose: Thread job: sceMcGetInfo + sync -> card type/free/format state
 * Info: proposed name McJobGetInfo (T1); TU memcard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Worker job: reads the card info (type, free space, formatted) and records it.
int McJobGetInfo(struct S *saveManager)
{
    void *worker = saveManager->memcard;
    int syncState;
    int r;
    syncState = sceMcGetInfo(saveManager->port, saveManager->slot, &saveManager->cardType, &saveManager->freeClusters, &saveManager->isFormatted);
    r = McSyncResult(worker, syncState, 1, 0);
    switch (r) {
    case 11:
        saveManager->nextState = 1;
        break;
    case 9:
        saveManager->nextState = 3;
        break;
    case 10:
    case 12:
        r = 12;
        saveManager->nextState = 2;
        break;
    case 0:
        if (saveManager->isFormatted) { if (saveManager->nextState < 3) saveManager->nextState = 3; } else saveManager->nextState = 2;
        break;
    }
    if (r == 0 || r == 12) {
        if (saveManager->cardType != 2) {
            r = 11;
            saveManager->nextState = 1;
        }
    }
    saveManager->tNextPoll = g_clock.realTime + 0.3f;
    return r;
}
} } // namespace M_McJobGetInfo

namespace M_func_001AC6A0 { extern "C" {
struct S { int port; int slot; int pad[4]; int cardType; int freeClusters; int isFormatted; char pad2[0xFD5C - 0x24]; char memcard[1]; };
int sceMcGetInfo(int port, int slot, int *type, int *free, int *format);
int McSyncResult(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1AC6A0
 * Purpose: sceMcGetInfo + McSyncResult
 * Info: proposed name McGetInfoSync (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcGetInfo followed by McSyncResult; returns the result.
int McGetInfoSync(struct S *saveManager, int *freeBytes)
{
    void *worker = saveManager->memcard;
    int syncState;
    int r;
    syncState = sceMcGetInfo(saveManager->port, saveManager->slot, &saveManager->cardType, &saveManager->freeClusters, &saveManager->isFormatted);
    r = McSyncResult(worker, syncState, 1, 0);
    switch (r) {
    case 9:
        r = 0;
        break;
    case 10:
        r = 12;
        break;
    }
    if (r == 0)
        *freeBytes = saveManager->freeClusters << 10;
    else
        *freeBytes = 0;
    return r;
}
} } // namespace M_func_001AC6A0

/** @sly2-annot addr=0x1AC748
 * Purpose: sceMcMkdir wrapper + sync
 * Info: proposed name McMkdir (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcMkdir and wait for the result.
extern "C" int McMkdir(SAVE *saveManager, const char *pchz)
{
    int nResult = McSyncResult(&saveManager->memcard, sceMcMkdir(saveManager->port, saveManager->slot, pchz), 0xB, 0);
    return ((nResult ^ 0x18) == 0) ? 0 : nResult;
}

/** @sly2-annot addr=0x1AC7A0
 * Purpose: sceMcOpen wrapper + sync
 * Info: proposed name McOpen (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcOpen and wait for the result.
extern "C" void McOpen(SAVE *saveManager, const char *pchz, int mode, int n)
{
    McSyncResult(&saveManager->memcard, sceMcOpen(saveManager->port, saveManager->slot, pchz, mode), 2, n);
}

/** @sly2-annot addr=0x1AC808
 * Purpose: sceMcClose wrapper + sync
 * Info: proposed name McClose (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcClose and wait for the result.
extern "C" void McClose(SAVE *saveManager, int fd)
{
    McSyncResult(&saveManager->memcard, sceMcClose(fd), 3, 0);
}

// 0x1AC848: dead-code scrap (sly2/scrap.h)
SLY2_SCRAP("addiu $sp, $sp, 0x30");

namespace M_McRead { extern "C" {
int sceMcRead(int fd, int buffer, int size);
void McSyncResult(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char memcard[4]; };
/** @sly2-annot addr=0x1AC850
 * Purpose: sceMcRead + McSyncResult
 * Info: proposed name McRead (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcRead and wait for the result.
void McRead(struct S *saveManager, int fd, int buffer, int size, int numRead)
{
    McSyncResult(saveManager->memcard, sceMcRead(fd, buffer, size), 5, numRead);
}
} } // namespace M_McRead

namespace M_McWrite { extern "C" {
int sceMcWrite(int fd, int buffer, int size);
void McSyncResult(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char memcard[4]; };
/** @sly2-annot addr=0x1AC8A8
 * Purpose: sceMcWrite + McSyncResult
 * Info: proposed name McWrite (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcWrite and wait for the result.
void McWrite(struct S *saveManager, int fd, int buffer, int size, int numWritten)
{
    McSyncResult(saveManager->memcard, sceMcWrite(fd, buffer, size), 6, numWritten);
}
} } // namespace M_McWrite

namespace M_McWriteFile { extern "C" {
int McOpen(void *saveManager, void *path, int mode, int *fd);
int McWrite(void *saveManager, int fd, void *data, int size, int *numWritten);
void McClose(void *saveManager, int fd);
/** @sly2-annot addr=0x1AC900
 * Purpose: McOpen(create) + McWrite + McClose
 * Info: proposed name McWriteFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes McWriteFile for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McWriteFile (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Writes a whole file: open (create), write, close.
int McWriteFile(void *saveManager, void *path, void *data, int size)
{
    int fd = -1;
    int numWritten;
    int r = McOpen(saveManager, path, 0x202, &fd);   /* create | write */
    if (r)
        return r;
    r = McWrite(saveManager, fd, data, size, &numWritten);
    if (!r && numWritten != size)
        r = 7;
    McClose(saveManager, fd);
    return r;
}
} } // namespace M_McWriteFile

namespace M_McReadFile { extern "C" {
int McOpen(void *saveManager, void *path, int mode, int *fd);
int McRead(void *saveManager, int fd, void *data, int size, int *numRead);
void McClose(void *saveManager, int fd);
/** @sly2-annot addr=0x1AC9A0
 * Purpose: McOpen + McRead + McClose
 * Info: proposed name McReadFile (T1); TU memcard
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Reads a whole file: open, read, close.
int McReadFile(void *saveManager, void *path, void *data, int size)
{
    int fd = -1;
    int numRead;
    int r = McOpen(saveManager, path, 1, &fd);   /* read */
    if (r)
        return r;
    r = McRead(saveManager, fd, data, size, &numRead);
    if (!r && numRead != size)
        r = 8;
    McClose(saveManager, fd);
    return r;
}
} } // namespace M_McReadFile

/** @sly2-annot addr=0x1ACA40
 * Purpose: sceMcDelete wrapper
 * Info: proposed name McDelete (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// sceMcDelete and wait for the result.
extern "C" int McDelete(SAVE *saveManager, const char *pchz)
{
    int nResult = McSyncResult(&saveManager->memcard, sceMcDelete(saveManager->port, saveManager->slot, pchz), 0xF, 0);
    return ((nResult ^ 0xE) == 0) ? 0 : nResult;
}

/** @sly2-annot addr=0x1ACA98
 * Purpose: ctor: UI base ctor 0x1DA728, vptr 0x2D3750, id 0x52C, state x40=-1, register 0x1DA7D0
 * Info: proposed name CMemcardDialog_ctor (T1); class CMemcardDialog; TU memcard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMemcardDialog constructor: UI base, vtable, text id 0x52C, no state.
extern "C" MCDLG *CMemcardDialog_ctor(MCDLG *dialog)
{
    ConstructScreen(dialog);
    dialog->vtable = D_002D3750;
    dialog->id = 0x52C;
    dialog->nState = -1;
    ResetScreenMode(dialog);
    return dialog;
}

/** @sly2-annot addr=0x1ACAE8
 * Purpose: UI slot 12: base 0x1DA770 then SetMcDialogState
 * Info: proposed name CMemcardDialog_vt12 (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMemcardDialog slot 12 (post-load): base version, then re-applies the current dialog state.
/* CMemcardDialog slot 12. */
void CMemcardDialog::PostLoad()   /* key function: CMemcardDialog's vtable comes out here */
{
    MCDLG *dialog = (MCDLG *)this;
    MCCHILD *pchild;
    PostLoadScreen(dialog);
    pchild = dialog->pchild;
    if (pchild != 0) {
        pchild->unknownB0 = 1;
        if (dialog->nState != -1) {
            SetMcDialogState(dialog, dialog->nState);
        }
    }
}

/** @sly2-annot addr=0x1ACB38
 * Purpose: UI slot 13: forwards to 0x1DA7B0
 * Info: proposed name CMemcardDialog_vt13 (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMemcardDialog slot 13: forwards to the UI base (0x1DA7B0).
/* CMemcardDialog slot 13. */
extern "C" void CMemcardDialog_Slot13(MCDLG *dialog)
{
    ResetScreen(dialog);
}

namespace M_SetMcDialogState { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x14]; struct O *child; char pad3[0x18]; int state; };
/** @sly2-annot addr=0x1ACB58
 * Purpose: x40 = state; if shown (x24) call vt10 and child vt49(-1)
 * Info: proposed name SetMcDialogState (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions, docs/engine/save-memcard.md#open-questions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Sets the dialog state; when the dialog is shown, refreshes it (slot 10) and shows its child widget.
void SetMcDialogState(struct O *dialog, int state)
{
    dialog->state = state;
    if (dialog->child) {
        { struct VTE *e = &dialog->vt[10]; e->pfn((char *)dialog + e->d); }
        { struct O *child = dialog->child; struct VTE *e = &child->vt[49]; e->pfn((char *)child + e->d, -1); }
    }
}
} } // namespace M_SetMcDialogState

namespace M_ClearMcDialogState { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x14]; struct O *child; char pad3[0x18]; int state; };
/** @sly2-annot addr=0x1ACBB8
 * Purpose: x40=-1; vt11 and child vt50
 * Info: proposed name ClearMcDialogState (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Clears the dialog state and hides the dialog (slot 11) and its child widget.
void ClearMcDialogState(struct O *dialog)
{
    dialog->state = -1;
    if (dialog->child) {
        { struct VTE *e = &dialog->vt[11]; e->pfn((char *)dialog + e->d); }
        { struct O *child = dialog->child; struct VTE *e = &child->vt[50]; e->pfn((char *)child + e->d); }
    }
}
} } // namespace M_ClearMcDialogState

/** @sly2-annot addr=0x1ACC18
 * Purpose: vt14: x28=mode, x2C=time if changed
 * Info: proposed name CMemcardDialog_SetMode (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMemcardDialog slot 14: sets the mode, stamping the real time when it changes.
/* slot 14: set the mode (stamping the real time). */
extern "C" void CMemcardDialog_SetMode(MCDLG *dialog, int nMode)
{
    if (nMode != dialog->nMode) {
        dialog->nMode = nMode;
        dialog->tMode = D_002DDED0.realTime;
    }
}

namespace M_func_001ACC38 { extern "C" {
struct H { char pad[0xFD50]; int currentSlot; };
extern struct H D_002E65E0;
struct CLOCK { float t; float dt; char pad[0x34]; float realTime; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x44]; int phase; float tPhase; };
/** @sly2-annot addr=0x1ACC38
 * Purpose: Memcard dialog (D_002E6550): set phase +0x44 and its start time +0x48 (g_clock t3C); phases 2/3
 *   clear g_save+0xFD50
 * Info: proposed name SetMcDialogPhase (T2); TU memcard
 * Notes: v3: superseded SetMcUiPhase | top50_round (0.45): Body. Neighbours SetMcDialogState 0x1ACB58
 *   (+0x40), CMemcardDialog_SetMode (+0x28). All callers pass D_002E6550 (memcard job/dialog code: McJob*,
 *   OnMcDialogChoice, RunNextBootStep, UpdateTitleScreen). Phase meaning unkno
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r4_17F090+top50_round
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Sets the dialog phase and its start time; phases 2 and 3 forget the current slot.
void SetMcDialogPhase(struct S *dialog, int phase)
{
    if (phase == dialog->phase) return;
    switch (phase) { case 2: case 3: D_002E65E0.currentSlot = 0; }
    dialog->phase = phase;
    dialog->tPhase = D_002DDED0.realTime;
}
} } // namespace M_func_001ACC38
extern SAVE D_002E65E0;

namespace M_func_001ACC80 { extern "C" {
struct S { char pad[0x40]; int state; };
/** @sly2-annot addr=0x1ACC80
 * Purpose: State -> 1 (one-button) / 2 (two-choice) / 0
 * Info: proposed name KMcDialogButtons (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Returns the button layout of the dialog state: 1 one button, 2 two choices, 0 none.
int GetMcDialogButtonKind(struct S *dialog)
{
    switch (dialog->state) {
    case 0x11: case 0x14: case 0x17: case 0x18: case 0x1B: case 0x1D: case 0x22: case 0x23: case 0x24: case 0x2E:
        return 1;
    case 0x12: case 0x15: case 0x1C: case 0x20:
        return 2;
    }
    return 0;
}
} } // namespace M_func_001ACC80

namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_McJobCreateSave() __asm__("McJobCreateSave");
extern const unsigned int r_D_002A6D88[2] __asm__("D_002A6D88") = {0xFFFF0000, (unsigned int)p_McJobCreateSave};
}
namespace M_CMemcardDialog_Update { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short index; void (*pfn)(void *); };
struct P { char pad[0x58]; int displayState; float tDisplayState; int pad2; int targetDisplayState; };   /* the dialog's child widget (2: fully shown) */
struct S { char pad[0x10]; void *joypad; char pad2[0x24 - 0x14]; struct P *child; char pad3[0x40 - 0x28]; int state; int phase; int pad4; int followUpAction; };
struct MC { char pad[8]; int state; char pad2[0xFD58 - 0xC]; int selectedSlot; char pad3[0xFD6C - 0xFD5C]; int jobResult; };   /* SAVE; jobResult: memcard.jobResult */
struct CLOCK { char pad[0x3C]; float realTime; };
extern struct MC D_002E65E0;
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern char D_003D9ED0[];
extern char D_002F633C[] __asm__("D_002E65E0+0xFD5C");
extern const struct LONGU D_002A6D88;
int IsSwTransitionBusy(void *g);
int GetMcDialogButtonKind(struct S *s);
int HandleJoyButtons(void *p, int n);
void ConfirmMcDialog(struct S *s);
void OnMcDialogChoice(struct S *s);
int IsMemcardThreadValid__FP7MEMCARD(void *p);
void CreateMemcardThread(void *p, long pmf);
void SetMcDialogState(struct S *s, int n);
void ClearMcDialogState(struct S *s);
void ResetGameSequence(void);
void PopPauseMenuToRoot(void *p);
/** @sly2-annot addr=0x1ACCC8
 * Purpose: vt7: when dialog fully shown >0.1s handle pad input (0x187D80) per KMcDialogButtons, start jobs
 *   (CreateMemcardThread), transition (0x1ACFE0)
 * Info: proposed name CMemcardDialog_Update (T1); class CMemcardDialog; TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMemcardDialog's per-frame update (slot 7): once the dialog has been shown for 0.1 s, handles the pad per the button
// layout, starts the card jobs and moves to the follow-up state.
void CMemcardDialog_Update(struct S *dialog)
{
    struct P *child = dialog->child;
    int c;
    int buttonKind;
    if (!child)
        return;
    if (child->displayState != 2)
        return;
    c = child->targetDisplayState;
    if (c != 2)
        return;
    if (IsSwTransitionBusy(D_003DD490))
        return;
    if (D_002DDED0.realTime - dialog->child->tDisplayState <= 0.1f)
        return;
    buttonKind = GetMcDialogButtonKind(dialog);
    if (buttonKind != 1) {
        if (buttonKind == c) {
            if (dialog->joypad && HandleJoyButtons(dialog->joypad, 0x80)) {
                ConfirmMcDialog(dialog);
                return;
            }
            if (dialog->joypad && HandleJoyButtons(dialog->joypad, 0x10)) {
                OnMcDialogChoice(dialog);
                return;
            }
        }
    } else {
        if (dialog->joypad && HandleJoyButtons(dialog->joypad, 0x80)) {
            OnMcDialogChoice(dialog);
            return;
        }
    }
    switch (dialog->state) {
    case 0x20:
    case 0x22:
        if (dialog->phase == 1)
            return;
        ClearMcDialogState(dialog);
        return;
    case 0x11:
    case 0x12:
    case 0x15:
    case 0x18:
    case 0x2E:
        if (D_002E65E0.state != 6)
            return;
        ClearMcDialogState(dialog);
        return;
    case 0x13:
        if (IsMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.jobResult) {
            CreateMemcardThread(D_002F633C, D_002A6D88.l);
            SetMcDialogState(dialog, 0x16);
        } else
            SetMcDialogState(dialog, 0x14);
        return;
    case 0x16:
        if (IsMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.jobResult) {
            ClearMcDialogState(dialog);
            return;
        }
        SetMcDialogState(dialog, 0x17);
        return;
    case 0x1A:
        if (IsMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.jobResult) {
            ClearMcDialogState(dialog);
            switch (dialog->followUpAction) {
            case 1:
                ResetGameSequence();
                return;
            case 2:
                PopPauseMenuToRoot(D_003D9ED0);
                return;
            }
            return;
        }
        if (dialog->followUpAction == 3)
            SetMcDialogState(dialog, 0x23);
        else
            SetMcDialogState(dialog, 0x1D);
        return;
    case 0x21:
        if (IsMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        D_002E65E0.selectedSlot = 0;
        if (!D_002E65E0.jobResult) {
            ClearMcDialogState(dialog);
            return;
        }
        SetMcDialogState(dialog, 0x1D);
        return;
    }
}
} } // namespace M_CMemcardDialog_Update
extern SAVE D_002E65E0;

namespace M_func_001ACFE0 {
// Game-state transition handler: switch on the new state id (state) and run the menu/HUD hooks for it.
// State 34 sends an option message (CRef true) to the global option object D_003E0FE4.
//
// Match notes: the three-argument call to SetGlobalSaveVar uses the same idiom as SetBootyContainerKind / ResetRumbleSaveVar:
// SetPv receives the context at the call site (evaluated before the CRef copy) but its body names
// D_003D4A60 again after the copy. That is what puts `lui a3` after the copy ctor yet keeps a3's low half
// ahead of the a2 setup (the scheduler boosts insns that set a register exactly once in the function).
class CRef;
extern "C" void Decref__4CRef(CRef *pref);                       // ~CRef
extern "C" CRef *__4CRefRC4CRef(CRef *, const CRef *);            // CRef copy ctor, returns this
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &ref) { __4CRefRC4CRef(this, &ref); }
    ~CRef() { Decref__4CRef(this); }
};
class O;
struct S { char pad[0x40]; int state; int phase; float tPhase; int followUpAction; };   // the memcard dialog
extern "C" {
extern char D_003DA530[], D_003D9ED0[], D_003D4A60[];
extern char D_002E65E0[];

int IsScreenShown(void *p);
int IsPauseMenuOnLoadPageA(void *p);
void ClearMcDialogState(S *s);
void SetMcDialogPhase(S *s, int n);
void PopPauseMenuToRoot(void *p);
void StartNewGame(void *p);
int IsPauseMenuOnSavePage(void *p);
void PopPauseMenuPage(void *p);
void SetMcDialogState(S *s, int n);
void StartSaveSlotJob(void *p);
void OnMcDialogChoice(S *s);
CRef SetGlobalSaveVar(O *, const CRef &, void *);                   // O::Set(const CRef &, context), returns a CRef
}
static inline void SetPv(O *po, const CRef &r, void *) { CRef arg(r); const CRef &ret = SetGlobalSaveVar(po, arg, D_003D4A60); }
/** @sly2-annot addr=0x1ACFE0
 * Purpose: Switch on dialog state x40 and run follow-up (UI hooks, SAVE jobs, 0x1ABD40)
 * Info: proposed name OnMcDialogChoice (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Runs the follow-up of the dialog state the player answered (UI hooks, save jobs).
void OnMcDialogChoice(S *dialog)
{
    switch (dialog->state) {
    case 17: case 18: case 21: case 24:
        {
            void *titleScreen = D_003DA530;
            if (IsScreenShown(titleScreen) && IsPauseMenuOnLoadPageA(D_003D9ED0)) {
                ClearMcDialogState(dialog);
                SetMcDialogPhase(dialog, 2);
                PopPauseMenuToRoot(D_003D9ED0);
                StartNewGame(titleScreen);
                break;
            }
        }
        /* fallthrough */
    case 20: case 23: case 29: case 35: case 36: case 46:
        ClearMcDialogState(dialog);
        SetMcDialogPhase(dialog, 2);
        if (IsPauseMenuOnSavePage(D_003D9ED0))
            PopPauseMenuPage(D_003D9ED0);
        break;
    case 27: case 28: case 32:
        ClearMcDialogState(dialog);
        break;
    case 34:
        {
            O *po = (*(O * *)&g_sw.psvpAutosaveWarned);
            CRef ref(true);
            SetPv(po, ref, D_003D4A60);
        }
        if (dialog->followUpAction < 4) {
            if (dialog->followUpAction > 0) {
                SetMcDialogState(dialog, 0x1A);
                StartSaveSlotJob(D_002E65E0);
            }
        }
        break;
    }
}
} // namespace M_func_001ACFE0
extern SAVE D_002E65E0;

namespace M_func_001AD168 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { char pad[0x40]; int state; };
extern const struct LONGU D_002A6E88, D_002A6D88, D_002A6E90;
extern char D_002F633C[] __asm__("D_002E65E0+0xFD5C");
void SaveToCurrentSlot(void);
void CreateMemcardThread(void *t, long pmf);
void SetMcDialogState(struct S *s, int n);
/** @sly2-annot addr=0x1AD168
 * Purpose: Confirm path: 0x1AD3E0 or CreateMemcardThread
 * Info: proposed name McDialogConfirm (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Confirm path of the dialog: saves to the current slot or queues the pending job.
void ConfirmMcDialog(struct S *dialog)
{
    switch (dialog->state) {
    case 0x1C:
        SaveToCurrentSlot();
        break;
    case 0x12:
        CreateMemcardThread(D_002F633C, D_002A6E88.l);
        SetMcDialogState(dialog, 0x13);
        break;
    case 0x15:
        CreateMemcardThread(D_002F633C, D_002A6D88.l);
        SetMcDialogState(dialog, 0x16);
        break;
    case 0x20:
        CreateMemcardThread(D_002F633C, D_002A6E90.l);
        SetMcDialogState(dialog, 0x21);
        break;
    }
}
} } // namespace M_func_001AD168

namespace M_func_001AD250 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B slots[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *currentSlot; int newestSlot; struct B *selectedSlot; };
struct S { int x0, x4, slotIndex; char pad[0x10]; int flags; };   /* the menu item (flags bit 1: handled, inferred) */
extern struct G D_002E65E0;
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void ResetSaveGs(struct B *b, int n);
void StartGameFromSave(struct B *b);
void *CreateSfxSound(int n);
void SetSndSourceLo(void *p, int n);
/** @sly2-annot addr=0x1AD250
 * Purpose: Menu action slot i: if slot empty (Splice eval) make it current and start (0x1D8D80, 0x165E38)
 *   else error sound 0x36
 * Info: proposed name SelectSaveSlotNew (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Menu action for slot i on the new-game page: when the slot is empty, makes it current and starts a new game;
// otherwise plays the error sound.
void StartNewGameInSlot(struct S *item)
{
    struct CRef ref;
    struct B *slot = &D_002E65E0.slots[item->slotIndex];
    float dt;
    GetGlobalSaveVar(&ref, (*(int *)&g_sw.psvpDt), slot);
    dt = ref.m_tag;
    Decref__4CRef(&ref);
    if (dt == 0.0f) {
        D_002E65E0.currentSlot = slot;
        ResetSaveGs(slot, 1);
        StartGameFromSave(D_002E65E0.currentSlot);
    } else {
        SetSndSourceLo(CreateSfxSound(0x36), 0);
        if (item)
            item->flags |= 2;
    }
}
} } // namespace M_func_001AD250
extern SAVE D_002E65E0;

namespace M_func_001AD320 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B slots[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *currentSlot; int newestSlot; struct B *selectedSlot; };
struct S { int x0, x4, slotIndex; char pad[0x10]; int flags; };   /* the menu item (flags bit 1: handled, inferred) */
extern struct G D_002E65E0;
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void ResetSaveGs(struct B *b, int n);
void StartGameFromSave(struct B *b);
/** @sly2-annot addr=0x1AD320
 * Purpose: Menu action slot i: make current, ResetGs, start (0x165E38)
 * Info: proposed name SelectSaveSlotLoad (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Menu action for slot i on the load page: makes it current, resets the GS state and starts the game from it.
void LoadGameFromSlot(struct S *item)
{
    struct CRef ref;
    struct B *slot = &D_002E65E0.slots[item->slotIndex];
    float dt;
    GetGlobalSaveVar(&ref, (*(int *)&g_sw.psvpDt), slot);
    dt = ref.m_tag;
    Decref__4CRef(&ref);
    D_002E65E0.currentSlot = slot;
    if (dt == 0.0f) {
        ResetSaveGs(slot, 1);
        StartGameFromSave(D_002E65E0.currentSlot);
    } else {
        StartGameFromSave(slot);
        if (item)
            item->flags |= 2;
    }
}
} } // namespace M_func_001AD320
extern SAVE D_002E65E0;

namespace M_func_001AD3E0 { extern "C" {
struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0xFD50]; int currentSlot; int newestSlot; int selectedSlot; };
struct H { char pad[0x4C]; int followUpAction; };
extern struct G D_002E65E0;
extern struct H D_002E6550;
extern char D_003D4A60[];
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void StartSaveSlotJob(struct G *g);
/** @sly2-annot addr=0x1AD3E0
 * Purpose: Eval slot, set dialog state, start save job (0x1ABD40)
 * Info: proposed name SaveToSlot (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Saves to the current slot: evaluates the slot, sets the dialog state and starts the save job.
void SaveToCurrentSlot(void)
{
    struct G *saveManager = &D_002E65E0;
    struct H *dialog = &D_002E6550;
    struct CRef ref;
    int isWarned;
    saveManager->currentSlot = saveManager->selectedSlot;
    dialog->followUpAction = 2;
    GetGlobalSaveVar(&ref, (*(int *)&g_sw.psvpAutosaveWarned), D_003D4A60);
    isWarned = ref.m_tag;
    Decref__4CRef(&ref);
    if (!isWarned) {
        SetMcDialogState(dialog, 0x22);
    } else {
        SetMcDialogState(dialog, 0x1A);
        StartSaveSlotJob(saveManager);
    }
}
} } // namespace M_func_001AD3E0
extern SAVE D_002E65E0;

namespace M_func_001AD480 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B slots[3]; char pad2[0xFD58 - 0x24 - 3 * 0x5464]; struct B *selectedSlot; };
struct H { char pad[0x4C]; int followUpAction; };
struct S { int x0, x4, slotIndex; char pad[0x10]; int flags; };   /* the menu item (flags bit 1: handled, inferred) */
extern struct G D_002E65E0;
extern struct H D_002E6550;
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void SaveToCurrentSlot(void);
/** @sly2-annot addr=0x1AD480
 * Purpose: Menu action slot i (group 3): confirm overwrite or SaveToSlot
 * Info: proposed name SelectSaveSlotSave (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Menu action for slot i on the save page: asks to confirm an overwrite, or saves directly to an empty slot.
void SelectSaveSlotToSave(struct S *item)
{
    struct CRef ref;
    struct B *slot = &D_002E65E0.slots[item->slotIndex];
    float dt;
    GetGlobalSaveVar(&ref, (*(int *)&g_sw.psvpDt), slot);
    dt = ref.m_tag;
    Decref__4CRef(&ref);
    D_002E65E0.selectedSlot = slot;
    if (dt == 0.0f)
        SaveToCurrentSlot();
    else
        SetMcDialogState(&D_002E6550, 0x1C);
    if (item)
        item->flags |= 2;
}
} } // namespace M_func_001AD480
extern SAVE D_002E65E0;

namespace M_func_001AD538 {
struct CRef;
extern "C" void Decref__4CRef(CRef *);
struct CRef { int m_tagk; float m_tag; CRef(const CRef &) __asm__("__4CRefRC4CRef"); ~CRef() { Decref__4CRef(this); } };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B slots[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *currentSlot; };
struct GD { char pad[0x3B1C]; int saveVarIsUsed; };   /* g_sw */
struct E { char pad[0x14]; int selectedIndex; int pad2[1]; };   /* a pause menu page */
struct H { char pad[0x24]; int topPage; };   /* the pause menu: index of the top page */
extern "C" {
extern struct G D_002E65E0;
extern struct B D_002E6604[] __asm__("D_002E65E0+0x24");
extern char D_002E6550[];
extern struct GD D_003DD490;
extern struct H D_003D9ED0;
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
void SetMcDialogPhase(void *p, int n);
int GetPauseMenuTopPageId(void *g);
}
CRef Eval(int a, void *b) __asm__("GetGlobalSaveVar");
static inline float EvalSaveVar(int saveVar, void *slot) { return Eval(saveVar, slot).m_tag; }
/** @sly2-annot addr=0x1AD538
 * Purpose: Menu hook: eval 3 slots' Splice values and update entries
 * Info: proposed name UpdateSaveMenuSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Menu hook: re-evaluates the three slots' Splice values and updates their entries.
extern "C" void UpdateSaveMenuSlots(void)
{
    float isUsed[3];
    struct E *page;
    int i;
    SetMcDialogPhase(D_002E6550, 1);
    if (D_003D9ED0.topPage <= 0)
        return;
    page = &D_003D9EDC[D_003D9ED0.topPage];
    {
        struct B *slot = D_002E6604;
        for (i = 0; i < 3; i++, slot++) {
            isUsed[i] = EvalSaveVar(D_003DD490.saveVarIsUsed, slot);
        }
    }
    switch (GetPauseMenuTopPageId(&D_003D9ED0)) {
    case 0x1E:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (isUsed[j] == 0.0f) {
                    page->selectedIndex = j;
                    break;
                }
            }
        }
        break;
    case 0x1F:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (isUsed[j] != 0.0f) {
                    page->selectedIndex = j;
                    break;
                }
            }
        }
        break;
    case 0x19:
        if (D_002E65E0.currentSlot)
            page->selectedIndex = D_002E65E0.currentSlot - D_002E65E0.slots;
        break;
    }
}
} // namespace M_func_001AD538
extern SAVE D_002E65E0;
#undef D_003D9EDC

namespace M_func_001AD740 { extern "C" {
struct E { int unknown0, unknown4; void *widget; int unknownC, unknown10; char *selectedIndex; int unknown18; };   /* a pause menu page */
struct G { int pad[9]; int topPage; };
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
extern struct G D_003D9ED0;
struct S { int unknown0, unknown4; char *slotIndex; };   /* the menu item */
void PopPauseMenuPage(struct G *g);
int IsPauseMenuOnSavePage(struct G *g);
void PlayWidgetAnim(void *a, char *b);
/** @sly2-annot addr=0x1AD740
 * Purpose: Menu hook: set text id 0x5C8+i or 0x5C5+i for slot i
 * Info: proposed name SetSaveSlotLabel (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Menu hook: sets slot i's label text (0x5C8+i for a used slot, 0x5C5+i for an empty one).
void SetSaveSlotLabel(struct S *item)
{
    struct E *page;
    if (::D_002E6550.phase != 1) {
        PopPauseMenuPage(&D_003D9ED0);
        return;
    }
    if (!IsPauseMenuOnSavePage(&D_003D9ED0))
        return;
    page = &D_003D9EDC[D_003D9ED0.topPage];
    if (page->selectedIndex == item->slotIndex)
        PlayWidgetAnim(page->widget, item->slotIndex + 0x5C8);
    else
        PlayWidgetAnim(page->widget, item->slotIndex + 0x5C5);
}
} } // namespace M_func_001AD740
#undef D_003D9EDC

/** @sly2-annot addr=0x1AD7E0
 * Purpose: Dynamic text provider (langstr 0x2F2): strcpy name lookup
 * Info: proposed name TextMcCardName (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text (string 0x2F2): the memory card's name.
/* Dynamic text: the card name. */
extern "C" void FormatMcCardNameText(char *pch)
{
    strcpy(pch, FindSwLocalizedString(&D_003DD490, ::D_002E6550.state));
}

namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_McJobFormat() __asm__("McJobFormat");
extern const unsigned int r_D_002A6E88[2] __asm__("D_002A6E88") = {0xFFFF0000, (unsigned int)p_McJobFormat};
}
namespace RO_ORDER {   /* pointers through asm-labelled declarations */
void p_func_001ABC68() __asm__("McJobSaveSlotResetGs");
extern const unsigned int r_D_002A6E90[2] __asm__("D_002A6E90") = {0xFFFF0000, (unsigned int)p_func_001ABC68};
}
namespace M_func_001AD820 { extern "C" {
extern char D_002E6550[], D_003DD490[];
int GetMcDialogButtonKind(void *p);
char *FindSwLocalizedString__FP2SWi(void *p, int n);
void strcpy1(char *a, const char *b);
/** @sly2-annot addr=0x1AD820
 * Purpose: Dynamic text provider (0x2F3) depending on dialog state
 * Info: proposed name TextMcDialog (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text (string 0x2F3): the dialog's message for its state.
void FormatMcDialogText(char *buf)
{
    switch (GetMcDialogButtonKind(D_002E6550)) {
    case 1:
        strcpy(buf, "&2S&.: ");
        strcpy1(buf, FindSwLocalizedString__FP2SWi(D_003DD490, 0x38));
        break;
    case 2:
        strcpy(buf, "&2S&. : ");
        strcpy1(buf, FindSwLocalizedString__FP2SWi(D_003DD490, 0x36));
        strcpy1(buf, "       ");
        strcpy1(buf, FindSwLocalizedString__FP2SWi(D_003DD490, 0x37));
        strcpy1(buf, " : &2T&.");
        break;
    }
}
} } // namespace M_func_001AD820

/** @sly2-annot addr=0x1AD928
 * Purpose: Dynamic text provider (0x5CB)
 * Info: proposed name TextSaveA (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text (string 0x5CB) of the save page. (inferred: content not identified)
extern "C" void FormatSavePageTextA(char *pch)
{
    strcpy(pch, FindSwLocalizedString(&D_003DD490, GetPauseMenuTopPageId(D_003D9ED0)));
}

namespace M_func_001AD980 { extern "C" {
struct G { char pad[0x3A18]; char gameState[1]; };   /* g_sw; +0x3A18: the game-progress object */
extern struct G D_003DD490;
extern char D_003DA530[], D_003D4A60[];
int IsScreenShown(void *p);
void *GetBestEpisode(void);
int GetWorldNameTextId(void *p);
char *FindSwLocalizedString__FP2SWi(void *p, int n);
char *GetCompletionPercent(void *p, void *q);
void sprintf(char *buf, char *fmt, ...);
/** @sly2-annot addr=0x1AD980
 * Purpose: Text '%s : %d%%' with GetCompletionPercent
 * Info: proposed name TextSaveCompletion (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text: "<label> : <n>%" with the game's completion percentage.
void FormatSaveCompletionText(char *buf)
{
    char *episodeName;
    struct G *world;
    char *fmt;
    if (IsScreenShown(D_003DA530))
        return;
    fmt = "%s : %d%%";
    world = &D_003DD490;
    episodeName = FindSwLocalizedString__FP2SWi(world, GetWorldNameTextId(GetBestEpisode()));
    sprintf(buf, fmt, episodeName, GetCompletionPercent(world->gameState, D_003D4A60));
}
} } // namespace M_func_001AD980

namespace M_func_001ADA20 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct E { int unknown0, unknown4, widget, unknownC, unknown10; int selectedIndex; int unknown18; };   /* a pause menu page */
struct G { int pad[9]; int topPage; };
struct GD { char pad[0x3B1C]; int saveVarIsUsed; };
struct B { char data[0x5464]; };
extern struct G D_003D9ED0;
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
extern struct GD D_003DD490;
extern struct B D_002E6604[] __asm__("D_002E65E0+0x24");
int IsPauseMenuOnSavePage(struct G *g);
int GetPauseMenuTopPageId(struct G *g);
void GetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
char *FindSwLocalizedString__FP2SWi(void *p, int n);
char *strcpy(char *d, const char *s);
char *strcpy1(char *d, const char *s);
/** @sly2-annot addr=0x1ADA20
 * Purpose: Dynamic text provider (0x5CD)
 * Info: proposed name TextSaveB (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text (string 0x5CD) of the save page. (inferred: content not identified)
void FormatSavePageTextB(char *buf)
{
    struct G *menu = &D_003D9ED0;
    struct GD *world;
    struct CRef ref;
    float isUsed;
    if (!IsPauseMenuOnSavePage(menu))
        return;
    world = &D_003DD490;
    {
    struct E *page = &D_003D9EDC[menu->topPage];
    GetGlobalSaveVar(&ref, world->saveVarIsUsed, &D_002E6604[page->selectedIndex]);
    }
    isUsed = ref.m_tag;
    Decref__4CRef(&ref);
    switch (GetPauseMenuTopPageId(menu)) {
    case 0x1E:
        if (isUsed != 0.0f)
            strcpy(buf, FindSwLocalizedString__FP2SWi(world, 0x25));
        else
            strcpy(buf, FindSwLocalizedString__FP2SWi(world, 0x26));
        break;
    case 0x1F:
        if (isUsed != 0.0f)
            strcpy(buf, FindSwLocalizedString__FP2SWi(world, 0x27));
        else
            strcpy(buf, FindSwLocalizedString__FP2SWi(world, 0x26));
        break;
    case 0x19:
        strcpy(buf, FindSwLocalizedString__FP2SWi(world, 0x29));
        if (isUsed != 0.0f) {
            strcpy1(buf, "\n");
            strcpy1(buf, FindSwLocalizedString__FP2SWi(world, 0x25));
        }
        break;
    }
}
} } // namespace M_func_001ADA20
#undef D_003D9EDC

namespace M_func_001ADBB8 { extern "C" {
extern char D_003D9ED0[], D_003DD490[];
int GetPauseMenuTopPageId(void *p);
void *FindSwLocalizedString__FP2SWi(void *p, int n);
void strcpy(void *s, void *q);
void strcpy1(void *s, void *q);
/** @sly2-annot addr=0x1ADBB8
 * Purpose: Dynamic text provider (0x5CE) with ' : &2T&.' format
 * Info: proposed name TextSaveTime (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Dynamic text (string 0x5CE): the play time, formatted " : &2T&.".
void FormatSaveTimeText(void *s)
{
    switch (GetPauseMenuTopPageId(D_003D9ED0)) {
    case 0x19:
    case 0x1E:
    case 0x1F:
        strcpy(s, FindSwLocalizedString__FP2SWi(D_003DD490, 0x40));
        strcpy1(s, (char *)" : &2T&.");
        break;
    }
}
} } // namespace M_func_001ADBB8

namespace M_func_001ADC40 {
/* (Claude agent, 2026-10-17) frame pointer copy pf used only for the push call reproduces the a0->s5 copy. */
struct REF{int tag;union{int n;float g;};};
struct G{char pad[0x3A18];char object[0x104];void*primary;int pad20[2];void*fallback;};
struct B{char data[0x5464];};
extern "C" {
 extern G D_003DD490;
 extern B D_002E6604[] __asm__("D_002E65E0+0x24");
 extern char D_003D9ED0[];
 void*GetGlobalSaveVar(REF*,void*,void*);
 void Decref__4CRef(REF*);
 void*STACKSCOPE_ctor(void*);
 void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
 void*AllocSaveKey(void*,...);
 int GetWorldNameTextId(int);
 void*FindSwLocalizedString__FP2SWi(void*,int);
 int GetCompletionPercent(void*,void*);
 int sprintf(char*,const char*,...);
 char*strcpy(char*,const char*);
 int GetPauseMenuTopPageId(void*);
}
static inline float GetG(REF&r,void*a,B*b){GetGlobalSaveVar(&r,a,b);float g=r.g;Decref__4CRef(&r);return g;}
static inline int GetN(REF&r,void*a,B*b){GetGlobalSaveVar(&r,a,b);int n=r.n;Decref__4CRef(&r);return n;}
/** @sly2-annot addr=0x1ADC40
 * Purpose: Per-slot text (0x5CF..0x5D1, arg=slot): episode best key + completion %
 * Info: proposed name TextSaveSlotSummary (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Per-slot text (strings 0x5CF..0x5D1): the slot's episode and completion percentage.
extern "C" void FormatSaveSlotSummaryText(char*buf,int idx){
 REF ref;
 G*pg=&D_003DD490;
 B*pb=&D_002E6604[idx];
 if(GetG(ref,pg->primary,pb)!=0.0f){
  int frame[4];int*pf=frame;STACKSCOPE_ctor(pf);
  void*value=AllocSaveKey((char *)"episodeBest",0);
  int tag;
  if(value)tag=GetN(ref,value,pb);else tag=GetN(ref,pg->fallback,pb);
  sprintf(buf,"%s : %d%%",(char*)FindSwLocalizedString__FP2SWi(&D_003DD490,GetWorldNameTextId(tag)),GetCompletionPercent(D_003DD490.object,pb));
  STACKSCOPE_ctor_1AE4A0(frame,2);
 }else{
  switch(GetPauseMenuTopPageId(D_003D9ED0)){
   case 0x1E:case 0x1F:strcpy(buf,(char*)FindSwLocalizedString__FP2SWi(pg,0x44));break;
   case 0x19:strcpy(buf,(char*)FindSwLocalizedString__FP2SWi(pg,0x45));break;
  }
 }
}
} // namespace M_func_001ADC40

/* CMcStatus (DRAFT name, from the functions' annotations): the memory-card status widget (0x40; vtable 0x2D36D0,
   memcard's): a SCREEN with its own Update (slot 7) and SetMode (slot 14, the key function). */
struct CMcStatus : SCREEN
{
    char pad30[0x40 - 0x30];
    CMcStatus() __asm__("CMcStatus_ctor");
    virtual void SetMode(int nMode) __asm__("CMcStatus_SetMode");   /* slot 14 */
    virtual void Update() __asm__("CMcStatus_Update");   /* slot 7 */
};
CMcStatus::CMcStatus()
{
    idTextQueued = 0x5D4;
    ResetMode();
}

namespace M_func_001ADE38 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *child; };
/** @sly2-annot addr=0x1ADE38
 * Purpose: small
 * Info: TU memcard
 * Notes: descriptive only: (McStatus helper) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r4_17F090
 * @end-sly2-annot */
// Shows the status widget's child (slot 49 with -1).
void ShowMcDialogWidget(struct S *dialog) 
{
    struct O *child = dialog->child;
    if (child) { struct VTE *e = &child->vt[49]; e->pfn((char *)child + e->d, -1); }
}
} } // namespace M_func_001ADE38

namespace M_HideMcDialogWidget { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *child; };
/** @sly2-annot addr=0x1ADE70
 * Purpose: small
 * Info: proposed name HideMcDialogWidget (T1); TU memcard
 * Notes: verifier PLAUSIBLE: (McStatus show/hide) -> HideMcDialogWidget
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// Hides the status widget's child (slot 50).
void HideMcDialogWidget(struct S *dialog) 
{
    struct O *child = dialog->child;
    if (child) { struct VTE *e = &child->vt[50]; e->pfn((char *)child + e->d); }
}
} } // namespace M_HideMcDialogWidget

namespace M_func_001ADEA8 {
struct O { char pad[0x58]; int displayState; int pad2[2]; int targetDisplayState; };
struct S { char pad[0x24]; O *child; };
/** @sly2-annot addr=0x1ADEA8
 * Purpose: child x58>=2 or x64>=2
 * Info: proposed name FMcStatusVisible (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// True when the status widget's child is shown or showing.
extern "C" int IsMcStatusVisible(S *dialog)
{
    O *child = dialog->child;
    if (!child) return 0;
    if (child->displayState < 2 && child->targetDisplayState < 2) return 0;
    return 1;
}
} // namespace M_func_001ADEA8

/** @sly2-annot addr=0x1ADEE8
 * Purpose: vt7 (vtable 0x2D36D0): base 0x1DA808; when shown >1s and thread idle hide (0x1ADE70)
 * Info: proposed name CMcStatus_Update (T2); class CMcStatus; TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMcStatus's per-frame update (slot 7): hides the status once it has been shown for over a second and the worker
// is idle.
/* CMcStatus slot 7: hide once shown for over a second and the worker is idle. */
extern "C" void CMcStatus_Update(MCDLG *dialog)
{
    MCCHILD *pchild;
    UpdateTimedScreen(dialog);
    pchild = dialog->pchild;
    if (pchild != 0) {
        if (pchild->displayState == 2 && pchild->targetDisplayState == pchild->displayState && D_002DDED0.realTime - pchild->tDisplayState > 1.0f &&
            IsMemcardThreadValid(&D_002F633C) == 0) {
            HideMcDialogWidget(dialog);
        }
    }
}

/** @sly2-annot addr=0x1ADF78
 * Purpose: vt14 same body as 0x1ACC18
 * Info: proposed name CMcStatus_SetMode (T2); class CMcStatus; TU memcard
 * Notes: runtime4 CORRECTION: x1AC4 = Fists of Flame overlay ASEG (oid 0x156); x1AC8/x1ACC are constants
 * Confidence: low (draft) [T2 0.30] - evidence: runtime (save-state RAM); sources:
 *   r4_17F090+runtime-corrected
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
// CMcStatus slot 14: sets the mode, stamping the real time when it changes (same body as CMemcardDialog's).
/* slot 14: set the mode (stamping the real time). */
void CMcStatus::SetMode(int nMode)
{
    MCDLG *dialog = (MCDLG *)this;
    if (nMode != dialog->nMode) {
        dialog->nMode = nMode;
        dialog->tMode = D_002DDED0.realTime;
    }
}


/** @sly2-annot addr=0x258248
 * Purpose: memset(p,0,8); p->x8=-1; p->xC=0 [game linkonce (inline/template)]
 * Info: proposed name InitFileKey (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
// Clears a file key (zeroed, index -1). A linkonce inline homed here.
/* linkonce copy (0x258248): clears a file key. */
extern "C" inline void InitFileKey(FILEKEY *pfk)
{
    memset(pfk, 0, 8);
    pfk->bC = 0;
    pfk->n8 = -1;
}

// ---- memcard's strings (0x2A6BB0-0x2A6EE0: the save directory and file names, the format strings) are the
// functions' own literals, pooled per TU, so they are spelled as literals where they are used (tu-merging.md
// "Phase 6: one link mode"); the pointer-to-member records are defined among the functions (RO_ORDER).

// ---- memcard's .rodata.early (tu-merging.md "Phase 6"): the FILEREC key of the save icon file (GetDiscFileEntrySize,
// load_file_from_disc), between tu_1A4E08's keys and menu's in TU order. Asm-labelled: the declarations above stay.
namespace MEMCARD_KEYS {
extern const unsigned int r_D_002A38E8[16] __asm__("D_002A38E8") __attribute__((section(".rodata.early"))) = {0xBD87DF12, 0xE9F4E46C, 0x0E45625C, 0x8F049FCC, 0x478171EA, 0x9B7A071D, 0x844C0FEE, 0x53E2403F, 0xC179D2D6, 0xD52DEF52, 0x5842CD0B, 0x2190978E, 0xC22AC32F, 0xBD961318, 0x0E456593, 0xE9F48034};
}
