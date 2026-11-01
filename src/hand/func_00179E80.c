struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct T { char pad[0x2CC]; float g2CC; float g2D0; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x1C]; float g30; };
float GRandInRange__Fff(float a, float b);
void func_00179E80(struct S *p) {
    if (p->g30 < 3.40282346638528859812e+38f) return;
    if (p->pt->g2CC == 3.40282346638528859812e+38f) p->g30 = 3.40282346638528859812e+38f;
    else { float t = D_002DDED0.t; p->g30 = t + GRandInRange__Fff(p->pt->g2CC, p->pt->g2D0); }
}
