extern "C" {
struct RGBA { unsigned char r, g, b, a; RGBA() {} };
typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
typedef struct X {
    char pad[0x50];
    RGBA rgba50, rgba54, rgba58;
    float d5C, d60, d64;
    float t68;
    int k6C;
} X;
typedef struct BUF { char ab[0x250]; } __attribute__((aligned(16))) BUF;
typedef struct Q { int x0; float x4; int x8; } Q;
typedef struct R { char pad[0xC]; int xC; } R;
extern R D_002DEA28;
extern void func_0016F960(BUF *pbuf, RGBA *prgba, int n);
extern void func_001574B8(R *);
extern void func_002270C8(R *, int, void *, int);
extern void func_001576F0(R *);
extern void func_00157620(R *);
extern Q *func_001CD998(void);
}
static inline int NLerp(float g, int a, int b)
{
    if (a == b)
        return b;
    if (a < b)
        return a + (int)(g * ((float)b + 0.999f - (float)a));
    return b + (int)((1.0f - g) * ((float)a + 0.999f - (float)b));
}
static inline RGBA RgbaLerp(float t, RGBA a, RGBA b)
{
    RGBA r;
    r.r = NLerp(t, a.r, b.r);
    r.g = NLerp(t, a.g, b.g);
    r.b = NLerp(t, a.b, b.b);
    r.a = NLerp(t, a.a, b.a);
    return r;
}
static inline void Build(BUF *pbuf, RGBA rgba)
{
    func_0016F960(pbuf, &rgba, 3);
}
/** @sly2-annot addr=0x15DD48
 * Purpose: Draws the timed object (1168-byte body)
 * Info: proposed name RenderCid116Self (T2); class CID116 slot19; TU tu_15DC80
 * Notes: Cid74->Cid116 (CID 0x74=116, vtable)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/rendering.md#2-render-list-entries-rpl,
 *   docs/reference/functions/gameplay-objects.md#tu_15dc80-0x15dc80-0x15e380
 * @end-sly2-annot */
extern "C" void func_0015DD48(X *p)
{
    RGBA rgba;
    RGBA a, b;
    float d;
    float dt = D_002DDED0.t - p->t68;
    R *pr;
    int x;
    Q *pq;
    if (dt < p->d5C) {
        a = p->rgba50;
        b = p->rgba54;
        d = p->d5C;
    } else {
        dt -= p->d5C;
        if (dt < p->d60) {
            a = p->rgba54;
            b = p->rgba54;
            d = p->d60;
        } else {
            dt -= p->d60;
            if (dt < p->d64) {
                a = p->rgba54;
                d = p->d64;
                b = p->rgba58;
            }
        }
    }
    rgba = RgbaLerp(dt / d, a, b);
    BUF buf;
    BUF *pbuf = &buf;
    Build(pbuf, rgba);
    {
    pr = &D_002DEA28;
    x = pr->xC;
    func_001574B8(pr);
    func_002270C8(pr, 0x25, pbuf, 0);
    }
    func_001576F0(pr);
    func_00157620(pr);
    pq = func_001CD998();
    pq->x8 = x;
    pq->x4 = -1.0f;
    switch (p->k6C) {
    case 0:
        pq->x0 = 1;
        break;
    case 1:
        pq->x0 = 0xE;
        break;
    }
}
