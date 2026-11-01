typedef int qword __attribute__((mode(TI)));
typedef struct M4 { qword r[4]; } M4;
typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct LO LO;
struct LO {
    char pad[0xC];
    VT *vt;
    char pad2[0x18];
    int x28;
    signed char ips;
    char x2D;
    long k : 4;
    unsigned long b : 8;
    unsigned int f : 1;
    char pad3[0x20];
    LO *x50;
    char pad4[0xC];
    M4 m60;
    LO *xA0;
};
M4 func_0011C168(void *, M4 *);
void func_001A5900(LO *, LO *, LO *, int);

/** @sly2-annot addr=0x2315F0
 * Purpose: re-parent keeping world matrix (0x1A5900)
 * Info: proposed name SetXfmParent (T1); class XFM slot10; TU xform
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
void func_002315F0(LO *p, LO *pNew, LO *pParent, int grf)
{
    M4 m;
    if (grf & 3) {
        if (p->f) {
            m = *((M4 * (*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
        } else {
            LO *q;
            m = p->m60;
            for (q = p->x50; q; q = q->xA0)
            {
                M4 t = func_0011C168(q, &m);
                m = t;
            }
        }
    }
    if (pParent)
        p->x50 = pParent;
    else if (pNew)
        p->x50 = *(LO **)((char *)pNew + 0x60);
    else
        p->x50 = 0;
    func_001A5900(p, pNew, pParent, grf);
    if (grf & 1)
        ((void (*)(void *, qword))p->vt[29].pfn)((char *)p + p->vt[29].d, m.r[3]);
    if (grf & 2)
        ((void (*)(void *, M4 *))p->vt[30].pfn)((char *)p + p->vt[30].d, &m);
}
