typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[9]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x28 - 0x10]; ulong fl; char pad3[0x12C - 0x30]; float g12C; };
void SetSoFadeRate(struct P *, float);
void SetSoAlpha(struct P *, float);
void FadeSoIn(struct P *p, float f)
{
    if (f <= 0.0f) {
        SetSoFadeRate(p, 0.0f);
        SetSoAlpha(p, 1.0f);
    } else {
        if (p->fl & (1L << 56)) {
            p->pvt->a[8].pfn((char *)p + p->pvt->a[8].d);
            p->g12C = 0.0f;
        }
        SetSoFadeRate(p, 1.0f / f);
    }
}
