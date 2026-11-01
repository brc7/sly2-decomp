struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
float GRandInRange__Fff(float a, float b);
struct X { char pad[0x1138]; float a0, b0, a1, b1; int f; float tNext; };
void func_00142880(struct X *p)
{
    float t;
    if (p->f) {
        t = D_002DDED0.t;
        t += GRandInRange__Fff(p->a0, p->b0);
        p->tNext = t;
    } else {
        t = D_002DDED0.t;
        t += GRandInRange__Fff(p->a1, p->b1);
        p->tNext = t;
    }
}
