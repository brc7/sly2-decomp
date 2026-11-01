struct Y { char pad[0x12C]; float g; };
struct P {
    char pad0[0x2D]; unsigned char b2D; char pad1[0x80 - 0x2E]; char m80[0x12C - 0x80]; float g12C;
    char pad2[0x134 - 0x130]; int x134; char pad3[0x4C8 - 0x138]; int x4C8; char pad4[0xF04 - 0x4CC]; int xF04;
    char pad5[0xF20 - 0xF08]; int xF20; char pad6[0x10D4 - 0xF24]; float g10D4; float g10D8;
    char pad7[0x19A0 - 0x10DC]; struct Y *py0; struct Y *py1;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern int D_002DDF54;
extern struct P *D_002E1E40;
extern float D_002BD268, D_002BD26C;
extern char D_002BD270[];
void func_001F3C10(struct P *p);
float GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, int n);
void func_00168638(void *p, int n, int m, float a, float b);
void func_001425B0(struct P *p)
{
    float g0, g1;
    if (!(p->b2D & D_002DDF54)) return;
    func_001F3C10(p);
    if (p->xF04 == 1 && p->xF20 != 2) return;
    if (p->x4C8 == 2) {
        g0 = D_002BD268;
        g1 = D_002BD26C;
    } else {
        g1 = 1.0f;
        g0 = g1;
    }
    p->g10D4 = GSmooth__FfffP3SMPPf(p->g10D4, g0, D_002DDED0.dt, D_002BD270, 0);
    p->g10D8 = GSmooth__FfffP3SMPPf(p->g10D8, g1, D_002DDED0.dt, D_002BD270, 0);
    if (p == D_002E1E40) {
        p->py0->g = p->g10D4;
        p->py1->g = p->g10D4;
    }
    func_00168638(p->m80, 0, p->x134, p->g12C * p->g10D4, p->g12C * p->g10D8);
}
