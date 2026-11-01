typedef struct VTE { short d; short pad; void (*pfn)(); } VTE;
typedef struct AREQ { int cb; int align; } AREQ;
typedef struct O {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x60 - 0x10];
    float mat[16];
    int xA0[0x10];
    int xE0;
    int xE4;
    int padE8;
    float *xEC;
    void *xF0;
    void *xF4;
    char padF8[0x1C0 - 0xF8];
    int x1C0;
    char pad1C4[0x1D8 - 0x1C4];
    int x1D8;
    char pad1DC[0x1E8 - 0x1DC];
    int x1E8;
    void *x1EC;
} O;
typedef struct DL { O *pvFirst; O *pvLast; int ibDle; } DL;
extern DL D_003E1A40;
O *func_0013E3C8(int a, int b, int c, int d);
void func_002333B4(void *p, int c, int n);
void *func_001AE1D0(AREQ *preq);
void func_0011A220(DL *pdl, O *p);
void func_0011A2E8(DL *pdl, O *p);

/** @sly2-annot addr=0x1BDA00
 * Purpose: Create merged obstacle object (PloNew) holding member polygons
 * Info: proposed name PcnvoNewMerged (T2); TU tu_1BC050
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
O *func_001BDA00(void)
{
    O *p;
    int i, j;
    AREQ req;
    float *pm;

    p = D_003E1A40.pvFirst;
    if (!p) {
        for (i = 0; i < 16; i++) {
            O *q = func_0013E3C8(0xE, 0, 0x2CD, -1);
            q->xE0 = 0;
            q->xE4 = 1;
            q->x1C0 = 1;
            pm = q->mat;
            func_002333B4(pm, 0, 0x40);
            pm[15] = 1.0f;
            pm[10] = 1.0f;
            pm[5] = 1.0f;
            pm[0] = 1.0f;
            req.cb = 0x80;
            req.align = 0x10;
            q->xEC = (float *)func_001AE1D0(&req);
            for (j = 0; j < 32; j++) q->xEC[j] = (float)j;
            req.cb = 0x80;
            req.align = 0x10;
            q->xF0 = func_001AE1D0(&req);
            req.cb = 0x200;
            req.align = 0x10;
            q->xF4 = func_001AE1D0(&req);
            req.cb = 0x1F0;
            req.align = 0x10;
            q->x1EC = func_001AE1D0(&req);
            ((void (*)(void *))q->vt[9].pfn)((char *)q + q->vt[9].d);
            func_0011A220(&D_003E1A40, q);
        }
        p = D_003E1A40.pvFirst;
    }
    func_0011A2E8(&D_003E1A40, p);
    p->x1E8 = -1;
    ((void (*)(void *))p->vt[8].pfn)((char *)p + p->vt[8].d);
    p->x1D8 = 0;
    return p;
}
