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
// func_001ADDF0) and destructors (func_00258348, in-charge flag 2): g++ generates the 93-instruction
// construct-and-destruct StaticInit_memcard and the _GLOBAL_$D$ stub func_001AE130 (cm.cpp's g_cm pattern).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_memcard
// SLY2_ALIAS: _GLOBAL_$I$*=func_001AE110
// SLY2_ALIAS: _GLOBAL_$D$*=func_001AE130

#include "sly2/basic.h"
#include "sly2/sw.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
extern "C" char base_D_002DDED0[] __asm__("D_002DDED0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */
extern "C" char base_D_002E65A0[] __asm__("D_002E65A0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */
extern "C" char base_D_003D9ED0[] __asm__("D_003D9ED0");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */
extern "C" char base_D_003DD490[] __asm__("D_003DD490");   /* base of the address names below: see docs/guides/tu-merging.md "Data" */

/* The two dialog widgets (views of their bytes; constructed and destroyed by C-named functions). */
struct MCDLGG;
struct MCDLG;
extern "C" {
MCDLG *CMemcardDialog_ctor(MCDLG *pdlg);
void *func_001ADDF0(void *pv);        /* CMcStatus constructor */
void func_00258348(MCDLGG *pv, int grfDelete);
}
struct MCDLGG
{
    char ab[0x50];
    MCDLGG(int f) { if (f) CMemcardDialog_ctor((MCDLG *)this); else func_001ADDF0(this); }
    ~MCDLGG() { func_00258348(this, 2); }
};
MCDLGG D_002E6550(1);   /* the memory card dialog (CMemcardDialog) */
MCDLGG D_002E65A0(0);   /* the card status widget (CMcStatus) */

// 0x1ADF98 StaticInit_memcard (the two widgets, then the six slot pools; their destruction) and its stubs.
SLY2_INSTANTIATE_SLOT_POOLS()

/* The memory card worker: semaphore, thread id (-1: none), priority. */
struct MEMCARD
{
    int sema;
    int tid;                /* +0x04 */
    int pad8[2];
    int prio;               /* +0x10 */
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
    char pad10[0x24 - 0x10];
    char aslot[3][0x5464];  /* +0x24 */
    int islotCur;           /* +0xFD50 */
    char padFD54[0xFD5C - 0xFD54];
    MEMCARD memcard;        /* +0xFD5C */
};
/* A dialog widget: vtable +0xC, id +0x20, its child +0x24, mode and when it was set, state. */
struct MCCHILD { char pad0[0x58]; int n58; float t5C; char pad60[4]; int n64; char pad68[0xB0 - 0x68]; int fB0; };
struct MCDLG
{
    char pad0[0xC];
    void *pvt;              /* +0x0C */
    char pad10[0x20 - 0x10];
    int id;                 /* +0x20 */
    MCCHILD *pchild;        /* +0x24 */
    int nMode;              /* +0x28 */
    float tMode;            /* +0x2C */
    char pad30[0x40 - 0x30];
    int nState;             /* +0x40 */
};
/* A file key (linkonce func_00258248). */
struct FILEKEY { int an[2]; int n8; char bC; };

extern SW D_003DD490;
extern "C" {
extern char D_002DEB30[];       /* the game */
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */   /* the save manager */
extern int D_004425D8;
#undef D_003E0EA8
#define D_003E0EA8 (*(int *)((char *)base_D_003DD490 + 0x3A18))   /* g_sw+0x3A18: SW::savevars */
extern int D_002E6590;          /* a langstr id */
extern char D_003D9ED0[];       /* the pause menu */
extern MEMCARD D_002F633C;      /* the save manager's worker (D_002E65E0 + 0xFD5C) */
extern CLOCK D_002DDED0;
extern void *D_002D3750[];      /* CMemcardDialog vtable */
extern char D_002A38E8[];   /* the icon file */
extern const char D_002A6CA0[];   /* its card name */
void InitGame(void *pgame);
void *memset(void *pv, int b, unsigned int cb);
char *strcpy(char *pchDst, const char *pchSrc);
void SignalSema(int sema);
void WaitSema(int sema);
void func_002348B0(int tid, int prio);
int McSyncResult(MEMCARD *pmemcard, int nResult, int nJob, int n);
int sceMcFormat(int port, int slot);
int sceMcMkdir(int port, int slot, const char *pchz);
int sceMcOpen(int port, int slot, const char *pchz, int mode);
int sceMcClose(int fd);
int sceMcDelete(int port, int slot, const char *pchz);
void SetSaveState(SAVE *psave, int nState);
int func_001AC570(SAVE *psave, void *pslot);
void func_001ACC38(void *pdlg, int n);
int McJobGetInfo(SAVE *psave);
int WriteSaveSlotFile(SAVE *psave, int islot);
void SetMcDialogState(void *pdlg, int nState);
void HideMcDialogWidget(void *pdlg);
void load_file_from_disc(int n0, int n1, char *pchzFile, char *pchzCard);
void ResetGsIfSchemaChanged(int *pn);
MCDLG *func_001DA728(MCDLG *pdlg);   /* the widget base constructor (returns the widget) */
int func_001DA7D0(MCDLG *pdlg);
void func_001DA770(MCDLG *pdlg);
void func_001DA7B0(MCDLG *pdlg);
void func_001DA808(MCDLG *pdlg);
int IdPauseTopPage(void *pmenu);
}
void InitSave(SAVE *psave, int n0, int n1);
int SemaCreate(int cInit, int cMax);
int FMemcardThreadValid(MEMCARD *pmemcard);
void SetMemcardThreadHighPrio(MEMCARD *pmemcard);
char *PchzFindSwLangstr(SW *psw, int id);

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
extern "C" int FUN_0018b910__Fi(int n)
{
    if (n != 0) return n <= 0 ? 0x1F : 0x1E;
    return 0x1D;
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
extern "C" int FUN_0018b930__Fi(int n)
{
    switch (n) {
    case 0: return 0x21;
    case 1: return 0x22;
    case -1: return 0x20;
    }
    return 0x1F;
}
} // namespace M_FUN_0018b930__Fi

namespace M_MsgFromMcError { extern "C" {
/* MsgFromMcError (0x1AABC0): maps (kind 1..21, error code) to a message id.  Error codes are mostly small negatives
   (-1..-8, -403); code 0 is success and gives message 0, unknown codes fall back to 11 (code < -9) or 31.
   Kinds 7..9 and anything outside 1..21 give 31 (jump-table default).

   Compile note (lane 10): kinds 16/17 write `result = 0` first and then branch to the shared `done:` return at the
   end of the function.  Those conditional branches to the function's return label are what leave the label in
   front of the last `jr ra` while dbr runs, so the final "result = 31; return" keeps an unfilled delay slot like
   the original.  Kind 1's `result = 9` is plain.  No register pin or asm is needed. */
static inline int Fallback(int err) { int msg = 11; if (err >= -9) msg = 31; return msg; }

/** @sly2-annot addr=0x1AABC0
 * Purpose: (kind 1..21, sceMc error) -> message id for the UI
 * Info: proposed name MsgFromMcError (EXISTING); TU memcard
 * Notes: agent/verifier proposes MsgFromMcError for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> MsgFromMcError (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int MsgFromMcError(int kind, int err)
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
 * Purpose: MEMCARD init: SemaCreate(1,1), thread id -1, prio 0x20
 * Info: proposed name InitMemcard (T1); TU memcard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void InitMemcard(MEMCARD *pmemcard)
{
    pmemcard->sema = SemaCreate(1, 1);
    pmemcard->tid = -1;
    pmemcard->prio = 0x20;
}

namespace M_MemcardThreadMain { extern "C" {
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int x0; int x4; struct PMF pmf; int x10; };
#undef D_002E65E0
#define D_002E65E0 (*(char (*)[])((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
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
void MemcardThreadMain(struct S *s)
{
    func_00234A60(s->x0);
    s->x10 = s->pmf.pfn(D_002E65E0 + s->pmf.delta);
    func_002333B4(&s->pmf, 0, sizeof(s->pmf));
    s->x4 = -1;
    D_004425D8[0] = -1;
    func_00234A40(s->x0);
    ExitDeleteThread();
}
} } // namespace M_MemcardThreadMain
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */

namespace M_CreateMemcardThread { extern "C" {
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int x0; int x4; struct PMF pmf; int x10; };
struct ThreadParam { int attr; void *entry; void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr2; int option; int waitType; int waitId; int wakeupCount; };
extern char D_002E5270[] __asm__("_gp");   /* _gp (linker-defined), not an object */
extern char D_002E2080[];
extern int D_004425D8[];
void MemcardThreadMain(struct S *s);
void SetMemcardThreadHighPrio__FP7MEMCARD(struct S *s);
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
void CreateMemcardThread(struct S *s, struct PMF pmf)
{
    struct ThreadParam tp;
    SetMemcardThreadHighPrio__FP7MEMCARD(s);
    s->pmf = pmf;
    s->x10 = 0x21;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.entry = MemcardThreadMain;
    tp.stack = D_002E2080;
    tp.gpReg = D_002E5270;
    tp.stackSize = 0x4000;
    tp.initPriority = 3;
    s->x4 = func_00234820(&tp);
    D_004425D8[0] = s->x4;
    func_002355E0(s->x4, s);
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
void SetMemcardThreadHighPrio(MEMCARD *pmemcard)
{
    if (FMemcardThreadValid(pmemcard) != 0) {
        func_002348B0(pmemcard->tid, 1);
    }
    WaitSema(pmemcard->sema);
    SignalSema(pmemcard->sema);
}

/** @sly2-annot addr=0x1AB088
 * Purpose: Thread id (+4) != -1
 * Info: proposed name FMemcardThreadValid__FP7MEMCARD (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FMemcardThreadValid(MEMCARD *pmemcard)
{
    return ~pmemcard->tid != 0;
}

namespace M_McSyncResult { extern "C" {
int func_001AAB48(void *p) __asm__("FUN_0018b910__Fi");
int func_00244450(int n, int *a, int *b) __asm__("sceMcSync");
int func_001AAB68(int n) __asm__("FUN_0018b930__Fi");
int MsgFromMcError(void *p, int n);
/** @sly2-annot addr=0x1AB098
 * Purpose: Poll sceMcSync(0,&cmd,&res) and map to result; on error map through MsgFromMcError
 * Info: proposed name McSyncResult (EXISTING); TU memcard
 * Notes: agent/verifier proposes McSyncResult for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McSyncResult (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McSyncResult(void *a0, void *a1, void *a2, int *a3)
{
    int buf[2];
    int r = func_001AAB48(a1);
    if (r != 0x1D)
        return r;
    r = func_001AAB68(func_00244450(0, &buf[0], &buf[1]));
    if (a3)
        *a3 = buf[1];
    if (r == 0x22)
        r = MsgFromMcError(a2, buf[1]);
    return r;
}
} } // namespace M_McSyncResult

namespace M_InitSave__FP4SAVEii {
struct SAVE {
    int a;
    int b;
    int n8;
    int nC;
    float g10;
    float g14;
    int n18;
    int n1C;
    int n20;
};
extern "C" void sceMcInit(void);
extern "C" void InitMemcard__FP7MEMCARD(void *);
/** @sly2-annot addr=0x1AB128
 * Purpose: sceMcInit, InitMemcard(+0xFD5C), zero SAVE header
 * Info: proposed name InitSave__FP4SAVEii (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void InitSave__FP4SAVEii(SAVE *psave, int a, int b)
{
    sceMcInit();
    InitMemcard__FP7MEMCARD((char *)psave + 0xFD5C);
    psave->a = a;
    psave->b = b;
    psave->n8 = -1;
    psave->nC = 0;
    psave->g10 = psave->g14 = 0.0f;
    psave->n18 = 0;
    psave->n1C = 0;
    psave->n20 = 0;
}
} // namespace M_InitSave__FP4SAVEii

namespace M_UpdateSave__FP4SAVE { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct H { char pad[0x44]; int x44; };
struct S { int x0, x4; int x8; int xC; float x10; float x14; char pad[0xFD5C - 0x18]; char xFD5C[1]; };
extern char D_003DD490[], D_002F66B0[], D_003D9ED0[];
extern int D_002DEB4C;
extern struct H D_002E6550;
#undef D_002DDF0C
#define D_002DDF0C (*(float *)((char *)base_D_002DDED0 + 0x3C))   /* g_clock (CLOCK)+0x3C */
extern const struct LONGU D_002A6BA0;
int func_0020E040(void *g);
int FMemcardThreadValid__FP7MEMCARD(void *p);
void CreateMemcardThread(void *t, long pmf);
void func_001AB630(struct S *s);
void SetSaveState(struct S *s, int);
int func_001B42C0(void *p);
int func_001B01E8(void *p);
void SetMcDialogState(struct H *h, int n);
/** @sly2-annot addr=0x1AB1A0
 * Purpose: Per-frame save state machine: skip during transitions/when thread busy; poll card (job D_002A6BA0.l)
 *   or advance state (0x1AB418); autosave trigger with 0.3s debounce
 * Info: proposed name UpdateSave (T1); TU memcard
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r4_17F090+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void UpdateSave__FP4SAVE(struct S *s)
{
    if (func_0020E040(D_003DD490))
        return;
    if (D_002DEB4C)
        return;
    if (D_002E6550.x44 == 3)
        return;
    if (FMemcardThreadValid__FP7MEMCARD(s->xFD5C))
        return;
    if (s->x8 == s->xC) {
        if (s->x14 <= D_002DDF0C)
            CreateMemcardThread(s->xFD5C, D_002A6BA0.l);
        else
            func_001AB630(s);
    } else {
        SetSaveState(s, s->xC);
    }
    if (FMemcardThreadValid__FP7MEMCARD(s->xFD5C))
        return;
    if (func_001B42C0(D_002F66B0))
        return;
    if (D_002E6550.x44 != 1)
        return;
    if (0.3f <= D_002DDF0C - s->x10) {
    if (func_001B01E8(D_003D9ED0)) {
        if (s->x8 != 6)
            SetMcDialogState(&D_002E6550, 0x2E);
        return;
    }
    switch (s->x8) {
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
#undef D_002DDF0C

/** @sly2-annot addr=0x1AB398
 * Purpose: existing
 * Info: proposed name CompareSaves__FP4SAVET0 (EXISTING); TU memcard
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
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
    CRef(const CRef &) __asm__("func_001291A8");
    ~CRef() { Decref__4CRef(this); }
};
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct SLOT { char pad[0x5408]; char x5408[0x5C]; };
struct THR;
struct MC {
    int x0; int x4; int state; int xC; float x10; char pad[0x24 - 0x14];
    SLOT aslot[3];
    int xFD50; SLOT *pslotCur; int xFD58; THR *thr[1];
};
struct G { char pad[0x3B1C]; int x3B1C; };
struct CLOCK { char pad[0x3C]; float t3c; };
struct ST { char pad[0x44]; int x44; };
extern "C" {
extern G D_003DD490;
extern CLOCK D_002DDED0;
extern ST D_002E6550;
extern char D_002E65A0[];
void func_001ACC38(ST *, int);
int func_001ADEA8(void *);
void SetMcDialogState(ST *, int);
void HideMcDialogWidget(void *);
int CompareSaves__FP4SAVET0(char *, char *);
void func_001D8D80(SLOT *, int);
void SetSaveState(MC *p, int state);
}
CRef Eval(int, SLOT *) __asm__("RefGetGlobalSaveVar");
static inline float GetF(int k, SLOT *p) { return Eval(k, p).u.g; }
struct X { int A() __asm__("func_001AB0A0"); int B() __asm__("func_001AB0F0"); };
void _iEnableDmac(void *, int (X::*)()) __asm__("CreateMemcardThread");

/** @sly2-annot addr=0x1AB418
 * Purpose: SAVE state change (x8) with per-state actions on the 3 slots (Splice CRef evals), UI state
 *   updates, ResetGs on load
 * Info: proposed name SetSaveState (T1); TU memcard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void SetSaveState(MC *p, int state)
{
    int i;
    if (state == p->state)
        return;
    if (p->state == 6) {
        p->pslotCur = 0;
        if (p->xFD50) {
            ST *pst = &D_002E6550;
            if (pst->x44 == 1) {
                func_001ACC38(pst, 3);
                if (func_001ADEA8(D_002E65A0)) {
                    SetMcDialogState(pst, 0x23);
                    HideMcDialogWidget(D_002E65A0);
                } else {
                    SetMcDialogState(pst, 0x24);
                }
            }
        }
    }
    switch (state) {
    case 0:
    case 1:
        _iEnableDmac(&p->thr, &X::A);
        break;
    case 3:
        _iEnableDmac(&p->thr, &X::B);
        break;
    case 6:
        p->pslotCur = &p->aslot[0];
        for (i = 0; i < 3; i++) {
            SLOT *pslot = &p->aslot[i];
            float g = GetF(D_003DD490.x3B1C, pslot);
            if (g != 0.0f) {
                if (CompareSaves__FP4SAVET0(p->pslotCur->x5408, pslot->x5408) < 0)
                    p->pslotCur = pslot;
            } else {
                func_001D8D80(pslot, 0);
            }
        }
        break;
    }
    p->state = state;
    p->x10 = D_002DDED0.t3c;
}
} // namespace M_SetSaveState

/** @sly2-annot addr=0x1AB630
 * Purpose: If state 5 and D_003E0EA8: wait for thread, re-init all 3 slots (stride 0x5464, 0x1AC570), go to
 *   state 6
 * Info: proposed name ReloadSaveSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* Reloads the three slots once the worker is idle (state 5 and D_003E0EA8 set). */
extern "C" void func_001AB630(SAVE *psave)
{
    int i;
    char *pslot;
    if (psave->nState == 5 && D_003E0EA8 != 0) {
        pslot = psave->aslot[0];
        i = 2;
        SetMemcardThreadHighPrio(&psave->memcard);
        do {
            func_001AC570(psave, pslot);
            i--;
            pslot += 0x5464;
        } while (i >= 0);
        psave->nStateNext = 6;
        SetSaveState(psave, 6);
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
extern "C" int McJobFormat(SAVE *psave)
{
    int nResult = McSyncResult(&psave->memcard, sceMcFormat(psave->port, psave->slot), 0x10, 0);
    if (nResult != 0) {
        psave->nStateNext = 2;
        func_001ACC38(&D_002E6550, 3);
    } else {
        psave->nStateNext = 3;
    }
    return nResult;
}

namespace M_FOpenSaveIconFile {
struct S { int x0; int x4; char pad[0x2C]; int x34; int x38; int x3C; };
struct K;
extern "C" void func_00258248(K *k);
extern "C" int func_0014B670(K *k);
struct K { int a; int b; int c; char pad[0x104];
  K() { c = 2; func_00258248(this); }
  bool FSet(S *p) { if (!p) return false; b = p->x4 ^ p->x3C; a = (p->x0 ^ p->x34) + 0xCA0; c = 2; return b != 0; }
};
/** @sly2-annot addr=0x1AB750
 * Purpose: Build a disc FLOC (lba=(w0^w13)+0xCA0,size=w1^w15) for a file entry and open it via 0x14B670
 * Info: proposed name FOpenSaveIconFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes FOpenSaveIconFile for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> FOpenSaveIconFile
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" int FOpenSaveIconFile(S *p)
{ K k; bool f = k.FSet(p); if (f) return func_0014B670(&k); else return 0; }
} // namespace M_FOpenSaveIconFile

namespace M_CreateSaveDir { extern "C" {
extern char D_002A6BB0[], D_002A6BC8[], D_002A6BF8[];
void *McMkdir(void *s, char *a);
void *McWriteFile(void *s, char *a, char *b, int n);
void *WriteIconSys(void *s, char *a, char *b);
void *load_sly_icon_from_disc(void *s, char *a);
/** @sly2-annot addr=0x1AB7D0
 * Purpose: mkdir /BASCUS-97316YAOTWTD!, write data file, write icon.sys (0x1AC008), copy icon from disc
 *   (load_sly_icon_from_disc)
 * Info: proposed name CreateSaveDir (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *CreateSaveDir(void *s)
{
    void *p = McMkdir(s, D_002A6BB0);
    if (p)
        return p;
    p = McWriteFile(s, D_002A6BC8, D_002A6BF8, 5);
    if (p)
        return p;
    p = WriteIconSys(s, D_002A6BB0, D_002A6BF8);
    if (p)
        return p;
    return load_sly_icon_from_disc(s, D_002A6BB0);
}
} } // namespace M_CreateSaveDir

namespace M_McJobCreateSave { extern "C" {
struct B { char data[0x5464]; };
struct S { int x0; int x4; int x8; int xC; int x10, x14, x18; int x1C; int x20; struct B a[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char xFD5C[1]; };
extern char D_002A6C00[], D_002A38E8[], D_002A6C08[], D_002A6C28[], D_002A6BC8[], D_002A6BB0[], D_002E6550[];
int sceMcGetEntSpace(int a, int b, char *c);
int McSyncResult(void *p, int a, int b, int *c);
int FOpenSaveIconFile(void *p);
int CreateSaveDir(struct S *s);
int WriteSaveSlotFileReset(struct S *s, struct B *b);
void BuildSaveFilePath(struct S *s, struct B *b, char *name);
void McDelete(struct S *s, char *name);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1AB868
 * Purpose: Thread job: check space (sceMcGetEntSpace), delete stale, CreateSaveDir, write slot files
 * Info: proposed name McJobCreateSave (T1); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c250 (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McJobCreateSave(struct S *s)
{
    char name[0x50];
    int n;
    int k;
    int r;
    int i;
    void *p = s->xFD5C;
    k = 1;
    r = McSyncResult(p, sceMcGetEntSpace(s->x0, s->x4, D_002A6C00), 0x12, &n);
    if (r == 0) {
        if (!n)
            k = 2;
        k += 5;
        k += (FOpenSaveIconFile(D_002A38E8) + 0x3FF) / 0x400;
        k += 0x42;
        if (s->x1C < k) {
            r = 1;
        } else {
            r = CreateSaveDir(s);
            if (r == 0) {
                for (i = 0; i < 3; i++) {
                    r = WriteSaveSlotFileReset(s, &s->a[i]);
                    if (r)
                        goto cleanup;
                }
                if (s->xC < 5)
                    s->xC = 5;
                goto done;
            }
cleanup:
            for (i = 0; i < 3; i++) {
                BuildSaveFilePath(s, &s->a[i], name);
                McDelete(s, name);
            }
            McDelete(s, D_002A6C08);
            McDelete(s, D_002A6C28);
            McDelete(s, D_002A6BC8);
            McDelete(s, D_002A6BB0);
        }
    }
    if (r)
        func_001ACC38(D_002E6550, 3);
done:
    return r;
}
} } // namespace M_McJobCreateSave

namespace M_McJobGetDir { extern "C" {
struct B { char data[0x5464]; };
struct S { int x0; int x4; int x8; int xC; int pad[5]; struct B a[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char xFD5C[1]; };
extern char D_002A6BB0[];
int sceMcGetDir(int a, int b, char *c, int d, int e, int f);
int McSyncResult(void *p, int a, int b, int *c);
int func_001AC6A0(struct S *s, int *out);
int func_001AC4C8(struct S *s, struct B *b);
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
int McJobGetDir(struct S *s)
{
    void *p = s->xFD5C;
    int n[2];
    int r;
    int i;
    r = McSyncResult(p, sceMcGetDir(s->x0, s->x4, D_002A6BB0, 0, -1, 0), 0xD, &n[0]);
    if (n[0] <= 0) {
        func_001AC6A0(s, &n[1]);
        if (n[1] <= 0x18FFF)
            s->xC = 4;
        return 0xE;
    }
    for (i = 0; r == 0 && i < 3; i++)
        func_001AC4C8(s, &s->a[i]);
    if (r == 0 && s->xC < 5)
        s->xC = 5;
    return r;
}
} } // namespace M_McJobGetDir

namespace M_BuildSaveFilePath { extern "C" {
struct SAVE { char ab[0x5464]; };
struct M { char pad[0x24]; struct SAVE asave[4]; };
extern char D_002A6C50[], D_002A6C60[], D_002A6C78[];
int sprintf(char *buf, char *fmt, ...);
/** @sly2-annot addr=0x1ABB10
 * Purpose: sprintf('/%s/%s%d', 'BASCUS-97316YAOTWTD!', 'save', i)
 * Info: proposed name BuildSaveFilePath (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void BuildSaveFilePath(struct M *pm, struct SAVE *ps, char *buf)
{
    sprintf(buf, D_002A6C50, D_002A6C60, D_002A6C78, ps - pm->asave + 1);
}
} } // namespace M_BuildSaveFilePath

namespace M_McJobSaveSlot { extern "C" {
struct S { char pad[0xFD50]; void *x; };
extern char D_002E6550[];
void *McJobGetInfo(struct S *s);
void *CreateSaveDir(struct S *s);
void *WriteSaveSlotFile(struct S *s, void *x);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1ABB60
 * Purpose: Thread job: card info check, CreateSaveDir, write one slot file (0x1AC420)
 * Info: proposed name McJobSaveSlot (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *McJobSaveSlot(struct S *s)
{
    void *p = McJobGetInfo(s); if (!p) p = CreateSaveDir(s); if (!p) p = WriteSaveSlotFile(s, s->x); if (p) func_001ACC38(D_002E6550, 3); return p;
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
/* Thread job: check the card, then write the current slot. */
extern "C" int func_001ABBE0(SAVE *psave)
{
    int nResult = McJobGetInfo(psave);
    if (nResult != 0 || (nResult = WriteSaveSlotFile(psave, psave->islotCur)) != 0) {
        func_001ACC38(&D_002E6550, 3);
        SetMcDialogState(&D_002E6550, 0x23);
        HideMcDialogWidget(&D_002E65A0);
    }
    return nResult;
}

namespace M_func_001ABC68 { extern "C" {
struct S { char pad[0xFD58]; void *x; };
extern char D_002E6550[];
void *McJobGetInfo(struct S *s);
void *CreateSaveDir(struct S *s);
void *WriteSaveSlotFileReset(struct S *s, void *x);
void func_001ACC38(void *p, int n);
/** @sly2-annot addr=0x1ABC68
 * Purpose: Thread job: card check, CreateSaveDir, write via 0x1AC468 (with ResetGs)
 * Info: proposed name McJobWriteSave (T2); TU memcard
 * Notes: names_v2 v1 proposed FUN_0018c7f8__FP4SAVE (0.000)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void *func_001ABC68(struct S *s)
{
    void *p = McJobGetInfo(s); if (!p) p = CreateSaveDir(s); if (!p) p = WriteSaveSlotFileReset(s, s->x); if (p) func_001ACC38(D_002E6550, 3); return p;
}
} } // namespace M_func_001ABC68

namespace M_func_001ABCE8 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct K { int a, b; };
extern const struct LONGU D_002A6C88;
#undef D_003E0EA8
#define D_003E0EA8 (*(char (*)[])((char *)base_D_003DD490 + 0x3A18))   /* g_sw+0x3A18: SW::savevars */
struct S { char pad[0xFD50]; int xfd50; int pad2[2]; char xfd5c[4]; };
void ResetRumbleSaveVar(struct S *p);
void func_001D84B0(void *p, int n);
void CreateMemcardThread(void *p, long k);
/** @sly2-annot addr=0x1ABCE8
 * Purpose: Build GS mask (BuildMemcardGsMask) and CreateMemcardThread for a save
 * Info: proposed name StartSaveJobA (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABCE8(struct S *p)
{
    ResetRumbleSaveVar(p);
    func_001D84B0(D_003E0EA8, p->xfd50);
    CreateMemcardThread(p->xfd5c, D_002A6C88.l);
}
} } // namespace M_func_001ABCE8
#undef D_003E0EA8
#define D_003E0EA8 (*(int *)((char *)base_D_003DD490 + 0x3A18))   /* g_sw+0x3A18: SW::savevars */

namespace M_func_001ABD40 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct K { int a, b; };
extern const struct LONGU D_002A6C90;
#undef D_003E0EA8
#define D_003E0EA8 (*(char (*)[])((char *)base_D_003DD490 + 0x3A18))   /* g_sw+0x3A18: SW::savevars */
struct S { char pad[0xFD50]; int xfd50; int pad2[2]; char xfd5c[4]; };
void ResetRumbleSaveVar(struct S *p);
void func_001D84B0(void *p, int n);
void CreateMemcardThread(void *p, long k);
/** @sly2-annot addr=0x1ABD40
 * Purpose: Same as 0x1ABCE8 with other job
 * Info: proposed name StartSaveJobB (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABD40(struct S *p)
{
    ResetRumbleSaveVar(p);
    func_001D84B0(D_003E0EA8, p->xfd50);
    CreateMemcardThread(p->xfd5c, D_002A6C90.l);
}
} } // namespace M_func_001ABD40
#undef D_003E0EA8
#define D_003E0EA8 (*(int *)((char *)base_D_003DD490 + 0x3A18))   /* g_sw+0x3A18: SW::savevars */

namespace M_func_001ABD98 { extern "C" {
struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0x3B54]; int x3B54; };
struct H { char pad[0x4C]; int x4C; };
struct S { char pad[0xFD50]; void *xFD50; };
extern struct G D_003DD490;
extern struct H D_002E6550;
extern int D_002DEB4C;
extern char D_003D4A60[], D_002E65A0[];
int func_0020E040(struct G *g);
int NWorldCur(void);
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001ABCE8(struct S *s);
void func_001ADE38(void *p);
/** @sly2-annot addr=0x1ABD98
 * Purpose: If a slot is current and not in transition: eval Splice flag; if card missing show UI state 0x22
 *   else start save job
 * Info: proposed name AutoSaveGame (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABD98(struct S *s, int force)
{
    struct G *g;
    struct CRef ref;
    int t;
    if (!s->xFD50)
        return;
    g = &D_003DD490;
    if (func_0020E040(g))
        return;
    if (D_002DEB4C)
        return;
    if (!NWorldCur() && !force)
        return;
    RefGetGlobalSaveVar(&ref, g->x3B54, D_003D4A60);
    t = ref.m_tag;
    Decref__4CRef(&ref);
    if (!t) {
        D_002E6550.x4C = 3;
        SetMcDialogState(&D_002E6550, 0x22);
    } else {
        func_001ABCE8(s);
        func_001ADE38(D_002E65A0);
    }
}
} } // namespace M_func_001ABD98

namespace M_ReadMcDir { extern "C" {
typedef struct { char a[0x40]; } ENT;
typedef struct { char a[0x5C]; } OUT;
extern char D_002E6500[];
extern ENT D_002E6080[];
unsigned int func_0021ED28(const char *) __asm__("strlen");
char *strcpy(char *, const char *);
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
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
int ReadMcDir(int *p, char *path, int c, OUT *aout, int *pc)
{
    int f;
    int r;
    int i;
    if (c > 0x12)
        return 0x25;
    if (path) {
        if (func_0021ED28(path) > 0x40)
            return 0xD;
        strcpy(D_002E6500, path);
        f = 0;
    } else {
        f = 1;
        path = D_002E6500;
    }
    func_002333B4(D_002E6080, 0, 0x480);
    r = McSyncResult((char *)p + 0xFD5C, sceMcGetDir(p[0], p[1], path, f, c, D_002E6080), 0xD, pc);
    if (r)
        return r;
    for (i = 0; i < *pc; i++)
        FillMcDirInfo(&D_002E6080[i], &aout[i]);
    return 0;
}
} } // namespace M_ReadMcDir

namespace M_ReadMcDirWrapper { extern "C" {
int ReadMcDir(int a, int b, int c, int d, int *pn);
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
int ReadMcDirWrapper(int a, int b, int c)
{
    int n;
    int r = ReadMcDir(a, b, 1, c, &n);
    if (r) return r;
    return n == 1 ? 0 : 0xE;
}
} } // namespace M_ReadMcDirWrapper

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
void *func_002333B4(void *, int, unsigned int) __asm__("memset");
void ConvertPchzToSjis(char *, unsigned char *);
int McWriteFile(void *s, char *name, void *b, int n);

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
int WriteIconSys(void *p, char *dir, char *title)
{
    ICONSYS icon;
    char name[0x50];
    func_002333B4(&icon, 0, sizeof(icon));
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
    ConvertPchzToSjis(title, icon.TitleName);
    strcpy((char *)icon.FnameView, "memcard_icon.ico");
    strcpy((char *)icon.FnameCopy, "memcard_icon.ico");
    strcpy((char *)icon.FnameDel, "memcard_icon.ico");
    sprintf(name, "%s/%s", dir, "icon.sys");
    return McWriteFile(p, name, &icon, sizeof(icon));
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
extern "C" void load_sly_icon_from_disc(int n0, int n1)
{
    load_file_from_disc(n0, n1, D_002A38E8, (char *)D_002A6CA0);
}

namespace M_load_file_from_disc {
typedef unsigned int uint;
extern "C" void STACKSCOPE_ctor_1AE4A0(STACKSCOPE*,int) __asm__("_$_10STACKSCOPE");
struct REQUEST {uint bytes,alignment;};
struct X;
extern "C" void func_00258248(X *px);
struct X { uint x0; uint x4; int x8; char pad[0x110 - 0xC]; X() { x8 = 2; func_00258248(this); } };
struct Y;extern "C" void func_00184950(Y*,int);
struct Y {int flags;unsigned char*cursor;int left,consumed;unsigned char*raw;int rawBytes,decompFlags,pending,word20;unsigned char*window;int word28,word2C;void*vt;int capacity;unsigned char*buffer;int handle3C,word40,word44;uint word48;int word4C,word50,word54,word58,word5C,word60;char tail64[0xC];Y(int cb,void*pv,int flags) __asm__("func_001848A8");~Y(){func_00184950(this,2);}};
struct K {uint x0,x4;char pad[0x34-8];uint x34;int pad2;uint x3C;};
extern "C" {
extern char D_002A6CB8[];
int sprintf(char *buf, const char *fmt, ...);
int McOpen(void *p, char *name, int mode, int *ph);
void *func_001AE318(REQUEST *pmsg);
int func_00184D60(Y *py, X *px);
int McWrite(void *p, int h, char *buf, int n, int *pn);
int CbRead__18CBinaryInputStreamiPv(Y *py, int cb, char *buf);
void CStreamBis_Close(Y *py);
int McClose(void *p, int h);
int load_file_from_disc(void *p, const char *a, K *k, const char *b);
}
static inline int SetKey(X *px, K *k)
{
    if (!k)
        return 0;
    uint v;
    uint k34 = k->x34;
    uint k3C = k->x3C;
    px->x8 = 2;
    px->x0 = (k->x0 ^ k34) + 0xCA0;
    v = k->x4 ^ k3C;
    px->x4 = v;
    return v != 0;
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char REQUEST_bytes_Offset[FIELD_OFFSET(REQUEST,bytes)==0?1:-1];
typedef char REQUEST_alignment_Offset[FIELD_OFFSET(REQUEST,alignment)==4?1:-1];
typedef char X_x0_Offset[FIELD_OFFSET(X,x0)==0?1:-1];
typedef char X_x4_Offset[FIELD_OFFSET(X,x4)==4?1:-1];
typedef char X_x8_Offset[FIELD_OFFSET(X,x8)==8?1:-1];
typedef char K_x0_Offset[FIELD_OFFSET(K,x0)==0?1:-1];
typedef char K_x4_Offset[FIELD_OFFSET(K,x4)==4?1:-1];
typedef char K_x34_Offset[FIELD_OFFSET(K,x34)==52?1:-1];
typedef char K_x3C_Offset[FIELD_OFFSET(K,x3C)==60?1:-1];
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
int load_file_from_disc(void *p, const char *a, K *k, const char *b)
{
    STACKSCOPE frame;
    char name[0x50];
    int ret;
    sprintf(name, D_002A6CB8, a, b);
    int h = -1;
    ret = McOpen(p, name, 0x202, &h);
    if (ret == 0) {
        X x;
        REQUEST msg;
        REQUEST *pmsg = &msg;
        pmsg->bytes = 0x403F;
        pmsg->alignment = 0x10;
        void *pv = func_001AE318(pmsg);
        Y y(0x403F, pv, 0);
        if (!SetKey(&x, k) || !func_00184D60(&y, &x))
            ret = 0x27;
        else {
            char buf[0x1000];
            int n;
            int out;
            while ((n = CbRead__18CBinaryInputStreamiPv(&y, 0x1000, buf)) != 0) {
                ret = McWrite(p, h, buf, n, &out);
                if (ret)
                    goto done;
                if (out != n) {
                    ret = 7;
                    goto done;
                }
            }
            ret = 0;
        }
    done:
        CStreamBis_Close(&y);
        int r2 = McClose(p, h);
        if (!ret)
            ret = r2;
    }
    return ret;
}
} // namespace M_load_file_from_disc
#undef FIELD_OFFSET

namespace M_WriteSaveSlotFile { extern "C" {
struct B { int a[20]; };
void BuildSaveFilePath(void *a, void *b, struct B *pb);
void McWriteFile(void *a, struct B *pb, void *b, int n);
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
void WriteSaveSlotFile(void *a, void *b)
{
    struct B buf;
    BuildSaveFilePath(a, b, &buf);
    McWriteFile(a, &buf, b, 0x5408);
}
} } // namespace M_WriteSaveSlotFile

namespace M_WriteSaveSlotFileReset { extern "C" {
struct B { char pad[0x5408]; char x5408[0x44]; int x544C; int x5450; };
void func_001D8D80(struct B *p, int n);
void BuildSaveFilePath(void *s, void *b, char *name);
int McWriteFile(void *s, char *name, void *b, int n);
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
int WriteSaveSlotFileReset(void *s, struct B *b)
{
    char name[0x50];
    func_001D8D80(b, 0);
    BuildSaveFilePath(s, b, name);
    return McWriteFile(s, name, b, 0x5408);
}
} } // namespace M_WriteSaveSlotFileReset

namespace M_func_001AC4C8 { extern "C" {
struct B { char pad[0x5408]; char x5408[0x44]; int x544C; int x5450; };
void func_002333B4(void *p, int c, int n) __asm__("memset");
void BuildSaveFilePath(void *s, void *b, char *name);
int ReadMcDirWrapper(void *s, char *name, void *p);
int McReadFile(void *s, char *name, void *b, int n);
/** @sly2-annot addr=0x1AC4C8
 * Purpose: For each slot: BuildSaveFilePath, read dir entry, McReadFile
 * Info: proposed name ReadSaveSlotHeaders (T2); TU memcard
 * Notes: names_v2 v1 proposed McWriteFile (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001AC4C8(void *s, struct B *b)
{
    char name[0x50];
    int r;
    func_002333B4(b, 0, 0x5464);
    BuildSaveFilePath(s, b, name);
    r = ReadMcDirWrapper(s, name, b->x5408);
    if (r)
        return r;
    if ((b->x5450 & 0xD4) != 4)
        return 0x14;
    if (b->x544C != 0x5408)
        return 0x1C;
    return McReadFile(s, name, b, 0x5408);
}
} } // namespace M_func_001AC4C8

/** @sly2-annot addr=0x1AC570
 * Purpose: Thin wrapper over ResetGsIfSchemaChanged for a slot
 * Info: proposed name ResetSaveSlot (T2); TU memcard
 * Notes: names_v2 C proposed Clear__13CFileLocation (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" int func_001AC570(SAVE *psave, void *pslot)
{
    ResetGsIfSchemaChanged(&D_003E0EA8);
    return 0;
}

namespace M_McJobGetInfo { extern "C" {
struct S { int x0; int x4; int x8; int xC; int x10; float x14; int x18; int x1C; int x20; char pad2[0xFD5C - 0x24]; char xFD5C[1]; };
#undef D_002DDF0C
#define D_002DDF0C (*(float *)((char *)base_D_002DDED0 + 0x3C))   /* g_clock (CLOCK)+0x3C */
int sceMcGetInfo(int a, int b, int *c, int *d, int *e);
int McSyncResult(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1AC598
 * Purpose: Thread job: sceMcGetInfo + sync -> card type/free/format state
 * Info: proposed name McJobGetInfo (T1); TU memcard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McJobGetInfo(struct S *s)
{
    void *p = s->xFD5C;
    int h;
    int r;
    h = sceMcGetInfo(s->x0, s->x4, &s->x18, &s->x1C, &s->x20);
    r = McSyncResult(p, h, 1, 0);
    switch (r) {
    case 11:
        s->xC = 1;
        break;
    case 9:
        s->xC = 3;
        break;
    case 10:
    case 12:
        r = 12;
        s->xC = 2;
        break;
    case 0:
        if (s->x20) { if (s->xC < 3) s->xC = 3; } else s->xC = 2;
        break;
    }
    if (r == 0 || r == 12) {
        if (s->x18 != 2) {
            r = 11;
            s->xC = 1;
        }
    }
    s->x14 = D_002DDF0C + 0.3f;
    return r;
}
} } // namespace M_McJobGetInfo
#undef D_002DDF0C

namespace M_func_001AC6A0 { extern "C" {
struct S { int x0; int x4; int pad[4]; int x18; int x1C; int x20; char pad2[0xFD5C - 0x24]; char xFD5C[1]; };
int sceMcGetInfo(int a, int b, int *c, int *d, int *e);
int McSyncResult(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1AC6A0
 * Purpose: sceMcGetInfo + McSyncResult
 * Info: proposed name McGetInfoSync (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001AC6A0(struct S *s, int *out)
{
    void *p = s->xFD5C;
    int h;
    int r;
    h = sceMcGetInfo(s->x0, s->x4, &s->x18, &s->x1C, &s->x20);
    r = McSyncResult(p, h, 1, 0);
    switch (r) {
    case 9:
        r = 0;
        break;
    case 10:
        r = 12;
        break;
    }
    if (r == 0)
        *out = s->x1C << 10;
    else
        *out = 0;
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
extern "C" int McMkdir(SAVE *psave, const char *pchz)
{
    int nResult = McSyncResult(&psave->memcard, sceMcMkdir(psave->port, psave->slot, pchz), 0xB, 0);
    return ((nResult ^ 0x18) == 0) ? 0 : nResult;
}

/** @sly2-annot addr=0x1AC7A0
 * Purpose: sceMcOpen wrapper + sync
 * Info: proposed name McOpen (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void McOpen(SAVE *psave, const char *pchz, int mode, int n)
{
    McSyncResult(&psave->memcard, sceMcOpen(psave->port, psave->slot, pchz, mode), 2, n);
}

/** @sly2-annot addr=0x1AC808
 * Purpose: sceMcClose wrapper + sync
 * Info: proposed name McClose (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void McClose(SAVE *psave, int fd)
{
    McSyncResult(&psave->memcard, sceMcClose(fd), 3, 0);
}

namespace M_McRead { extern "C" {
int sceMcRead(int a, int b, int c);
void McSyncResult(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char x[4]; };
/** @sly2-annot addr=0x1AC850
 * Purpose: sceMcRead + McSyncResult
 * Info: proposed name McRead (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McRead(struct S *p, int a, int b, int c, int d)
{
    McSyncResult(p->x, sceMcRead(a, b, c), 5, d);
}
} } // namespace M_McRead

namespace M_McWrite { extern "C" {
int sceMcWrite(int a, int b, int c);
void McSyncResult(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char x[4]; };
/** @sly2-annot addr=0x1AC8A8
 * Purpose: sceMcWrite + McSyncResult
 * Info: proposed name McWrite (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McWrite(struct S *p, int a, int b, int c, int d)
{
    McSyncResult(p->x, sceMcWrite(a, b, c), 6, d);
}
} } // namespace M_McWrite

namespace M_McWriteFile { extern "C" {
int McOpen(void *s, void *a, int n, int *out);
int McWrite(void *s, int h, void *b, int c, int *out);
void McClose(void *s, int h);
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
int McWriteFile(void *s, void *a, void *b, int c)
{
    int h = -1;
    int v;
    int r = McOpen(s, a, 0x202, &h);
    if (r)
        return r;
    r = McWrite(s, h, b, c, &v);
    if (!r && v != c)
        r = 7;
    McClose(s, h);
    return r;
}
} } // namespace M_McWriteFile

namespace M_McReadFile { extern "C" {
int McOpen(void *s, void *a, int n, int *out);
int McRead(void *s, int h, void *b, int c, int *out);
void McClose(void *s, int h);
/** @sly2-annot addr=0x1AC9A0
 * Purpose: McOpen + McRead + McClose
 * Info: proposed name McReadFile (T1); TU memcard
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McReadFile(void *s, void *a, void *b, int c)
{
    int h = -1;
    int v;
    int r = McOpen(s, a, 1, &h);
    if (r)
        return r;
    r = McRead(s, h, b, c, &v);
    if (!r && v != c)
        r = 8;
    McClose(s, h);
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
extern "C" int McDelete(SAVE *psave, const char *pchz)
{
    int nResult = McSyncResult(&psave->memcard, sceMcDelete(psave->port, psave->slot, pchz), 0xF, 0);
    return ((nResult ^ 0xE) == 0) ? 0 : nResult;
}

/** @sly2-annot addr=0x1ACA98
 * Purpose: ctor: UI base ctor 0x1DA728, vptr 0x2D3750, id 0x52C, state x40=-1, register 0x1DA7D0
 * Info: proposed name CMemcardDialog_ctor (T1); class CMemcardDialog; TU memcard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" MCDLG *CMemcardDialog_ctor(MCDLG *pdlg)
{
    func_001DA728(pdlg);
    pdlg->pvt = D_002D3750;
    pdlg->id = 0x52C;
    pdlg->nState = -1;
    func_001DA7D0(pdlg);
    return pdlg;
}

/** @sly2-annot addr=0x1ACAE8
 * Purpose: UI slot 12: base 0x1DA770 then SetMcDialogState
 * Info: proposed name CMemcardDialog_vt12 (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* CMemcardDialog slot 12. */
extern "C" void func_001ACAE8(MCDLG *pdlg)
{
    MCCHILD *pchild;
    func_001DA770(pdlg);
    pchild = pdlg->pchild;
    if (pchild != 0) {
        pchild->fB0 = 1;
        if (pdlg->nState != -1) {
            SetMcDialogState(pdlg, pdlg->nState);
        }
    }
}

/** @sly2-annot addr=0x1ACB38
 * Purpose: UI slot 13: forwards to 0x1DA7B0
 * Info: proposed name CMemcardDialog_vt13 (T2); class CMemcardDialog; TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* CMemcardDialog slot 13. */
extern "C" void func_001ACB38(MCDLG *pdlg)
{
    func_001DA7B0(pdlg);
}

namespace M_SetMcDialogState { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x14]; struct O *x24; char pad3[0x18]; int x40; };
/** @sly2-annot addr=0x1ACB58
 * Purpose: x40 = state; if shown (x24) call vt10 and child vt49(-1)
 * Info: proposed name SetMcDialogState (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions, docs/engine/save-memcard.md#open-questions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void SetMcDialogState(struct O *p, int a)
{
    p->x40 = a;
    if (p->x24) {
        { struct VTE *e = &p->vt[10]; e->pfn((char *)p + e->d); }
        { struct O *o = p->x24; struct VTE *e = &o->vt[49]; e->pfn((char *)o + e->d, -1); }
    }
}
} } // namespace M_SetMcDialogState

namespace M_ClearMcDialogState { extern "C" {
struct VTE { short d; short pad; void (*pfn)(...); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x14]; struct O *x24; char pad3[0x18]; int x40; };
/** @sly2-annot addr=0x1ACBB8
 * Purpose: x40=-1; vt11 and child vt50
 * Info: proposed name ClearMcDialogState (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void ClearMcDialogState(struct O *p)
{
    p->x40 = -1;
    if (p->x24) {
        { struct VTE *e = &p->vt[11]; e->pfn((char *)p + e->d); }
        { struct O *o = p->x24; struct VTE *e = &o->vt[50]; e->pfn((char *)o + e->d); }
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
/* slot 14: set the mode (stamping the real time). */
extern "C" void func_001ACC18(MCDLG *pdlg, int nMode)
{
    if (nMode != pdlg->nMode) {
        pdlg->nMode = nMode;
        pdlg->tMode = D_002DDED0.tReal;
    }
}

namespace M_func_001ACC38 { extern "C" {
struct H { char pad[0xFD50]; int x; };
#undef D_002E65E0
#define D_002E65E0 (*(struct H *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
struct CLOCK { float t; float dt; char pad[0x34]; float t3c; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x44]; int x44; float x48; };
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
void func_001ACC38(struct S *p, int n)
{
    if (n == p->x44) return;
    switch (n) { case 2: case 3: D_002E65E0.x = 0; }
    p->x44 = n;
    p->x48 = D_002DDED0.t3c;
}
} } // namespace M_func_001ACC38
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */

namespace M_func_001ACC80 { extern "C" {
struct S { char pad[0x40]; int x40; };
/** @sly2-annot addr=0x1ACC80
 * Purpose: State -> 1 (one-button) / 2 (two-choice) / 0
 * Info: proposed name KMcDialogButtons (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001ACC80(struct S *p)
{
    switch (p->x40) {
    case 0x11: case 0x14: case 0x17: case 0x18: case 0x1B: case 0x1D: case 0x22: case 0x23: case 0x24: case 0x2E:
        return 1;
    case 0x12: case 0x15: case 0x1C: case 0x20:
        return 2;
    }
    return 0;
}
} } // namespace M_func_001ACC80

namespace M_CMemcardDialog_Update { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short index; void (*pfn)(void *); };
struct P { char pad[0x58]; int x58; float x5C; int pad2; int x64; };
struct S { char pad[0x10]; void *x10; char pad2[0x24 - 0x14]; struct P *x24; char pad3[0x40 - 0x28]; int x40; int x44; int pad4; int x4C; };
struct MC { char pad[8]; int x8; char pad2[0xFD58 - 0xC]; int xFD58; char pad3[0xFD6C - 0xFD5C]; int xFD6C; };
struct CLOCK { char pad[0x3C]; float t3c; };
#undef D_002E65E0
#define D_002E65E0 (*(struct MC *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern char D_003D9ED0[];
extern char D_002F633C[];
extern const struct LONGU D_002A6D88;
int func_0020E040(void *g);
int func_001ACC80(struct S *s);
int GrfHandleJoyBtn(void *p, int n);
void func_001AD168(struct S *s);
void func_001ACFE0(struct S *s);
int FMemcardThreadValid__FP7MEMCARD(void *p);
void CreateMemcardThread(void *p, long pmf);
void SetMcDialogState(struct S *s, int n);
void ClearMcDialogState(struct S *s);
void func_001660F8(void);
void func_001AF660(void *p);
/** @sly2-annot addr=0x1ACCC8
 * Purpose: vt7: when dialog fully shown >0.1s handle pad input (0x187D80) per KMcDialogButtons, start jobs
 *   (CreateMemcardThread), transition (0x1ACFE0)
 * Info: proposed name CMemcardDialog_Update (T1); class CMemcardDialog; TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void CMemcardDialog_Update(struct S *s)
{
    struct P *p = s->x24;
    int c;
    int r;
    if (!p)
        return;
    if (p->x58 != 2)
        return;
    c = p->x64;
    if (c != 2)
        return;
    if (func_0020E040(D_003DD490))
        return;
    if (D_002DDED0.t3c - s->x24->x5C <= 0.1f)
        return;
    r = func_001ACC80(s);
    if (r != 1) {
        if (r == c) {
            if (s->x10 && GrfHandleJoyBtn(s->x10, 0x80)) {
                func_001AD168(s);
                return;
            }
            if (s->x10 && GrfHandleJoyBtn(s->x10, 0x10)) {
                func_001ACFE0(s);
                return;
            }
        }
    } else {
        if (s->x10 && GrfHandleJoyBtn(s->x10, 0x80)) {
            func_001ACFE0(s);
            return;
        }
    }
    switch (s->x40) {
    case 0x20:
    case 0x22:
        if (s->x44 == 1)
            return;
        ClearMcDialogState(s);
        return;
    case 0x11:
    case 0x12:
    case 0x15:
    case 0x18:
    case 0x2E:
        if (D_002E65E0.x8 != 6)
            return;
        ClearMcDialogState(s);
        return;
    case 0x13:
        if (FMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            CreateMemcardThread(D_002F633C, D_002A6D88.l);
            SetMcDialogState(s, 0x16);
        } else
            SetMcDialogState(s, 0x14);
        return;
    case 0x16:
        if (FMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            ClearMcDialogState(s);
            return;
        }
        SetMcDialogState(s, 0x17);
        return;
    case 0x1A:
        if (FMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        if (!D_002E65E0.xFD6C) {
            ClearMcDialogState(s);
            switch (s->x4C) {
            case 1:
                func_001660F8();
                return;
            case 2:
                func_001AF660(D_003D9ED0);
                return;
            }
            return;
        }
        if (s->x4C == 3)
            SetMcDialogState(s, 0x23);
        else
            SetMcDialogState(s, 0x1D);
        return;
    case 0x21:
        if (FMemcardThreadValid__FP7MEMCARD(D_002F633C))
            return;
        D_002E65E0.xFD58 = 0;
        if (!D_002E65E0.xFD6C) {
            ClearMcDialogState(s);
            return;
        }
        SetMcDialogState(s, 0x1D);
        return;
    }
}
} } // namespace M_CMemcardDialog_Update
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */

namespace M_func_001ACFE0 {
// Game-state transition handler: switch on the new state id (x40) and run the menu/HUD hooks for it.
// State 34 sends an option message (CRef true) to the global option object D_003E0FE4.
//
// Match notes: the three-argument call to RefSetGlobalSaveVar uses the same idiom as func_0013D328 / ResetRumbleSaveVar:
// SetPv receives the context at the call site (evaluated before the CRef copy) but its body names
// D_003D4A60 again after the copy. That is what puts `lui a3` after the copy ctor yet keeps a3's low half
// ahead of the a2 setup (the scheduler boosts insns that set a register exactly once in the function).
class CRef;
extern "C" void Decref__4CRef(CRef *pref);                       // ~CRef
extern "C" CRef *func_001291A8(CRef *, const CRef *);            // CRef copy ctor, returns this
class CRef {
public:
    int m_tagk;
    int m_tag;
    CRef(bool f) { m_tagk = 0x17; m_tag = f; }
    CRef(const CRef &ref) { func_001291A8(this, &ref); }
    ~CRef() { Decref__4CRef(this); }
};
class O;
struct S { char pad[0x40]; int x40; int x44; float x48; int x4C; };   // x40 = state id, x4C = counter
extern "C" {
extern char D_003DA530[], D_003D9ED0[], D_003D4A60[];
#undef D_002E65E0
#define D_002E65E0 (*(char (*)[])((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */

#undef D_003E0FE4
#define D_003E0FE4 (*(O * *)((char *)base_D_003DD490 + 0x3B54))   /* g_sw+0x3B54: SW::psvpAutosaveWarned */
int FScreenShown(void *p);
int func_001B0160(void *p);
void ClearMcDialogState(S *s);
void func_001ACC38(S *s, int n);
void func_001AF660(void *p);
void StartNewGame(void *p);
int func_001B0108(void *p);
void func_001AF608(void *p);
void SetMcDialogState(S *s, int n);
void func_001ABD40(void *p);
void func_001ACFE0(S *s);
CRef RefSetGlobalSaveVar(O *, const CRef &, void *);                   // O::Set(const CRef &, context), returns a CRef
}
static inline void SetPv(O *po, const CRef &r, void *) { CRef arg(r); const CRef &ret = RefSetGlobalSaveVar(po, arg, D_003D4A60); }
/** @sly2-annot addr=0x1ACFE0
 * Purpose: Switch on dialog state x40 and run follow-up (UI hooks, SAVE jobs, 0x1ABD40)
 * Info: proposed name OnMcDialogChoice (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ACFE0(S *s)
{
    switch (s->x40) {
    case 17: case 18: case 21: case 24:
        {
            void *g = D_003DA530;
            if (FScreenShown(g) && func_001B0160(D_003D9ED0)) {
                ClearMcDialogState(s);
                func_001ACC38(s, 2);
                func_001AF660(D_003D9ED0);
                StartNewGame(g);
                break;
            }
        }
        /* fallthrough */
    case 20: case 23: case 29: case 35: case 36: case 46:
        ClearMcDialogState(s);
        func_001ACC38(s, 2);
        if (func_001B0108(D_003D9ED0))
            func_001AF608(D_003D9ED0);
        break;
    case 27: case 28: case 32:
        ClearMcDialogState(s);
        break;
    case 34:
        {
            O *po = D_003E0FE4;
            CRef ref(true);
            SetPv(po, ref, D_003D4A60);
        }
        if (s->x4C < 4) {
            if (s->x4C > 0) {
                SetMcDialogState(s, 0x1A);
                func_001ABD40(D_002E65E0);
            }
        }
        break;
    }
}
} // namespace M_func_001ACFE0
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FE4

namespace M_func_001AD168 { extern "C" {
struct LONGU { long l; } __attribute__((packed));   /* B4: the 8-byte struct passed as a packed long */
struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { char pad[0x40]; int x40; };
extern const struct LONGU D_002A6E88, D_002A6D88, D_002A6E90;
extern char D_002F633C[];
void func_001AD3E0(void);
void CreateMemcardThread(void *t, long pmf);
void SetMcDialogState(struct S *s, int n);
/** @sly2-annot addr=0x1AD168
 * Purpose: Confirm path: 0x1AD3E0 or CreateMemcardThread
 * Info: proposed name McDialogConfirm (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD168(struct S *s)
{
    switch (s->x40) {
    case 0x1C:
        func_001AD3E0();
        break;
    case 0x12:
        CreateMemcardThread(D_002F633C, D_002A6E88.l);
        SetMcDialogState(s, 0x13);
        break;
    case 0x15:
        CreateMemcardThread(D_002F633C, D_002A6D88.l);
        SetMcDialogState(s, 0x16);
        break;
    case 0x20:
        CreateMemcardThread(D_002F633C, D_002A6E90.l);
        SetMcDialogState(s, 0x21);
        break;
    }
}
} } // namespace M_func_001AD168

namespace M_func_001AD250 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *xFD50; int xFD54; struct B *xFD58; };
struct S { int x0, x4, x8; char pad[0x10]; int x1C; };
#undef D_002E65E0
#define D_002E65E0 (*(struct G *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FAC
#define D_003E0FAC (*(int *)((char *)base_D_003DD490 + 0x3B1C))   /* g_sw+0x3B1C: SW::psvpDt */
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void func_001D8D80(struct B *b, int n);
void func_00165E38(struct B *b);
void *PsndStartSound(int n);
void SetSndSourceLo(void *p, int n);
/** @sly2-annot addr=0x1AD250
 * Purpose: Menu action slot i: if slot empty (Splice eval) make it current and start (0x1D8D80, 0x165E38)
 *   else error sound 0x36
 * Info: proposed name SelectSaveSlotNew (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD250(struct S *s)
{
    struct CRef ref;
    struct B *p = &D_002E65E0.a[s->x8];
    float f;
    RefGetGlobalSaveVar(&ref, D_003E0FAC, p);
    f = ref.m_tag;
    Decref__4CRef(&ref);
    if (f == 0.0f) {
        D_002E65E0.xFD50 = p;
        func_001D8D80(p, 1);
        func_00165E38(D_002E65E0.xFD50);
    } else {
        SetSndSourceLo(PsndStartSound(0x36), 0);
        if (s)
            s->x1C |= 2;
    }
}
} } // namespace M_func_001AD250
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FAC

namespace M_func_001AD320 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *xFD50; int xFD54; struct B *xFD58; };
struct S { int x0, x4, x8; char pad[0x10]; int x1C; };
#undef D_002E65E0
#define D_002E65E0 (*(struct G *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FAC
#define D_003E0FAC (*(int *)((char *)base_D_003DD490 + 0x3B1C))   /* g_sw+0x3B1C: SW::psvpDt */
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void func_001D8D80(struct B *b, int n);
void func_00165E38(struct B *b);
/** @sly2-annot addr=0x1AD320
 * Purpose: Menu action slot i: make current, ResetGs, start (0x165E38)
 * Info: proposed name SelectSaveSlotLoad (T2); TU memcard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD320(struct S *s)
{
    struct CRef ref;
    struct B *p = &D_002E65E0.a[s->x8];
    float f;
    RefGetGlobalSaveVar(&ref, D_003E0FAC, p);
    f = ref.m_tag;
    Decref__4CRef(&ref);
    D_002E65E0.xFD50 = p;
    if (f == 0.0f) {
        func_001D8D80(p, 1);
        func_00165E38(D_002E65E0.xFD50);
    } else {
        func_00165E38(p);
        if (s)
            s->x1C |= 2;
    }
}
} } // namespace M_func_001AD320
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FAC

namespace M_func_001AD3E0 { extern "C" {
struct CRef { int m_tagk; int m_tag; };
struct G { char pad[0xFD50]; int xFD50; int xFD54; int xFD58; };
struct H { char pad[0x4C]; int x4C; };
#undef D_002E65E0
#define D_002E65E0 (*(struct G *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
extern struct H D_002E6550;
#undef D_003E0FE4
#define D_003E0FE4 (*(int *)((char *)base_D_003DD490 + 0x3B54))   /* g_sw+0x3B54: SW::psvpAutosaveWarned */
extern char D_003D4A60[];
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001ABD40(struct G *g);
/** @sly2-annot addr=0x1AD3E0
 * Purpose: Eval slot, set dialog state, start save job (0x1ABD40)
 * Info: proposed name SaveToSlot (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD3E0(void)
{
    struct G *g = &D_002E65E0;
    struct H *h = &D_002E6550;
    struct CRef ref;
    int t;
    g->xFD50 = g->xFD58;
    h->x4C = 2;
    RefGetGlobalSaveVar(&ref, D_003E0FE4, D_003D4A60);
    t = ref.m_tag;
    Decref__4CRef(&ref);
    if (!t) {
        SetMcDialogState(h, 0x22);
    } else {
        SetMcDialogState(h, 0x1A);
        func_001ABD40(g);
    }
}
} } // namespace M_func_001AD3E0
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FE4

namespace M_func_001AD480 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD58 - 0x24 - 3 * 0x5464]; struct B *xFD58; };
struct H { char pad[0x4C]; int x4C; };
struct S { int x0, x4, x8; char pad[0x10]; int x1C; };
#undef D_002E65E0
#define D_002E65E0 (*(struct G *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
extern struct H D_002E6550;
#undef D_003E0FAC
#define D_003E0FAC (*(int *)((char *)base_D_003DD490 + 0x3B1C))   /* g_sw+0x3B1C: SW::psvpDt */
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
void SetMcDialogState(struct H *h, int n);
void func_001AD3E0(void);
/** @sly2-annot addr=0x1AD480
 * Purpose: Menu action slot i (group 3): confirm overwrite or SaveToSlot
 * Info: proposed name SelectSaveSlotSave (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD480(struct S *s)
{
    struct CRef ref;
    struct B *p = &D_002E65E0.a[s->x8];
    float f;
    RefGetGlobalSaveVar(&ref, D_003E0FAC, p);
    f = ref.m_tag;
    Decref__4CRef(&ref);
    D_002E65E0.xFD58 = p;
    if (f == 0.0f)
        func_001AD3E0();
    else
        SetMcDialogState(&D_002E6550, 0x1C);
    if (s)
        s->x1C |= 2;
}
} } // namespace M_func_001AD480
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003E0FAC

namespace M_func_001AD538 {
struct CRef;
extern "C" void Decref__4CRef(CRef *);
struct CRef { int m_tagk; float m_tag; CRef(const CRef &) __asm__("func_001291A8"); ~CRef() { Decref__4CRef(this); } };
struct B { char data[0x5464]; };
struct G { char pad[0x24]; struct B a[3]; char pad2[0xFD50 - 0x24 - 3 * 0x5464]; struct B *xFD50; };
struct GD { char pad[0x3B1C]; int x3B1C; };
struct E { char pad[0x14]; int x14; int pad2[1]; };
struct H { char pad[0x24]; int x24; };
extern "C" {
#undef D_002E65E0
#define D_002E65E0 (*(struct G *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
extern struct B D_002E6604[];
extern char D_002E6550[];
extern struct GD D_003DD490;
extern struct H D_003D9ED0;
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
void func_001ACC38(void *p, int n);
int IdPauseTopPage(void *g);
}
CRef Eval(int a, void *b) __asm__("RefGetGlobalSaveVar");
static inline float F(int x, void *pb) { return Eval(x, pb).m_tag; }
/** @sly2-annot addr=0x1AD538
 * Purpose: Menu hook: eval 3 slots' Splice values and update entries
 * Info: proposed name UpdateSaveMenuSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void func_001AD538(void)
{
    float a[3];
    struct E *pe;
    int i;
    func_001ACC38(D_002E6550, 1);
    if (D_003D9ED0.x24 <= 0)
        return;
    pe = &D_003D9EDC[D_003D9ED0.x24];
    {
        struct B *pb = D_002E6604;
        for (i = 0; i < 3; i++, pb++) {
            a[i] = F(D_003DD490.x3B1C, pb);
        }
    }
    switch (IdPauseTopPage(&D_003D9ED0)) {
    case 0x1E:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (a[j] == 0.0f) {
                    pe->x14 = j;
                    break;
                }
            }
        }
        break;
    case 0x1F:
        {
            int j;
            for (j = 0; j < 3; j++) {
                if (a[j] != 0.0f) {
                    pe->x14 = j;
                    break;
                }
            }
        }
        break;
    case 0x19:
        if (D_002E65E0.xFD50)
            pe->x14 = D_002E65E0.xFD50 - D_002E65E0.a;
        break;
    }
}
} // namespace M_func_001AD538
#undef D_002E65E0
#define D_002E65E0 (*(SAVE *)((char *)base_D_002E65A0 + 0x40))   /* memcard save block+0x40 */
#undef D_003D9EDC

namespace M_func_001AD740 { extern "C" {
struct E { int x0, x4; void *x8; int xc, x10; char *x14; int x18; };
struct G { int pad[9]; int x24; };
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
extern struct G D_003D9ED0;
extern int D_002E6594;
struct S { int x0, x4; char *x8; };
void func_001AF608(struct G *g);
int func_001B0108(struct G *g);
void PlayWidgetAnim(void *a, char *b);
/** @sly2-annot addr=0x1AD740
 * Purpose: Menu hook: set text id 0x5C8+i or 0x5C5+i for slot i
 * Info: proposed name SetSaveSlotLabel (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD740(struct S *s)
{
    struct E *e;
    if (D_002E6594 != 1) {
        func_001AF608(&D_003D9ED0);
        return;
    }
    if (!func_001B0108(&D_003D9ED0))
        return;
    e = &D_003D9EDC[D_003D9ED0.x24];
    if (e->x14 == s->x8)
        PlayWidgetAnim(e->x8, s->x8 + 0x5C8);
    else
        PlayWidgetAnim(e->x8, s->x8 + 0x5C5);
}
} } // namespace M_func_001AD740
#undef D_003D9EDC

/** @sly2-annot addr=0x1AD7E0
 * Purpose: Dynamic text provider (langstr 0x2F2): strcpy name lookup
 * Info: proposed name TextMcCardName (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* Dynamic text: the card name. */
extern "C" void func_001AD7E0(char *pch)
{
    strcpy(pch, PchzFindSwLangstr(&D_003DD490, D_002E6590));
}

namespace M_func_001AD820 { extern "C" {
extern char D_002E6550[], D_003DD490[];
int func_001ACC80(void *p);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
void strcpy1(char *a, const char *b);
/** @sly2-annot addr=0x1AD820
 * Purpose: Dynamic text provider (0x2F3) depending on dialog state
 * Info: proposed name TextMcDialog (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD820(char *buf)
{
    switch (func_001ACC80(D_002E6550)) {
    case 1:
        strcpy(buf, "&2S&.: ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x38));
        break;
    case 2:
        strcpy(buf, "&2S&. : ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x36));
        strcpy1(buf, "       ");
        strcpy1(buf, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x37));
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
extern "C" void func_001AD928(char *pch)
{
    strcpy(pch, PchzFindSwLangstr(&D_003DD490, IdPauseTopPage(D_003D9ED0)));
}

namespace M_func_001AD980 { extern "C" {
struct G { char pad[0x3A18]; char x3A18[1]; };
extern struct G D_003DD490;
extern char D_003DA530[], D_002A6EC8[], D_003D4A60[];
int FScreenShown(void *p);
void *NEpisodeBest(void);
int IdsFromWorld(void *p);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *NCompletionPercentGs(void *p, void *q);
void sprintf(char *buf, char *fmt, ...);
/** @sly2-annot addr=0x1AD980
 * Purpose: Text '%s : %d%%' with NCompletionPercentGs
 * Info: proposed name TextSaveCompletion (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD980(char *buf)
{
    char *a;
    struct G *g;
    char *fmt;
    if (FScreenShown(D_003DA530))
        return;
    fmt = D_002A6EC8;
    g = &D_003DD490;
    a = PchzFindSwLangstr__FP2SWi(g, IdsFromWorld(NEpisodeBest()));
    sprintf(buf, fmt, a, NCompletionPercentGs(g->x3A18, D_003D4A60));
}
} } // namespace M_func_001AD980

namespace M_func_001ADA20 { extern "C" {
struct CRef { int m_tagk; float m_tag; };
struct E { int x0, x4, x8, xC, x10; int x14; int x18; };
struct G { int pad[9]; int x24; };
struct GD { char pad[0x3B1C]; int x3B1C; };
struct B { char data[0x5464]; };
extern struct G D_003D9ED0;
#undef D_003D9EDC
#define D_003D9EDC (*(struct E (*)[])((char *)base_D_003D9ED0 + 0xC))   /* screen's object+0xC */
extern struct GD D_003DD490;
extern struct B D_002E6604[];
int func_001B0108(struct G *g);
int IdPauseTopPage(struct G *g);
void RefGetGlobalSaveVar(struct CRef *ret, int a, void *b);
void Decref__4CRef(struct CRef *);
char *PchzFindSwLangstr__FP2SWi(void *p, int n);
char *strcpy(char *d, const char *s);
char *strcpy1(char *d, const char *s);
/** @sly2-annot addr=0x1ADA20
 * Purpose: Dynamic text provider (0x5CD)
 * Info: proposed name TextSaveB (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ADA20(char *buf)
{
    struct G *g = &D_003D9ED0;
    struct GD *gd;
    struct CRef ref;
    float f;
    if (!func_001B0108(g))
        return;
    gd = &D_003DD490;
    {
    struct E *e = &D_003D9EDC[g->x24];
    RefGetGlobalSaveVar(&ref, gd->x3B1C, &D_002E6604[e->x14]);
    }
    f = ref.m_tag;
    Decref__4CRef(&ref);
    switch (IdPauseTopPage(g)) {
    case 0x1E:
        if (f != 0.0f)
            strcpy(buf, PchzFindSwLangstr__FP2SWi(gd, 0x25));
        else
            strcpy(buf, PchzFindSwLangstr__FP2SWi(gd, 0x26));
        break;
    case 0x1F:
        if (f != 0.0f)
            strcpy(buf, PchzFindSwLangstr__FP2SWi(gd, 0x27));
        else
            strcpy(buf, PchzFindSwLangstr__FP2SWi(gd, 0x26));
        break;
    case 0x19:
        strcpy(buf, PchzFindSwLangstr__FP2SWi(gd, 0x29));
        if (f != 0.0f) {
            strcpy1(buf, "\n");
            strcpy1(buf, PchzFindSwLangstr__FP2SWi(gd, 0x25));
        }
        break;
    }
}
} } // namespace M_func_001ADA20
#undef D_003D9EDC

namespace M_func_001ADBB8 { extern "C" {
extern char D_003D9ED0[], D_003DD490[], D_002A6EB8[];
int IdPauseTopPage(void *p);
void *PchzFindSwLangstr__FP2SWi(void *p, int n);
void strcpy(void *s, void *q);
void strcpy1(void *s, void *q);
/** @sly2-annot addr=0x1ADBB8
 * Purpose: Dynamic text provider (0x5CE) with ' : &2T&.' format
 * Info: proposed name TextSaveTime (T2); TU memcard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ADBB8(void *s)
{
    switch (IdPauseTopPage(D_003D9ED0)) {
    case 0x19:
    case 0x1E:
    case 0x1F:
        strcpy(s, PchzFindSwLangstr__FP2SWi(D_003DD490, 0x40));
        strcpy1(s, D_002A6EB8);
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
 extern B D_002E6604[];
 extern char D_003D9ED0[],D_002A6EE0[],D_002A6EC8[];
 void*RefGetGlobalSaveVar(REF*,void*,void*);
 void Decref__4CRef(REF*);
 void*STACKSCOPE_ctor(void*);
 void STACKSCOPE_ctor_1AE4A0(void*,int) __asm__("_$_10STACKSCOPE");
 void*PkeyAllocSaveKey(void*,...);
 int IdsFromWorld(int);
 void*PchzFindSwLangstr__FP2SWi(void*,int);
 int NCompletionPercentGs(void*,void*);
 int sprintf(char*,const char*,...);
 char*strcpy(char*,const char*);
 int IdPauseTopPage(void*);
}
static inline float GetG(REF&r,void*a,B*b){RefGetGlobalSaveVar(&r,a,b);float g=r.g;Decref__4CRef(&r);return g;}
static inline int GetN(REF&r,void*a,B*b){RefGetGlobalSaveVar(&r,a,b);int n=r.n;Decref__4CRef(&r);return n;}
/** @sly2-annot addr=0x1ADC40
 * Purpose: Per-slot text (0x5CF..0x5D1, arg=slot): episode best key + completion %
 * Info: proposed name TextSaveSlotSummary (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void func_001ADC40(char*buf,int idx){
 REF ref;
 G*pg=&D_003DD490;
 B*pb=&D_002E6604[idx];
 if(GetG(ref,pg->primary,pb)!=0.0f){
  int frame[4];int*pf=frame;STACKSCOPE_ctor(pf);
  void*value=PkeyAllocSaveKey(D_002A6EE0,0);
  int tag;
  if(value)tag=GetN(ref,value,pb);else tag=GetN(ref,pg->fallback,pb);
  sprintf(buf,D_002A6EC8,(char*)PchzFindSwLangstr__FP2SWi(&D_003DD490,IdsFromWorld(tag)),NCompletionPercentGs(D_003DD490.object,pb));
  STACKSCOPE_ctor_1AE4A0(frame,2);
 }else{
  switch(IdPauseTopPage(D_003D9ED0)){
   case 0x1E:case 0x1F:strcpy(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x44));break;
   case 0x19:strcpy(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x45));break;
  }
 }
}
} // namespace M_func_001ADC40

namespace M_func_001ADDF0 {
extern "C" void func_001DA7D0(void *p);
struct CBase {
    int a, b, c;
    CBase() __asm__("func_001DA728");
    virtual void f();
};
struct CDer : CBase {
    int x10, x14, x18, x1c, x20;
    CDer() __asm__("func_001ADDF0");
    virtual void f();
};
CDer::CDer()
{
    x20 = 0x5D4;
    func_001DA7D0(this);
}
} // namespace M_func_001ADDF0

namespace M_func_001ADE38 { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1ADE38
 * Purpose: small
 * Info: TU memcard
 * Notes: descriptive only: (McStatus helper) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r4_17F090
 * @end-sly2-annot */
void func_001ADE38(struct S *p) { struct O *o = p->po; if (o) { struct VTE *e = &o->vt[49]; e->pfn((char *)o + e->d, -1); } }
} } // namespace M_func_001ADE38

namespace M_HideMcDialogWidget { extern "C" {
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1ADE70
 * Purpose: small
 * Info: proposed name HideMcDialogWidget (T1); TU memcard
 * Notes: verifier PLAUSIBLE: (McStatus show/hide) -> HideMcDialogWidget
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void HideMcDialogWidget(struct S *p) { struct O *o = p->po; if (o) { struct VTE *e = &o->vt[50]; e->pfn((char *)o + e->d); } }
} } // namespace M_HideMcDialogWidget

namespace M_func_001ADEA8 {
struct O { char pad[0x58]; int x58; int pad2[2]; int x64; };
struct S { char pad[0x24]; O *po; };
/** @sly2-annot addr=0x1ADEA8
 * Purpose: child x58>=2 or x64>=2
 * Info: proposed name FMcStatusVisible (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" int func_001ADEA8(S *p)
{
    O *o = p->po;
    if (!o) return 0;
    if (o->x58 < 2 && o->x64 < 2) return 0; return 1;
}
} // namespace M_func_001ADEA8

/** @sly2-annot addr=0x1ADEE8
 * Purpose: vt7 (vtable 0x2D36D0): base 0x1DA808; when shown >1s and thread idle hide (0x1ADE70)
 * Info: proposed name CMcStatus_Update (T2); class CMcStatus; TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
/* CMcStatus slot 7: hide once shown for over a second and the worker is idle. */
extern "C" void func_001ADEE8(MCDLG *pdlg)
{
    MCCHILD *pchild;
    func_001DA808(pdlg);
    pchild = pdlg->pchild;
    if (pchild != 0) {
        if (pchild->n58 == 2 && pchild->n64 == pchild->n58 && D_002DDED0.tReal - pchild->t5C > 1.0f &&
            FMemcardThreadValid(&D_002F633C) == 0) {
            HideMcDialogWidget(pdlg);
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
/* slot 14: set the mode (stamping the real time). */
extern "C" void func_001ADF78(MCDLG *pdlg, int nMode)
{
    if (nMode != pdlg->nMode) {
        pdlg->nMode = nMode;
        pdlg->tMode = D_002DDED0.tReal;
    }
}


/** @sly2-annot addr=0x258248
 * Purpose: memset(p,0,8); p->x8=-1; p->xC=0 [game linkonce (inline/template)]
 * Info: proposed name InitFileKey (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
/* linkonce copy (0x258248): clears a file key. */
extern "C" void func_00258248(FILEKEY *pfk)
{
    memset(pfk, 0, 8);
    pfk->bC = 0;
    pfk->n8 = -1;
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
extern const char D_002A6BB0[] = "/BASCUS-97316YAOTWTD!";
extern const char D_002A6BC8[] = "/BASCUS-97316YAOTWTD!/BASCUS-97316YAOTWTD!";
extern const char D_002A6BF8[] = "Sly 2";
extern const char D_002A6C00[] = "/";
extern const char D_002A6C08[] = "/BASCUS-97316YAOTWTD!/icon.sys";
extern const char D_002A6C28[] = "/BASCUS-97316YAOTWTD!/memcard_icon.ico";
extern const char D_002A6C50[] = "/%s/%s%d";
extern const char D_002A6C60[] = "BASCUS-97316YAOTWTD!";
extern const char D_002A6C78[] = "save";
extern const char D_002A6CA0[] = "memcard_icon.ico";
extern const char D_002A6CB8[] = "%s/%s";
extern const char D_002A6EB8[] = " : &2T&.";
extern const char D_002A6EC8[] = "%s : %d%%";
extern const char D_002A6EE0[] = "episodeBest";
char D_002E2080[0x4000];   /* the memcard thread's stack (stackSize 0x4000) */
namespace M_ReadMcDir { extern "C" { ENT D_002E6080[0x480 / sizeof(ENT)]; } }   /* sceMcGetDir table, cleared as 0x480 bytes */
char D_002E6500[0x50];   /* directory path */
int D_002E6590;
int D_002E6594;
