#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct M3{qword r[3];};
struct CONTROL;
struct P{char pad[0x124];int kind;char pad128[0x2C0-0x128];unsigned long flags;char pad2C8[0xF80-0x2C8];int mode;P*linked;char padF88[0xFD0-0xF88];char control[0];};
struct INFO{char pad[0x58];P*target;int reverse;int extra;};
struct DRIVER{char pad[8];INFO*info;};
struct C{char pad[0x10];P*actor;char pad14[0x40-0x14];DRIVER*driver;char pad44[0x50-0x44];U point;P*target;int reverse,extra,basisValid;M3 matrix;};
union TEMP{struct{U direction;M3 rotation;}first;M3 basis;};
struct WORK{TEMP temp;float curve;};
struct LIMIT{float low,high;};
extern "C"{
 extern unsigned long D_002DFB70;
 extern int D_002DFB78;
 extern qword D_002A2DC0;
 extern LIMIT D_002BE6B8;
 unsigned long func_00207A20(int);
 int func_00132B10(P*,int);
 qword func_001A62E8(P*);
 M3*func_001A6318(P*);
 void func_001E6F60(P*,qword,void*,qword*,qword*,float*,float*);
 float func_002458F0(float,float);
 float RadNormalize__Ff(float);
 void func_0011B500(M3*,float,qword);
 M3*BuildOrthonormalMatrixZ(M3*,qword,qword);
 void func_001C3500(P*,int,M3*,float);
 void func_0017A468(C*);
 int func_00173610(qword*,float,float,int,int(*)(void));
 void func_00174950(C*);
 void func_00209818(CONTROL*,float);
 void func_00209840(CONTROL*,M3*);
 void func_0020C6F8(INFO*,qword);
 void func_001EEFC0(C*);
}
static inline CONTROL*Control(P*p){return (CONTROL*)((char*)p+0xFD0);}
static inline float Angle(qword delta){qword x;int y;float fx,fy;__asm__("qmfc2.ni %0,%1":"=r"(x):"j"(delta));__asm__("prot3w %0,%1":"=r"(y):"r"(x));__asm__("mtc1 %1,%0":"=f"(fx):"r"(x));__asm__("mtc1 %1,%0":"=f"(fy):"r"(y));return func_002458F0(fy,fx);}
static inline qword HeadingInputs(C*p,U*home,qword current,P**actor){
 register qword a __asm__("vf1"),b __asm__("vf2");P*o;
 __asm__("qmtc2.ni %3,%1\n\tlw %2,%4\n\tlqc2 %0,%5":"=j"(a),"=j"(b),"=&r"(o):"r"(current),"m"(p->actor),"m"(home->q));
 __asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));*actor=o;return a;
}
static inline qword BasisInputs(C*p,U*home,qword current,P**actor){
 register qword a __asm__("vf1"),b __asm__("vf2");P*o;
 __asm__("lqc2 %0,%5\n\tqmtc2.ni %3,%1\n\tlw %2,%4":"=j"(a),"=j"(b),"=&r"(o):"r"(current),"m"(p->actor),"m"(home->q));
 __asm__("vsub.xyzw %0,%1,%0":"+j"(b):"j"(a));*actor=o;return b;
}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char TEMP_Size[sizeof(TEMP)==64?1:-1];
typedef char WORK_Size[sizeof(WORK)==80?1:-1];
typedef char P_kind_Offset[FIELD_OFFSET(P,kind)==292?1:-1];
typedef char P_flags_Offset[FIELD_OFFSET(P,flags)==704?1:-1];
typedef char P_mode_Offset[FIELD_OFFSET(P,mode)==3968?1:-1];
typedef char P_linked_Offset[FIELD_OFFSET(P,linked)==3972?1:-1];
typedef char P_control_Offset[FIELD_OFFSET(P,control)==4048?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==16?1:-1];
typedef char C_driver_Offset[FIELD_OFFSET(C,driver)==64?1:-1];
typedef char C_point_Offset[FIELD_OFFSET(C,point)==80?1:-1];
typedef char C_target_Offset[FIELD_OFFSET(C,target)==96?1:-1];
typedef char C_reverse_Offset[FIELD_OFFSET(C,reverse)==100?1:-1];
typedef char C_extra_Offset[FIELD_OFFSET(C,extra)==104?1:-1];
typedef char C_basisValid_Offset[FIELD_OFFSET(C,basisValid)==108?1:-1];
typedef char C_matrix_Offset[FIELD_OFFSET(C,matrix)==112?1:-1];
typedef char INFO_target_Offset[FIELD_OFFSET(INFO,target)==88?1:-1];
typedef char INFO_reverse_Offset[FIELD_OFFSET(INFO,reverse)==92?1:-1];
typedef char INFO_extra_Offset[FIELD_OFFSET(INFO,extra)==96?1:-1];
typedef char DRIVER_info_Offset[FIELD_OFFSET(DRIVER,info)==8?1:-1];
typedef char WORK_temp_Offset[FIELD_OFFSET(WORK,temp)==0?1:-1];
typedef char WORK_curve_Offset[FIELD_OFFSET(WORK,curve)==64?1:-1];
typedef char TEMP_first_direction_Offset[FIELD_OFFSET(TEMP,first.direction)==0?1:-1];
typedef char TEMP_first_rotation_Offset[FIELD_OFFSET(TEMP,first.rotation)==16?1:-1];
typedef char TEMP_basis_Offset[FIELD_OFFSET(TEMP,basis)==0?1:-1];
typedef char LIMIT_low_Offset[FIELD_OFFSET(LIMIT,low)==0?1:-1];
typedef char LIMIT_high_Offset[FIELD_OFFSET(LIMIT,high)==4?1:-1];
extern "C" void func_00179FA0(C*p){
 WORK work;U point,otherPoint,direction;
 if(!D_002DFB78){
  unsigned long a=func_00207A20(0x1B),b=func_00207A20(0x27),c=func_00207A20(0x28),d=func_00207A20(0x29);
  D_002DFB78=1;D_002DFB70=a|b|c|d;
 }
 unsigned long mask=D_002DFB70;
 unsigned long old=p->actor->flags&mask;
 p->actor->flags &= ~mask;
 unsigned long active=0;
 if(p->target){
  float heading;
  if(func_00132B10(p->target,10)){
   P*target=p->target;
   qword position=func_001A62E8(p->actor);
   func_001E6F60(target,position,0,&p->point.q,&work.temp.first.direction.q,0,&work.curve);
   heading=func_002458F0(work.temp.first.direction.f[1],work.temp.first.direction.f[0]);
  }else{
   p->point.q=func_001A62E8(p->target);
   M3*a=func_001A6318(p->target);
   M3*b=func_001A6318(p->target);
   heading=func_002458F0(((U*)a->r)->f[1],((U*)b->r)->f[0]);
  }
  if(func_00132B10(p->target,13)){
   active|=func_00207A20(0x1B);
   if(!p->reverse)heading=RadNormalize__Ff(heading+3.1415927410125732421875f);
   p->basisValid=1;
   func_0011B500(&work.temp.first.rotation,heading,D_002A2DC0);
   p->matrix=work.temp.first.rotation;
  }else if(func_00132B10(p->target,0x27)){
   active|=func_00207A20(0x29);
  }else if(func_00132B10(p->target,11)){
   active|=func_00207A20(0x27);
   p->basisValid=1;
   func_001C3500(p->target,p->reverse,&p->matrix,work.curve);
   func_0017A468(p);
  }else if(func_00132B10(p->target,0x13)&&p->target->kind==3){
   active|=func_00207A20(0x28);
   p->basisValid=1;
   M3*m=func_001A6318(p->target);
   (void)BuildOrthonormalMatrixZ(&work.temp.basis,m->r[0],D_002A2DC0);
   p->matrix=work.temp.basis;
   func_0017A468(p);
  }else{
  (void)func_00173610(&p->point.q,D_002BE6B8.high,D_002BE6B8.low,1,0);
  p->target=0;
  }
 }
 p->actor->flags|=active;
 func_00174950(p);
 p->actor->flags &= ~active;
 p->actor->flags |= old;
 if(p->driver){
  switch(p->actor->mode){
  case -1:case 0:case 3:{
   point=p->point;
   qword current=func_001A62E8(p->actor);
   P*actor; qword delta=HeadingInputs(p,&point,current,&actor);
   CONTROL*control=Control(actor);
   float angle=Angle(delta);
   func_00209818(control,angle);
   break;
  }
  case 2:{
   otherPoint=p->point;
   qword current=func_001A62E8(p->actor);
   P*actor;direction.q=BasisInputs(p,&otherPoint,current,&actor);
   M3*m=func_001A6318(actor->linked);
   (void)BuildOrthonormalMatrixZ(&work.temp.basis,m->r[0],direction.q);
   func_00209840(Control(p->actor),&work.temp.basis);
   break;
  }
  }
  p->driver->info->extra=p->extra;
  p->driver->info->target=p->target;
  p->driver->info->reverse=p->reverse;
  func_0020C6F8(p->driver->info,p->point.q);
 }
 func_001EEFC0(p);
}
