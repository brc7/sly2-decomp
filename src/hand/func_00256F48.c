typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; int x14; int x18; char pad[0x40]; } N;
extern POOL D_00452120;
extern void *D_00452110[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2B4;
extern int D_002DD598;
void func_001286E0(void *);
void *func_0011CCC0(POOL *);
void func_0011CD98(POOL *, void *);
N *func_00255A58(N *);

/** @sly2-annot addr=0x256F48
 * Purpose: allocate Proc node from its slotheap (runs CGc_Collect when exhausted), then InitNode [game
 *   linkonce (inline/template)]
 * Info: proposed name PvNewProc (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18,
 *   docs/reference/functions/splice-scripting.md#proc-0x128f70-0x1291a8
 * @end-sly2-annot */
N *func_00256F48(void)
{
    N *p;
    if (D_00452120.xC == 0) {
        if (!D_003E0EA4 || D_002DD2B4 >= 4)
            func_001286E0(&D_002DD598);
        if (D_00452120.xC == 0) {
            void *pv = func_0011CCC0(&D_00452120);
            if (D_002DD2B4)
                func_0011CD98(&D_00452120, pv);
            D_00452110[D_002DD2B4++] = pv;
        }
    }
    p = (N *)func_0011CCC0(&D_00452120);
    func_00255A58(p);
    p->x8 = 0;
    p->x14 = 0;
    p->x18 = 0;
    return p;
}
