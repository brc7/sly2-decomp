// Transition actor controller state.
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct M3 {qword r[3];};
struct NODE {M3 basis;qword orientation;char pad40[0x60];NODE *parent;char padA4[0xC];V velocity;};
struct O;
struct S {char pad[0x58];NODE *positionNode,*rotationNode;char pad60[0xF0];int state;float time;int sentinel,kind;O *nearest;char pad164[4];void *animation;char pad16C[0x10];void *rotationJob,*positionJob;char pad184[0xC];qword direction;};
struct WORLD;
struct CLOCK {float t,dt;};
extern WORLD D_003DD490;
extern CLOCK D_002DDED0;
extern qword D_002A2DC0;
extern float D_002A97A8;
extern O *D_002E1E40;
extern int D_002C4398[];
extern "C" {
void ReleaseAct(void **);
void RecalcSwOxfFilterForObject(WORLD *,S *);
void SetSoCnstrForce(S *,int);
void SetSoCnstrTorque(S *,int);
qword func_0011BFF0(const void *);
void *func_0012DC18(S *,NODE *,int,int,int,qword);
int func_00224688(S *);
qword ConvertXfVec(NODE *,NODE *,qword);
M3 *BuildOrthonormalMatrixZ(M3 *,qword,qword);
void SetLoParent_1A58D0(S *,int,int);
int CploFindSwObjectsByClass(WORLD *,int,int,int,int,O **);
qword func_001A62E8(void *);
void FadeSoOut(S *,float);
void SetSmaGoalIfReachable(void *,int);
}
static inline qword ToVF(qword v){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(v));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
static inline qword Normalize(qword v){qword len,one,result;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(len),"=&j"(one):"j"(v));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));
 __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(result):"j"(v),"0"(len));return result;}
static inline float Length(qword v){qword one;float f;
 __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(v),"=j"(one));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(v));
 __asm__("qmfc2.ni %0,%1":"=r"(f):"j"(v));return f;}
static inline void Zero(float *v){v[0]=v[1]=v[2]=0.0f;}static inline void ZeroVelocity(NODE *n){Zero(n->velocity.f);}
static inline O *FindNearest(S *p,int count,O **objects,O *nearest,float best){
  for(int i=0;i<count;i++){
   O *object=objects[i];
   qword position=ToVF(func_001A62E8(object));
   qword origin=func_001A62E8(p);
   V delta;
   delta.q=Sub(position,origin);
   delta.f[2]+=delta.f[2];
   float distance=Length(delta.q);
   if(distance<best){best=distance;nearest=object;}
  }
return nearest;}
static inline M3 MakeBasis(qword direction,qword rawUp) return result;{
 qword v=rawUp,len,one,normalized;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(len),"=&j"(one):"j"(v));
 qword first=direction;
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));
 __asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=&j"(normalized):"j"(v),"j"(len));
 BuildOrthonormalMatrixZ(&result,first,normalized);
}
/** @sly2-annot addr=0x224EC8
 * Purpose: CID100 (Bentley gadget object, id +0x15C) state: -1 pooled, 0 held by owner, 1 flight, 2 stuck to
 *   surface +0x1A0, 3 detonate/release, 5 health extract; each plays SMA goal D_002C4398[state] = 109..114
 * Info: proposed name SetCid100State (T2); class CID100 slot76; TU tu_223F70
 * Notes: runtime5a #19 CLARIFIED: states -1 pooled, 0 held, 1 flight, 2 stuck, 3 detonate, 5 health-extract;
 *   SMA goal D_002C4398[state] | T2: placeholder token Cid100
 * Confidence: low (draft) [T2 0.55] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/player.md#7-bentley-bentleycpp-0x134648,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
extern "C" void func_00224EC8(S *p,int state){
 int old=p->state;
 if(state==old)return;
 p->state=state;p->time=D_002DDED0.t;p->sentinel=-1;
 ReleaseAct(&p->positionJob);
 ReleaseAct(&p->rotationJob);
 if(old==0||old==4)RecalcSwOxfFilterForObject(&D_003DD490,p);
 switch(p->state){
 case 0:{
  SetSoCnstrForce(p,0);SetSoCnstrTorque(p,0);
  ZeroVelocity(p->positionNode);ZeroVelocity(p->rotationNode);
  qword position=p->positionNode->orientation;
  qword rotation=func_0011BFF0(p->rotationNode);
  p->positionJob=func_0012DC18(p,p->positionNode,0,7,2,position);
  p->rotationJob=func_0012DC18(p,p->rotationNode,0,7,2,rotation);
  RecalcSwOxfFilterForObject(&D_003DD490,p);
  break;
 }
 case 2:{
  SetSoCnstrForce(p,1);SetSoCnstrTorque(p,1);
  ZeroVelocity(p->positionNode);ZeroVelocity(p->rotationNode);
  if(func_00224688(p)&&p->rotationNode){
   qword direction=Normalize(ToVF(ConvertXfVec(0,p->rotationNode->parent,p->direction)));
   qword rawUp=ConvertXfVec(0,p->rotationNode->parent,D_002A2DC0);
   M3 basis;basis=MakeBasis(direction,rawUp);
   p->rotationJob=func_0012DC18(p,p->rotationNode,0,7,2,func_0011BFF0(&basis));
  }
  break;
 }
 case 1:{
  SetLoParent_1A58D0(p,0,15);SetSoCnstrForce(p,0);
  if(p->kind!=0x29){SetSoCnstrTorque(p,0);break;}
  SetSoCnstrTorque(p,1);
  O *objects[32];
  int count=CploFindSwObjectsByClass(&D_003DD490,0x105,0x38,0,32,objects);
  p->nearest=FindNearest(p,count,objects,D_002E1E40,D_002A97A8);
  break;
 }
 case 4:
  RecalcSwOxfFilterForObject(&D_003DD490,p);
  FadeSoOut(p,0.25f);
  break;
 }
 if(p->animation&&p->state>=0)SetSmaGoalIfReachable(p->animation,D_002C4398[p->state]);
}
