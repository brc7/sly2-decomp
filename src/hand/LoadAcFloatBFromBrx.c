typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct KEY { float a, b; } KEY;
typedef struct T { int pad; int c; float *at; KEY *ak[3]; int pad2[2]; int v[4]; } T;
void func_00117EB0(void *pbis, void *pv);
int func_00117C20(void *pbis);
int func_00117BD0(void *pbis);
float func_00117E38(void *pbis);
void *func_001AE1D0(AREQ *preq);
void LoadAcFloatBFromBrx(T *t, void *pbis)
{
    AREQ req;
    int i, j, mask, grf;
    int c;
    func_00117EB0(pbis, t->v);
    c = func_00117C20(pbis);
    req.cb = c * 4;
    req.cbAlign = 0x10;
    t->c = c;
    t->at = func_001AE1D0(&req);
    for (i = 0; i < t->c; i++)
        t->at[i] = func_00117C20(pbis) * 0.016666668f;
    grf = func_00117BD0(pbis);
    for (j = 0, mask = 1; j < 3; j++, mask <<= 1) {
        if (mask & grf) {
            KEY *ak;
            req.cb = t->c * 8;
            req.cbAlign = 0x10;
            ak = func_001AE1D0(&req);
            t->ak[j] = ak;
            for (i = 0; i < t->c; i++) {
                ak[i].a = func_00117E38(pbis);
                ak[i].b = func_00117E38(pbis);
            }
        }
    }
}
