typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_00452170;
extern void *D_00452160[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2D4;
extern int D_002DD598;
void Collect__3CGc(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002560C8(N *);

/** @sly2-annot addr=0x259C18
 * Purpose: allocate Pool6 node from its slotheap (runs CGc_Collect when exhausted), then InitNode [game
 *   linkonce (inline/template)]
 * Info: proposed name PvNewPool6 (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
N *func_00259C18(void)
{
    N *p;
    if (D_00452170.xC == 0) {
        if (!D_003E0EA4 || D_002DD2D4 >= 4)
            Collect__3CGc(&D_002DD598);
        if (D_00452170.xC == 0) {
            void *pv = PvAllocSlotheapClear(&D_00452170);
            if (D_002DD2D4)
                func_0011CD98(&D_00452170, pv);
            D_00452160[D_002DD2D4++] = pv;
        }
    }
    p = (N *)PvAllocSlotheapClear(&D_00452170);
    func_002560C8(p);
    return p;
}
