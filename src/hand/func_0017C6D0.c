/* func_0017C6D0 (Claude agent): state-machine controller. pos is a plain qword (spilled pseudo) so p->v60=pos goes qmfc2+sq; the goto loop + explicit 250.0f local stops loop.c hoisting the case-1 constant. */
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
struct X {qword header,v;};
struct P {char pad10[0x10];void*p10;int pad14;void*p18;char pad1C[4];int pending;char pad24[12];void*p30;char pad34[0x40-0x34];qword v40;int state;float time;int pad58[2];qword v60;};
void UpdateBhv(struct P*);
int FBhvBusyAbove(struct P*);
qword func_001A62E8(void*);
qword ConvertAloPos(void*,void*,qword);
int func_00209098(void*);
void func_0017C928(struct P*,int);
void func_002562A8(struct X*);
void SetBhvgotoGoal(void*,const struct X*);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(a),"j"(b));return r;}
static inline qword SubIn(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
static inline float LenXY(qword v){float r;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(v):"0"(v));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(v):"0"(v));__asm__("qmfc2.ni %0,%1":"=r"(r):"j"(v));return r;}
void func_0017C6D0(struct P*p){
 struct X x;qword pos;float dist;int state;float lim=250.0f;
 UpdateBhv(p);
 pos=func_001A62E8(p->p30);
 dist=LenXY(Sub(pos,func_001A62E8(p->p10)));
 goto test;
 top:{
  state=p->state;
  switch(state){
  case 0:
   if(func_00209098(p->p10))func_0017C928(p,2);
   else if(1000.0f<dist)func_0017C928(p,1);
   else if(lim<LenXY(SubIn(p->v60,pos))){
    qword v;p->v60=pos;v=ConvertAloPos(p->p30,0,p->v40);
    func_002562A8(&x);x.v=v;SetBhvgotoGoal(p->p18,&x);
   }
   break;
  case 1:
   if(func_00209098(p->p10))func_0017C928(p,2);
   else if(dist<700.0f)func_0017C928(p,0);
   else if(lim<LenXY(SubIn(p->v60,pos))){
    p->v60=pos;func_002562A8(&x);x.v=pos;SetBhvgotoGoal(p->p18,&x);
   }
   break;
  case 2:if(!func_00209098(p->p10))func_0017C928(p,0);break;
  }
  if(state==p->state)return;
 }
 test:
 if(!FBhvBusyAbove(p))goto top;
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_X[(sizeof(struct X)==32)?1:-1];
typedef char layout_P[(sizeof(struct P)==0x70)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_P_p10[((unsigned)&((struct P*)0)->p10==0x10)?1:-1];
typedef char layout_P_p18[((unsigned)&((struct P*)0)->p18==0x18)?1:-1];
typedef char layout_P_pending[((unsigned)&((struct P*)0)->pending==0x20)?1:-1];
typedef char layout_P_p30[((unsigned)&((struct P*)0)->p30==0x30)?1:-1];
typedef char layout_P_v40[((unsigned)&((struct P*)0)->v40==0x40)?1:-1];
typedef char layout_P_state[((unsigned)&((struct P*)0)->state==0x50)?1:-1];
typedef char layout_P_time[((unsigned)&((struct P*)0)->time==0x54)?1:-1];
typedef char layout_P_v60[((unsigned)&((struct P*)0)->v60==0x60)?1:-1];
typedef char layout_X_header[((unsigned)&((struct X*)0)->header==0x0)?1:-1];
typedef char layout_X_v[((unsigned)&((struct X*)0)->v==0x10)?1:-1];
