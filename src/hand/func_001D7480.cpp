struct CRef {
    int m_tagk;
    int m_tag;
    CRef() { m_tagk = -1; }
    CRef(const CRef &);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &);
    void Decref();
};
typedef struct { short d; short pad; void *pfn; } VTE;
struct SO { char pad0[0xC]; VTE *vt; };
struct SUB { char pad[0x74]; int x74; };
struct GS { char pad[0x3B0C]; SUB *x3B0C; };
struct OBJ {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x180 - 0x10];
    int x180;
    void *x184;
    char x188[4];
};
extern "C" {
extern GS D_003DD490;
extern char D_002A7878[];
extern int D_002DFBC0;
extern char D_002DFC00[], D_002E0D00[];
void func_001F1448(void *, OBJ *, int, int, void *);
void func_00162E38(SUB *);
int CploFindSwObjectsByClass(GS *, int, int, int, int, SO **);
void func_001433C8(SO *);
void func_001440D0(SO *, int);
CRef func_001D8A38(const char *, ...);
int func_001F0400(void *, int);
int func_001F04F8(void *, int);
void func_001D77D0(OBJ *, int);
void func_00187E78(void *);
}
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
#define VCALL5(o, n, a1, a2, a3, a4, a5) ((void (*)(void *, int, int, int, int, int))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d, a1, a2, a3, a4, a5)
extern "C" void func_001D7480(OBJ *p, int s)
{
    if (p->x180 == s)
        return;
    if (p->x180 == 1) {
        func_001F1448(0, p, -1, 0, p->x188);
        VCALL5(p, 3, 6, 0, 0, 0, 0);
    }
    p->x180 = s;
    if (s == 1) {
        SO *aso0[4];
        SO *aso1[32];
        SO *aso2[32];
        int n;
        int idx;
        if (D_003DD490.x3B0C && !D_003DD490.x3B0C->x74)
            func_00162E38(D_003DD490.x3B0C);
        n = CploFindSwObjectsByClass(&D_003DD490, 5, 0x34, 0, 3, aso0);
        for (int i0 = 0; i0 < n; i0++) {
            SO *so = aso0[i0];
            func_001433C8(so);
            func_001440D0(so, 1);
        }
        n = CploFindSwObjectsByClass(&D_003DD490, 0x105, 0x64, 0, 0x20, aso1);
        for (int i1 = 0; i1 < n; i1++)
            ((void (*)(void *, int))aso1[i1]->vt[76].pfn)((char *)aso1[i1] + aso1[i1]->vt[76].d, 4);
        n = CploFindSwObjectsByClass(&D_003DD490, 0x105, 0x4F, 0, 0x20, aso2);
        for (int i2 = 0; i2 < n; i2++)
            VCALL(aso2[i2], 9);
        if (p->x184) {
            int v = func_001D8A38(D_002A7878, 0).m_tag;
            if (func_001F0400(p->x184, v) == -1)
                idx = func_001F04F8(p->x184, 0);
            else
                idx = v;
        } else {
            idx = -1;
        }
        func_001F1448(p->x184, p, idx, 0, p->x188);
        func_001D77D0(p, 0);
        func_00187E78((D_002DFBC0 & 2) ? D_002DFC00 : D_002E0D00);
        VCALL5(p, 3, 5, 0, 0, 0, 0);
    }
}
