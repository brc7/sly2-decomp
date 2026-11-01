typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; } N;
extern POOL D_004520A8;
extern void *D_00452098[4];
extern int D_003E0EA4;
extern unsigned int D_002DD284;
extern int D_002DD598;
void func_001286E0(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002550A0(N *);

N *func_00256B48(void)
{
    N *p;
    if (D_004520A8.xC == 0) {
        if (!D_003E0EA4 || D_002DD284 >= 4)
            func_001286E0(&D_002DD598);
        if (D_004520A8.xC == 0) {
            void *pv = PvAllocSlotheapClear(&D_004520A8);
            if (D_002DD284)
                func_0011CD98(&D_004520A8, pv);
            D_00452098[D_002DD284++] = pv;
        }
    }
    p = (N *)PvAllocSlotheapClear(&D_004520A8);
    func_002550A0(p);
    p->x8 = -1;
    p->x10 = 0;
    return p;
}
