#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct M3{qword rows[3];};
struct SPACE;
struct NODE{char pad[0x30];qword position;char pad40[0x60];SPACE*space;};
struct MOTION{char pad[0x20];qword angles;};
struct VOLUME;
struct POOL;
struct P{char pad[0xFE0];P*next;char padFE4[0x2C];VOLUME*first,*second;int state;float changed;int id;POOL*animation;P*target;MOTION*motion;int pad1030;NODE*node;char pad1038[8];qword axis;};
struct ROW{int key;P*object;};
struct GAME;
extern "C"{
void func_00207350(P*);
int func_00209A68(P*,int);
qword func_001A62E8(P*);
qword ConvertAloPos(P*,void*,qword);
int func_00227C90(VOLUME*,qword);
void func_001BA408(P*,int);
int func_00131968(POOL*);
qword func_0022FC48(SPACE*,SPACE*,qword);
M3*BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(M3*,qword,qword);
qword func_0011BFF0(M3*);
void*func_001AE478(void*);
void func_001AE4A0(void*,int);
int func_001BA100(void*,ROW*);
void func_0020FAA0(GAME*,qword,qword,int(*)(void*,ROW*),void*,int*,ROW***);
extern P*D_003E1444;
extern float D_002A7620;
extern qword D_002C0A80,D_002C0A90,D_002C0AA0;
extern GAME D_003DD490;
}
struct MARK{MARK(){func_001AE478(this);}~MARK(){func_001AE4A0(this,2);}};
static inline qword Vector(qword r){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(r));return v;}
static inline qword Sub(qword a,qword b){qword q;__asm__("vsub.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline qword Add(qword a,qword b){qword q;__asm__("vadd.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline float Square(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(a));return f;}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char ROW_Size[sizeof(ROW)==8?1:-1];
typedef char NODE_position_Offset[FIELD_OFFSET(NODE,position)==48?1:-1];
typedef char NODE_space_Offset[FIELD_OFFSET(NODE,space)==160?1:-1];
typedef char MOTION_angles_Offset[FIELD_OFFSET(MOTION,angles)==32?1:-1];
typedef char ROW_object_Offset[FIELD_OFFSET(ROW,object)==4?1:-1];
typedef char P_next_Offset[FIELD_OFFSET(P,next)==4064?1:-1];
typedef char P_first_Offset[FIELD_OFFSET(P,first)==4112?1:-1];
typedef char P_second_Offset[FIELD_OFFSET(P,second)==4116?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==4120?1:-1];
typedef char P_animation_Offset[FIELD_OFFSET(P,animation)==4132?1:-1];
typedef char P_target_Offset[FIELD_OFFSET(P,target)==4136?1:-1];
typedef char P_motion_Offset[FIELD_OFFSET(P,motion)==4140?1:-1];
typedef char P_node_Offset[FIELD_OFFSET(P,node)==4148?1:-1];
typedef char P_axis_Offset[FIELD_OFFSET(P,axis)==4160?1:-1];
extern "C" void func_001BA128(P*p){
 func_00207350(p);
 switch(p->state){
 case 0:{
  if(!func_00209A68(p,0x25))break;
  VOLUME*volume;
  if(func_00209A68(p,9)){volume=p->first;p->target=0;}
  else if(func_00209A68(p,2)){volume=p->second;p->target=0;}
  else break;
  float nearest=D_002A7620;
  for(P*node=D_003E1444;node;node=node->next){
   if(node==p||func_00209A68(node,7))continue;
   qword origin=Vector(func_001A62E8(p));
   float distance=Square(Sub(origin,func_001A62E8(node)));
   if(nearest<distance)continue;
   if(func_00227C90(volume,func_001A62E8(node))){p->target=node;nearest=distance;}
  }
  if(!p->target&&!func_00209A68(p,2)){
   qword position=Vector(ConvertAloPos(p,0,D_002C0A80));
   qword low=Add(position,D_002C0A90);
   qword high=Add(position,D_002C0AA0);
   MARK arena;
   int count;ROW**rows;
   func_0020FAA0(&D_003DD490,low,high,func_001BA100,0,&count,&rows);
   for(int i=0;i<count;i++){
    ROW*row=rows[i];
    if(func_00227C90(volume,func_001A62E8(row->object))){p->target=row->object;break;}
   }
  }
  if(p->target)func_001BA408(p,1);
  break;
 }
 case 1:{
  if(func_00131968(p->animation)){func_001BA408(p,0);break;}
  NODE*node=p->node;
  qword position=func_001A62E8(p->target);
  qword local=func_0022FC48(0,node->space,position);
  qword direction=Sub(local,p->node->position);
  M3 matrix;BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(&matrix,p->axis,direction);
  MOTION*motion=p->motion;
  motion->angles=func_0011BFF0(&matrix);
  break;
 }
 default:break;
 }
}
