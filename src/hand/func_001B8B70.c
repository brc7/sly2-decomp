typedef int qword __attribute__((mode(TI)));
typedef union U { qword q; float f[4]; } U;
struct Q { char pad[0x14]; void *table; float angle,time; };
struct PARAM { float value; int kind,id,reserved; };
struct LIMIT { float lo,hi; };
struct POLY { float a,b,c; };
struct P {
    char pad[0x150]; struct Q *orbit;
    char pad154[0x190-0x154]; qword velocity;
    char pad1A0[0x1030-0x1A0]; struct Q *source;
    char pad1034[0x10DC-0x1034]; void *focus; int reserved10E0,mode;
    char pad10E8[0x1700-0x10E8]; qword direction;
    char pad1710[0x1818-0x1710]; int blend;
};
extern struct POLY D_002C0A20;
extern struct LIMIT D_002BBB60;
qword func_00204E38(struct P*,struct Q*,float);
struct PARAM *func_0012F938(void*,int);
qword SetVectorCylind(float,float,float);
qword ConvertAloVec(void*,void*,qword);
void func_002046F0(struct P*,void*);
static inline float Clamp(float f,float lo,float hi){if(f<lo)return lo;if(hi<f)return hi;return f;}
static inline float Polynomial(struct POLY*p,float t){return p->a+t*(p->b+t*p->c);}
static inline void Blend(qword value,float g,qword*velocity,U*scalar){
    register qword input __asm__("vf1"),strength __asm__("vf5"),negative __asm__("vf2"),other __asm__("vf4"),complement __asm__("vf3"),current __asm__("vf1");
    register int bits __asm__("$3"),rest __asm__("$2");
    register float neg __asm__("$f0")=-1.0f;register float one __asm__("$f1")=1.0f;float remain;
    __asm__("mfc1 %0,%2\n\tqmtc2.ni %3,%1":"=r"(bits),"=j"(input):"f"(g),"r"(value));
    __asm__("qmtc2.ni %1,%0":"=j"(strength):"r"(bits));
    __asm__("mfc1 %0,%1":"=r"(bits):"f"(neg));
    __asm__("qmtc2.ni %1,%0":"=j"(negative):"r"(bits));
    remain=one-g;
    scalar->q=negative;
    __asm__("vmulx.xyzw %0,%1,%2x":"=j"(other):"j"(input),"j"(negative));
    __asm__("mfc1 %0,%1":"=r"(rest):"f"(remain));
    __asm__("lqc2 %0,%2\n\tqmtc2.ni %3,%1":"=j"(current),"=j"(complement):"m"(*velocity),"r"(rest));
    *scalar=(U){complement};
    __asm__("vmulax.xyzw $ACC,%0,%2x\n\tvmaddx.xyzw %0,%1,%3x":"+j"(current):"j"(other),"j"(complement),"j"(strength));
    *velocity=current;
    scalar->q=strength;
}
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
static inline qword OrbitVector(struct P*p){
    register struct Q*orbit __asm__("$2");
    register qword d __asm__("vf1");
    register qword bits __asm__("$2");
    register float magnitude __asm__("$f0"),angle __asm__("$f12"),radius __asm__("$f13"),zero __asm__("$f14");
    __asm__("lw %0,%2\n\tlqc2 %1,%3":"=r"(orbit),"=j"(d):"m"(p->orbit),"m"(p->direction));
    __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y":"+j"(d));
    magnitude=orbit->time;
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
    angle=orbit->angle;
    __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
    zero=0.0f;
    __asm__("mtc1 %1,%0":"=f"(radius):"r"(bits));
    return SetVectorCylind(angle,magnitude*radius,zero);
}
void func_001B8B70(struct P*p){
    U scalar;
    if(p->source){
        qword value=func_00204E38(p,p->source,p->source->time);
        NegateSource(value,&p->velocity,&scalar);
        {
            register int active __asm__("$3")=p->blend;
            if(active){
            struct PARAM *start=func_0012F938(p->source->table,0x240);
            struct PARAM *end=func_0012F938(p->source->table,0x242);
            float t=(p->source->time-start->value)/(end->value-start->value);
            float blend=Clamp(Polynomial(&D_002C0A20,t),D_002BBB60.lo,D_002BBB60.hi);
            qword desired=OrbitVector(p);
            desired=ConvertAloVec(0,p,desired);
            Blend(desired,blend,&p->velocity,&scalar);
            }
        }
    }
    if(p->focus&&p->mode==4)func_002046F0(p,p->focus);
}
