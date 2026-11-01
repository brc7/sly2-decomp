typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
struct LO{char pad[0x30];qword pos;char pad40[0x60];struct LO*parent;char padA4[0xC];qword velocity;};
struct ACC{char pad[0x80];qword v;};
struct O{char pad[0x58];struct LO*lo;char pad5C[0x8C];struct ACC*acc;};
struct X{char pad[0x180];struct O*object;};
struct ANIM{char pad[0x40];float rate;};
struct P{char pad[0x1030];struct ANIM*anim;char pad1034[0x3BC];struct X*x;char pad13F4[0x58];int state;};
extern qword D_002A2DC0;
extern float D_002C09C8;
qword func_0022FCC8(struct LO*,struct LO*,qword);
int CSolveQuadratic__FfffPf(float,float,float,float*);
void func_00130D30(struct ANIM*,int,float,float);
static inline float Clamp(float inputValue,float inputLower,const float*upper){register float value __asm__("$f0")=inputValue;register float lower __asm__("$f12")=inputLower;float limit=*upper;if(value<lower)return lower;if(limit<value)return limit;return value;}
static inline qword Scale(qword v,float f,U*u){qword s,r;__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(f));u->q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(u->q));return r;}
static inline qword FinalLaunchScale(const qword*up,int bits,U*scalar){register qword factor __asm__("vf2"),source __asm__("vf1"),result __asm__("vf3");__asm__("qmtc2.ni %4,%0\n\tlqc2 %1,%5\n\tvmulx.xyzw %2,%1,%0x\n\tsqc2 %0,%3":"=j"(factor),"=j"(source),"=j"(result),"=m"(scalar->q):"r"(bits),"m"(*up));return result;}
static inline qword AddScaled(qword a,qword b,qword s,U*u){qword r;u->q=s;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(u->q));return r;}
static inline qword AddScaledMemory(const U*p,qword b,qword s){qword a,r;__asm__("lqc2 %1,%2\n\tvadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r),"=&j"(a):"m"(p->q),"j"(b),"j"(s));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Div(qword v,qword s,U*u){qword r;u->q=s;__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(u->q),"j"(v));return r;}
static inline void Store(qword*p,qword q){__asm__("sqc2 %1,%0":"=m"(*p):"j"(q));}
static inline struct P*CaptureActor(struct P*input){register struct P*output __asm__("$16");__asm__("daddu %0,%1,$zero":"=r"(output):"r"(input));return output;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char U_q_Offset[FIELD_OFFSET(U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(U,f)==0?1:-1];
typedef char LO_pos_Offset[FIELD_OFFSET(struct LO,pos)==48?1:-1];
typedef char LO_parent_Offset[FIELD_OFFSET(struct LO,parent)==160?1:-1];
typedef char LO_velocity_Offset[FIELD_OFFSET(struct LO,velocity)==176?1:-1];
typedef char ACC_v_Offset[FIELD_OFFSET(struct ACC,v)==128?1:-1];
typedef char O_lo_Offset[FIELD_OFFSET(struct O,lo)==88?1:-1];
typedef char O_acc_Offset[FIELD_OFFSET(struct O,acc)==232?1:-1];
typedef char X_object_Offset[FIELD_OFFSET(struct X,object)==384?1:-1];
typedef char ANIM_rate_Offset[FIELD_OFFSET(struct ANIM,rate)==64?1:-1];
typedef char P_anim_Offset[FIELD_OFFSET(struct P,anim)==4144?1:-1];
typedef char P_x_Offset[FIELD_OFFSET(struct P,x)==5104?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(struct P,state)==5196?1:-1];
typedef char qword_PrefixSize[sizeof(qword)==16?1:-1];
typedef char U_PrefixSize[sizeof(U)==16?1:-1];
typedef char LO_PrefixSize[sizeof(struct LO)==192?1:-1];
typedef char ACC_PrefixSize[sizeof(struct ACC)==144?1:-1];
typedef char O_PrefixSize[sizeof(struct O)==236?1:-1];
typedef char X_PrefixSize[sizeof(struct X)==388?1:-1];
typedef char ANIM_PrefixSize[sizeof(struct ANIM)==68?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==5200?1:-1];
void func_001B7700(struct P*p,qword goal,float*animation,float*duration,qword*outStart,qword*outEnd){
 struct P*owner=CaptureActor(p);
 U roots;U u;U start;
 qword end;
 switch(owner->state){
 case 0:{
  struct LO*parent=owner->x->object->lo->parent;
  qword first=func_0022FCC8(0,parent,Scale(D_002A2DC0,2500.0f,&roots));
  {
   register struct LO*space __asm__("$4");register int bits __asm__("$2");
   __asm__("sq %3,%0\n\tdaddu %1,%z5,$zero\n\tmfc1 %2,%4":"=m"(start.q),"=r"(space),"=r"(bits):"r"(first),"f"(-2000.0f),"rJ"((struct LO*)0));
   end=func_0022FCC8(space,parent,FinalLaunchScale(&D_002A2DC0,bits,&roots));
  }
  break;
 }
 case 1:{
  U v;
  register qword pos __asm__("$18");
  qword acceleration;
  float t;
  int count;
  {
   struct X*x=owner->x;
   register struct O*object __asm__("$3");
   __asm__("lw %0,%1":"=r"(object):"m"(x->object));
   {
    register struct LO*lo __asm__("$2");register struct ACC*source __asm__("$5");
    register qword velocity __asm__("$3"),argument __asm__("$6");
    __asm__("lw %0,%2\n\tlw %1,%3":"=&r"(lo),"=r"(source):"m"(object->lo),"m"(object->acc));
    __asm__("lq %0,%3\n\tlq %1,%4\n\tlq %2,%5":"=&r"(velocity),"=&r"(argument),"=r"(pos):"m"(lo->velocity),"m"(source->v),"m"(lo->pos));
    v.q=velocity;
    acceleration=func_0022FCC8(0,lo->parent,argument);
   }
  }
  {
   register float accelerationZ __asm__("$f12"),positionZ __asm__("$f0"),goalZ __asm__("$f14");
   register int positionWord __asm__("$3"),accelerationWord __asm__("$2");
   register float half __asm__("$f1");register float*outputRoots __asm__("$4");
   register int goalWord __asm__("$2");register qword velocityWord __asm__("$3");float velocityZ;
   __asm__("pextuw %0,$zero,%5\n\tpextuw %1,$zero,%6\n\tli.s %2,%7\n\tmtc1 %1,%3\n\tdaddu %4,%8,$zero":"=&r"(positionWord),"=&r"(accelerationWord),"=f"(half),"=f"(accelerationZ),"=r"(outputRoots):"r"(pos),"r"(acceleration),"F"(0.5f),"r"(roots.f));
   __asm__("pextuw %0,$zero,%2\n\tmtc1 %3,%1":"=&r"(goalWord),"=f"(positionZ):"r"(goal),"r"(positionWord));
   __asm__("mtc1 %2,%0\n\tlq %1,%3":"=f"(goalZ),"=r"(velocityWord):"r"(goalWord),"m"(v.q));
   __asm__("mul.s %0,%0,%2\n\tpextuw %1,$zero,%3":"+f"(accelerationZ),"=r"(velocityZ):"f"(half),"1"(velocityWord));
   count=CSolveQuadratic__FfffPf(accelerationZ,velocityZ,positionZ-goalZ,outputRoots);
  }
  if(!count)t=D_002C09C8;
  else t=Clamp(roots.f[count-1],D_002C09C8,duration);
  func_00130D30(owner->anim,0,*animation-t*owner->anim->rate,owner->anim->rate);
  {
   register qword actualAcceleration __asm__("vf3");register qword qt __asm__("vf1");
   register float averageTime __asm__("$f0")=0.5f;register int bits __asm__("$2");
   qword predictedEnd;
   __asm__("qmtc2.ni %4,%0\n\tmfc1 %3,%5\n\tmul.s %2,%5,%2\n\tqmtc2.ni %3,%1":"=j"(actualAcceleration),"=j"(qt),"+f"(averageTime),"=&r"(bits):"r"(acceleration),"f"(t));
   predictedEnd=AddScaledMemory(&v,actualAcceleration,qt);
   {
    register qword mean __asm__("vf2"),actualPosition __asm__("vf5"),averageScalar __asm__("vf3");
    qword predicted;
    __asm__("lqc2 %1,%4\n\tsqc2 %5,%3\n\tvadd.xyzw %0,%1,%6\n\tmfc1 %2,%7\n\tqmtc2.ni %8,%1":"=j"(mean),"=&j"(actualPosition),"=&r"(bits),"=m"(u.q):"m"(v.q),"j"(qt),"j"(predictedEnd),"f"(averageTime),"r"(pos));
    __asm__("swc1 %2,%0\n\tqmtc2.ni %3,%1":"=m"(*duration),"=j"(averageScalar):"f"(t),"r"(bits));
    predicted=AddScaled(actualPosition,mean,averageScalar,&u);
  {
   qword correction=Div(Sub(goal,predicted),qt,&u);
   register qword initial __asm__("vf5");register qword final __asm__("vf2");
   __asm__("lqc2 %0,%2\n\tvadd.xyzw %0,%0,%3\n\tvadd.xyzw %1,%4,%3":"=&j"(initial),"=j"(final):"m"(v.q),"j"(correction),"j"(predictedEnd));
   start.q=initial;end=final;
  }}}
  break;
 }
 default:break;
 }
 *outStart=start.q;
 Store(outEnd,end);
}

typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char LO_Alignment[__alignof__(struct LO)==16?1:-1];
typedef char ACC_Alignment[__alignof__(struct ACC)==16?1:-1];
typedef char O_Alignment[__alignof__(struct O)==4?1:-1];
typedef char X_Alignment[__alignof__(struct X)==4?1:-1];
typedef char ANIM_Alignment[__alignof__(struct ANIM)==4?1:-1];
typedef char P_Alignment[__alignof__(struct P)==4?1:-1];
