// Read transform records and their optional animation jobs from a binary stream.
typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
struct V {float f[4];} __attribute__((aligned(16)));
struct V2 {float x,y;};
struct M4 {V r[4];};
struct S;struct E;struct JOB;
struct RESOURCE {char pad[0x30];float position[3];};
struct VE {short delta,pad;RESOURCE *(*get)(void *,int);};
struct JOB {int kind;S *owner;E *entry;int order;JOB *next;int id;void *vt;};
struct J1 {JOB h;int selector;qword initial;};
struct J2 {JOB h;int pad1C;qword initial;V begin,end;V2 timingBegin,timingEnd;float g;int a,b,c;char pad70[0x10];};
struct J4 {JOB h;float lo,hi;int pad24;};
struct J9 {JOB h;int pad1C;V value;};
struct E {M4 matrix;char pad40[0x50];JOB *jobs;int kind,pad98,tag;RESOURCE *reference;int padA4;RESOURCE *origin;short lo,hi;char padB0[0x10];qword current;char padD0[0x10];qword previous;char padF0[0x10];};
struct S {char pad0[0xC];VE *vt;char pad10[0x30];int flags;char pad44[0xC];int count;E *records;RESOURCE *a,*b,*c;};
struct REQ {int size,kind;};
extern char D_002CA820[],D_002CA7E0[],D_002CA7A0[],D_002CA6A0[];
extern "C" {
int func_00117D50(void *);
int func_00117DB8(void *);
int func_00117D00(void *);
int func_00117BD0(void *);
float func_00117E38(void *);
void func_00117F90(void *,V *);
void func_00117FB0(void *,V2 *);
void func_00117EB0(void *,V *);
void func_00117F18(void *,M4 *);
void *func_001AE2A0(REQ *);
void *func_0012E1F0(int);
void *func_002333B4(void *,int,int);
qword func_0022CA18(E *,E *);
void func_0022C818(S *,E *);
void AddXfAct(S *,E *,JOB *);
void UpdateSoXfWorldMatrices(S *);
void LoadSoIkFromBrx(S *,void *);
void func_001603B8(S *,void *);
void func_0021C888(S *,void *);
}
static inline J1 *CreateJ1(){J1 *j;j=(J1 *)func_0012E1F0(sizeof(J1));j->h.vt=D_002CA820;j->h.kind=1;j->selector=2;return j;}
static inline J2 *CreateJ2(){J2 *j;j=(J2 *)func_0012E1F0(sizeof(J2));j->h.kind=2;j->h.vt=D_002CA7E0;return j;}
static inline J4 *CreateJ4(){J4 *j;j=(J4 *)func_0012E1F0(sizeof(J4));j->h.kind=4;j->h.vt=D_002CA7A0;return j;}
static inline J9 *CreateJ9(){J9 *j;j=(J9 *)func_0012E1F0(sizeof(J9));j->h.vt=D_002CA6A0;j->h.kind=9;j->h.id=20;return j;}
static inline RESOURCE *Lookup(S *p,int id){VE *ve=&p->vt[51];return ve->get((char *)p+ve->delta,id);}
static inline int ToInt(float f){return (int)f;}
static inline void Identity(M4 *m){func_002333B4(m,0,sizeof(*m));m->r[3].f[3]=1.0f;m->r[2].f[2]=1.0f;m->r[1].f[1]=1.0f;m->r[0].f[0]=1.0f;}
static inline void ClearFlag(S *p){int f=p->flags;if((f^1)&1)*(int *)f&=~0x20;else p->flags=f&~0x20;}
extern "C" void LoadSoXfsFromBrx(S *p,void *stream){
 int i=0;
 p->count=func_00117D50(stream);
 REQ request;request.size=p->count*sizeof(E);request.kind=16;
 p->records=(E *)func_001AE2A0(&request);
 for(;i<p->count;i++){
  E *e=&p->records[i];
  e->reference=Lookup(p,func_00117D50(stream));
  e->kind=func_00117D50(stream);
  e->tag=func_00117DB8(stream);
  int flags=func_00117BD0(stream);
  if(flags&1){e->lo=ToInt(func_00117E38(stream)*1000.0f);e->hi=1000;}
  else if(flags&2){float thousand=1000.0f;e->lo=ToInt(func_00117E38(stream)*thousand);e->hi=ToInt(func_00117E38(stream)*thousand);}
  else{e->lo=1000;e->hi=1000;}
  J1 *j1;J2 *j2;J4 *j4;J9 *j9;
  int has1=flags&4;
  if(has1){j1=CreateJ1();j1->h.id=func_00117D00(stream);}
  int has2=flags&8;
  if(has2){j2=CreateJ2();j2->h.id=func_00117D00(stream);j2->g=func_00117E38(stream);func_00117F90(stream,&j2->begin);func_00117FB0(stream,&j2->timingBegin);func_00117F90(stream,&j2->end);func_00117FB0(stream,&j2->timingEnd);j2->a=func_00117D00(stream);j2->b=func_00117D00(stream);j2->c=func_00117DB8(stream);}
  int has4=flags&16;
  if(has4){j4=CreateJ4();j4->h.id=func_00117D00(stream);j4->lo=func_00117E38(stream);j4->hi=func_00117E38(stream);}
  flags&=32;
  if(flags){j9=CreateJ9();func_00117EB0(stream,&j9->value);}
  switch(e->kind){
  case 0:case 13:case 15:func_00117F18(stream,&e->matrix);break;
  case 8:case 14:{Identity(&e->matrix);e->origin=Lookup(p,func_00117D50(stream));e->matrix.r[3].f[0]=-e->origin->position[0];e->matrix.r[3].f[1]=-e->origin->position[1];e->matrix.r[3].f[2]=-e->origin->position[2];break;}
  case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 9:case 10:case 11:case 12:default:
   func_00117F18(stream,&e->matrix);e->current=e->previous=func_0022CA18(e,e);break;
  }
  func_0022C818(p,e);
  if(has1){j1->h.owner=p;j1->h.entry=e;j1->h.order=-1;j1->initial=func_0022CA18(e,e);AddXfAct(p,e,&j1->h);}
  if(has2){j2->h.owner=p;j2->h.entry=e;j2->h.order=j2->b;j2->initial=func_0022CA18(e,e);AddXfAct(p,e,&j2->h);}
  if(has4){j4->h.owner=p;j4->h.entry=e;j4->h.order=-1;AddXfAct(p,e,&j4->h);}
  if(flags){j9->h.owner=p;j9->h.entry=e;j9->h.order=-1;AddXfAct(p,e,&j9->h);}
 }
 UpdateSoXfWorldMatrices(p);
 LoadSoIkFromBrx(p,stream);
 func_001603B8(p,stream);
 func_0021C888(p,stream);
 p->a=Lookup(p,func_00117D50(stream));p->b=Lookup(p,func_00117D50(stream));p->c=Lookup(p,func_00117D50(stream));
 if(p->count>0)ClearFlag(p);
}
