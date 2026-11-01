typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct VT { short d; short pad; void *pfn; };
struct LO { char pad[0x28]; unsigned long x28; };
struct T { char pad[0x180]; struct LO *po; };
struct PL { char pad[0x18]; float g18; float g1C; char pad2[0x30 - 0x20]; int x30; };
struct P {
    char pad0[0xC]; struct VT *vt; char pad1[0x150 - 0x10]; struct PL *pl;
    char pad2[0x190 - 0x154]; qword v190; char pad3[0x2C0 - 0x1A0]; char x2C0[0xF00 - 0x2C0]; char xF00[0x1030 - 0xF00];
    struct PL *p1030; char pad4[0x13D0 - 0x1034]; int x13D0; char pad5[0x13F0 - 0x13D4]; struct T *p13F0;
    char pad6[0x1438 - 0x13F4]; int x1438; int x143C; char pad7[0x1458 - 0x1440]; int x1458; char pad8[0x1460 - 0x145C]; char x1460[1];
};
void DetachMurrayCarried(struct P *p);
void SetMurrayState(struct P *p, int n);
int GrfPressedWithin__5POCTLUif(struct PL *pl,unsigned int k,float g);
int GrfConsumePressed__5POCTLUif(struct PL *pl, int k, float g);
void ClearPoctlJoyPressed(struct PL *pl, int k);
int func_001DAEA8(int n);
void func_00195A00(void *p, int a, int b, int c);
void func_0019CC50(void *p, int a, int b, int c);
void SetMurrayCarrySubstate(struct P *p, int n);
void func_00204660(struct P *p, float g);
qword func_00204E38(struct P *p,struct PL*pl,float g);
void func_0020BB58(void *p, float g);
void func_0020BC08(void *p);
static inline void NegateSource(qword value,qword*velocity,U*scalar){
    register qword input __asm__("vf3"),neg __asm__("vf1"),result __asm__("vf2");
    register int bits __asm__("$3");
    float negative=-1.0f;
    __asm__("qmtc2.ni %1,%0":"=j"(input):"r"(value));
    __asm__("mfc1 %0,%2\n\tqmtc2.ni %0,%1":"=&r"(bits),"=j"(neg):"f"(negative));
    __asm__("vmulx.xyzw %0,%1,%2x":"=j"(result):"j"(input),"j"(neg));
    scalar->q=neg;
    *velocity=result;
}
static inline struct PL*CopyPL(struct PL*p){register struct PL*source __asm__("$2")=p;register struct PL*copy __asm__("$4");__asm__("daddu %0,%1,$zero":"=r"(copy):"r"(source));return copy;}
void UpdateMurrayCarry(struct P *p)
{
    int s;
    for (;;) {
        s = p->x1458;
        switch (s) {
        case 0: case 1: case 2:
            if (!((int)(p->p13F0->po->x28 >> 60) & 1)) {
                DetachMurrayCarried(p);
                p->p13F0 = 0;
                SetMurrayState(p, 0);
            } else if (GrfPressedWithin__5POCTLUif(p->pl, 0x40, 0.1f) && func_001DAEA8(0x12)) {
                ClearPoctlJoyPressed(p->pl, 0x40);
                func_00195A00(p->x2C0, 0xF, 0, 0);
            } else if (GrfConsumePressed__5POCTLUif(p->pl, 0xA0, 0.0f)) {
                SetMurrayState(p, 7);
            } else if (GrfConsumePressed__5POCTLUif(p->pl, 0x10, 0.0f) && p->x1438) {
                p->x143C = 0;
                SetMurrayState(p, 8);
            } else if (GrfPressedWithin__5POCTLUif(p->pl, 0x400, 0.15f) && ((int (*)(void *, int))p->vt[99].pfn)((char *)p + p->vt[99].d, 3)) {
                ClearPoctlJoyPressed(p->pl, 0x400);
                func_0019CC50(p->xF00, 0, 0, 0);
            } else {
                struct PL*source=p->pl;
                struct PL*pl=CopyPL(source);
                float g=source->g1C;
                if (p->x1458 == 0 ? 0.1f < g : 0.0f < g) {
                    if ((pl->x30 & 8) && func_001DAEA8(0x12))
                        SetMurrayCarrySubstate(p, 2);
                    else
                        SetMurrayCarrySubstate(p, 1);
                } else {
                    SetMurrayCarrySubstate(p, 0);
                }
            }
            break;
        }
        if (p->x1458 == s || p->x13D0 != 5)
            break;
    }
    switch (p->x1458) {
    case 0:
        break;
    case 1:
        func_00204660(p, p->pl->g18);
        func_0020BB58(p->x1460, p->pl->g1C);
        func_0020BC08(p->x1460);
        break;
    case 2: {
        U u;
        qword v;
        func_00204660(p, p->pl->g18);
        v = func_00204E38(p,p->p1030,p->p1030->g1C);
        NegateSource(v,&p->v190,&u);
        break;
    }
    }
}
