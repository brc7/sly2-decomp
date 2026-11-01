struct AREQ { int cb; int cbAlign; };
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
struct T { char pad[0xE4]; int c; void *p; int pad2; int c2; void *q; };
extern "C" {
void *func_001AE378(AREQ *preq);
void *func_001AE238(AREQ *preq, void *pv);
void LoadXfmFromBrx(T *t, void *p);
void func_0012EB78(T *t, void *p);
}
void func_0012EB78(T *t, void *p)
{
    OBJ obj;
    AREQ req;
    AREQ *preq = &req;
    req.cb = 0xA0;
    preq->cbAlign = 0x10;
    t->p = func_001AE378(preq);
    req.cb = 0x80;
    preq->cbAlign = 0x10;
    t->q = func_001AE378(preq);
    LoadXfmFromBrx(t, p);
    req.cb = t->c * 0x14;
    preq->cbAlign = 0x10;
    t->p = func_001AE238(preq, t->p);
    req.cb = t->c2 * 0x10;
    preq->cbAlign = 0x10;
    t->q = func_001AE238(preq, t->q);
}
