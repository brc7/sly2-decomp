typedef struct POOL { int x0, x4, x8; void *xC; } POOL;
typedef struct N { int x0; int x4; int x8; int a[4]; int x1C; } N;
extern POOL D_004520F8;
extern void *D_004520E8[4];
extern int D_003E0EA4;
extern unsigned int D_002DD2A4;
extern int D_002DD598;
void Collect__3CGc(void *);
void *PvAllocSlotheapClear(POOL *);
void func_0011CD98(POOL *, void *);
N *func_00255718(N *);

N *func_00256DD0(void)
{
    N *p;
    if (D_004520F8.xC == 0) {
        if (!D_003E0EA4 || D_002DD2A4 >= 4)
            Collect__3CGc(&D_002DD598);
        if (D_004520F8.xC == 0) {
            void *pv = PvAllocSlotheapClear(&D_004520F8);
            if (D_002DD2A4)
                func_0011CD98(&D_004520F8, pv);
            D_004520E8[D_002DD2A4++] = pv;
        }
    }
    p = (N *)PvAllocSlotheapClear(&D_004520F8);
    func_00255718(p);
    {
        int *q = p->a;
        int n = 3;
        do {
            *q = 0;
            n--;
            q++;
        } while (n != -1);
    }
    p->x1C = 0;
    return p;
}
