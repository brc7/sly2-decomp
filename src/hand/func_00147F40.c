typedef int qword __attribute__((mode(TI)));
typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct SO;
struct VT { char pad[0xB0]; short d16; short p16; void (*pfn22)(struct SO *); short d17; short p17; void (*pfn23)(struct SO *); };
struct SO { char pad[0xC]; struct VT *pvt; char pad2[0x28 - 0x10]; long grf; float g30; float g34; float g38; };
struct ITEM { struct SO *pso; };
struct CM { char pad[0x14]; int x14; char pad2[0x90 - 0x18]; qword pos; };
struct P { char pad[0x20]; DL dl; char pad2[0x2C - 0x2C]; char x2C[0x39A4 - 0x2C]; void *pvStop; int x39A8; qword q39B0; qword q39C0; };
extern struct CM D_002DDF40;
extern float D_002BD450;
void *func_001C4840(void);
qword func_001A62E8(void *p);
void func_00147EC8(struct P *p, void *pv);
void func_00147EE8(struct P *p, void *pv);
int FFindDlEntry__FP2DLPv(void *p, void *pv);
void AppendDlEntry__FP2DLPv(void *p, void *pv);
void RemoveDlEntry__FP2DLPv(void *p, void *pv);

static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
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

void func_00147F40(struct P *p)
{
    DLI dli;
    qword pos, posB;
    float gA, gB;
    int n;
    void *pvB;
    void *pv;
    pos = D_002DDF40.pos;
    n = D_002DDF40.x14;
    pvB = func_001C4840();
    if (pvB)
        posB = func_001A62E8(pvB);
    else
        posB = pos;
    gA = GDist(pos, p->q39B0);
    gB = GDist(posB, p->q39C0);
    if (D_002BD450 < gA || D_002BD450 < gB || n != p->x39A8) {
        p->pvStop = 0;
        p->x39A8 = n;
        p->q39B0 = pos;
        p->q39C0 = posB;
    }
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != p->pvStop) {
        struct SO *pso = ((struct ITEM *)pv)->pso;
        int fIn;
        switch ((int)((pso->grf << 8) >> 60)) {
        case 2:
            fIn = 0;
            func_00147EC8(p, pv);
            break;
        case 1:
            fIn = 1;
            func_00147EC8(p, pv);
            break;
        case 0:
            if (!(n & ((unsigned char *)pso)[0x2D])) {
                fIn = 0;
                goto leave;
            } else {
                float d;
                d = GDist(func_001A62E8(pso), pos) - pso->g38;
                fIn = d < 0.0f;
                if (D_002BD450 < __builtin_fabsf(d) - gA) {
                leave:
                    func_00147EE8(p, pv);
                }
            }
            break;
        case 3:
            if (!(n & ((unsigned char *)pso)[0x2D])) {
                fIn = 0;
            } else {
                float d;
                d = GDist(func_001A62E8(pso), posB) - pso->g38;
                fIn = d < 0.0f;
                if (D_002BD450 < __builtin_fabsf(d) - gB)
                    func_00147EE8(p, pv);
            }
            break;
        }
        if (fIn != FFindDlEntry__FP2DLPv(p->x2C, pv)) {
            if (fIn) {
                AppendDlEntry__FP2DLPv(p->x2C, pv);
                pso->pvt->pfn23((struct SO *)((char *)pso + pso->pvt->d17));
            } else {
                pso->pvt->pfn22((struct SO *)((char *)pso + pso->pvt->d16));
                RemoveDlEntry__FP2DLPv(p->x2C, pv);
            }
        }
    }
    D_002DD2FC = dli.m_pdliNext;
}
