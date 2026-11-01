typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int xC; int x10; int x14; int x18; char pad[0x40]; } N;
extern POOL D_004520D0;
extern void *D_004520C0[4];
extern int D_003E0EA4;
extern unsigned int D_002DD294;
extern int D_002DD598;
void func_001286E0(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_002553E0(N *);

N *func_00257340(void)
{
    N *p;
    if (D_004520D0.xC == 0) {
        if (!D_003E0EA4 || D_002DD294 >= 4)
            func_001286E0(&D_002DD598);
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
