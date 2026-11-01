typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0x28]; ulong fl; };
struct P { char pad[0xC]; struct VTE *pvt; float f10; char pad1[0x1C - 0x14]; struct O *po; qword v20; char x30[0x14]; float f44; };
void func_001FB098(struct P *, float);
qword ConvertAloPos(struct O *, int, qword);
void func_001FB778(struct P *, qword, void *, float *, int *);
void func_001FB658(struct P *, int, int);
void func_001FB268(struct P *, int);
void func_001FB528(struct P *p)
{
    float f;
    int n;
    float r;
    if (p->po && !((p->po->fl >> 60) & 1)) {
        func_001FB098(p, 0.0f);
        return;
    }
    func_001FB778(p, ConvertAloPos(p->po, 0, p->v20), p->x30, &f, &n);
    r = p->f10 * f;
    if ((int)(r * 1024.0f) != (int)(p->f44 * 1024.0f)) {
        if (p->f44 == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 0);
        else if (r == 0.0f) p->pvt[15].pfn((char *)p + p->pvt[15].d, 1);
        p->f44 = r;
        func_001FB658(p, 0x10, 0x10);
    }
    func_001FB268(p, n);
}
