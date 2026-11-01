// MATCH (Claude agent, laneC20 2026-10-23): v4 + func_001291A8 (CRef copy ctor) declared returning void * instead of void.
struct CRef;
extern "C" void func_00129EC8(CRef *);
extern "C" void *func_001291A8(CRef *, const CRef *); // CRef copy ctor returns this (void *); declaring it void scheduled the pv copy early
extern "C" CRef *func_00129658(CRef *, const CRef *);
struct CRef {
    int tagk; int tag;
    CRef() { tagk = -1; }
    CRef(int k, int n) { tagk = k; tag = n; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { func_00129EC8(this); }
    CRef &operator=(const CRef &r) { func_00129658(this, &r); return *this; }
};
struct CRef16 : CRef { int pad[2]; CRef16(int k, int n) : CRef(k, n) {} };
struct MARK { int a[4]; MARK(); ~MARK(); };
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct Y { int x0; int x4; };
struct X { char pad[0x158]; Y *p158; void *p15C; };
struct LO { char pad[0xC]; VTE *vt; char pad2[0x18 - 0x10]; void *p18; char pad3[0x11A8 - 0x1C]; X *p11A8; };
struct Q { char pad[0x194]; void *p194; };
struct SO { char pad[0x180]; int x180; };
struct K { char pad[0xC]; unsigned short xC; };
struct S { int c; K *pk; };
struct BL { int c; void *pv; };
struct G;
extern "C" {
extern G D_003D4A60;
extern char D_003DD490[];
extern char D_002A4DB0[], D_002A4D50[], D_002A4DC0[], D_002A4DC8[];
void *func_00143410(void *p);
int func_0013C478(void *p);
void *func_0013D1A8(Q *p);
S *func_00143AF0(LO *p, int n);
int func_00210A88(void);
BL *func_002109B8(int);
int func_00233178(const void *a, const void *b, int n);
int CploFindSwObjectsByClass(void *p, int a, int b, int c, int d, SO **apso);
int func_0021ECD8(char *, const char *, ...);
CRef func_00258200(void *p, const CRef &ref, G *pv);
}
CRef func_001D8B20(const CRef &ref, const void *o, ...);
CRef func_001D8A38(const char *, ...);
static inline void Send(void *pv, CRef ref) { func_00258200(pv, ref, &D_003D4A60); }
struct CRefRaw { int tagk; int tag; };
static inline void SetPv(void *pv, const CRef &r, void *)
{
    CRefRaw arg;
    func_001291A8((CRef *)&arg, &r);
    func_00258200(pv, *(CRef *)&arg, &D_003D4A60);
    func_00129EC8((CRef *)&arg);
}
static inline void *Vcall103(LO *p) { VTE *e = &p->vt[103]; return e->pfn((char *)p + e->d); }

extern "C" void func_00143678(LO *p, int n, Q *q)
{
    MARK m;
    switch (n) {
    case 0: {
        CRef ret = func_001D8B20(CRef(0x15, q->p194 ? func_0013C478(q->p194) : -1), Vcall103(p), D_002A4DB0, 0);
        if (p->p11A8) {
            if (!q->p194) {
                int dummy[2];
                void *pv0 = func_00143410(p->p18);
                CRef16 a0(0x15, -1);
                SetPv(pv0, a0, &D_003D4A60);
            }
            void *pv = func_0013D1A8(q);
            CRef a(0x15, func_0013C478(p->p11A8));
            SetPv(pv, a, &D_003D4A60);
        } else {
            void *pv = func_0013D1A8(q);
            CRef a(0x15, (int)p->p18);
            SetPv(pv, a, &D_003D4A60);
        }
        break;
    }
    case 1: {
        void *pv = func_00143410(p->p18);
        if (pv) {
            CRef a(0x15, -1);
            Send(pv, a);
        }
        CRef ret = func_001D8B20(CRef(0x15, -1), Vcall103(p), D_002A4DB0, 0);
        Y *py = p->p11A8->p158;
        if (py->x4 != -1) {
            char ach[32];
            func_0021ECD8(ach, D_002A4D50, py->x4);
            ret = func_001D8A38(D_002A4DC0, D_002A4DC8, ach, 0);
            ret = func_001D8B20(CRef(0x15, ret.tag + 1), D_002A4DC0, D_002A4DC8, ach, 0);
        }
        break;
    }
    case 2: {
        S *ps = func_00143AF0(p, func_0013C478(p->p11A8));
        int i = func_00210A88();
        if (ps && i != -1) {
            BL *pb = func_002109B8(i);
            int f = 0;
            if (pb->c >= 4 && func_00233178(ps->pk, pb->pv, 8) == 0) f = 1;
            if (f) {
                SO *apso[15];
                int c = CploFindSwObjectsByClass(D_003DD490, 5, 0x25, 0, 0xF, apso);
                for (int j = 0; j < c; j++) {
                    if (apso[j]->x180 == ps->pk->xC) {
                        p->p11A8->p15C = apso[j];
                        break;
                    }
                }
            }
        }
        break;
    }
    }
}
