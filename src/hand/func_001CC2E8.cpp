typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {qword r[3];};
struct M4 {qword r[4];};
struct LO;
struct VE {short delta,pad;M4*(*matrix)(void*);};
struct LO {char padC[0xC];VE*vt;};
struct FRAME {char pad40[0x40];M3 basis;};
struct SPACE {char padA0[0xA0];FRAME*frame;};
struct ANIM {void*player;};
struct MOTION {char pad74[0x74];float duration;};
struct P {LO*owner;ANIM*animation;float weight;char padC[8];void*pathA,*pathC;float fraction;char pad20[8];void*pathB,*pathD;MOTION*motion;char pad34[4];SPACE*space;char pad3C[4];M4 matrix;void*job,*path;float time;LO*follow;int mode;char pad94[0x10];int active;};
struct CTX {P*p;qword position;};

struct EARLY {M4 product, temporary, inverse;};
struct LATE {CTX context; U local; M3 transformed, temporary;};
union STAGE {EARLY early; LATE late;};
struct WORK {U position; M3 basis; STAGE stage; float t,result,projection; U saved;};

extern "C" {
qword func_001A62E8(LO*);
void func_001E7380(void*,qword,void*,qword*,qword*,float*,float*,float);
M4 InvertMatrix4Rigid(const M4*);
M4 *__ml__FRC7MATRIX4T0(M4*,const M4*,const M4*);
qword func_001A6F00(LO*,LO*,qword);
void Minimize(float(*)(void*,float),void*,float,float,float,float,float*,float*) __asm__("MinimizeRange__FPFPvf_fPvffffPfT6");
float func_001CC2B8(void*,float);
float func_001CC288(void*,float);
void func_001CC1F8(P*,qword,float*,float*,float);
void func_001CC168(P*,qword,float*,float*,float);
M3 *func_0022FD38(M3*,FRAME*,FRAME*,const M3*);
qword func_0011BFF0(const M3*);
void func_0012D108(void*,qword);
void func_00130D30(void*,int,float,float);
void func_001384C0(MOTION*);
void func_002046B0(LO*,qword);
void func_00204CA8(LO*);
void func_001CC2E8(P*);
}
static inline void*operator new(unsigned size,void*destination){return destination;}
struct COPYPREFIX {U position;M3 basis;M4 product;};
static inline CTX*PublishProductAndBasis(WORK*w){register qword first __asm__("$6"),last __asm__("$4"),second __asm__("$2"),third __asm__("$3");register CTX*address __asm__("$5"),*context __asm__("$16");__asm__("lq %0,128(%7)\n\taddiu %4,%7,64\n\tlq %1,176(%7)\n\tdaddu %5,%4,$zero\n\tlq %2,144(%7)\n\tlq %3,160(%7)\n\tsq %2,80+%6\n\tsq %3,96+%6\n\tsq %1,%6\n\tsq %0,16+%6\n\tsq %0,64+%6\n\tsq %1,112+%6\n\tlq %2,16(%4)\n\tsq %2,32+%6\n\tlq %3,32(%4)\n\tsq %3,48+%6":"=&r"(first),"=&r"(last),"=&r"(second),"=&r"(third),"=&r"(address),"=&r"(context),"=m"(*(COPYPREFIX*)w):"r"(w),"m"(w->stage.early.temporary));return context;}
static inline qword LengthXY(qword v){register qword d __asm__("vf2");qword word;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(d):"j"(v));__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));__asm__("qmfc2.ni %0,%1":"=r"(word):"j"(d));return word;}
static inline void StoreX(float*p,qword q){__asm__("sw %1,%0":"=m"(*p):"r"(q));}
static inline qword DifferenceFromSaved(const U*saved,qword raw,P*p,LO*&receiver){register qword point __asm__("vf1"),current __asm__("vf2");register LO*owner __asm__("$4");__asm__("lqc2 %0,%3\n\tqmtc2.ni %4,%1\n\tlw %2,%5\n\tvsub.xyzw %0,%0,%1":"=&j"(point),"=&j"(current),"=r"(owner):"m"(*saved),"r"(raw),"m"(p->owner));receiver=owner;return point;}
static inline const M4*InverseArgument(const M4*value){register const M4*arg __asm__("$6");__asm__("daddu %0,%1,$zero":"=r"(arg):"r"(value));return arg;}
void func_001CC2E8(P*p){
 WORK w;register void*cursor __asm__("$16");
 if(p->active==-1)return;
 switch(p->mode){
 default:cursor=&w.stage.late.context;break;
 case 0:{cursor=p->path;func_001E7380(cursor,func_001A62E8(p->owner),0,&w.position.q,0,0,&p->time,p->time);cursor=&w.stage.late.context;break;}
 case 1:{qword found=func_001A62E8(p->follow);__asm__("sq %1,%0":"=m"(w.position.q):"r"(found));cursor=&w.stage.late.context;break;}
 case 2:{
  M4*output=&w.stage.early.temporary;
  M4*world=p->follow->vt[31].matrix((char*)p->follow+p->follow->vt[31].delta);
  cursor=&w.stage.early.inverse;new(cursor)M4(InvertMatrix4Rigid(&p->matrix));
  const M4*inverseArg=InverseArgument((M4*)cursor);
  __ml__FRC7MATRIX4T0(output,world,inverseArg);
  cursor=PublishProductAndBasis(&w);
  break;
 }
 }
 w.stage.late.context.p=p;
 {qword raw=func_001A6F00(0,p->owner,w.position.q);register qword point __asm__("vf1");__asm__("qmtc2.ni %1,%0":"=j"(point):"r"(raw));qword length=LengthXY(point);w.stage.late.local.q=point;w.stage.late.local.f[1]=0.0f;StoreX(&w.stage.late.local.f[0],length);}

 w.stage.late.context.position=func_001A6F00(p->owner,0,w.stage.late.local.q);
 if(p->mode==2){
  Minimize(func_001CC2B8,cursor,0.5f,0.1f,0.0f,1.0f,&w.t,&w.result);
  func_001CC1F8(p,w.stage.late.context.position,&w.projection,0,w.t);
  func_0022FD38(&w.stage.late.temporary,0,p->space->frame,&w.basis);
  w.stage.late.transformed=w.stage.late.temporary;
  func_0012D108(p->job,func_0011BFF0(&w.stage.late.transformed));
 }else{
  Minimize(func_001CC288,cursor,0.5f,0.1f,0.0f,1.0f,&w.t,&w.result);
  func_001CC168(p,w.stage.late.context.position,&w.projection,0,w.t);
 }
 func_00130D30(p->animation->player,0,w.t*p->motion->duration,0.0f);
 p->fraction=w.projection;p->weight=1.0f-w.projection;
 func_001384C0(p->motion);
 w.saved.q=w.position.q;
 LO*receiver;qword delta=DifferenceFromSaved(&w.saved,func_001A62E8(p->owner),p,receiver);
 func_002046B0(receiver,delta);
 func_00204CA8(p->owner);
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char path_layout[(sizeof(qword)==0x10 && sizeof(U)==0x10 && sizeof(M3)==0x30 && sizeof(M4)==0x40 && sizeof(CTX)==0x20 && sizeof(EARLY)==0xC0 && sizeof(LATE)==0x90 && sizeof(STAGE)==0xC0 && sizeof(WORK)==0x120 && sizeof(P)==0xB0 && sizeof(VE)==0x8 && sizeof(void*)==0x4 && sizeof(float)==0x4 && OFF(LO,vt)==0xC && OFF(SPACE,frame)==0xA0 && sizeof(SPACE)==0xA4 && OFF(FRAME,basis)==0x40 && sizeof(FRAME)==0x70 && OFF(ANIM,player)==0x0 && OFF(MOTION,duration)==0x74 && OFF(P,owner)==0x0 && OFF(P,animation)==0x4 && OFF(P,weight)==0x8 && OFF(P,pathA)==0x14 && OFF(P,pathC)==0x18 && OFF(P,fraction)==0x1C && OFF(P,pathB)==0x28 && OFF(P,pathD)==0x2C && OFF(P,motion)==0x30 && OFF(P,space)==0x38 && OFF(P,matrix)==0x40 && OFF(P,job)==0x80 && OFF(P,path)==0x84 && OFF(P,time)==0x88 && OFF(P,follow)==0x8C && OFF(P,mode)==0x90 && OFF(P,active)==0xA4 && OFF(CTX,p)==0x0 && OFF(CTX,position)==0x10 && OFF(EARLY,product)==0x0 && OFF(EARLY,temporary)==0x40 && OFF(EARLY,inverse)==0x80 && OFF(LATE,context)==0x0 && OFF(LATE,local)==0x20 && OFF(LATE,transformed)==0x30 && OFF(LATE,temporary)==0x60 && OFF(WORK,position)==0x0 && OFF(WORK,basis)==0x10 && OFF(WORK,stage)==0x40 && OFF(WORK,t)==0x100 && OFF(WORK,result)==0x104 && OFF(WORK,projection)==0x108 && OFF(WORK,saved)==0x110 && OFF(VE,delta)==0x0 && OFF(VE,matrix)==0x4)?1:-1];

typedef char copy_layout[(sizeof(COPYPREFIX)==0x80 && __alignof__(COPYPREFIX)==0x10 && OFF(COPYPREFIX,position)==0x0 && OFF(COPYPREFIX,basis)==0x10 && OFF(COPYPREFIX,product)==0x40 && OFF(WORK,stage)+OFF(EARLY,temporary)==0x80 && OFF(WORK,stage)+OFF(EARLY,inverse)==0xC0)?1:-1];
typedef char cc2e8_final_0[(sizeof(int)==4)?1:-1];
typedef char cc2e8_final_1[(sizeof(short)==2)?1:-1];
typedef char cc2e8_final_2[(__alignof__(qword)==16)?1:-1];
typedef char cc2e8_final_3[(__alignof__(U)==16)?1:-1];
typedef char cc2e8_final_4[(__alignof__(M3)==16)?1:-1];
typedef char cc2e8_final_5[(__alignof__(M4)==16)?1:-1];
typedef char cc2e8_final_6[(__alignof__(CTX)==16)?1:-1];
typedef char cc2e8_final_7[(__alignof__(WORK)==16)?1:-1];
typedef char cc2e8_final_8[(OFF(U,q)==0)?1:-1];
typedef char cc2e8_final_9[(OFF(U,f)==0)?1:-1];
typedef char cc2e8_final_10[(OFF(U,f[1])==4)?1:-1];
typedef char cc2e8_final_11[(OFF(U,f[2])==8)?1:-1];
typedef char cc2e8_final_12[(OFF(U,f[3])==12)?1:-1];
typedef char cc2e8_final_13[(OFF(WORK,stage)+OFF(LATE,context)+OFF(CTX,p)==0x40)?1:-1];
typedef char cc2e8_final_14[(OFF(WORK,stage)+OFF(LATE,context)+OFF(CTX,position)==0x50)?1:-1];
typedef char cc2e8_final_15[(OFF(WORK,stage)+OFF(LATE,local)==0x60)?1:-1];
typedef char cc2e8_final_16[(OFF(WORK,stage)+OFF(LATE,transformed)==0x70)?1:-1];
typedef char cc2e8_final_17[(OFF(WORK,stage)+OFF(LATE,temporary)==0xA0)?1:-1];
typedef char cc2e8_final_18[(sizeof(((WORK*)0)->saved)==16)?1:-1];
typedef char cc2e8_final_19[(sizeof(((WORK*)0)->position)==16)?1:-1];
typedef char cc2e8_final_20[(sizeof(((P*)0)->matrix)==64)?1:-1];
typedef char cc2e8_final_21[(31*sizeof(VE)+OFF(VE,delta)==0xF8)?1:-1];
typedef char cc2e8_final_22[(31*sizeof(VE)+OFF(VE,matrix)==0xFC)?1:-1];
