typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_00452170;
extern void *D_00452160[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2D4;
extern int D_002DD598;
void func_001286E0(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002560C8(N *);

N *func_00259C18(void)
{
    N *p;
    if (D_00452170.xC == 0) {
        if (!D_003E0EA4 || D_002DD2D4 >= 4)
            func_001286E0(&D_002DD598);
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
