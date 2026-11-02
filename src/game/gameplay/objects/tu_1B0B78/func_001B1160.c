typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct VT { short d; short pad; void *pfn; };
struct CURVE;
struct P { char pad0[0xC]; struct VT *vt; char pad1[0x16C - 0x10]; struct CURVE *p16C; struct CURVE *p170; float g174; float g178;
    char pad2[0x1CC - 0x17C]; float g1CC; float g1D0; char pad3[0x220 - 0x1D4]; int x220; };
extern qword D_002A2DA0;
qword func_001A62E8(void *p);
struct SPACE;
void FindShapeClosestPointFromU(struct CURVE*,qword,struct SPACE*,qword*,qword*,float*,float*,float);
float UWrapShape(struct CURVE *p, float g);
float UMinShape(struct CURVE *p);
float UMaxShape(struct CURVE *p);
void EvaluateShapeU(struct CURVE*,float,qword*,qword*);
void func_001B1328(struct P *p, int n);
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qword r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char VU_Size[sizeof(VU)==16?1:-1];
typedef char VT_Size[sizeof(struct VT)==8?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==0x224?1:-1];
typedef char VT_d_Offset[FIELD_OFFSET(VT,d)==0?1:-1];
typedef char VT_pfn_Offset[FIELD_OFFSET(VT,pfn)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_p16C_Offset[FIELD_OFFSET(P,p16C)==364?1:-1];
typedef char P_p170_Offset[FIELD_OFFSET(P,p170)==368?1:-1];
typedef char P_g174_Offset[FIELD_OFFSET(P,g174)==372?1:-1];
typedef char P_g178_Offset[FIELD_OFFSET(P,g178)==376?1:-1];
typedef char P_g1CC_Offset[FIELD_OFFSET(P,g1CC)==460?1:-1];
typedef char P_g1D0_Offset[FIELD_OFFSET(P,g1D0)==464?1:-1];
typedef char P_x220_Offset[FIELD_OFFSET(P,x220)==544?1:-1];
typedef char LengthBits_Size[sizeof(int)==4?1:-1];
typedef char FallbackPointer_Size[sizeof(const qword*)==4?1:-1];
/** @sly2-annot addr=0x1B1160
 * Purpose: Advance along curve x16C (closest point, wrap/clamp, speed scaling), switch state at ends
 * Info: proposed name UpdateCid96PathFollow (T2); class CID96; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B1160(struct P *p)
{
    struct CURVE *q;
    float g, t;
    VU upos;
    switch (p->x220) {
    case 1:
        q = p->p16C;
        g = p->g1CC;
        break;
    case 4:
        q = p->p170;
        g = p->g1D0;
        break;
    default:
        q = 0;
        break;
    }
    if (!q)
        return;
    upos.q = func_001A62E8(p);
    FindShapeClosestPointFromU(q,upos.q,0,0,0,&p->g174,0,p->g174);
    t = UWrapShape(q, p->g174 + p->g178);
    if (t < UMinShape(q) || UMaxShape(q) < t) {
        if (p->x220 == 1)
            func_001B1328(p, 2);
    } else {
        VU v, uc, ur, ub;
        qword d, c, l, n, b, r;
        register qword k __asm__("$2");
        float gl;
        register int lengthBits __asm__("$3");
        EvaluateShapeU(q,t,&v.q,0);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v.q), "j"(upos.q));
        {
            register const qword *address __asm__("$2") = &D_002A2DA0;
            k = *address;
        }
        __asm__("vmove.xyzw %0, %1" : "=j"(c) : "j"(d));
        uc.q = c;
        l = VLen(uc.q);
        __asm__("qmfc2.ni %0,%2\n\tmtc1 %0,%1" : "=&r"(lengthBits), "=f"(gl) : "j"(l));
        if (gl < 1e-4f) {
            n = k;
        } else {
            __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(n) : "j"(d), "j"(l));
        }
        __asm__("qmtc2.ni %1, %0" : "=j"(b) : "r"(g));
        ub.q = b;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(n), "j"(ub.q));
        ur.q = r;
        ((void (*)(void *, qword))p->vt[35].pfn)((char *)p + p->vt[35].d, ur.q);
    }
}
