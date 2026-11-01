struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[3]; };
struct X { char pad[0xC]; struct VT *pvt; };
void func_001F0598(struct X *);
struct X *PsmaApplySmEx(void *, int, int, int, int);
void ReplaceSmaEx(void *pv, int a, int b, int c, int d, struct X **ppx)
{
    if (*ppx) {
        func_001F0598(*ppx);
        (*ppx)->pvt->a[2].pfn((char *)*ppx + (*ppx)->pvt->a[2].d);
        *ppx = 0;
    }
    if (pv) {
        struct X *px = PsmaApplySmEx(pv, a, b, c, d);
        *ppx = px;
        px->pvt->a[1].pfn((char *)px + px->pvt->a[1].d);
    }
}
