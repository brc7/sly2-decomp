typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_00452148;
extern void *D_00452138[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2C4;
extern int D_002DD598;
void func_001286E0(void *);
void *func_0011CCC0(POOL *);
void func_0011CD98(POOL *, void *);
N *func_00255D90(N *);

/** @sly2-annot addr=0x257D80
 * Purpose: allocate Sidebag node from its slotheap (runs CGc_Collect when exhausted), then InitNode [game
 *   linkonce (inline/template)]
 * Info: proposed name PvNewSidebag (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
N *func_00257D80(void)
{
    N *p;
    if (D_00452148.xC == 0) {
        if (!D_003E0EA4 || D_002DD2C4 >= 4)
            func_001286E0(&D_002DD598);
        if (D_00452148.xC == 0) {
            void *pv = func_0011CCC0(&D_00452148);
            if (D_002DD2C4)
                func_0011CD98(&D_00452148, pv);
            D_00452138[D_002DD2C4++] = pv;
        }
    }
    p = (N *)func_0011CCC0(&D_00452148);
    func_00255D90(p);
    p->x8 = 0;
    return p;
}
