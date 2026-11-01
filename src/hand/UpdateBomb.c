typedef int qword __attribute__((mode(TI)));
struct VT { short d; short pad; void *pfn; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x154 - 0x10]; float t154; float g158; char pad3[0x1F4 - 0x15C]; int f1F4; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
void UpdateSo(struct P *p);
void DetonateBomb(struct P *p);
void UpdateBombCustomXps(struct P *p);
static __inline__ float GLength(qword v)
{
    qword d, t;
    float g;
    __asm__("vmul.xyz %0, %1, %1\n"
        "vaddw.x %2, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %2, %0z" : "=j"(d), "+j"(v), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
void UpdateBomb(struct P *p)
{
    UpdateSo(p);
    if (p->t154 <= D_002DDED0.t)
        goto out;
    {
        struct VT *vt = p->pvt;
        qword v = ((qword (*)(void *))vt[33].pfn)((char *)p + vt[33].d);
        int f = GLength(v) < p->g158;
        if (f) {
        out:
            DetonateBomb(p);
        } else if (p->f1F4) {
            UpdateBombCustomXps(p);
        }
    }
}
