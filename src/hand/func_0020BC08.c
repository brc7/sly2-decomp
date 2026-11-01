typedef int qword __attribute__((mode(TI)));
struct Z { float a; float b; };
struct EN { int a; int b; struct Z *pz; };
struct X { char pad[0x18]; void *p18; float g; };
struct Y { char pad[0x50]; struct X *px; char pad2[0x74 - 0x54]; float f74; char pad3[0xD0 - 0x78]; struct EN *aen; };
struct S { int pad0; struct Y *py; char pad1[0x50 - 8]; float t50; char pad2[0x90 - 0x54]; int h90; float f94; float f98; int af[2]; };
float GModPositive__Fff(float a, float b);
void *func_001FBB90(int h);
void func_001FB2F8(void *p, void *source);
qword func_001A62E8(void *p);
void func_001FB3F0(void *p, qword q);
void func_001FB4F0(void *p, float a, float b);
void func_001FB1F8(void *p, float t);

void func_0020BC08(struct S *p)
{
    struct X *x = p->py->px;
    float g = x->g;
    int i;
    for (i = 0; i < 2; i++) {
        struct Y *y = p->py;
        struct Z *z = y->aen[i].pz;
        int f = z->b <= GModPositive__Fff(g - z->a, y->f74);
        if (f == p->af[i])
            continue;
        if (!f) {
            float t = p->t50;
            void *h;
            { float u = 1.0f - t; t *= p->f98; t += u * p->f94; }
            h = func_001FBB90(p->h90);
            func_001FB2F8(h, 0);
            func_001FB3F0(h, func_001A62E8(x->p18));
            func_001FB4F0(h, 500.0f, 2000.0f);
            func_001FB1F8(h, t);
        }
        p->af[i] = f;
    }
}
