// g_clock (D_002DDED0) must be a struct: a scalar float global is assumed not to alias the struct-field stores, which lets gcc CSE the clock reloads.
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
struct VT{short delta,pad;void*fn;};
struct LO{char pad[0xC];struct VT*vt;char pad10[0xF54];void*xF64;};
struct SELECTOR{int kind;char pad4[0x10];int used;float ready;};
struct ENTRY{struct LO*object;float time,wait;struct SELECTOR*selector;int status;char pad14[12];qword point;};
struct EVENT{float time;char pad4[8];struct LO*tag;char pad10[0x10];struct EVENT*next;char pad24[4];int kind;struct LO*previous;};
struct P{char pad[0xC];struct VT*vt;char pad10[0x48];int count,capacity;char pad60[0x10];qword last;char pad80[0x28];float next;int mode;};
struct M3{qword r[3];};
struct LINK{struct LO**pp;};
struct CRef{int tag;void*value;};
extern char D_003DD490[];
struct CLOCK{float t,dt;};extern struct CLOCK D_002DDED0;extern float D_002BE5CC,D_002BE5D0,D_002BE5D4,D_002BE5D8;
void func_001A5E28(struct P*);
qword func_00173A18(struct P*);
void func_00173D18(struct P*,qword);
struct LO*func_001C4840(void*);
struct EVENT*func_001F8240(struct LO*);
struct ENTRY*func_00173E78(struct P*,struct LO*);
struct SELECTOR*func_00173F18(struct P*,qword,qword*,struct M3*,int*);
void func_00173928(struct P*,struct ENTRY*,int,int);
struct LO*func_00174490(struct P*,struct SELECTOR*,qword*,struct M3*,int);
void func_001F7D10(struct LO*,float);
void func_001F7C68(struct LO*,float);
void func_00154038(void*,struct LO*,int);
static inline float DistanceXY(qword a,qword b){qword d;float f;__asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));__asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y":"+j"(d));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
/** @sly2-annot addr=0x173A68
 * Purpose: Periodically spawns a guard at a concealed spawn point when a slot is free (gated by the player's
 *   last alert event), aims it and sends message 0x19
 * Info: proposed name UpdateCid119 (T2); class CID119 slot16; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, vtable)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r3b_rest+verdicts_tu_r3r5
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_00173A68(struct P*p){
 U point;
 struct M3 rotation;
 int flag;
 struct LO*object;
 struct LINK link;
 qword pos;
 struct EVENT*event;
 struct ENTRY*entry;
 struct SELECTOR*selector;
 struct LO*context;
 func_001A5E28(p);
 pos=func_00173A18(p);
 func_00173D18(p,pos);
 if(D_002DDED0.t<p->next)return;
 p->next=D_002DDED0.t+D_002BE5D0;
 if(p->count==p->capacity){int close=DistanceXY(pos,p->last)<D_002BE5D4;if(close)return;}
 event=0;context=func_001C4840(D_003DD490);
 {
 if(context){
  event=func_001F8240(context);
  if(event){
   if(!event->kind)event=0;
   else switch(p->mode){
   case 0:return;
   case 1:if(D_002DDED0.t-event->time>D_002BE5CC)return;break;
   case 2:break;
   }
  }
 }
 }
 entry=func_00173E78(p,event&&event->kind==1?event->previous:0);
 if(!entry)return;
 selector=func_00173F18(p,pos,&point.q,&rotation,&flag);
 if(!selector)return;
 p->last=pos;
 if(entry->object)func_00173928(p,entry,0,1);
 object=func_00174490(p,selector,&point.q,&rotation,flag);
 if(!object)return;
 func_001F7D10(object,0.01f);
 func_001F7C68(object,0.5f);
 if(event)func_00154038(object->xF64,event->tag,2);
 entry->object=object;
 entry->time=D_002DDED0.t;
 entry->selector=selector;
 p->count++;
 selector->used=1;
 link.pp=&object;
 p->next=D_002DDED0.t+D_002BE5D8;
 ((void(*)(void*,int,int,int,void*const*,struct CRef*))p->vt[3].fn)((char*)p+p->vt[3].delta,0x19,0,1,(void*const*)&link,0);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_VT[(sizeof(struct VT)==8)?1:-1];
typedef char layout_P[(sizeof(struct P)==0xB0)?1:-1];
typedef char layout_LO[(sizeof(struct LO)==0xF68)?1:-1];
typedef char layout_SELECTOR[(sizeof(struct SELECTOR)==0x1C)?1:-1];
typedef char layout_ENTRY[(sizeof(struct ENTRY)==0x30)?1:-1];
typedef char layout_EVENT[(sizeof(struct EVENT)==0x30)?1:-1];
typedef char layout_M3[(sizeof(struct M3)==0x30)?1:-1];
typedef char layout_LINK[(sizeof(struct LINK)==4)?1:-1];
typedef char layout_CRef[(sizeof(struct CRef)==8)?1:-1];
typedef char layout_VT_delta[((unsigned)&((struct VT*)0)->delta==0x0)?1:-1];
typedef char layout_VT_fn[((unsigned)&((struct VT*)0)->fn==0x4)?1:-1];
typedef char layout_P_vt[((unsigned)&((struct P*)0)->vt==0xc)?1:-1];
typedef char layout_P_count[((unsigned)&((struct P*)0)->count==0x58)?1:-1];
typedef char layout_P_capacity[((unsigned)&((struct P*)0)->capacity==0x5c)?1:-1];
typedef char layout_P_last[((unsigned)&((struct P*)0)->last==0x70)?1:-1];
typedef char layout_P_next[((unsigned)&((struct P*)0)->next==0xa8)?1:-1];
typedef char layout_P_mode[((unsigned)&((struct P*)0)->mode==0xac)?1:-1];
typedef char layout_LO_vt[((unsigned)&((struct LO*)0)->vt==0xc)?1:-1];
typedef char layout_LO_xF64[((unsigned)&((struct LO*)0)->xF64==0xf64)?1:-1];
typedef char layout_SELECTOR_kind[((unsigned)&((struct SELECTOR*)0)->kind==0x0)?1:-1];
typedef char layout_SELECTOR_used[((unsigned)&((struct SELECTOR*)0)->used==0x14)?1:-1];
typedef char layout_SELECTOR_ready[((unsigned)&((struct SELECTOR*)0)->ready==0x18)?1:-1];
typedef char layout_ENTRY_object[((unsigned)&((struct ENTRY*)0)->object==0x0)?1:-1];
typedef char layout_ENTRY_time[((unsigned)&((struct ENTRY*)0)->time==0x4)?1:-1];
typedef char layout_ENTRY_wait[((unsigned)&((struct ENTRY*)0)->wait==0x8)?1:-1];
typedef char layout_ENTRY_selector[((unsigned)&((struct ENTRY*)0)->selector==0xc)?1:-1];
typedef char layout_ENTRY_status[((unsigned)&((struct ENTRY*)0)->status==0x10)?1:-1];
typedef char layout_ENTRY_point[((unsigned)&((struct ENTRY*)0)->point==0x20)?1:-1];
typedef char layout_EVENT_time[((unsigned)&((struct EVENT*)0)->time==0x0)?1:-1];
typedef char layout_EVENT_tag[((unsigned)&((struct EVENT*)0)->tag==0xc)?1:-1];
typedef char layout_EVENT_next[((unsigned)&((struct EVENT*)0)->next==0x20)?1:-1];
typedef char layout_EVENT_kind[((unsigned)&((struct EVENT*)0)->kind==0x28)?1:-1];
typedef char layout_EVENT_previous[((unsigned)&((struct EVENT*)0)->previous==0x2c)?1:-1];
typedef char layout_LINK_pp[((unsigned)&((struct LINK*)0)->pp==0x0)?1:-1];
typedef char layout_CRef_tag[((unsigned)&((struct CRef*)0)->tag==0x0)?1:-1];
typedef char layout_CRef_value[((unsigned)&((struct CRef*)0)->value==0x4)?1:-1];
