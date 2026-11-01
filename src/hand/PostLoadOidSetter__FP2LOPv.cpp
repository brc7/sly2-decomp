class X;
typedef void (X::*PMF)(void *);
struct D { int id; PMF pmf; };
extern "C" char D_003DD490[];
extern "C" void *PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, X *);
extern "C" void PostLoadOidSetter__FP2LOPv(X *p, D *pd)
{
    void *pv = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pd->id, p);
    (p->*(pd->pmf))(pv);
}
