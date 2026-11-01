typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_00452148;
extern void *D_00452138[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2C4;
extern int D_002DD598;
void func_001286E0(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_00255D90(N *);

N *func_00257D80(void)
{
    N *p;
    if (D_00452148.xC == 0) {
        if (!D_003E0EA4 || D_002DD2C4 >= 4)
            func_001286E0(&D_002DD598);
        if (D_00452148.xC == 0) {
            void *pv = PvAllocSlotheapClear(&D_00452148);
            if (D_002DD2C4)
                func_0011CD98(&D_00452148, pv);
            D_00452138[D_002DD2C4++] = pv;
        }
    }
    p = (N *)PvAllocSlotheapClear(&D_00452148);
    func_00255D90(p);
    p->x8 = 0;
    return p;
}
