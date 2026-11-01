struct T { float a; float b; float c; };
struct V2 { float x, y; };
struct P {
    int n0; int n4; int pad8; void *pvt; int n10; char pad14[0x1C - 0x14]; char x1C[0x18]; char x34[0x18]; char x4C[0xC];
    int n58; char pad5C[0x68 - 0x5C]; int n68; char pad6C[0x80 - 0x6C]; char x80[0x10]; char x90[0x10];
    char padA0[0x104 - 0xA0]; int n104; char pad108[0x114 - 0x108]; struct T t114; struct T t120; struct T t12C;
    char pad138[0x200 - 0x138]; int n200; int pad204; int a208[8]; int n228;
};
extern char D_002CA510[];
extern const struct V2 D_002A83B8;
extern const struct V2 D_002A83C0;
void func_0011CCA8(void *, int, int, int);
void InitDl__FP2DLi(void *, int);
void func_0011D8D8(float, float, float, void *, float);
static inline void InitT(struct T *pt, float a, float c)
{
    pt->a = a;
    pt->b = 0.0f;
    pt->c = c;
}
struct P *func_001FCD38(struct P *p)
{
    int n;
    int *q;
    p->n0 = 0;
    p->n4 = 0;
    p->pvt = D_002CA510;
    func_0011CCA8(p->x1C, 0x70, 0x80, 0x10);
    func_0011CCA8(p->x34, 0x3C0, 0x10, 0x10);
    p->n200 = 0;
    q = p->a208;
    for (n = 7; n != -1; n--) *q++ = 0;
    p->n228 = 0;
    InitDl__FP2DLi(p->x4C, 0x38);
    p->n68 = 3;
    p->n10 = 0;
    p->n104 = 0;
    InitT(&p->t12C, 1.0f, 0.25f);
    InitT(&p->t120, 1.0f, 0.25f);
    {
        struct T *pt = &p->t114;
        float b;
        float h = 0.5f;
        InitT(pt, 3.0f, 0.25f);
        b = pt->b;
        func_0011D8D8(b, D_002A83B8.y, h, p->x80, D_002A83B8.x);
        func_0011D8D8(b, D_002A83C0.y, h, p->x90, D_002A83C0.x);
    }
    p->n58 = 0;
    return p;
}
