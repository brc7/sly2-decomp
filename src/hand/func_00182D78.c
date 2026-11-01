typedef int qword __attribute__((mode(TI)));
struct VT { struct { short d; short pad; void *(*pfn)(void *); } a[8]; };
struct O { char pad[0x20]; struct VT *pvt; };
extern qword func_001A62E8(void *p);
extern qword SetVectorSphere(float a, float b, float c);
extern float GRandInRange__Fff(float a, float b);
extern float D_002BE8B0;
extern qword D_002A2DC0;
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
void func_00182D78(struct O *p, qword *pv, qword *pdv)
{
    void *po = p->pvt->a[6].pfn((char *)p + p->pvt->a[6].d);
    float gPan = GRandInRange__Fff(-3.14159265f, 3.14159265f);
    float t = GRandInRange__Fff(-1.0f, 1.0f);
    float gTilt = __builtin_fabsf(t) * t * 1.5707964f;
    float gR = GRandInRange__Fff(0.0f, D_002BE8B0);
    *pv = VAdd(func_001A62E8(po), SetVectorSphere(gPan, gTilt, gR));
    *pdv = D_002A2DC0;
}
