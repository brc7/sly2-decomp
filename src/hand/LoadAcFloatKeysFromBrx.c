typedef struct KEY { float t; float g; float dgIn; float dgOut; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
int func_00117C20(void *pbis);
float func_00117E38(void *pbis);
KEY *func_001AE2A0(AREQ *preq);
void LoadAcFloatKeysFromBrx(T *t, void *pbis)
{
    int i;
    int c = func_00117C20(pbis);
    AREQ req;
    req.cb = c << 4;
    req.cbAlign = 0x10;
    t->c = c;
    t->a = func_001AE2A0(&req);
    for (i = 0; i < t->c; i++) {
        KEY *pkey = &t->a[i];
        pkey->t = func_00117C20(pbis) * (1.0f / 60.0f);
        pkey->g = func_00117E38(pbis);
        pkey->dgIn = func_00117E38(pbis);
        pkey->dgOut = func_00117E38(pbis);
    }
}
