// Compute motion correction in a predicted target frame.

typedef int qword __attribute__((mode(TI)));
struct VTE { short delta,pad; void *fn; };
struct M4 { qword row[4]; };
union V { qword q; float a[4]; };
struct MOTION { char before[0x80]; qword acceleration; };
struct OBJECT {
    char beforeVt[0xC]; VTE *vt; char beforeMotion[0xE8-0x10];
    MOTION *motion; char beforeRadius[0x1020-0xEC]; float radius;
};
struct CONTEXT { OBJECT *owner; };
struct SPACE;
extern "C" {
    qword func_001A62E8(OBJECT *);
    void LoadIdentityMatrix4(SPACE *,SPACE *,M4 *,M4 *,float);
    M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
    extern qword D_002A2DD0;
    extern float D_002BF910;
}
typedef qword (*VECTOR_GETTER)(OBJECT *);
typedef SPACE *(*INVERSE_GETTER)(OBJECT *,M4 *);
static inline float Abs(float a) {
    float r;__asm__("abs.s %0,%1":"=f"(r):"f"(a));return r;
}
static inline float Min(float a,float b) {
    float r;__asm__("min.s %0,%1,%2":"=f"(r):"f"(a),"f"(b));return r;
}
typedef char Qword_Size[(sizeof(qword))==16?1:-1];
typedef char Pointer_Size[(sizeof(void*))==4?1:-1];
typedef char Unsigned_Size[(sizeof(unsigned))==4?1:-1];
typedef char Int_Size[(sizeof(int))==4?1:-1];
typedef char Float_Size[(sizeof(float))==4?1:-1];
typedef char Short_Size[(sizeof(short))==2?1:-1];
typedef char V_Size[(sizeof(V))==16?1:-1];
typedef char V_Align[(__alignof__(V))==16?1:-1];
typedef char M4_Size[(sizeof(M4))==64?1:-1];
typedef char M4_Align[(__alignof__(M4))==16?1:-1];
typedef char VTE_Size[(sizeof(VTE))==8?1:-1];
typedef char VTE_Align[(__alignof__(VTE))==4?1:-1];
typedef char MOTION_Size[(sizeof(MOTION))==144?1:-1];
typedef char MOTION_Align[(__alignof__(MOTION))==16?1:-1];
typedef char OBJECT_Size[(sizeof(OBJECT))==4132?1:-1];
typedef char OBJECT_Align[(__alignof__(OBJECT))==4?1:-1];
typedef char CONTEXT_Size[(sizeof(CONTEXT))==4?1:-1];
typedef char CONTEXT_Align[(__alignof__(CONTEXT))==4?1:-1];
typedef char V_q_Offset[((unsigned)&((V*)0)->q)==0?1:-1];
typedef char V_a_Offset[((unsigned)&((V*)0)->a)==0?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((VTE*)0)->delta)==0?1:-1];
typedef char VTE_pad_Offset[((unsigned)&((VTE*)0)->pad)==2?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((VTE*)0)->fn)==4?1:-1];
typedef char MOTION_acceleration_Offset[((unsigned)&((MOTION*)0)->acceleration)==128?1:-1];
typedef char OBJECT_vt_Offset[((unsigned)&((OBJECT*)0)->vt)==12?1:-1];
typedef char OBJECT_motion_Offset[((unsigned)&((OBJECT*)0)->motion)==232?1:-1];
typedef char OBJECT_radius_Offset[((unsigned)&((OBJECT*)0)->radius)==4128?1:-1];
typedef char CONTEXT_owner_Offset[((unsigned)&((CONTEXT*)0)->owner)==0?1:-1];
typedef char M4_row0_Offset[((unsigned)&((M4*)0)->row[0])==0?1:-1];
typedef char M4_row1_Offset[((unsigned)&((M4*)0)->row[1])==16?1:-1];
typedef char M4_row2_Offset[((unsigned)&((M4*)0)->row[2])==32?1:-1];
typedef char M4_row3_Offset[((unsigned)&((M4*)0)->row[3])==48?1:-1];
extern "C" void func_0019C300(CONTEXT *context,OBJECT *target,qword localPoint,
                             float *score,qword *point,qword *velocity,float duration) {
    V delta;
    M4 predicted,inverse;
    V scalar;
    M4 product;
    V future,position,oldVelocity,comparison,prediction;
    position.q=func_001A62E8(context->owner);
    OBJECT *owner=context->owner;
    VTE *vt=owner->vt;
    oldVelocity.q=((VECTOR_GETTER)vt[33].fn)((OBJECT*)((char*)owner+vt[33].delta));
    register M4*inverseOwner __asm__("$17");
    SPACE*frame;
    {
      register M4*inverseArgument __asm__("$5");
      register VTE*targetVt __asm__("$6");
      register MOTION*motion __asm__("$3");
      register qword time __asm__("vf3");
      __asm__("la %0,%a6\n\tlw $2,%7\n\tdaddu %1,%0,$zero\n\tmfc1 $3,%8\n\tqmtc2.ni $3,%2\n\tlw %3,%9\n\tlw %4,0xe8($2)\n\tsqc2 %2,%5"
        :"=&r"(inverseOwner),"=&r"(inverseArgument),"=j"(time),"=&r"(targetVt),"=&r"(motion),"=m"(scalar.q)
        :"p"(&inverse),"m"(context->owner),"f"(duration),"m"(target->vt):"$2","memory");
      register qword v __asm__("vf1"),acceleration __asm__("vf2");
      register float half __asm__("$f0")=0.5f;
      register int delta __asm__("$4");
      __asm__("lqc2 %0,%4\n\tlqc2 %1,%5\n\tmul.s %2,%6,%2\n\tlh %3,%7"
        :"=j"(v),"=j"(acceleration),"+f"(half),"=r"(delta)
        :"m"(oldVelocity.q),"m"(motion->acceleration),"f"(duration),"m"(targetVt[32].delta));
      __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%3x\n\tsqc2 %0,%1"
        :"+j"(v),"=m"(future.q):"j"(acceleration),"j"(time));
      register OBJECT*receiver __asm__("$4");
      register qword halfVector __asm__("vf1");
      __asm__("mfc1 $2,%4\n\taddu %0,%3,%2\n\tqmtc2.ni $2,%1"
        :"=r"(receiver),"=j"(halfVector):"0"(delta),"r"(target),"f"(half):"$2");
      __asm__ volatile("lqc2 $vf3,%1\n\tlqc2 $vf4,%2\n\tlqc2 $vf2,%3\n\tvadda.xyzw $ACC,$vf2,$vf0\n\tvmaddax.xyzw $ACC,$vf3,%4x\n\tvmaddx.xyzw $vf2,$vf4,%4x\n\tsqc2 $vf2,%0"
        :"=m"(prediction.q):"m"(oldVelocity.q),"m"(future.q),"m"(position.q),"j"(halfVector):"vf2","vf3","vf4");
      INVERSE_GETTER inverseFunction=(INVERSE_GETTER)targetVt[32].fn;
      scalar.q=halfVector;
      frame=inverseFunction(receiver,inverseArgument);
    }
    {
      register qword snapshot __asm__("$3")=prediction.q;
      register M4*published __asm__("$16");
      register SPACE*receiver __asm__("$4");
      register float dtArgument __asm__("$f12");
      __asm__ volatile("la %0,%a4\n\tdaddu %1,%5,$zero\n\tmov.s %2,%6\n\tsq %7,%3":"=&r"(published),"=&r"(receiver),"=f"(dtArgument),"=m"(comparison.q):"p"(&predicted),"r"(frame),"f"(duration),"r"(snapshot));
      register SPACE*sourceSpace __asm__("$5");register M4*optionalOutput __asm__("$7");const int nullValue=0;
      __asm__("daddu %0,%z2,$zero\n\tdaddu %1,%z2,$zero":"=r"(optionalOutput),"=r"(sourceSpace):"rJ"(nullValue));
      LoadIdentityMatrix4(receiver,sourceSpace,published,optionalOutput,dtArgument);
      __ml__FRC7MATRIX4T0(&product,published,inverseOwner);
    }
    register float squared __asm__("$f2"),radius __asm__("$f1"),radiusSquared __asm__("$f0");
    register OBJECT*radiusOwner __asm__("$2");
    {
 register qword a __asm__("vf1"),b __asm__("vf2"),c __asm__("vf3"),d __asm__("vf4"),p __asm__("vf6");
 __asm__("lqc2 %0,%5\n\tlqc2 %1,%6\n\tlqc2 %2,%7\n\tlqc2 %3,%8\n\tlqc2 %4,%9":"=j"(a),"=j"(b),"=j"(c),"=j"(d),"=j"(p):"m"(product.row[0]),"m"(product.row[1]),"m"(product.row[2]),"m"(product.row[3]),"m"(localPoint));
 register qword q __asm__("vf5");
 /* Capture the inherited VF5.W after the actual matrix-product call. */
 __asm__ volatile("vmulax.xyz $ACC,%2,%6x\n\tvmadday.xyz $ACC,%3,%6y\n\tvmaddaz.xyz $ACC,%4,%6z\n\tvmaddw.xyz %0,%5,$vf0w\n\tsqc2 %2,%1":"=&j"(q),"=m"(predicted.row[0]):"j"(a),"j"(b),"j"(c),"j"(d),"j"(p));
 {
 register qword other __asm__("vf1");
 __asm__("lqc2 %3,%6\n\tsqc2 %4,%0\n\tvsub.xyzw %2,%2,%3\n\tsqc2 %5,%1":"=m"(predicted.row[1]),"=m"(predicted.row[2]),"+&j"(q),"=&j"(other):"j"(b),"j"(c),"m"(comparison.q));
 }
 __asm__("vmul.xyz $vf1,%4,%4\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tsqc2 %5,%0\n\tsqc2 %4,%1\n\tqmfc2.ni $2,$vf1\n\tmtc1 $2,%2\n\tlw %3,%6":"=m"(predicted.row[3]),"=m"(delta.q),"=f"(squared),"=&r"(radiusOwner):"j"(q),"j"(d),"m"(context->owner):"vf1","vf2");

    }
    __asm__("lwc1 %0,%2\n\tmul.s %1,%0,%0":"=f"(radius),"=f"(radiusSquared):"m"(radiusOwner->radius));
    if(squared<radiusSquared) {
        if(score){register float distance __asm__("$f0");__asm__("sqrt.s %0,%1\n\tsub.s %0,%0,%2":"=&f"(distance):"f"(squared),"f"(radius));*score=distance;}
        if(point){register qword value __asm__("$3")=prediction.q;*point=value;}
    } else {
        float vertical=Abs(delta.a[2]);
        if(radius<vertical || duration==0.0f) {
            if(score)*score=vertical+1010000.0f;
            if(point){register qword value __asm__("$3")=position.q;*point=value;}
            goto done;
        }
        delta.a[2]=0.0f;
        register qword length __asm__("vf2");
        {
          register unsigned zeroAddress __asm__("$2");
          __asm__("lui %0,%%hi(%1)":"=r"(zeroAddress):"i"(&D_002A2DD0));
          register qword direction __asm__("vf3");
          __asm__("addiu %0,%0,%%lo(%4)\n\tlqc2 %1,%3\n\tvmul.xyz $vf1,%1,%1\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,%2,$vf1z\n\tvsqrt $Q,$vf1x\n\tvwaitq\n\tvaddq.x %2,$vf0,$Q":"+&r"(zeroAddress),"=j"(direction),"=&j"(length):"m"(delta.q),"i"(&D_002A2DD0):"vf1");
          register float magnitude __asm__("$f0");
          __asm__("qmfc2.ni $3,%1\n\tmtc1 $3,%0":"=f"(magnitude):"j"(length):"$3");
          if(magnitude<0.0001f){register qword value __asm__("$2");__asm__("lq %0,%1":"=r"(value):"m"(*(const qword*)zeroAddress));delta.q=value;}
          else{register qword normalized __asm__("vf1");__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=j"(normalized):"j"(direction),"j"(length));delta.q=normalized;}
        }
        register OBJECT*forceOwner __asm__("$2")=context->owner;
        register float deficit __asm__("$f2")=vertical;
        __asm__("mul.s %0,%0,%0\n\tqmfc2.ni $3,%2\n\tlwc1 $f0,%1\n\tmtc1 $3,$f3\n\tmul.s $f0,$f0,$f0\n\tsub.s $f0,$f0,%0\n\tsqrt.s $f1,$f0\n\tsub.s %0,$f3,$f1":"+f"(deficit):"m"(forceOwner->radius),"j"(length):"$3","$f0","$f1","$f3");
        register qword corrected __asm__("vf2");
        {
          register unsigned scratch __asm__("$2")=(unsigned)forceOwner;
          __asm__("lqc2 %0,%5\n\tmul.s $f1,%3,%3\n\tlqc2 $vf4,%6\n\tmfc1 $3,%3\n\tadd.s $f0,%4,%4\n\tqmtc2.ni $3,$vf3\n\tlw $3,0xe8(%2)\n\tdiv.s $f0,$f0,$f1\n\tmfc1 %2,$f0\n\tqmtc2.ni %2,$vf1\n\tsqc2 $vf1,%1\n\tsqc2 $vf3,%1\n\tvmulx.xyzw $vf1,%0,$vf1x\n\tlqc2 %0,0x80($3)\n\tvadda.xyzw $ACC,$vf4,$vf0\n\tvmaddax.xyzw $ACC,%0,$vf3x\n\tvmaddx.xyzw %0,$vf1,$vf3x"
            :"=&j"(corrected),"=m"(scalar.q),"+r"(scratch):"f"(duration),"f"(deficit),"m"(delta.q),"m"(oldVelocity.q):"$3","$f0","$f1","vf1","vf3","vf4","memory");
        }
        future.q=corrected;
        if(score)*score=Min((D_002BF910+D_002BF910)*deficit/duration,1000000.0f);
        if(point){
          register qword origin __asm__("vf3")=position.q,initial __asm__("vf4")=oldVelocity.q,k __asm__("vf1");
          float half=duration*0.5f;
          register int bits __asm__("$2");
          __asm__("mfc1 %0,%1":"=r"(bits):"f"(half));
          __asm__("qmtc2.ni %1,%0":"=j"(k):"r"(bits));
          __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddax.xyzw $ACC,%2,%3x\n\tvmaddx.xyzw %0,%0,%3x":"+j"(corrected):"j"(origin),"j"(initial),"j"(k));
          scalar.q=k;*point=corrected;
        }
    }
done:
    if(velocity)*velocity=future.q;
}
