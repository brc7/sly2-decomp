typedef struct O O;
typedef struct PAIR PAIR;
typedef struct EVENT EVENT;
typedef struct VE VE;
typedef struct SOURCE SOURCE;
typedef struct COLL COLL;
typedef struct PHYS PHYS;
typedef struct META META;
typedef struct LINK LINK;
typedef struct STATE STATE;
typedef struct N N;
typedef struct LSG LSG;
typedef struct REQ REQ;
typedef struct MARK MARK;
typedef struct WORLD WORLD;
typedef struct WORK WORK;
typedef union V V;
typedef union SCRATCH SCRATCH;
// Track collider membership and emit contact and impact events.
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
union V {qword q;float f[4];};
struct O;
struct PAIR {O *owner,*object;};
struct EVENT {int kind,pad4;float strength;int padC;qword normal;V position;char pad30[0x10];};
struct VE {short delta,pad;union {qword (*velocity)(void *);void (*signal)(void *,int,PAIR *);void (*event)(void *,EVENT *);} f;};
struct SOURCE {char pad[0x70];V position;};
struct COLL {SOURCE *source;char pad4[0xC];ulong flags;char pad18[0x38];V position;char pad60[0x50];};
struct PHYS {char pad[0xD4];float scale;};
struct META {char pad[0x98];short kind;};
struct LINK {int pad0;LINK *next;int pad8;O *object;};
struct STATE {V minimum,maximum;char pad20[0x44];LINK *first;char pad68[8];ulong flags;};
struct N {O *owner,*object;int pad8;N *next;};
struct O {char pad[0xC];VE *vt;char pad10[0x48];META *body;char pad5C[0x84];int count;COLL *colliders;PHYS *physics;STATE *state;char padF0[0x58];long flags;N *first;char pad154[0x1C];O *recipient;char pad174[0xD4];float height;};
struct LSG {V first;char pad10[0x50];};
struct REQ {int size,alignment;};
struct MARK;
struct WORLD;
extern "C" {
extern WORLD D_003DD490;
extern qword D_002A2DC0,D_002A2DD0;

void UpdateSo(O *);
MARK *STACKSCOPE_ctor(MARK *);
void STACKSCOPE_ctor_1AE4A0(MARK *,int);
void *func_001AE318(const REQ *);
int func_00229D60(void *,COLL *);
int func_001F7E08(O *,int (*)(void *,COLL *),void *,COLL **);
int func_001F6838(O *);
int func_00230380(int);
float func_00229468(O *,O *,qword *,qword *);
N *func_002296A0(WORLD *);
int FIsBasicDerivedFrom__FP5BASIC3CID(O *,int);
int ClsgClipEdgeToObjects(qword,qword,int,COLL **,int,LSG *);
void *PsndStartSound(int);
void SetSndSourceLo(void *,O *);
void SetSndRange(void *,float,float);
void func_001FB1F8(void *,float);
void *func_002333B4(void *,int,int);
void func_00228D20(O *,N **);
}
struct MARK {};
union SCRATCH {REQ request;PAIR pair;V velocity;};
struct WORK {MARK mark;SCRATCH scratch;V start,end;LSG intervals[16];V origin;EVENT event;int count;N **head;REQ *request;};
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"0"(a),"j"(b));return r;}
static inline REQ*Fields(REQ*p){register REQ*copy __asm__("$3");__asm__("daddu %0,%1,$zero":"=r"(copy):"r"(p));return copy;}
static inline float Min(float a,float b){return b<a?b:a;}
static inline qword Velocity(O *o){VE *v=o->vt;return v[33].f.velocity((char *)o+v[33].delta);}
static inline void Impact(O *o,EVENT *e){VE *v=o->vt;v[51].f.event((char *)o+v[51].delta,e);}
static inline void Signal(O *o,int event,PAIR *pair){VE *v=o->vt;v[4].f.signal((char *)o+v[4].delta,event,pair);}
/** @sly2-annot addr=0x228E50
 * Purpose: tracks objects inside the water, emits contact/impact (splash) events
 * Info: proposed name UpdateWater (T1); class WATER slot16; TU water
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
extern "C" void UpdateWater(O *p){
 WORK work;N *n;COLL **objects;LINK *link;
 UpdateSo(p);
 if(!p->count)return;
 work.request=&work.scratch.request;work.head=&p->first;
 for(n=*work.head;n;n=n->next)n->object->flags|=0x80000000L;
 STACKSCOPE_ctor(&work.mark);
 {register int size __asm__("$3")=p->count;register int alignment __asm__("$2")=16;register REQ*req __asm__("$4")=work.request;size*=4;work.scratch.request.size=size;Fields(req)->alignment=alignment;
 objects=(COLL **)func_001AE318(req);}
 work.count=func_001F7E08(p,func_00229D60,0,objects);
 for(link=p->state->first;link;link=link->next){
  O *o=link->object;
  if(!o->physics||o->physics->scale<1e-4f||func_001F6838(o)||!func_00230380(o->body->kind))continue;
  if(func_00229468(p,o,0,0)==0.0f)continue;
  o->flags|=0x100000000L;
  if(o->flags&0x80000000L)continue;
  {register WORLD*world __asm__("$4")=&D_003DD490;n=func_002296A0(world);}
  n->owner=p;n->object=o;n->next=p->first;p->first=n;
  if(!p->recipient)continue;
  work.scratch.velocity.q=Velocity(o);
  if(work.scratch.velocity.f[2]<-100.0f){
  {int i;for(i=0;i<o->count;i++){
   COLL *c=&o->colliders[i];float dz;
   if(c->flags&0x5000000000000L)continue;
   work.origin.q=D_002A2DD0;
   work.start.q=Sub(c->position.q,work.origin.q);
   work.end.q=Sub(c->source->position.q,work.origin.q);
   if(FIsBasicDerivedFrom__FP5BASIC3CID(o,0x33))dz=o->height;
   else dz=o->state->minimum.f[2]-c->source->position.f[2];
   work.start.f[2]+=dz;work.end.f[2]+=dz;
   if(ClsgClipEdgeToObjects(work.start.q,work.end.q,work.count,objects,16,work.intervals)>0){
    qword normal;
    float fall=Min(-work.scratch.velocity.f[2],600.0f)*(1.0f/600.0f);
    float radius=((o->state->maximum.f[0]-o->state->minimum.f[0])+(o->state->maximum.f[1]-o->state->minimum.f[1]))*0.5f;
    void *effect=PsndStartSound(0x13);
    SetSndSourceLo(effect,o);
    SetSndRange(effect,100.0f,1000.0f);
    func_001FB1F8(effect,fall);
    func_002333B4(&work.event,0,sizeof(work.event));
    normal=D_002A2DC0;
    work.event.position.q=Add(work.intervals[0].first.q,work.origin.q);
    work.event.kind=14;work.event.strength=radius*fall;work.event.normal=normal;
    Impact(p->recipient,&work.event);
   }
  }
  }
  }
 }
 for(n=p->first;n;n=n->next){
  O *o=n->object;
  long flags=o->flags;
  if(((int)(flags>>31)&1)!=((flags>>32)&1)){
   work.scratch.pair.owner=p;work.scratch.pair.object=o;
   Signal(o,o->flags&0x80000000L?5:4,(PAIR *)work.request);
  }
 }
 func_00228D20(p,work.head);STACKSCOPE_ctor_1AE4A0(&work.mark,2);
}
