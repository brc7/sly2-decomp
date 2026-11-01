typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct REC { int id; float v[2]; float g; } REC;
typedef struct T { char pad[0xE4]; int c; void *pad2; void **ap; int crec; REC *arec; } T;
typedef struct G { char pad[0x228]; void *p; } G;
extern G D_003DD040;
void PostLoLoad(T *t);
void **func_001AE2A0(AREQ *preq);
void *func_001FE990(G *pg, int id);
void func_001FC408(float g, void *p, void *q, T *t, float *pv);
void func_0012EC50(T *t)
{
    AREQ req;
    void *p;
    PostLoLoad(t);
    req.cb = t->c * 4;
    req.cbAlign = 0x10;
    t->ap = func_001AE2A0(&req);
    p = D_003DD040.p;
    if (p) {
        int i;
        for (i = 0; i < t->crec; i++) {
            REC *prec = &t->arec[i];
            func_001FC408(prec->g, p, func_001FE990(&D_003DD040, prec->id), t, prec->v);
        }
    }
}
