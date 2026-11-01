typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];int i[4];};
struct M4 {U r[4];};
struct VTE {short delta,pad;void*fn;};
struct P;
struct EFFECT {char pad[0xC];VTE*vt;};
struct L {char pad[0xC];VTE*vt;char pad10[0x164];EFFECT*effect;char pad178[0xC];int kind,tag;};
struct ENTRY{P*object;float time;};
struct P {char pad[0xC];VTE*vt;char pad10[0x2CC];int state;char pad2E0[0xB14];int mode;char padDF8[0x294];L*link;ENTRY*entries;int count;float pad1098[2];float height;};
struct HIT {L*target;int pad4;P*source;int tag,kind;char pad14[0xC];U point,direction;char pad40[0x20];};
struct MSG {int type;char pad4[0x1C];U position;};
struct CLOCK {float time,dt;};
extern "C" CLOCK D_002DDED0;
extern "C" qword func_001A62E8(P*);
extern "C" void*func_002333B4(void*,int,int);
extern "C" P*func_001F7598(HIT*);
extern "C" int func_001A2788(P*,L*,qword*);
extern "C" void func_002309E8(void*,void*,M4*,M4*,float);
extern "C" M4 func_0011C168(const M4*,const M4*);
static inline float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static inline qword Direction(P*p){return ((qword(*)(void*))p->vt[33].fn)((char*)p+p->vt[33].delta);}
/** @sly2-annot addr=0x1A2460
 * Purpose: From UpdateStepplayer: if the player's (height-adjusted) point is inside the linked volume (vt73)
 *   inflict zap/drown (InflictZap) and probe surface (0x1A2788)
 * Info: proposed name UpdateStepplayerWater (T2); TU hurt
 * Notes: runtime3: water zap kind 0x2000
 * Confidence: low (draft) [T2 0.40] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
extern "C" void func_001A2460(P*p){
 U point;
 int valid;
 if(p->link){
  point.q=func_001A62E8(p);
  point.f[2]+=p->height;
  if(p->mode==3||p->mode==10)point.f[2]-=10.0f;
  L*link=p->link;
  valid=((int(*)(void*,qword))link->vt[73].fn)((char*)link+link->vt[73].delta,point.q);
 }else valid=0;
 if(!valid)return;
 if(p->link->kind){
  int notify=0;
  if(Z(Direction(p))<0.0f||(p->state==-1&&p->mode!=7))notify=1;
  if(notify){
   HIT hit;
   func_002333B4(&hit,0,sizeof(hit));
   hit.target=p->link;
   hit.source=p;
   hit.tag=p->link->tag;
   hit.kind=p->link->kind;
   hit.point.q=func_001A62E8(p);
   hit.direction.q=Direction(p);
   (void)func_001F7598(&hit);
  }
 }
 if(!p->link->effect)return;
 if(!func_001A2788(p,p->link,&point.q))return;
 int i=0;
 if(p->count>0){
 U position;
 M4 predicted,base;
 M4*pbase=&base;
 M4*ppred=&predicted;
 // Preserve the saved register for the predicted position.
 register U*predpos __asm__("$20")=&predicted.r[3];
 for(;i<p->count;i++){
  ENTRY*entry=&p->entries[i];
  if(D_002DDED0.time-entry->time<0.05f)continue;
  P*object=entry->object;
  position.q=func_001A62E8(object);
  void*frame=((void*(*)(void*,M4*))object->vt[32].fn)((char*)object+object->vt[32].delta,pbase);
  func_002309E8(frame,0,ppred,0,D_002DDED0.dt);
  predicted=func_0011C168(ppred,pbase);
  int crossed=0;
  if(point.f[2]<=position.f[2]){if(predpos->f[2]<=point.f[2])crossed=1;}
  else if(point.f[2]<=predpos->f[2])crossed=1;
  if(crossed){
   MSG msg;
   msg.position=position;
   msg.position.f[2]=point.f[2];
   msg.type=4;
   EFFECT*effect=p->link->effect;
   ((void(*)(void*,MSG*))effect->vt[51].fn)((char*)effect+effect->vt[51].delta,&msg);
   entry->time=D_002DDED0.time;
  }
 }
}
}
