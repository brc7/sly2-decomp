struct MSG { int cb; int nKind; };
struct Q { char pad[0x20]; struct E *pr; int n; };
struct E { int oid; int c; int x8; int xc; char dl[0xC]; int x1c; float g; void *pv; int x28; float h; };
struct R { int a; void *p; int x8; int n; unsigned long fl; char pad[0x98]; };
struct S { char pad[0x28]; unsigned long fl; char p0[0x30]; int x60; char p1[0x7C]; int ce0; struct R *ar; char p2[0xE0]; int c; struct E ae[4]; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *g, int n, int oid, struct S *p);
void InitDl__FP2DLi(void *pdl, int ib);
void *func_001AE2A0(struct MSG *pmsg);
void AppendDlEntry__FP2DLPv(void *pdl, void *pv);
void func_0017F138(struct S *p);
void AddSoParts(struct S *p);
void func_00181840(struct S *p)
{
    int i, j;
    struct E *pe;
    struct R *pr;
    for (i = 0; i < p->c; i++) {
        pe = &p->ae[i];
        pe->pv = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oid, p);
        if (!pe->pv) {
            i--;
            p->c--;
            continue;
        }
        InitDl__FP2DLi(pe->dl, 0x60);
        for (j = 0; j < pe->c; j++) {
            struct MSG msg;
            struct Q *q;
            msg.cb = 0x70;
            msg.nKind = 0x10;
            q = (struct Q *)func_001AE2A0(&msg);
            q->pr = pe;
            q->n = -1;
            AppendDlEntry__FP2DLPv(pe->dl, q);
        }
    }
    func_0017F138(p);
    pr = &p->ar[p->ce0++];
    pr->a = p->x60;
    pr->n = -1;
    pr->p = p;
    pr->fl |= 1UL << 48;
    pr->fl |= 1UL << 49;
    if ((int)(p->fl >> 60) & 1) AddSoParts(p);
}
