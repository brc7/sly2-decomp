struct CRef { int m_tagk; void *pv; };
struct N;
struct SP { struct N *p; };
struct N { int cref; int x4; struct CRef ref; struct SP sp; };
struct X { int f; struct N *pn; };
extern char D_004520A8[];
struct N *func_00256B48(void);
void func_00256C38(struct SP *psp, struct N *pn);
void func_00256C58(struct SP *psp, struct N *pn);
void func_00256D40(struct SP *psp, int n);
struct CRef *CRef_SetPair(struct CRef *pref, struct N *pn);
void CRef_SetSymid(struct CRef *pref, int n);
void CRef_SetBasic(struct CRef *pref, int n);
void func_00129EC8(struct CRef *pref);
void func_002550F8(void *pv);
void func_0011CD98(void *pool, void *pv);
struct SP *func_00132ED8(struct SP *ret, int a1, int a2, struct X *px)
{
    struct SP sp;
    struct N *p;
    func_00256C38(&sp, func_00256B48());
    {
        struct CRef *pref = &sp.p->ref;
        CRef_SetPair(pref, func_00256B48());
    }
    p = (struct N *)sp.p->ref.pv;
    CRef_SetSymid(&p->ref, a2);
    {
        struct N *pn = func_00256B48();
        func_00256C58(&p->sp, pn);
    }
    p = p->sp.p;
    CRef_SetBasic(&p->ref, a1);
    if (px && px->f)
        func_00256C58(&p->sp, px->pn);
    func_00256C38(ret, sp.p);
    {
        struct N *pt = sp.p;
        if (pt && --pt->cref == 0) {
            func_00256D40(&pt->sp, 2);
            func_00129EC8(&pt->ref);
            func_002550F8(pt);
            func_0011CD98(D_004520A8, pt);
        }
    }
    return ret;
}
