typedef int qword __attribute__((mode(TI)));
typedef union WORD { unsigned int bits; float value; } WORD;
typedef union U { qword q; float f[4]; } U;
struct ACTOR; struct MULTI;
struct A;
typedef struct { short d; short pad; void (*pfn)(struct A *, float, float, qword *, void *); } VT;
struct A { VT *vt; };
struct O { int x0; struct A *x4; };
struct K { char pad[0x74]; WORD x74; };
struct E { float x0; struct K *x4; int pad[2]; qword x10; };
struct O *func_0012FF10(struct K *k, struct ACTOR *p, void *a, int b);
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(struct ACTOR *p, int cb, int c, struct E *ae, int count, int *keys, int *counts, struct MULTI **pn);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char A_vt_Offset[FIELD_OFFSET(A,vt)==0?1:-1];
typedef char O_x4_Offset[FIELD_OFFSET(O,x4)==4?1:-1];
typedef char K_x74_Offset[FIELD_OFFSET(K,x74)==116?1:-1];
typedef char E_x0_Offset[FIELD_OFFSET(E,x0)==0?1:-1];
typedef char E_x4_Offset[FIELD_OFFSET(E,x4)==4?1:-1];
typedef char E_x10_Offset[FIELD_OFFSET(E,x10)==16?1:-1];
typedef char A_PrefixSize[sizeof(struct A)==4?1:-1];
typedef char O_PrefixSize[sizeof(struct O)==8?1:-1];
typedef char K_PrefixSize[sizeof(struct K)==120?1:-1];
typedef char E_PrefixSize[sizeof(struct E)==32?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char WORD_Size[sizeof(WORD)==4?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char VT_Size[sizeof(VT)==8?1:-1];
typedef char VT_d_Offset[((unsigned int)&(((VT*)0x10000)->d)-0x10000)==0?1:-1];
typedef char VT_fn_Offset[((unsigned int)&(((VT*)0x10000)->pfn)-0x10000)==4?1:-1];
void func_001A0300(struct ACTOR *p, struct E *ae, struct MULTI **pn)
{
    struct E *e = ae;
    int i;
    U v0, v1, t;
    struct MULTI *n;
    for (i = 0; i < 8; i++, e++) {
        if (e->x4) {
            struct O *o = func_0012FF10(e->x4, p, (void *)1, 1);
            if (o) {
                o->x4->vt[3].pfn((struct A *)((char *)o->x4 + o->x4->vt[3].d), 0.0f, 1.0f, &v0.q, 0);
                o->x4->vt[3].pfn((struct A *)((char *)o->x4 + o->x4->vt[3].d), e->x4->x74.value, 1.0f, &v1.q, 0);
                {
                    register qword d __asm__("vf2");
                    register qword scalar __asm__("vf1");
                    register struct K *clip __asm__("$2") = e->x4;
                    register unsigned int bits __asm__("$2") = clip->x74.bits;
                    __asm__("qmtc2.ni %1, %0" : "=j"(scalar) : "r"(bits));
                    d = v0.q;
                    __asm__("vsub.xyzw %0, %0, %1" : "+j"(d) : "j"(v1.q));
                    t.q = scalar;
                    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(d) : "j"(t.q));
                    e->x10 = d;
                }
            }
        }
    }
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p, 0x20, 8, ae, 0, 0, 0, &n);
    *pn = n;
}
