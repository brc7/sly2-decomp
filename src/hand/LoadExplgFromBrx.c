typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct C { int a[4]; };
struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; int pad[6]; unsigned long fl; };
struct DA { int n0; int cMax; int c; struct O **a; };
struct S { char pad[0xE0]; int c; struct O **apo; };
void func_001AE478(struct C *);
void func_001AE4A0(struct C *, int);
void LoadXfmFromBrx(struct S *p, void *q);
void CLoadCollectorArray_ctor(struct DA *pda);
void LoadXfmChildObjectsFromBrx(struct S *p, void *q, struct DA *pda);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void LoadExplgFromBrx(struct S *p, void *q)
{
    struct C ctx;
    struct DA da;
    MSG msg;
    int i;
    LoadXfmFromBrx(p, q);
    func_001AE478(&ctx);
    CLoadCollectorArray_ctor(&da);
    LoadXfmChildObjectsFromBrx(p, q, &da);
    p->apo = PvAllocMsg(&msg, da.c * 4, 0x10);
    for (i = 0; i < da.c; i++) {
        struct O *o = da.a[i];
        o->fl |= 0x200000000000000L;
        if (!FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x1E)) {
            struct VTE *e = &o->vt[9];
            e->pfn((char *)o + e->d);
        } else {
            p->apo[p->c++] = o;
        }
    }
    func_001AE4A0(&ctx, 2);
}
