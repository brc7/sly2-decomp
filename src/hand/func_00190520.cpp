// Update target-facing movement modes and animation weights.
typedef int qword __attribute__((mode(TI)));typedef unsigned int uint;
union U{qword q;float f[4];int i[4];};
struct M3{U row[3];M3(){}M3(const M3&m){row[0]=m.row[0];row[1]=m.row[1];row[2]=m.row[2];}M3&operator=(const M3&m){row[0]=m.row[0];row[1]=m.row[1];row[2]=m.row[2];return *this;}};
struct VE{short delta,pad;void*fn;};struct P;
struct PL{char pad0[0x18];float angle,amount;};
struct CURVE{char pad0[0x1C];float value;char pad20[0x20];float slope;};
struct ANIMATION{char pad0[0x74];float gain;};
struct LINK{char pad0[4];P*object;};
struct P{char pad0[12];VE*vt;char pad10[0x150-0x10];PL*input;char pad154[0x1A8-0x154];float angle;char pad1AC[0xF00-0x1AC];char action[0x50];char padF50[0x1030-0xF50];CURVE*curve;char pad1034[0x18C4-0x1034];P*target;char pad18C8[8];int state;float changed;char pad18D8[0x1C];float farTime;int previous,flipped;LINK*link;char pad1904[0x1970-0x1904];float combined;char pad1974[8];float activeOnly;char pad1980[8];float idle;char pad198C[8];ANIMATION*animation;float speedBlend,activeBlend;};
struct CLOCK{float time,dt;};struct LM{float values[3];};
struct WORK{U reference,direction,local;M3 matrix,returned;float heading;U target;};
extern "C"{
 void func_00190BE0(P*,float*);float RadNormalize__Ff(float);void func_00204660(P*,float);qword func_001A62E8(P*);M3* func_0011B500(M3*,qword,float);float func_002458F0(float,float);
 void func_0018C250(P*,int,int);void func_0018FC60(P*);void func_001F5078(P*);int func_001C4660(PL*,uint,float);void func_001C4738(PL*,int);void func_0019CC50(void*,int,P*,void*);
 float GSmooth__FfffP3SMPPf(float,float,float,LM*,float*);void func_001384C0(ANIMATION*);float func_0011CFE0(float,float);
 extern CLOCK D_002DDED0;extern qword D_002A2DC0;
 extern float D_002BF554,D_002BF558,D_002BF55C,D_002BF560,D_002BF564,D_002BF568,D_002BF56C,D_002BF58C,D_002BF590;
 extern LM D_002BF570,D_002BF580;
}
extern "C" float fabsf(float);
static inline float Abs(float a){return fabsf(a);}
static inline qword Local(const M3&m,qword v){qword q,one,a,b,c;__asm__("vmul.xyzw %2,%5,%8\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%6,%8\n\tvmul.xyzw %4,%7,%8\n\tvadday.x $ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z $ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y":"=j"(q),"=&j"(one),"=&j"(a),"=&j"(b),"=&j"(c):"j"(m.row[0].q),"j"(m.row[1].q),"j"(m.row[2].q),"j"(v));return q;}
static inline qword LengthSquared(qword value){qword n,one;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(n),"=&j"(one):"j"(value));return n;}
static inline float X(qword value){float f;__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(value));return f;}
static inline qword Velocity(P*p){return ((qword(*)(void*))p->vt[33].fn)((char*)p+p->vt[33].delta);}
static inline int Can(P*p,int mode){return ((int(*)(void*,int))p->vt[99].fn)((char*)p+p->vt[99].delta,mode);}
static inline float Clamp(float f,float low,float high){if(f<low)return low;else if(f>high)return high;return f;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char PL_Size[sizeof(PL)==32?1:-1];
typedef char CURVE_Size[sizeof(CURVE)==68?1:-1];
typedef char ANIMATION_Size[sizeof(ANIMATION)==120?1:-1];
typedef char LINK_Size[sizeof(LINK)==8?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char LM_Size[sizeof(LM)==12?1:-1];
typedef char WORK_Size[sizeof(WORK)==176?1:-1];
typedef char P_Size[sizeof(P)==6560?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_input_Offset[FIELD_OFFSET(P,input)==336?1:-1];
typedef char P_angle_Offset[FIELD_OFFSET(P,angle)==424?1:-1];
typedef char P_action_Offset[FIELD_OFFSET(P,action)==3840?1:-1];
typedef char P_curve_Offset[FIELD_OFFSET(P,curve)==4144?1:-1];
typedef char P_target_Offset[FIELD_OFFSET(P,target)==6340?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==6352?1:-1];
typedef char P_changed_Offset[FIELD_OFFSET(P,changed)==6356?1:-1];
typedef char P_farTime_Offset[FIELD_OFFSET(P,farTime)==6388?1:-1];
typedef char P_previous_Offset[FIELD_OFFSET(P,previous)==6392?1:-1];
typedef char P_flipped_Offset[FIELD_OFFSET(P,flipped)==6396?1:-1];
typedef char P_link_Offset[FIELD_OFFSET(P,link)==6400?1:-1];
typedef char P_combined_Offset[FIELD_OFFSET(P,combined)==6512?1:-1];
typedef char P_activeOnly_Offset[FIELD_OFFSET(P,activeOnly)==6524?1:-1];
typedef char P_idle_Offset[FIELD_OFFSET(P,idle)==6536?1:-1];
typedef char P_animation_Offset[FIELD_OFFSET(P,animation)==6548?1:-1];
typedef char P_speedBlend_Offset[FIELD_OFFSET(P,speedBlend)==6552?1:-1];
typedef char P_activeBlend_Offset[FIELD_OFFSET(P,activeBlend)==6556?1:-1];
typedef char PL_angle_Offset[FIELD_OFFSET(PL,angle)==24?1:-1];
typedef char PL_amount_Offset[FIELD_OFFSET(PL,amount)==28?1:-1];
typedef char CURVE_value_Offset[FIELD_OFFSET(CURVE,value)==28?1:-1];
typedef char CURVE_slope_Offset[FIELD_OFFSET(CURVE,slope)==64?1:-1];
typedef char ANIMATION_gain_Offset[FIELD_OFFSET(ANIMATION,gain)==116?1:-1];
typedef char LINK_object_Offset[FIELD_OFFSET(LINK,object)==4?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char WORK_reference_Offset[FIELD_OFFSET(WORK,reference)==0?1:-1];
typedef char WORK_direction_Offset[FIELD_OFFSET(WORK,direction)==16?1:-1];
typedef char WORK_local_Offset[FIELD_OFFSET(WORK,local)==32?1:-1];
typedef char WORK_matrix_Offset[FIELD_OFFSET(WORK,matrix)==48?1:-1];
typedef char WORK_returned_Offset[FIELD_OFFSET(WORK,returned)==96?1:-1];
typedef char WORK_heading_Offset[FIELD_OFFSET(WORK,heading)==144?1:-1];
typedef char WORK_target_Offset[FIELD_OFFSET(WORK,target)==160?1:-1];
extern "C" void func_00190520(P*p){
 WORK w;int near=0,far=0;
 func_00190BE0(p,&w.heading);
 if(p->flipped)w.heading=RadNormalize__Ff(w.heading+3.1415927410125732421875f);
 func_00204660(p,w.heading);
 float inputDifference=RadNormalize__Ff(p->input->angle-p->angle);
 if(p->input->amount>0.3f){float d=Abs(inputDifference);if(d<1.0471975803375244140625f){near=1;p->farTime=D_002DDED0.time;}else if(d>2.094395160675048828125f){far=1;p->farTime=D_002DDED0.time;}}
 else p->previous=-1;
 w.reference.q=func_001A62E8(p->target);register P*receiver __asm__("$4");
 __asm__("sq %2,%1\n\tdaddu %0,%3,$zero" : "=r"(receiver),"=m"(w.target.q) : "r"(w.reference.q),"r"(p));
 register qword positionBits __asm__("$2")=func_001A62E8(receiver);
 register qword position __asm__("vf2"),targetVector __asm__("vf1");
 register float epsilon __asm__("$f1");const uint epsilonBits=0x38D1B717;
 __asm__(".set noat\n\tqmtc2.ni %3,%0\n\tlui $1,%4\n\tori $1,$1,%5\n\tmtc1 $1,%2\n\tlqc2 %1,%6\n\t.set at"
 : "=j"(position),"=j"(targetVector),"=f"(epsilon) : "r"(positionBits),"i"(epsilonBits>>16),"i"(epsilonBits&0xFFFF),"m"(w.target.q) : "$1");
 register qword difference __asm__("vf3");
 register uint high __asm__("$2"),savedPage __asm__("$3");
 __asm__("lui %0,%%hi(%4)\n\tvsub.xyzw %1,%3,%2" : "=r"(high),"=j"(difference) : "j"(position),"j"(targetVector),"i"(&D_002A2DC0));
 register qword normalized __asm__("vf1"),one __asm__("vf2");
 register qword fallbackBits __asm__("$2");register qword*fallbackPointer __asm__("$2");float length;
 __asm__("daddu %0,%3,$zero\n\tvmul.xyz %1,%4,%4\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x $ACC,%1,%1y\n\tvmaddz.x %1,%2,%1z"
 : "=r"(savedPage),"=&j"(normalized),"=&j"(one) : "r"(high),"j"(difference));
 __asm__("addiu %0,%2,%%lo(%3)\n\tvsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %1,$vf0,$Q"
 : "=r"(fallbackPointer),"+j"(normalized) : "r"(high),"i"(&D_002A2DC0));
 __asm__("lq %0,%2\n\tqmfc2.ni %1,%3" : "=r"(fallbackBits),"=r"(length) : "m"(*fallbackPointer),"j"(normalized));
 if(length<epsilon)normalized=fallbackBits;
 else __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(normalized) : "j"(normalized),"j"(difference));
 qword normal=normalized;register qword*axisPointer __asm__("$2");
 register qword axis __asm__("$5");register float yaw __asm__("$f12");
 __asm__("addiu %0,%1,%%lo(%2)" : "=r"(axisPointer) : "r"(savedPage),"i"(&D_002A2DC0));
 register M3*rotationOutput __asm__("$4");
 __asm__("lwc1 %0,%3\n\tlq %1,%4\n\tla %2,%a5" : "=f"(yaw),"=r"(axis),"=r"(rotationOutput) : "m"(p->angle),"m"(*axisPointer),"p"(&w.returned));
 w.direction.q=normal;
 (void)func_0011B500(rotationOutput,axis,yaw);w.matrix=w.returned;w.local.q=Local(w.matrix,w.direction.q);
 float angle=RadNormalize__Ff(func_002458F0(w.local.f[2],w.local.f[0])-1.57079637050628662109375f);
 int next=p->state;
 switch(next){
 case 0:
  if(far)next=2;
  else if(near){if(D_002DDED0.time-p->changed>D_002BF554){p->previous=0;next=1;}}
  else if(D_002DDED0.time-p->farTime>D_002BF55C)next=1;
  break;
 case 2:
  if(near)next=0;
  else if(far){if(D_002DDED0.time-p->changed>D_002BF558){p->previous=2;next=1;}}
  else if(D_002DDED0.time-p->farTime>D_002BF55C)next=1;
  break;
 case 1:
  if(near&&p->previous!=0){next=0;p->previous=-1;}
  else if(far&&p->previous!=2){next=2;p->previous=-1;}
  else if(Abs(angle)<D_002BF560){qword magnitude=LengthSquared(Velocity(p));register float limit __asm__("$f1");
   __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tlwc1 %1,%2" : "+j"(magnitude),"=f"(limit) : "m"(D_002BF568));
   bool slow=X(magnitude)<limit;if(slow)next=3;}
  break;
 case 3:
  if(near&&p->previous!=0){next=0;p->previous=-1;}
  else if(far&&p->previous!=2){next=2;p->previous=-1;}
  else if(Abs(angle)>D_002BF564){qword magnitude=LengthSquared(Velocity(p));register float limit __asm__("$f1");
   __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tlwc1 %1,%2" : "+j"(magnitude),"=f"(limit) : "m"(D_002BF56C));
   bool fast=X(magnitude)>limit;if(fast)next=1;}
  break;
 }
 func_0018C250(p,3,next);func_0018FC60(p);
 if(p->link)func_001F5078(p->link->object);
 if(func_001C4660(p->input,0x400,0.15f)&&Can(p,3)){func_001C4738(p->input,0x400);func_0019CC50(p->action,0,0,0);}
 if(p->state==3)return;
 float activeTarget,speedTarget;
 switch(p->state){case 0:activeTarget=1.0f;speedTarget=1.0f;break;case 2:activeTarget=1.0f;speedTarget=0.0f;break;case 1:{register float half __asm__("$f0"),zero __asm__("$f20");const uint halfBits=0x3F000000;const int zeroBits=0;
 __asm__(".set noat\n\tlui $1,%2\n\tmtc1 $1,%0\n\tmtc1 %z3,%1\n\t.set at" : "=f"(half),"=f"(zero) : "i"(halfBits>>16),"rJ"(zeroBits) : "$1");
 activeTarget=zero;speedTarget=activeTarget;if(p->speedBlend>half)speedTarget=1.0f;break;}}
 p->activeBlend=GSmooth__FfffP3SMPPf(p->activeBlend,activeTarget,D_002DDED0.dt,&D_002BF570,0);
 p->speedBlend=GSmooth__FfffP3SMPPf(p->speedBlend,speedTarget,D_002DDED0.dt,&D_002BF580,0);
 float active=p->activeBlend,speed=p->speedBlend;float idle=1.0f-active;float combined=active*speed;float activeOnly=active*(1.0f-speed);p->combined=combined;p->idle=idle;p->activeOnly=activeOnly;func_001384C0(p->animation);
 float steering=Clamp(angle/D_002BF58C,-1.0f,1.0f);
 float target=p->animation->gain*(steering+1.0f);target*=0.5f;
 p->curve->slope=func_0011CFE0((target-p->curve->value)/D_002DDED0.dt,D_002BF590);
}

typedef char final_offset_0_0[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char final_offset_0_1[FIELD_OFFSET(VE,pad)==2?1:-1];
typedef char final_offset_0_2[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char final_offset_1_0[FIELD_OFFSET(U,q)==0?1:-1];
typedef char final_offset_1_1[FIELD_OFFSET(U,f)==0?1:-1];
typedef char final_offset_1_2[FIELD_OFFSET(U,i)==0?1:-1];
typedef char final_offset_2_0[FIELD_OFFSET(M3,row[0])==0?1:-1];
typedef char final_offset_2_1[FIELD_OFFSET(M3,row[1])==16?1:-1];
typedef char final_offset_2_2[FIELD_OFFSET(M3,row[2])==32?1:-1];
typedef char final_offset_3_0[FIELD_OFFSET(LM,values[0])==0?1:-1];
typedef char final_offset_3_1[FIELD_OFFSET(LM,values[1])==4?1:-1];
typedef char final_offset_3_2[FIELD_OFFSET(LM,values[2])==8?1:-1];
typedef char final_size_0[sizeof(qword)==16?1:-1];
typedef char final_size_1[sizeof(int)==4?1:-1];
typedef char final_size_2[sizeof(uint)==4?1:-1];
typedef char final_size_3[sizeof(float)==4?1:-1];
typedef char final_size_4[sizeof(short)==2?1:-1];
typedef char final_size_5[sizeof(void*)==4?1:-1];
typedef char final_alignment_0[__alignof__(qword)==16?1:-1];
typedef char final_alignment_1[__alignof__(U)==16?1:-1];
typedef char final_alignment_2[__alignof__(M3)==16?1:-1];
typedef char final_alignment_3[__alignof__(WORK)==16?1:-1];
typedef char final_alignment_4[__alignof__(P)==4?1:-1];
typedef char final_alignment_5[__alignof__(VE)==4?1:-1];
typedef char final_alignment_6[__alignof__(PL)==4?1:-1];
typedef char final_alignment_7[__alignof__(CURVE)==4?1:-1];
typedef char final_alignment_8[__alignof__(ANIMATION)==4?1:-1];
typedef char final_alignment_9[__alignof__(LINK)==4?1:-1];
typedef char final_alignment_10[__alignof__(CLOCK)==4?1:-1];
typedef char final_alignment_11[__alignof__(LM)==4?1:-1];
