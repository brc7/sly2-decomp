typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_004520A8;
extern void *D_00452098[4];
extern int D_003E0EA4;
extern unsigned int D_002DD284;
extern int D_002DD598;
void func_001286E0(void *);
void *func_0011CCC0(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002550A0(N *);

/** @sly2-annot addr=0x256B48
 * Purpose: allocate Pair node from its slotheap (runs CGc_Collect when exhausted), then InitNode [game
 *   linkonce (inline/template)]
 * Info: proposed name PvNewPair (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/splice-scripting.md#pair-0x128cb8-0x128f70
 * @end-sly2-annot */
N *func_00256B48(void)
{
    N *p;
    if (D_004520A8.xC == 0) {
        if (!D_003E0EA4 || D_002DD284 >= 4)
            func_001286E0(&D_002DD598);
        if (D_004520A8.xC == 0) {
            void *pv = func_0011CCC0(&D_004520A8);
            if (D_002DD284)
                func_0011CD98(&D_004520A8, pv);
            D_00452098[D_002DD284++] = pv;
        }
    }
    p = (N *)func_0011CCC0(&D_004520A8);
    func_002550A0(p);
    p->x8 = -1;
    p->x10 = 0;
    return p;
}
