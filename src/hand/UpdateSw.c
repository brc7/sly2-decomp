typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;     /* g_pdliFirst */
struct CLOCK { float t; float dt; char pad[8]; float g10; };
extern struct CLOCK D_002DDED0;
extern char D_002E65E0[];
extern char D_003DD040[];
extern char D_002DDF40[];
extern char D_002DD598[];
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { int grf; struct O *po; };
struct L { char pad[0x84]; struct L *pnext; };
struct S { char pad[0x3D88]; DL dl; char pad1[0x3EAC - 0x3D94]; struct L *pl; char pad2[0x3F14 - 0x3EB0]; int c; int x3F18; int *pi; struct E *ae;
    char pad3[0x3FFC - 0x3F24]; DL dl2; };
void ProcessSwCallbacks(struct S *p);
void UpdateSwBusyList(struct S *p);
void UpdateSwObjects(struct S *p);
void UpdatePoctl(struct L *pl);
void ExpireSwZapRecords(struct S *p);
void UpdateSwGuardOverride(struct S *p);
void func_00130918(void *pv);
void func_00156728(void *pv);
void SolveSw(struct S *p);
void func_001BE1E8(void);
void func_001EC968(void);
void func_001AB1A0(void *pv);
void func_001FD830(void *pv);
void func_0014D478(void *pv);
void func_0020F7E0(struct S *p);
void CGc_UpdateRecyclable(void *pv);
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
void UpdateSw(struct S *p)
{
    ProcessSwCallbacks(p);
    UpdateSwBusyList(p);
    if (D_002DDED0.dt <= 0.0f) {
        UpdateSwObjects(p);
    } else {
        struct L *pl;
        int i;
        DLI dli, dli2;
        void *pv;
        for (pl = p->pl; pl; pl = pl->pnext)
            UpdatePoctl(pl);
        ExpireSwZapRecords(p);
        p->pi = &i;
        for (i = 0; i < p->c; i++) {
            struct E *pe = &p->ae[i];
            if (pe->grf & 2) {
                struct O *po = pe->po;
                ((void (*)(void *))po->vt[16].pfn)((char *)po + po->vt[16].d);
            }
        }
        p->pi = 0;
        UpdateSwGuardOverride(p);
        InitDli(&dli, &p->dl);
        while ((pv = PvNextDli(&dli)) != 0)
            func_00130918(pv);
        InitDli(&dli2, &p->dl2);
        while ((pv = PvNextDli(&dli2)) != 0)
            func_00156728(pv);
        ProcessSwCallbacks(p);
        SolveSw(p);
        func_001BE1E8();
        func_001EC968();
        ProcessSwCallbacks(p);
        D_002DD2FC = dli.m_pdliNext;
    }
    func_001AB1A0(D_002E65E0);
    func_001FD830(D_003DD040);
    if (0.0f < D_002DDED0.g10)
        func_0014D478(D_002DDF40);
    func_0020F7E0(p);
    CGc_UpdateRecyclable(D_002DD598);
}
