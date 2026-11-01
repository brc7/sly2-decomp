typedef int qword __attribute__((mode(TI)));
struct M { qword a,b,c,d; };
struct VTE { short d; short pad; void (*pf)(); };
struct S { char pad[0xC]; struct VTE *vt; char pad2[0xE0-0x10]; int e0; char pad3[0xF4-0xE4]; qword *v; char pad4[0x140-0xF8]; qword *v140,*v144; };
union U { qword q; float f; };
int func_001E7538(struct S*,float,float*,float*);
void EvaluateBezierPos(float,float,float,qword,qword,qword,qword,qword*,qword*,qword*);
static inline qword ReadTangent(const qword*p){register qword a __asm__("vf2");__asm__("lqc2 %0,%1":"=j"(a):"m"(*p));return a;}
static inline qword ReadLinearSecond(const qword*p){register qword value __asm__("vf2");__asm__("lqc2 %0,%1":"=j"(value):"m"(*p));return value;}
static inline qword Sub(qword next,qword first){register qword a __asm__("vf2")=next,b __asm__("vf1")=first;__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
static inline qword Lerp(qword a,qword b,qword w0,qword w1) { __asm__("vmulax.xyzw $ACC,%1,%0x\n\tvmaddx.xyzw %0,%2,%3x":"+j"(w0):"j"(a),"j"(b),"j"(w1));return w0; }
static inline qword HermitePoint(struct M*m,qword value){register qword keep __asm__("vf2")=m->a,row1 __asm__("vf4")=m->b,row2 __asm__("vf3")=m->c,translation __asm__("vf1")=m->d,point __asm__("vf5")=value;__asm__("vmulax.xyz $ACC,%0,%4x\n\tvmadday.xyz $ACC,%1,%4y\n\tvmaddaz.xyz $ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(keep):"j"(row1),"j"(row2),"j"(translation),"j"(point));return keep;}
static inline qword PointKept(qword keep,struct M *m,qword a) {__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(keep):"j"(m->a),"j"(m->b),"j"(m->c),"j"(m->d),"j"(a));return keep;}
static inline qword HighWeight(float f){register qword value __asm__("vf4");__asm__("qmtc2.ni %1,%0":"=j"(value):"r"(f));return value;}
static inline qword LowWeight(float f,union U*home){register qword value __asm__("vf1");__asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1":"=j"(value),"=m"(home->q):"r"(f));return value;}
static inline qword FirstPointAndWeight(const qword*first,qword weight,union U*home){register qword point __asm__("vf3");__asm__("lqc2 %0,%2\n\tsqc2 %3,%1":"=&j"(point),"=m"(home->q):"m"(*first),"j"(weight));return point;}
static inline unsigned PointOffset(int i){register unsigned bytes __asm__("$3");__asm__("sll %0,%1,4":"=r"(bytes):"r"(i));return bytes;}
static inline qword TangentFirst(const qword*p){register qword point __asm__("vf1");__asm__("lqc2 %0,%1":"=j"(point):"m"(*p));return point;}
static inline int SegmentIndex(int value){register int index __asm__("$18");__asm__("daddu %0,%1,$zero":"=r"(index):"r"(value));return index;}
void func_001E67B8(struct S *p,float g,qword *pos,qword *dir)
{
    struct M m;
    float elapsed,duration;
    register qword row0 __asm__("vf5"),row1 __asm__("vf4"),row2 __asm__("vf3");
    register qword tangent __asm__("vf2");
    int i=SegmentIndex(func_001E7538(p,g,&elapsed,&duration));
    m=* ((struct M *(*)(void*))p->vt[31].pf)((char*)p+p->vt[31].d);
    if (p->e0==0) {
        if (pos) {
            union U scalar;float t=elapsed/duration;
            qword *linearPoint=(qword*)(PointOffset(i)+(unsigned)p->v);
            qword w1=HighWeight(t),w0=LowWeight(1.0f-t,&scalar);
            {qword first=FirstPointAndWeight(linearPoint,w1,&scalar),second=ReadLinearSecond(linearPoint+1);qword blend=Lerp(first,second,w0,w1);*pos=PointKept(blend,&m,blend);}
        }
        if (!dir) return;
        {register unsigned offset __asm__("$2")=(unsigned)i<<4;
        register qword *base __asm__("$3")=p->v;
        register qword *first __asm__("$4"),*second __asm__("$2");
        __asm__("lqc2 %0,%2\n\taddu %1,%3,%4":"=j"(row0),"=r"(first):"m"(m.a),"r"(offset),"r"(base));
        __asm__("lqc2 %0,%2\n\tdaddu %1,%3,$zero":"=j"(row1),"=r"(second):"m"(m.b),"r"(first));
        tangent=Sub(ReadTangent(second+1),TangentFirst(first));}
    } else {
        EvaluateBezierPos(duration,elapsed,1.0f,p->v[i],p->v144[i],p->v[i+1],p->v140[i+1],pos,dir,0);
        if (pos) *pos=HermitePoint(&m,*pos);
        if (!dir) return;
        row0=m.a;
        tangent=ReadTangent(dir);row1=m.b;
    }
    row2=m.c;
    {register qword d __asm__("vf1"),one __asm__("vf6");
    __asm__("vmulax.xyz $ACC,%4,%2x\n\tvmadday.xyz $ACC,%5,%2y\n\tvmaddz.xyz %2,%6,%2z\n\tvmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tvdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q\n\tsqc2 %0,%3":"=&j"(d),"=&j"(one),"+j"(tangent),"=m"(*dir):"j"(row0),"j"(row1),"j"(row2));}
}

#define FIELD_OFFSET(t,m) ((unsigned)&(((t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char M_Size[sizeof(struct M)==0x40?1:-1];
typedef char M_Alignment[__alignof__(struct M)==16?1:-1];
typedef char VTE_Size[sizeof(struct VTE)==8?1:-1];
typedef char S_Prefix_Size[sizeof(struct S)==0x148?1:-1];
typedef char U_Size[sizeof(union U)==16?1:-1];
typedef char U_Alignment[__alignof__(union U)==16?1:-1];
typedef char M_a_Offset[FIELD_OFFSET(struct M,a)==0?1:-1];
typedef char M_b_Offset[FIELD_OFFSET(struct M,b)==0x10?1:-1];
typedef char M_c_Offset[FIELD_OFFSET(struct M,c)==0x20?1:-1];
typedef char M_d_Offset[FIELD_OFFSET(struct M,d)==0x30?1:-1];
typedef char VTE_d_Offset[FIELD_OFFSET(struct VTE,d)==0?1:-1];
typedef char VTE_pf_Offset[FIELD_OFFSET(struct VTE,pf)==4?1:-1];
typedef char S_vt_Offset[FIELD_OFFSET(struct S,vt)==0xC?1:-1];
typedef char S_e0_Offset[FIELD_OFFSET(struct S,e0)==0xE0?1:-1];
typedef char S_v_Offset[FIELD_OFFSET(struct S,v)==0xF4?1:-1];
typedef char S_v140_Offset[FIELD_OFFSET(struct S,v140)==0x140?1:-1];
typedef char S_v144_Offset[FIELD_OFFSET(struct S,v144)==0x144?1:-1];
typedef char U_q_Offset[FIELD_OFFSET(union U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(union U,f)==0?1:-1];
