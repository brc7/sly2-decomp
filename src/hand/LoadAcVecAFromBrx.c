typedef struct VECTOR3 { float x, y, z; } VECTOR3;
typedef struct KEY { VECTOR3 pos; float t; VECTOR3 dpos; int pad; VECTOR3 dpos2; int pad2; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
int func_00117C20(void *pbis);
void func_00117EB0(void *pbis, VECTOR3 *pv);
KEY *func_001AE2A0(AREQ *preq);
void LoadAcVecAFromBrx(T *t, void *pbis)
{
    int i;
    int c = func_00117C20(pbis);
    AREQ req;
    req.cb = c * sizeof(KEY);
    req.cbAlign = 0x10;
    t->c = c;
    t->a = func_001AE2A0(&req);
    for (i = 0; i < t->c; i++) {
        KEY *pkey = &t->a[i];
        pkey->t = func_00117C20(pbis) * (1.0f / 60.0f);
        func_00117EB0(pbis, &pkey->pos);
        func_00117EB0(pbis, &pkey->dpos);
        func_00117EB0(pbis, &pkey->dpos2);
    }
}
