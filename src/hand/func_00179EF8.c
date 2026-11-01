struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct T { char pad[0x2D4]; float g2D4; float g2D8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x20]; float g34; };
float GRandInRange__Fff(float a, float b);
void func_00179EF8(struct S *p) {
    if (p->g34 < 3.40282346638528859812e+38f) return;
    if (p->pt->g2D4 == 3.40282346638528859812e+38f) p->g34 = 3.40282346638528859812e+38f;
    else { float t = D_002DDED0.t; p->g34 = t + GRandInRange__Fff(p->pt->g2D4, p->pt->g2D8); }
}
