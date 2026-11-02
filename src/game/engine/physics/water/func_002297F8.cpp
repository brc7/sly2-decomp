// Resolve one contact between two physics objects: spring push, friction impulses (linear and
// angular), restoring torque, and the water-splash / emitter-clear effects.
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct M3 {qword r[3];};
struct M4 {M3 basis;qword position;};
struct COEF {float f[3];};
struct O;
struct VE {short delta,pad;qword (*get)(void *);};
struct NODE {char pad[0x98];short kind;};
struct CONSTRAINT {qword direction;int kind;};
struct PHYS {char pad[0x80];V direction;char pad90[0x20];CONSTRAINT constraint;float mass,spring,friction;};
struct STATE {V minimum,maximum;};
struct EMITTER;
struct WATER;
struct O {char pad[0xC];VE *vt;char pad10[0x48];NODE *body,*rotation,*motion;char pad64[0x84];PHYS *physics;STATE *state;char padF0[0x84];EMITTER *emitter;};
union PHASE {V vectors[3];M3 basis;};
union AXIS {COEF coefficients;V vector;};
struct WORK {V point,normal,velocity;PHASE phase;AXIS axis;V angularNormal,angularTangent,scalar;float angle;};
extern qword D_002A2DD0;
extern M3 D_002A2D00;
extern COEF D_002C4470,D_002C4480;
extern float D_002DDED4;
extern "C" {
float func_00229468(O *,O *,qword *,qword *);
int FFloatsNear__Ffff(float,float,float);
qword ApplyConstr(CONSTRAINT *,NODE *,qword);
void AddSoAcceleration(O *,qword);
qword func_001A62E8(O *);
void CalculateWaterCurrent(O *,qword,qword *,qword *);
float func_0011E290(qword,qword,qword *,qword *);
int func_00230380(int);
void AccelSoTowardPosSpring(O *,qword,const COEF *,qword,const COEF *);
void AccelSoTowardMatSpring(O *,const M3 *,const COEF *,qword,const COEF *);
M4 *func_001A6318(O *);
void TiltMatUpright(const M3 *,M3 *,M3 *);
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(const M3 *,float *,qword *);
void func_001FAC98(O *,qword);
void WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(O *,WATER *,float,qword,qword,float,float);
int FIsBasicDerivedFrom__FP5BASIC3CID(O *,int);
void StopSnd(EMITTER *,float);
void RcPtr_Assign(EMITTER **,EMITTER *);
}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){qword v;float f;__asm__("prot3w %0,%1":"=r"(v):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
static inline float Z(qword q){qword v;float f;__asm__("pextuw %0,$zero,%1":"=r"(v):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
static inline bool Near(qword a,qword b,float epsilon){return FFloatsNear__Ffff(X(a),X(b),epsilon)&&FFloatsNear__Ffff(Y(a),Y(b),epsilon)&&FFloatsNear__Ffff(Z(a),Z(b),epsilon);}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword Scale(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(k));return r;}
static inline qword Get(O *o,int slot){VE *v=o->vt;return v[slot].get((char *)o+v[slot].delta);}
static inline qword Sub(qword a,qword b){qword r;asm("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"0"(b));return r;}
static inline qword AddNormal(qword motion,const V &projection){qword r;asm("vadd.xyzw %0,%1,%2":"=j"(r):"0"(motion),"j"(projection.q));return r;}
static inline qword AddTangent(qword motion,const V &projection){qword r;asm("vadd.xyzw %0,%1,%2":"=j"(r):"0"(motion),"j"(projection.q));return r;}
static inline qword NegativeAngular(qword angular,float sign,qword &scalar){qword factor;asm("qmtc2.ni %1,%0":"=j"(factor):"r"(sign));scalar=factor;return Scale(angular,factor);}
// The coefficient tables are indexed directly (not through a pointer) so each loop forms its own address.
#define GAIN2(G,amount,friction) {for(i=0;i<3;i++)coefficients->f[i]=G.f[i]*amount*friction;}
#define GAIN1(G,friction) {for(i=0;i<3;i++)coefficients->f[i]=G.f[i]*friction;}
static inline void Zero(V &v,float zero){v.f[0]=v.f[1]=v.f[2]=zero;}
static inline void Clear(EMITTER **emitter){if(emitter&&*emitter){StopSnd(*emitter,0.0f);RcPtr_Assign(emitter,0);}}
/** @sly2-annot addr=0x2297F8
 * Purpose: resolves one contact between two physics objects incl. water splash
 * Info: class SO slot72; TU water
 * Notes: descriptive only: SO slot72: resolve XA contact (spring/friction/splash) | no usable identifier;
 *   purpose only
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
extern "C" void func_002297F8(O *a,O *b){
 WORK w;
 int i;
 float span=func_00229468(b,a,&w.point.q,&w.normal.q);
 float epsilon=0.0001f;
 if(span<epsilon)return;
 float spring=Near(D_002A2DD0,a->physics->direction.q,epsilon)?0.0f:a->physics->spring*b->physics->spring;
 float friction=a->physics->friction*b->physics->friction;
 float factor;
 if(spring!=0.0f){
  asm volatile("": : :"memory"); // the original re-reads a->physics here instead of reusing the pointer loaded above
  qword k=Scalar(-spring*span);w.velocity.q=k;
  AddSoAcceleration(a,ApplyConstr(&a->physics->constraint,a->motion,Scale(a->physics->direction.q,k)));
 }
 if(friction!=0.0f){
  CalculateWaterCurrent(b,func_001A62E8(a),&w.velocity.q,&w.phase.vectors[0].q);
  qword linear=Get(a,33);qword relative=Sub(w.velocity.q,linear);
  float dot=func_0011E290(relative,w.normal.q,&w.phase.vectors[1].q,&w.phase.vectors[2].q);
  if(span==1.0f||0.0f<dot)factor=1.0f;else factor=span*0.5f+0.5f;
  if(func_00230380(a->body->kind)){
   COEF *coefficients=&w.axis.coefficients;
   GAIN2(D_002C4470,factor,friction);
   AccelSoTowardPosSpring(a,D_002A2DD0,0,AddNormal(linear,w.phase.vectors[1]),coefficients);
   GAIN2(D_002C4470,span,friction);
   AccelSoTowardPosSpring(a,D_002A2DD0,0,AddTangent(linear,w.phase.vectors[2]),coefficients);
  }
  if(func_00230380(a->rotation->kind)){
   qword angular=Get(a,34);
   qword k;
   qword negative=NegativeAngular(angular,-1.0f,k);
   w.scalar.q=k;func_0011E290(negative,w.normal.q,&w.angularNormal.q,&w.angularTangent.q);
   COEF *coefficients=&w.axis.coefficients;
   GAIN2(D_002C4480,span,friction);
   AccelSoTowardMatSpring(a,&D_002A2D00,0,AddNormal(angular,w.angularNormal),coefficients);
   GAIN1(D_002C4480,friction);
   AccelSoTowardMatSpring(a,&D_002A2D00,0,AddTangent(angular,w.angularTangent),coefficients);
  }
 }
 else Zero(w.velocity,0.0f);
 if(spring!=0.0f){
  TiltMatUpright(&func_001A6318(a)->basis,&w.phase.basis,0);
  DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&w.phase.basis,&w.angle,&w.axis.vector.q);
  float gain=spring*3.0f;qword k=Scalar(w.angle*gain*span);
  qword rotation=Scale(w.axis.vector.q,k);w.angularNormal.q=k;
  func_001FAC98(a,rotation);
 }
 if(0.0f<span&&span<1.0f&&0.9f<w.normal.f[2]){
  float radius=((a->state->maximum.f[0]-a->state->minimum.f[0])+(a->state->maximum.f[1]-a->state->minimum.f[1]))*0.25f;
  WakeSoWater__FP2SOP5WATERfP6VECTORT3ff(a,(WATER *)b,D_002DDED4,w.velocity.q,w.point.q,0.1f,radius);
 }
 else if(FIsBasicDerivedFrom__FP5BASIC3CID(a,0x32))Clear(&a->emitter);
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char contact_layout_assert[(sizeof(V)==0x10 && sizeof(M3)==0x30 && sizeof(M4)==0x40 && OFF(M4,basis)==0 && OFF(M4,position)==0x30 && sizeof(COEF)==0xC && sizeof(CONSTRAINT)==0x20 && sizeof(PHASE)==0x30 && sizeof(AXIS)==0x10 && sizeof(WORK)==0xB0 && sizeof(VE)==8 && OFF(O,vt)==0xc && OFF(O,body)==0x58 && OFF(O,rotation)==0x5c && OFF(O,motion)==0x60 && OFF(O,physics)==0xe8 && OFF(O,state)==0xec && OFF(O,emitter)==0x174 && OFF(NODE,kind)==0x98 && OFF(PHYS,direction)==0x80 && OFF(PHYS,constraint)==0xb0 && OFF(PHYS,mass)==0xd0 && OFF(PHYS,spring)==0xd4 && OFF(PHYS,friction)==0xd8 && OFF(CONSTRAINT,direction)==0x0 && OFF(CONSTRAINT,kind)==0x10 && OFF(STATE,minimum)==0x0 && OFF(STATE,maximum)==0x10 && OFF(WORK,point)==0x0 && OFF(WORK,normal)==0x10 && OFF(WORK,velocity)==0x20 && OFF(WORK,phase)==0x30 && OFF(WORK,axis)==0x60 && OFF(WORK,angularNormal)==0x70 && OFF(WORK,angularTangent)==0x80 && OFF(WORK,scalar)==0x90 && OFF(WORK,angle)==0xa0)?1:-1];
