struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE5A0;
struct T { char pad[0xE6C]; float gE6C; float gE70; int pad2; int nE78; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; float g38; float g3C; };
float GRandInRange__Fff(float a, float b);
void func_00172560(struct S *p) {
    float t = D_002DDED0.t;
    p->g38 = t + GRandInRange__Fff(p->pt->gE6C, p->pt->gE70);
    if (p->pt->nE78 == 1) p->g3C = D_002DDED0.t + D_002BE5A0;
    else p->g3C = 3.40282346638528859812e+38f;
}
