typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { short d; short pad; qword (*pfn)(void *); } VT;
struct E8 { char pad[0x80]; qword x80; };
struct O { char pad[0xC]; VT *vt; char pad2[0xE8-0x10]; struct E8 *xe8; };
struct S { char pad[0x1BC]; int x1bc; float x1c0; float x1c4; };
struct P { struct O *x0; int x4; int x8; int pad[2]; struct S *x14; int x18; float x1c; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct SMP { float a,b,c; };
extern struct SMP D_002BF700;
void func_001E67B8(struct S *s, U *position, U *direction, float g);
float GSmooth__FfffP3SMPPf(float a, float b, float dt, struct SMP *p, float *pg);
void func_001FAC30(struct O *o, qword q);
static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo)
        return lo;
    if (g > hi)
        return hi;
    return g;
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char E8_x80_Offset[FIELD_OFFSET(E8,x80)==128?1:-1];
typedef char O_vt_Offset[FIELD_OFFSET(O,vt)==12?1:-1];
typedef char O_xe8_Offset[FIELD_OFFSET(O,xe8)==232?1:-1];
typedef char S_x1bc_Offset[FIELD_OFFSET(S,x1bc)==444?1:-1];
typedef char S_x1c0_Offset[FIELD_OFFSET(S,x1c0)==448?1:-1];
typedef char S_x1c4_Offset[FIELD_OFFSET(S,x1c4)==452?1:-1];
typedef char P_x0_Offset[FIELD_OFFSET(P,x0)==0?1:-1];
typedef char P_x8_Offset[FIELD_OFFSET(P,x8)==8?1:-1];
typedef char P_x14_Offset[FIELD_OFFSET(P,x14)==20?1:-1];
typedef char P_x1c_Offset[FIELD_OFFSET(P,x1c)==28?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char SMP_a_Offset[FIELD_OFFSET(SMP,a)==0?1:-1];
typedef char SMP_b_Offset[FIELD_OFFSET(SMP,b)==4?1:-1];
typedef char SMP_c_Offset[FIELD_OFFSET(SMP,c)==8?1:-1];
typedef char E8_PrefixSize[sizeof(struct E8)==144?1:-1];
typedef char O_PrefixSize[sizeof(struct O)==236?1:-1];
typedef char S_PrefixSize[sizeof(struct S)==456?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==32?1:-1];
typedef char CLOCK_PrefixSize[sizeof(struct CLOCK)==8?1:-1];
typedef char SMP_PrefixSize[sizeof(struct SMP)==12?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char VT_Size[sizeof(VT)==8?1:-1];
typedef char VT_d_Offset[((unsigned int)&(((VT*)0x10000)->d)-0x10000)==0?1:-1];
typedef char VT_fn_Offset[((unsigned int)&(((VT*)0x10000)->pfn)-0x10000)==4?1:-1];
void func_00194170(struct P *p)
{
    U v, w, r, k;
    float a, b;
    struct S *s;
    float d1, d2, x;
    struct SMP *psmp;
    if (p->x8 != 3)
        return;
    s = p->x14;
    if (!s->x1bc)
        return;
    v.q = p->x0->vt[33].pfn((char *)p->x0 + p->x0->vt[33].d);
    func_001E67B8(s, 0, &w, p->x1c);
    psmp = &D_002BF700;
    {
    register float goal __asm__("$f13");
    register float dt __asm__("$f14");
    {
        register qword velocity __asm__("vf1") = v.q;
        register qword direction __asm__("vf3") = w.q;
        register qword other __asm__("vf2");
        register qword one __asm__("vf4");
        __asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(velocity), "=&j"(one) : "j"(direction));
        __asm__("qmfc2.ni %0,%1" : "=r"(d1) : "j"(velocity));
        other = p->x0->xe8->x80;
        __asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(direction), "=&j"(one) : "j"(other));
        {
            int bits;
            goal = s->x1c0;
            __asm__("qmfc2.ni %0,%1" : "=r"(bits) : "j"(direction));
            dt = D_002DDED0.dt;
            __asm__ __volatile__("mtc1 %1,%0" : "=f"(d2) : "r"(bits));
        }
    }
    (void)GSmooth__FfffP3SMPPf(d1, goal, dt, psmp, &a);
    }
    (void)GSmooth__FfffP3SMPPf(d1, -s->x1c0, D_002DDED0.dt, psmp, &b);
    x = GClamp(s->x1c4 * d2, b, a) - d2;
    __asm__("qmtc2.ni %1, %0" : "=j"(k.q) : "r"(x));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r.q) : "j"(w.q), "j"(k.q));
    func_001FAC30(p->x0, r.q);
}
