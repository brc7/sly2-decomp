struct S60 { char pad[0x40]; float x40; float x44; };
struct S150 { char pad[0x18]; float x18; };
struct S1030 { char pad[0x1C]; float x1c; char pad2[0x40-0x20]; float x40; };
struct O { char pad[0x60]; struct S60 *x60; char pad2[0x150-0x64]; struct S150 *x150; char pad3[0x1030-0x154]; struct S1030 *x1030; char x1034[4]; };
struct P { struct O *x0; int x4; int x8; int pad[4]; int x1c; char pad2[0x30-0x20]; int x30; char pad3[0x120-0x34]; void *a120[5]; void *a134[5]; };
float func_002458F0(float y, float x);
float RadNormalize__Ff(float g);
void func_001F15B8(void *p);
void func_001305B8(void *a, struct O *o, int n, void *b, float g, float h);
#define Get(p, i) ((p)->x1c == 2 ? (p)->a134[i] : (p)->a120[i])
void func_00197370(struct P *p)
{
    float d, ad;
    struct S1030 *s;
    void *pv;
    p->x30 = 1;
    d = RadNormalize__Ff(p->x0->x150->x18 - func_002458F0(p->x0->x60->x44, p->x0->x60->x40));
    ad = __builtin_fabsf(d);
    if (ad < 1.0995574f)
        p->x8 = 3;
    else if (ad > 2.0420351f)
        p->x8 = 4;
    else if (d > 0.0f)
        p->x8 = 2;
    else
        p->x8 = 1;
    s = p->x0->x1030;
    pv = Get(p, p->x8);
    if (!pv)
        pv = Get(p, 0);
    if (!pv)
        pv = p->a120[0];
    func_001F15B8(p->x0->x1034);
    func_001305B8(pv, p->x0, 1, &p->x0->x1030, s->x1c, s->x40);
}
