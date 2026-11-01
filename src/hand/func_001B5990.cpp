#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct M3{U rows[3];};
struct PL{char pad[0x18];float angle,strength;char pad20[0x10];uint flags;};
struct ANIM{char pad[0x40];float rate;};
struct ENTRY{float weight;int active;char pad8[8];U direction;};
struct VT{short delta,pad;void*fn;};
struct P{
 char pad[12];VT*vt;char pad10[0x140];PL*input;char pad154[0x3C];U acceleration;
 char pad1A0[0x10];M3 orientation;char pad1E0[0xE0];char controller[0x200];char context[0xA40];char requestF00[0x130];
 ANIM*animation;int pad1034,side;char pad103C[0xA0];void*target;
 char pad10E0[0x2F0];int mainState;char pad13D4[0x18];void*root;
 char pad13F0[0x5C];int reset;char pad1450[0x1E0];char motion[0xC0];
 ENTRY axes[8];void*request;int state;
};
extern "C"{
extern float D_002C08DC;
int func_00140AF8(P*,int);
void func_001B6078(P*,int);
int GrfConsumePressed__5POCTLUif(PL*,int,float);
int GrfPressedWithin__5POCTLUif(PL*,uint,float);
void ClearPoctlJoyPressed(PL*,int);
void func_00195A00(void*,int,int,void*);
void func_0019CC50(void*,int,P*,void*);
int func_001B70A0(P*);
int func_001938C0(void*,int,int);
int func_001B84E0(P*);
void func_001B8508(P*);
void func_001B85D0(P*);
void func_001B5D30(P*,int);
void func_002046F0(P*,void*);
void func_00204660(P*,float);
void func_0020BB58(void*,float);
void func_0020BC08(void*);
qword SetVectorCylind(float,float,float);
void func_001A0450(P*,ENTRY*,void*);
}
static inline qword Load(const U*p){qword q;__asm__("lqc2 %0,%1":"=j"(q):"m"(*p));return q;}
static inline float LengthXY(qword v){qword q;float f;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(q):"j"(v));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline qword Vector(qword q){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(q));return v;}
static inline qword Rotate(M3*m,qword vector){qword result=Load(&m->rows[0]),row1=m->rows[1].q,row2=m->rows[2].q; qword a,b,c,one;
 __asm__("vmul.xyzw %2,%0,%7\n\tvmaxw.xyzw %1,$vf0,$vf0w\n\tvmul.xyzw %3,%5,%7\n\tvmul.xyzw %4,%6,%7\n\tvadday.x $ACC,%2,%2y\n\tvmaddz.x %0,%1,%2z\n\tvaddax.y $ACC,%3,%3x\n\tvmaddz.y %0,%1,%3z\n\tvaddax.z $ACC,%4,%4x\n\tvmaddy.z %0,%1,%4y":"+j"(result),"=&j"(one),"=&j"(a),"=&j"(b),"=&j"(c):"j"(row1),"j"(row2),"j"(vector));return result;
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char PL_Size[sizeof(PL)==52?1:-1];
typedef char ANIM_Size[sizeof(ANIM)==68?1:-1];
typedef char ENTRY_Size[sizeof(ENTRY)==32?1:-1];
typedef char VT_Size[sizeof(VT)==8?1:-1];
typedef char PL_angle_Offset[FIELD_OFFSET(PL,angle)==24?1:-1];
typedef char PL_strength_Offset[FIELD_OFFSET(PL,strength)==28?1:-1];
typedef char PL_flags_Offset[FIELD_OFFSET(PL,flags)==48?1:-1];
typedef char ANIM_rate_Offset[FIELD_OFFSET(ANIM,rate)==64?1:-1];
typedef char ENTRY_weight_Offset[FIELD_OFFSET(ENTRY,weight)==0?1:-1];
typedef char ENTRY_active_Offset[FIELD_OFFSET(ENTRY,active)==4?1:-1];
typedef char ENTRY_direction_Offset[FIELD_OFFSET(ENTRY,direction)==16?1:-1];
typedef char VT_delta_Offset[FIELD_OFFSET(VT,delta)==0?1:-1];
typedef char VT_fn_Offset[FIELD_OFFSET(VT,fn)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_input_Offset[FIELD_OFFSET(P,input)==336?1:-1];
typedef char P_acceleration_Offset[FIELD_OFFSET(P,acceleration)==400?1:-1];
typedef char P_orientation_Offset[FIELD_OFFSET(P,orientation)==432?1:-1];
typedef char P_controller_Offset[FIELD_OFFSET(P,controller)==704?1:-1];
typedef char P_context_Offset[FIELD_OFFSET(P,context)==1216?1:-1];
typedef char P_requestF00_Offset[FIELD_OFFSET(P,requestF00)==3840?1:-1];
typedef char P_animation_Offset[FIELD_OFFSET(P,animation)==4144?1:-1];
typedef char P_side_Offset[FIELD_OFFSET(P,side)==4152?1:-1];
typedef char P_target_Offset[FIELD_OFFSET(P,target)==4316?1:-1];
typedef char P_mainState_Offset[FIELD_OFFSET(P,mainState)==5072?1:-1];
typedef char P_root_Offset[FIELD_OFFSET(P,root)==5100?1:-1];
typedef char P_reset_Offset[FIELD_OFFSET(P,reset)==5196?1:-1];
typedef char P_motion_Offset[FIELD_OFFSET(P,motion)==5680?1:-1];
typedef char P_axes_Offset[FIELD_OFFSET(P,axes)==5872?1:-1];
typedef char P_request_Offset[FIELD_OFFSET(P,request)==6128?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==6132?1:-1];
typedef char Axis1Direction_Offset[(FIELD_OFFSET(P,axes)+sizeof(ENTRY)+FIELD_OFFSET(ENTRY,direction))==0x1720?1:-1];
extern "C" void func_001B5990(P*p){
 int before;
 for(;;){
  before=p->state;
  switch(before){case 0:case 1:case 2:{
   if((p->input->flags&8)||!func_00140AF8(p,1))func_001B6078(p,0);
   else if(GrfConsumePressed__5POCTLUif(p->input,0x40,0.1f))func_00195A00(p->controller,1,0,0);
   else if(func_001B70A0(p)){}
   else if(GrfConsumePressed__5POCTLUif(p->input,0x20,0.2f)){
    if(!func_001938C0(p->context,0x80,0)&&p->root){p->reset=0;func_001B6078(p,4);}
   }
   else if(func_001B84E0(p)&&GrfConsumePressed__5POCTLUif(p->input,0x80,D_002C08DC))func_001B8508(p);
   else if(GrfPressedWithin__5POCTLUif(p->input,0x400,0.15f)&&((int(*)(P*,int))p->vt[99].fn)((P*)((char*)p+p->vt[99].delta),3)){
    ClearPoctlJoyPressed(p->input,0x400);func_0019CC50(p->requestF00,0,0,0);
   }
   else if(func_001B84E0(p)&&GrfConsumePressed__5POCTLUif(p->input,0x10,D_002C08DC))func_001B85D0(p);
   else {PL*pl=p->input;float strength=pl->strength;if(p->mainState==0?0.1f<strength:0.0f<strength)func_001B5D30(p,2);else func_001B5D30(p,0);}
  }break;
  }
  if(p->state==before||p->mainState!=9)break;
 }
 switch(p->state){
 case 0:func_002046F0(p,p->target);break;
 case 2:{
  func_002046F0(p,p->target);
  float length=LengthXY(p->axes[1].direction.q);
  PL*input=p->input;
  qword direction=Vector(SetVectorCylind(input->angle+3.1415927f,(input->strength*length)*p->animation->rate,0.0f));
  p->acceleration.q=direction;
  qword world=Rotate(&p->orientation,direction);
  p->side=-1;p->acceleration.q=world;
  func_001A0450(p,p->axes,p->request);
  break;
 }
 case 1:func_00204660(p,p->input->angle);func_0020BB58(p->motion,p->input->strength);func_0020BC08(p->motion);break;
 }
}
