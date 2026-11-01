typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct NODE { int pad[2]; CRef ref; struct NODE *pnext; } NODE;
typedef struct ARR { int c; short *a; } ARR;
short *func_001AE318(AREQ *preq);
void FillAsFromPair(NODE *pnode, ARR *parr)
{
    NODE *p;
    int i;
    parr->c = 0;
    parr->a = 0;
    for (p = pnode; p; p = p->pnext)
        parr->c++;
    if (parr->c) {
        AREQ req;
        req.cb = parr->c * 2;
        req.cbAlign = 0x10;
        parr->a = func_001AE318(&req);
        for (p = pnode, i = 0; p; p = p->pnext, i++) {
            if (p->ref.m_tagk == 0x15 || p->ref.m_tagk == 0x18)
                { int n = p->ref.u.n; parr->a[i] = n; }
        }
    }
}
