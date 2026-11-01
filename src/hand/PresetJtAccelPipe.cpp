typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
#include "mat.h"
typedef struct MATRIX3 M3;
typedef struct { short d; short pad; qword (*pfn)(void *); } VT;
struct S14 { char pad[0x74]; float x74; };
struct S1030 { char pad[0x14]; struct S14 *x14; int pad2; float x1c; };
struct O { char pad[0xC]; VT *vt; char pad2[0x1030-0x10]; struct S1030 *x1030; };
struct CURVE;
struct P { struct O *x0; int x4; int x8; struct CURVE *xc; float x10; int x14; int x18; };
struct CLOCK { float t; float dt; };
extern "C" struct CLOCK D_002DDED0;
extern "C" qword D_002A2DD0;
extern "C" void func_001C3500(struct CURVE *a, int b, M3 *pm, float g);
extern "C" M3 *func_001A6318(struct O *o);
extern "C" void EvaluateBezierMat(M3 *a, float g, float dt, float one, qword v, M3 *pm, qword z, M3 *pm2, qword *derivative);
extern "C" M3 MulMatrix3Matrix3(M3 *a, M3 *b);
extern "C" void func_0011BCF8(M3 *pm, float *pg, U *pv);
static inline qword Scale(qword v,float g){U u;register qword s __asm__("vf2");qword r;__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(g));u.q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(u.q));return r;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char S14_x74_Offset[FIELD_OFFSET(S14,x74)==116?1:-1];
typedef char S1030_x14_Offset[FIELD_OFFSET(S1030,x14)==20?1:-1];
typedef char S1030_x1c_Offset[FIELD_OFFSET(S1030,x1c)==28?1:-1];
typedef char O_vt_Offset[FIELD_OFFSET(O,vt)==12?1:-1];
typedef char O_x1030_Offset[FIELD_OFFSET(O,x1030)==4144?1:-1];
typedef char P_x0_Offset[FIELD_OFFSET(P,x0)==0?1:-1];
typedef char P_x4_Offset[FIELD_OFFSET(P,x4)==4?1:-1];
typedef char P_xc_Offset[FIELD_OFFSET(P,xc)==12?1:-1];
typedef char P_x10_Offset[FIELD_OFFSET(P,x10)==16?1:-1];
typedef char P_x18_Offset[FIELD_OFFSET(P,x18)==24?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char S14_PrefixSize[sizeof(struct S14)==120?1:-1];
typedef char S1030_PrefixSize[sizeof(struct S1030)==32?1:-1];
typedef char O_PrefixSize[sizeof(struct O)==4148?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==28?1:-1];
typedef char CLOCK_PrefixSize[sizeof(struct CLOCK)==8?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char VT_Size[sizeof(VT)==8?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char VT_d_Offset[((unsigned int)&(((VT*)0x10000)->d)-0x10000)==0?1:-1];
typedef char VT_fn_Offset[((unsigned int)&(((VT*)0x10000)->pfn)-0x10000)==4?1:-1];
typedef char M3_mat_Offset[((unsigned int)&(((M3*)0x10000)->mat)-0x10000)==0?1:-1];
extern "C" void PresetJtAccelPipe(struct P *p)
{
    M3 m, m2, m4;
    U vec;
    float f;
    M3 *pm;
    func_001C3500(p->xc, p->x18 == 0, &m, p->x10);
    if (p->x4 == 0 || p->x4 == 4) {
        struct O *o = p->x0;
        float dt = D_002DDED0.dt;
        float g = o->x1030->x14->x74 - o->x1030->x1c;
        M3 *pa;
        g = g > dt ? g : dt;
        pa = func_001A6318(o);
        EvaluateBezierMat(pa, g, dt, 1.0f, p->x0->vt[34].pfn((char *)p->x0 + p->x0->vt[34].d), &m, D_002A2DD0, &m2, 0);
    } else {
        m2 = m;
    }
    pm = &m2;
    m4 = MulMatrix3Matrix3(func_001A6318(p->x0), pm);
    func_0011BCF8(&m4, &f, &vec);
    {O *o=p->x0;VT *vt=o->vt;void (*call)(void *,qword)=(void(*)(void*,qword))vt[36].pfn;void *thisp=(char*)o+vt[36].d;call(thisp,Scale(vec.q,f/D_002DDED0.dt));}
}
