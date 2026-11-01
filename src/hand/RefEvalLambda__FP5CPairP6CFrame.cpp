struct X;
struct Y { int a, b; int k; int c; Y *pnext; };
struct O;
struct CRef { int m_tagk; int m_tag; CRef(O *po); CRef(const CRef &); void Decref(); ~CRef() { Decref(); } };
struct SPX { X *p; ~SPX(); void operator=(X *px); };
struct SPY { Y *p; ~SPY(); void operator=(Y *py); };
struct O {
    int cref;
    int x4;
    SPX spx;
    int xC;
    int x10;
    SPY spy14;
    SPY spy18;
    ~O();
};
struct SPO {
    O *p;
    void Set(O *po);
    SPO() {}
    SPO(O *po) { Set(po); }
    SPO(const SPO &);
    ~SPO() { O *po = p; if (po && --po->cref == 0) delete po; }
    O *operator->() { return p; }
};
struct Q { char pad[8]; int f; Y *py; Q *pnext; };
struct R { char pad[0x10]; Q *pq; };
struct P { char pad[0xC]; R *pr; };
extern "C" {
O *func_00256F48();
CRef RefEvalLambda__FP5CPairP6CFrame(P *p, X *px);
}
CRef RefEvalLambda__FP5CPairP6CFrame(P *p, X *px)
{
    Q *pq = p->pr->pq;
    Y *py = (Y *)pq->pnext;
    SPO sp(func_00256F48());
    sp->spx = px;
    if (pq->f == 0)
        sp->spy14 = 0;
    else
        sp->spy14 = pq->py;
    sp->spy18 = py;
    int f = 0;
    int c = 0;
    Y *pyT;
    for (pyT = sp->spy14.p; pyT;) {
        if (pyT->k == 0x14) {
            f = 1;
            pyT = 0;
        } else {
            pyT = pyT->pnext;
            c++;
        }
    }
    sp->xC = c;
    sp->x10 = f;
    return CRef(sp.p);
}
