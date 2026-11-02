typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; int x14; int x18; char pad[0x40]; } N;
extern POOL D_004520D0;
extern void *D_004520C0[4];
extern int D_003E0EA4;
extern unsigned int D_002DD294;
extern int D_002DD598;
void Collect__3CGc(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002553E0(N *);

/** @sly2-annot addr=0x257340
 * Purpose: allocate Binding node from its slotheap (runs CGc_Collect when exhausted), then InitNode [game
 *   linkonce (inline/template)]
 * Info: proposed name PvNewBinding (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138,
 *   docs/reference/functions/splice-scripting.md#varb-0x12b0e8-0x12b3b0
 * @end-sly2-annot */
N *func_00257340(void)
{
    N *p;
    if (D_004520D0.xC == 0) {
        if (!D_003E0EA4 || D_002DD294 >= 4)
            Collect__3CGc(&D_002DD598);
        if (D_004520D0.xC == 0) {
            void *pv = PvAllocSlotheapClear(&D_004520D0);
            if (D_002DD294)
                func_0011CD98(&D_004520D0, pv);
            D_004520C0[D_002DD294++] = pv;
        }
    }
    p = (N *)PvAllocSlotheapClear(&D_004520D0);
    func_002553E0(p);
    p->xC = -1;
    p->x14 = 0;
    return p;
}
