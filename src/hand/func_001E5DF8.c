typedef int qword __attribute__((mode(TI)));
struct MSG { int cb; int nKind; };
struct P {
    char pad[0xE0];
    int fE0;
    int nE4;
    int c;
    int *aE;
    float *aF0;
    qword *aF4;
    char pad2[0x140 - 0xF8];
    qword *a140;
    qword *a144;
};
void func_00231768(struct P *, void *);
int func_00117D00(void *);
int func_00117C20(void *);
void *func_001AE1D0(struct MSG *);
void func_00117968(void *, int, void *);
void func_00117EB0(void *, qword *);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(struct P *, void *);
void func_001E6180(struct P *);
void func_001E5DF8(struct P *p, void *pbis)
{
    struct MSG msg;
    int i;
    func_00231768(p, pbis);
    p->fE0 = func_00117D00(pbis);
    p->nE4 = func_00117D00(pbis);
    p->c = func_00117C20(pbis);
    msg.cb = p->c * 4;
    msg.nKind = 0x10;
    p->aE = func_001AE1D0(&msg);
    msg.cb = p->c * 4;
    msg.nKind = 0x10;
    p->aF0 = func_001AE1D0(&msg);
    msg.cb = p->c * 16;
    msg.nKind = 0x10;
    p->aF4 = func_001AE1D0(&msg);
    func_00117968(pbis, p->c * 4, p->aE);
    for (i = 0; i < p->c; i++)
        func_00117EB0(pbis, &p->aF4[i]);
    if (p->fE0) {
        msg.cb = p->c * 16;
        msg.nKind = 0x10;
        p->a140 = func_001AE1D0(&msg);
        msg.cb = p->c * 16;
        msg.nKind = 0x10;
        p->a144 = func_001AE1D0(&msg);
        for (i = 0; i < p->c; i++)
            func_00117EB0(pbis, &p->a140[i]);
        for (i = 0; i < p->c; i++)
            func_00117EB0(pbis, &p->a144[i]);
    }
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p, pbis);
    func_001E6180(p);
}
