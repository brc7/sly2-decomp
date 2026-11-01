// Select and blend the movement vector for the active mode.
typedef int qword __attribute__((mode(TI)));
typedef union U{qword q;float f[4];int bits[4];}U;
typedef union F{float value;int bits;}F;
struct VT{short delta,pad;void*fn;};
struct O{char pad[0xC];struct VT*vt;};
struct M3{U row[3];};
struct P{struct O*self;char pad4[0x30-4];struct O*target;char pad34[8];int mode;char pad40[0x20];U velocity;};
extern F D_002BFACC,D_002BFAD0,D_002BFAD4,D_002BFB00,D_002BFB04;
extern U D_002BFAE0,D_002BFAF0,D_002A2DA0,D_002A2DC0;
int func_00132B10(struct O*,int);
qword LimitVectorLength(qword,float);
qword func_001A62E8(struct O*);
struct M3*func_001A6318(struct O*);
qword func_001A6FC0(struct O*,struct O*,qword);
void func_001A0B40(struct P*,int);
static inline float Max(float a,float b){return a>b?a:b;}
static inline qword Scalar(float f){qword q;int bits;__asm__("mfc1 %0,%1":"=r"(bits):"f"(f));__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}
static inline qword Word(float f){qword q;__asm__("mfc1 %0,%1":"=r"(q):"f"(f));return q;}
static inline qword Scale(qword v,qword k){__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;}
static inline float Z(qword q){register int bits __asm__("$3");float f;__asm__("prot3w %0,%1":"=r"(bits):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(bits));return f;}
static inline qword Normal(U*u,qword fallback){
    register qword v __asm__("vf3")=u->q;
    register qword d __asm__("vf1"),one __asm__("vf2"),r __asm__("vf4");
    register int bits __asm__("$3");float length;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v));
    __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
    __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
    __asm__("mtc1 %1,%0":"=f"(length):"r"(bits));
    if(length<0.0001f)return fallback;
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(d),"j"(v));
    return r;
}
static inline qword Blend(qword direction,qword z,qword a,qword b){__asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%0,%3x":"+j"(z):"j"(direction),"j"(a),"j"(b));return z;}
static inline qword Intercept(U*self,U*target,struct M3*basis,U*negative){
    register qword position __asm__("vf2")=target->q;
    register qword axis __asm__("vf1")=basis->row[0].q;
    register qword scale __asm__("vf3")=negative->q;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x":"+j"(axis):"j"(position),"j"(scale));
    position=self->q;
    __asm__("vsub.xyzw %0,%0,%1":"+j"(position):"j"(axis));
    return position;
}
static inline qword Difference(U*first,qword target){
    register qword a __asm__("vf2");
    register qword b __asm__("vf3");
    register qword r __asm__("vf1");
    __asm__("lqc2 %0,%2\n\tqmtc2.ni %3,%1":"=j"(a),"=j"(b):"m"(first->q),"r"(target));
    __asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;
}
void func_001A16C8(struct P*p){
    U output=p->velocity;
    U scratch,scalar;
    U selfPosition,targetPosition,negative,otherPosition;
    switch(p->mode){
    case 0x200:
    case 0x8000:
        if(func_00132B10(p->target,0x33)){
            qword k=Scalar(D_002BFACC.value+1.0f);
            qword v=Scale(p->velocity.q,k);
            scratch.q=k;
            output.q=v;
            output.q=LimitVectorLength(v,D_002BFAD4.value);
            output.f[2]=Max(output.f[2],D_002BFAD0.value);
        }
        break;
    case 0x40:
        if(func_00132B10(p->target,0x36)){
            struct O*target=p->target;
            struct M3*basis;
            qword delta,local;
            selfPosition.q=func_001A62E8(p->self);
            targetPosition.q=func_001A62E8(target);
            {qword k=Word(-150.0f);scratch.q=k;negative.q=k;}
            basis=func_001A6318(target);
            delta=Intercept(&selfPosition,&targetPosition,basis,&negative);
            local=func_001A6FC0(0,target,delta);
            if(Z(local)>0.0f)output.q=func_001A6FC0(target,0,D_002BFAE0.q);
            else output.q=func_001A6FC0(target,0,D_002BFAF0.q);
        }
        break;
    case 0x100:
        if(func_00132B10(p->target,0x4F)){
            struct O*target=p->target;
            qword n,a,b,mixed,z;
            if(func_00132B10(target,0x51)){
                struct VT*vt=target->vt;
                struct M3*basis=((struct M3*(*)(void*))vt[31].fn)((char*)target+vt[31].delta);
                scratch=basis->row[0];
            }else{
                qword targetPoint;
                otherPosition.q=func_001A62E8(p->self);
                targetPoint=func_001A62E8(target);
                scratch.q=Difference(&otherPosition,targetPoint);
            }
            scratch.bits[2]=0;
            n=Normal(&scratch,D_002A2DA0.q);
            {
                register int first __asm__("$4"),second __asm__("$3");
                register U*up __asm__("$2");
                register qword weight __asm__("vf2"),lift __asm__("vf3"),axis __asm__("vf1");
                // Read blend data and initialize the scalar temporary.
                __asm__("lui %0,%%hi(%7)\n\tlui %1,%%hi(%8)\n\tlui %2,%%hi(%9)\n\tlw %0,%%lo(%7)(%0)\n\taddiu %2,%2,%%lo(%9)\n\tlw %1,%%lo(%8)(%1)\n\tlqc2 %5,0(%2)\n\tqmtc2.ni %0,%3\n\tqmtc2.ni %1,%4\n\tsqc2 %3,%6"
                    :"=&r"(first),"=&r"(second),"=&r"(up),"=j"(weight),"=j"(lift),"=j"(axis),"=m"(scalar.q)
                    :"i"(&D_002BFB00.bits),"i"(&D_002BFB04.bits),"i"(&D_002A2DC0):"memory");
                a=weight;b=lift;z=axis;
            }

            mixed=Blend(n,z,a,b);
            scratch.q=n;
            output.q=mixed;
            scalar=(U){b};
        }
        break;
    }
    {
        struct O*object=p->self;
        struct VT*vt=object->vt;
        ((void(*)(void*,qword))vt[35].fn)((char*)object+vt[35].delta,output.q);
    }
    func_001A0B40(p,7);
}
