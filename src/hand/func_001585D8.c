typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE378(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE378(pmsg); }
struct S { int cdzMax; char pad[0x4C]; int a; int b; int c; int d; void *pv; };
struct S *InitDzg(struct S *p, int c);
struct S *func_001585D8(struct S *p, int a, int b, int c, int d)
{
    MSG msg;
    InitDzg(p, a * 2);
    p->a = a;
    p->b = b;
    p->c = c;
    p->d = d;
    msg.nKind = 0x10;
    msg.cb = 0x60 * p->cdzMax;
    p->pv = func_001AE378(&msg);
    return p;
}
