typedef int qword __attribute__((mode(TI)));
struct VT { short d; short pad; void *pfn; };
struct O { char pad[0x80]; qword v80; };
struct P {
    char pad0[0xC]; struct VT *vt; char pad1[0xE8 - 0x10]; struct O *pE8; char pad1b[0x190 - 0xEC]; qword v190;
    char pad4[0x13D8 - 0x1A0]; int x13D8;
};
extern "C" qword D_002A2DC0;
enum { FACTOR_ADDRESS = 0x002C09A4 };
extern "C" void func_001FAC30(struct P *p, qword v);
typedef union { qword q; float f[4]; } U;
static inline qword Scale(qword a, float g) { U u; qword k, r; __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(g)); u.q = k; __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(a), "j"(u.q)); return r; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Factor(qword a,qword k,qword negative,U *pu) {
    U &u=*pu; register qword r __asm__("vf3");
    __asm__("sqc2 %2,%0\n\tvmulx.xyzw %1,%3,%2x\n\tsqc2 %4,%0":"=m"(u.q),"=&j"(r):"j"(k),"j"(a),"j"(negative));
    return r;
}
static inline qword Projection(qword a,qword b) {
    register qword r __asm__("vf3"),one __asm__("vf4");
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvmulx.xyzw %0,%3,%0x":"=&j"(r),"=&j"(one):"j"(a),"j"(b));
    return r;
}
/* The load replaces its address carrier with the actual scalar word. */
static inline int ReadFactor(void){unsigned int bits=FACTOR_ADDRESS & ~0xFFFF;__asm__("lw %0,%1":"+r"(bits):"m"(((int*)bits)[(FACTOR_ADDRESS&0xFFFF)/4]));return bits;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char VT_d_Offset[FIELD_OFFSET(VT,d)==0?1:-1];
typedef char VT_pfn_Offset[FIELD_OFFSET(VT,pfn)==4?1:-1];
typedef char O_v80_Offset[FIELD_OFFSET(O,v80)==128?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_pE8_Offset[FIELD_OFFSET(P,pE8)==232?1:-1];
typedef char P_v190_Offset[FIELD_OFFSET(P,v190)==400?1:-1];
typedef char P_x13D8_Offset[FIELD_OFFSET(P,x13D8)==5080?1:-1];
typedef char VT_PrefixSize[sizeof(struct VT)==8?1:-1];
typedef char O_PrefixSize[sizeof(struct O)==144?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==5088?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char int_Size[sizeof(int)==4?1:-1];
typedef char U_q_Offset[((unsigned int)&(((U*)0x10000)->q)-0x10000)==0?1:-1];
typedef char Factor_Address[FACTOR_ADDRESS==0x002C09A4?1:-1];
/** @sly2-annot addr=0x1B6CC0
 * Purpose: State-3 accel helper: substate 0 cancel ground velocity, substate 1 add horizontal correction
 *   toward desired velocity (AddSoVelocity)
 * Info: proposed name PresetMurrayFlopAccel (T2); TU murray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
extern "C" void func_001B6CC0(struct P *p)
{
    switch (p->x13D8) {
    case 0:
        func_001FAC30(p, Scale(p->pE8->v80, -1.0f));
    case 1: {
        qword v = ((qword (*)(void *))p->vt[33].pfn)((char *)p + p->vt[33].d);
        U u; register qword kn __asm__("vf5"); register qword n __asm__("vf1");
        __asm__("qmtc2.ni %1,%0":"=j"(kn):"r"(-1.0f));
        __asm__("vmulx.xyzw %0,%1,%2x":"=j"(n):"j"(p->v190),"j"(kn));
        int bits = ReadFactor();
        qword d = Sub(n, v);
        qword up = D_002A2DC0;
        qword projected = Projection(d,up);
        /* Projection has retired vf4; both outputs feed the final scale. */
        register qword factor __asm__("vf4"); register qword difference __asm__("vf1");
        __asm__("qmtc2.ni %2,%1\n\tvsub.xyzw %0,%3,%4":"=j"(difference),"=&j"(factor):"r"(bits),"j"(d),"j"(projected));
        d=difference;
        qword f = Factor(d, factor, kn, &u);
        func_001FAC30(p, f);
        break;
    }
    case 2:
        break;
    }
}
