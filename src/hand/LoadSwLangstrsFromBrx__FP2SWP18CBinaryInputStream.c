struct AREQ { int cb; int cbAlign; };
struct E { int x0; int x4; };
struct P { char pad[0x4640]; int c; struct E *ae; };
extern int D_002E1E54;
extern char D_002DD590[];
extern char D_003E1C20[];
int func_00117BD0(void *pbs);
int func_00117D50(void *pbs);
void *func_001AE1D0(struct AREQ *preq);
void CBinaryInputStream_ReadStringSw(void *pbs, int *pn, void *pv, int n);
void CBinaryInputStream_SkipStringSw(void *pbs);
void func_00210BE0(void *pv);
void LoadSwLangstrsFromBrx__FP2SWP18CBinaryInputStream(struct P *p, void *pbs)
{
    struct AREQ req;
    int i;
    int iMatch = 0;
    int c = func_00117BD0(pbs);
    {
        int j;
        for (j = 0; j < c; j++) {
            if (func_00117BD0(pbs) == D_002E1E54)
                iMatch = j;
        }
    }
    i = 0;
    p->c = func_00117D50(pbs);
    req.cbAlign = 0x10;
    req.cb = p->c * sizeof(struct E);
    p->ae = (struct E *)func_001AE1D0(&req);
    for (; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        int j;
        pe->x0 = func_00117D50(pbs);
        for (j = 0; j < c; j++) {
            if (j == iMatch)
                CBinaryInputStream_ReadStringSw(pbs, &pe->x4, D_002DD590, 0x2E);
            else
                CBinaryInputStream_SkipStringSw(pbs);
        }
    }
    func_00210BE0(D_003E1C20);
}
