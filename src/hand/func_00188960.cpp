// func_00188960 (Claude agent, lane G9, 2026-10-24): MATCH, no register pins, VU helper asms only (Lilly style).
// Keys: the 0x20 stack slot is the by-value VU_FLOAT temporary (not a WORK field); WORK = {a, b} at 0x0/0x10 shared by cases 14 and 24; Reject = QwProjectOut.

typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct M3{U r[3];M3(){}M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return*this;}};
struct M4{U r[4];};struct VTE{short delta,pad;void*fn;};
struct MODEL{char pad[0x1C];float base;};
struct ACTOR{char pad[0x80];qword response;};
struct P{char pad0[0xC];VTE*vt;char pad10[0xD8];ACTOR*actor;char padEC[0xF44];MODEL*model;char pad1034[0x894];int mode,unusedCC,submode;char pad18D4[0x1B0];void*lookup;};
struct CLOCK{float time,dt;};struct WORK{U a,b;};
extern "C"{
extern CLOCK D_002DDED0;extern qword D_002A2DC0;
extern float D_002BF288,D_002BF28C,D_002BF290,D_002BF294;
void func_001F9B20(P*);void func_00204108(P*);
void func_001903B8(P*);void func_0019DF70(void*);void func_00196DF8(void*);
void func_001A1148(void*);void func_00194170(void*);void func_00137248(void*);
qword func_00204E38(P*,MODEL*,float);float func_0012F990(void*,int);
void func_001FAC30(P*,qword);qword LimitVectorLength(qword,float);
M4*func_001A6318(P*);float func_0011E290(qword,qword,qword*,qword*);
M3 func_0011B500(qword,float);
}
struct VU_FLOAT{qword qw;};
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g));return v;}
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));return v;}
static inline qword QwAdd(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwScaleX(qword a,VU_FLOAT g){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(g.qw));return r;}
static inline qword QwDivide(qword a,VU_FLOAT g){qword r;__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(r):"j"(a),"j"(g.qw));return r;}
static inline float GZOfQw(qword a){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(a));return f;}
static inline qword QwRotateVecAt(M3*m,qword*pv){qword r;__asm__("vmulax.xyz ACC,%2,%1x\n\tvmadday.xyz ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z":"=j"(r):"j"(*pv),"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q));return r;}
static inline qword QwDot(qword a,qword b){qword r,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=j"(one):"j"(a),"j"(b));return r;}
static inline qword QwProjectOut(qword v,qword n){qword d=QwDot(v,n);qword al,ac;__asm__("vmulx.xyzw %0,%2,%1x":"=j"(al):"j"(d),"j"(n));__asm__("vsub.xyzw %0,%1,%2":"=j"(ac):"j"(v),"j"(al));return ac;}
extern "C" void func_00188960(P*p){
 WORK w;
 switch(p->mode){case 3:case 8:func_001F9B20(p);break;case 21:case 22:break;default:func_00204108(p);break;}
 switch(p->mode){
 case 3:func_001903B8(p);break;
 case 8:func_0019DF70((char*)p+0xF50);break;
 case 2:func_00196DF8((char*)p+0x2C0);break;
 case 4:func_001A1148((char*)p+0xDF0);break;
 case 7:func_00194170((char*)p+0x4C0);break;
 case 6:func_00137248((char*)p+0x1FF0);break;
 case 14:{
  VTE*e=&p->vt[33];
  w.a.q=((qword(*)(void*))e->fn)((char*)p+e->delta);
  w.b.q=func_00204E38(p,p->model,p->model->base);
  float cap=func_0012F990(p->lookup,0x16D);
  if(p->model->base<cap){
   qword resp=p->actor->response;
   func_001FAC30(p,QwScaleX(resp,VuFloat(-1.0f)));
  }else w.b.f[2]=w.a.f[2];
  func_001FAC30(p,LimitVectorLength(QwDivide(QwSub(w.b.q,w.a.q),VuFloatFromMemWord(*(int*)&D_002DDED0.dt)),D_002BF288));
  break;
 }
 case 24:
  if(p->submode==13){
   VTE*e=&p->vt[33];qword position=((qword(*)(void*))e->fn)((char*)p+e->delta);
   e=&p->vt[34];qword direction=((qword(*)(void*))e->fn)((char*)p+e->delta);
   M4*m=func_001A6318(p);M3 basis;basis.r[0]=m->r[0];basis.r[1]=m->r[1];basis.r[2]=m->r[2];
   float projection=func_0011E290(position,basis.r[0].q,&w.a.q,&w.b.q);
   float dt=D_002DDED0.dt;
   float gAlong=projection-D_002BF290*dt;
   float gPerp=1.0f-D_002BF294*dt;
   float angle=GZOfQw(direction)*dt;
   w.a.q=QwScaleX(basis.r[0].q,VuFloat(gAlong));
   w.b.q=QwScaleX(w.b.q,VuFloat(gPerp));
   M3 rotation;
   rotation=func_0011B500(D_002A2DC0,angle);
   qword sum=QwAdd(w.a.q,w.b.q);
   qword moved=QwSub(QwRotateVecAt(&rotation,&sum),position);
   qword velocity=QwDivide(moved,VuFloatFromMemWord(*(int*)&D_002DDED0.dt));
   func_001FAC30(p,LimitVectorLength(QwProjectOut(velocity,D_002A2DC0),D_002BF28C));
  }
  break;
 }
}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char Unsigned_Size[(sizeof(unsigned)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M3_Size[(sizeof(M3)==0x30)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char VTE_Size[(sizeof(VTE)==8)?1:-1];
//typedef char WORK_Size[(sizeof(WORK)==0x90)?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((VTE*)0)->delta==0)?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((VTE*)0)->fn==4)?1:-1];
typedef char MODEL_base_Offset[((unsigned)&((MODEL*)0)->base==28)?1:-1];
typedef char ACTOR_response_Offset[((unsigned)&((ACTOR*)0)->response==128)?1:-1];
typedef char P_vt_Offset[((unsigned)&((P*)0)->vt==12)?1:-1];
typedef char P_actor_Offset[((unsigned)&((P*)0)->actor==232)?1:-1];
typedef char P_model_Offset[((unsigned)&((P*)0)->model==4144)?1:-1];
typedef char P_mode_Offset[((unsigned)&((P*)0)->mode==6344)?1:-1];
typedef char P_submode_Offset[((unsigned)&((P*)0)->submode==6352)?1:-1];
typedef char P_lookup_Offset[((unsigned)&((P*)0)->lookup==6788)?1:-1];
typedef char CLOCK_dt_Offset[((unsigned)&((CLOCK*)0)->dt==4)?1:-1];
