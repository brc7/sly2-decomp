typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct WORK{U u0,v10,v20,s;float at;U uq;qword incoming;U sum;};
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
struct C { char pad[0x88]; float g88; };
struct B { char pad[0xE8]; struct C *pc; };
struct S { struct B *pb; };
struct SPACE;
typedef struct { short d; short pad; struct SPACE *(*pfn)(void *, void *); } VTE;
struct O { char pad[0xC]; VTE *vt; };
qword func_001A62E8(void *p);
qword GetXfVelocity(struct SPACE *p, struct SPACE *stop, qword q);
int FTimeJtJumpToTarget(struct S *p, qword a, qword b, qword c, qword d, float *pg);
void EvaluateBezierPos(float tMax, float t, float s, qword a, qword va, qword b, qword vb, qword *pp, qword *pv, qword *pa);
static __inline__ float LenXY0(qword v)
{
    register float r __asm__("$2");
    __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y" : "+j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(v));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(v));
    return r;
}
static __inline__ unsigned LenXY1(qword v)
{
    register unsigned bits __asm__("$3");
    __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y" : "+j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(v));
    __asm__("qmfc2.ni %0, %1" : "=r"(bits) : "j"(v));
    return bits;
}
static inline float GMax(float a, float b) { return a > b ? a : b; }
/** @sly2-annot addr=0x19A4A0
 * Purpose: Predict a jump from Sly's pos/velocity to a target point on (moving) object: time, cost g,
 *   predicted pos/velocity
 * Info: proposed name MeasureJtJumpToTarget (T1); TU air
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void MeasureJtJumpToTarget(struct S *p, qword aInput, struct O *o, qword bInput, qword c, float *pt, float *pg, qword *pv, qword *pw)
{
    struct WORK w;
    register qword q __asm__("$16");
    qword r;
    register qword b __asm__("$17"),a __asm__("$19"),pos __asm__("$18");
    int solved;
    register float time __asm__("$f1");
    struct SPACE*x;
    __asm__("por %0,$zero,%1":"=r"(b):"r"(bInput));
    __asm__("por %0,$zero,%1":"=r"(a):"r"(aInput));
    w.incoming=c;
    __asm__("por %0,$zero,%1":"=r"(pos):"r"(func_001A62E8(p->pb)));
    x=o->vt[32].pfn((char*)o+o->vt[32].d,0);
    { register qword got __asm__("$2")=GetXfVelocity(x, 0, b);
      register struct S*receiver __asm__("$4");
      register qword initial __asm__("$5"),velocity __asm__("$6");
      __asm__ volatile("sq %1,%0":"=m"(w.uq.q):"r"(got));
      __asm__("daddu %0,%4,$zero\n\tpor %1,$zero,%5\n\tpor %2,$zero,%6\n\tpor %3,$zero,%7"
       :"=&r"(receiver),"=&r"(initial),"=&r"(velocity),"=&r"(q)
       :"r"(p),"r"(pos),"r"(a),"r"(got));
      solved=FTimeJtJumpToTarget(receiver,initial,velocity,b,q,&w.at); }
    if (!solved) {
        float dz = GetZ(a) - GetZ(q);
        float g = p->pb->pc->g88;
        float t1 = -dz / g;
        float h;
        t1 = GMax(0.0f, t1);
        { register float height __asm__("$f6") = GetZ(pos);
          register float end __asm__("$f4") = GetZ(b);
          float th = t1 * 0.5f;
          __asm__("sub.s %0,%0,%1":"+f"(height):"f"(end));
          h = height + (g * th * t1 + dz * t1); }
        if (pt) *pt = t1;
        if (pg) *pg = 1010000.0f - h;
        return;
    }
    time=w.at;
    {
        register qword base __asm__("vf3");
        register float step __asm__("$f0");
        register unsigned bits __asm__("$2");
        __asm__("qmtc2.ni %1,%0":"=j"(base):"r"(b));
        step=0.15f;
        {
            __asm__("lqc2 $vf2,%4\n\tlqc2 $vf1,%5\n\tmin.s %0,%0,%3\n\tvadd.xyzw $vf1,$vf1,$vf2\n\tmfc1 %1,%0\n\tsqc2 $vf1,%2"
            : "+f"(step),"=r"(bits),"=m"(w.sum.q)
            : "f"(time),"m"(w.incoming),"m"(w.uq.q)
            : "vf1","vf2","memory");
        }
        {
            register qword scalar __asm__("vf1");
            register qword old __asm__("vf4")=w.uq.q;
            register qword out __asm__("vf2");
            __asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));
            __asm__("vadda.xyzw ACC,%2,$vf0\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %4,%1":"=j"(out),"=m"(w.s.q):"j"(base),"j"(old),"j"(scalar):"memory");
            w.u0.q=out;
        }
    }
    __asm__("lq %0,%1":"=r"(r):"m"(w.sum.q));
    if (pt) *pt = time;
    if (pg) {
        float d1;
        register unsigned second __asm__("$3");
        register float last __asm__("$f1");
        register float halfTime __asm__("$f0");
        register float half __asm__("$f3");
        register float limit __asm__("$f2");
        EvaluateBezierPos(w.at, 0.0f, 1.0f, pos, a, w.u0.q, w.sum.q, 0, 0, &w.v10.q);
        d1 = LenXY0(w.v10.q);
        EvaluateBezierPos(w.at, w.at, 1.0f, pos, a, w.u0.q, w.sum.q, 0, 0, &w.v20.q);
        second=LenXY1(w.v20.q);
        halfTime=w.at; half=0.5f; limit=1000000.0f;
        __asm__("mtc1 %4,%2\n\tmul.s %1,%1,%5\n\tadd.s %0,%0,%2\n\tmul.s %0,%0,%1\n\tmin.s %0,%0,%3"
         :"+f"(d1),"+f"(halfTime),"=&f"(last):"f"(limit),"r"(second),"f"(half));
        *pg=d1;
    }
    if (pv) *pv = w.u0.q;
    if (pw) *pw = r;
}

#define OFF(t,m) ((unsigned int)&(((t*)0)->m))
typedef char Pointer_size[sizeof(void*)==4?1:-1];
typedef char Float_size[sizeof(float)==4?1:-1];
typedef char Int_size[sizeof(int)==4?1:-1];
typedef char Short_size[sizeof(short)==2?1:-1];
typedef char Qword_size[sizeof(qword)==16?1:-1];
typedef char Qword_alignment[__alignof__(qword)==16?1:-1];
typedef char U_size[sizeof(U)==16?1:-1];
typedef char U_alignment[__alignof__(U)==16?1:-1];
typedef char U_scalar[OFF(U,f)==0?1:-1];
typedef char C_gravity[OFF(struct C,g88)==0x88?1:-1];
typedef char C_size[sizeof(struct C)==0x8C?1:-1];
typedef char B_physics[OFF(struct B,pc)==0xE8?1:-1];
typedef char B_size[sizeof(struct B)==0xEC?1:-1];
typedef char S_body[OFF(struct S,pb)==0?1:-1];
typedef char S_size[sizeof(struct S)==4?1:-1];
typedef char VTE_size[sizeof(VTE)==8?1:-1];
typedef char VTE_delta[OFF(VTE,d)==0?1:-1];
typedef char VTE_function[OFF(VTE,pfn)==4?1:-1];
typedef char O_vtable[OFF(struct O,vt)==0xC?1:-1];
typedef char O_prefix[sizeof(struct O)==0x10?1:-1];
typedef char WORK_size[sizeof(struct WORK)==0x80?1:-1];
typedef char WORK_alignment[__alignof__(struct WORK)==16?1:-1];
typedef char WORK_u0[OFF(struct WORK,u0)==0x00?1:-1];
typedef char WORK_v10[OFF(struct WORK,v10)==0x10?1:-1];
typedef char WORK_v20[OFF(struct WORK,v20)==0x20?1:-1];
typedef char WORK_s[OFF(struct WORK,s)==0x30?1:-1];
typedef char WORK_at[OFF(struct WORK,at)==0x40?1:-1];
typedef char WORK_uq[OFF(struct WORK,uq)==0x50?1:-1];
typedef char WORK_incoming[OFF(struct WORK,incoming)==0x60?1:-1];
typedef char WORK_sum[OFF(struct WORK,sum)==0x70?1:-1];
