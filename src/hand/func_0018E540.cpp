typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct VE{short delta,pad;void*fn;};
struct ASEG{char pad0[0x74];float duration;};
struct ASEGA{char pad0[0x1C];float localTime;char pad20[0x40-0x20];float elapsed;char pad44[4];float rate;};
struct EFFECT;struct ACTION;struct SM;struct COMPONENT;
struct NODE{char pad0[12];VE*vt;};
struct TARGET{char pad0[0x130];int exitEffect;};
struct POINT{char pad0[0x70];U position;};
struct BLEND{float incoming,unknown4,unknown8,outgoing,unknown10,unknown14;ASEG*clip;};
struct CLOCK{float t,dt;};
struct WORK{U average,half,first;};
struct P{
 char pad0[0x1028];
 SM * sm;
 char pad102c[0x8];
 COMPONENT * component;
 char pad1038[0x49c];
 ASEG * alternate10;
 ASEG * alternate11;
 char pad14dc[0x2f4];
 char track[0x30];
 ASEG * idle;
 char pad1804[0x7c];
 int reset;
 char pad1884[0x3c];
 int surface;
 TARGET * target;
 int mode;
 char pad18cc[0xc];
 int state;
 float stateTime;
 ASEGA * current;
 int finished;
 char pad18e8[0x4];
 int attack;
 char pad18f0[0xb0];
 NODE * normalNode;
 void * pointObject;
 NODE * alternateNode;
 char pad19ac[0x4];
 float deadline;
 char pad19b4[0xc];
 POINT * point;
 char pad19c4[0x40];
 BLEND blend4;
 BLEND blend5;
 BLEND blend6;
 BLEND blend7;
 ASEG * clip8;
 ASEG * clip9;
 char pad1a7c[0xc];
 ACTION * action;
 char pad1a8c[0x254];
 ASEG * clip12;
 ASEG * clip13;
 ASEG * clip14;
 ASEG * clip15;
 ASEG * clip16;
 ASEG * clip17;
 ASEG * clip18;
 ASEG * clip19;
 ASEG * clip20;
 ASEG * clip21;
 char pad1d08[0xc];
 ASEG * clip22;
 ASEG * clip23;
};
extern "C"{
extern CLOCK D_002DDED0;extern int D_002BF448[];extern float D_002BF430,D_002BF434,D_002BF438,D_002BF43C,D_002BF440;
extern void*D_002E1E44;extern void*D_002E1E48;
void func_001F2AD0(P*,ACTION*);void func_001F4BF8(P*);void func_001CCAC0(void*,int);
EFFECT*func_001FBB90(int);void func_001FB2F8(EFFECT*,void*);void func_001FB3F0(EFFECT*,qword);
qword func_001A62E8(void*);qword func_001A6F00(void*,void*,qword);
void func_0018F560(P*,BLEND*,ASEG*);int func_001F0400(SM*,int);void func_001F09D8(COMPONENT*,int);
void func_001305B8(ASEG*,P*,float,float,int,ASEGA**);void func_00131A90(ASEGA*,float);
}
static __inline__ void Emit(P*p,int id){if(id!=-1){EFFECT*effect=func_001FBB90(id);func_001FB2F8(effect,0);func_001FB3F0(effect,func_001A62E8(p));}}
static __inline__ void Active(NODE*p){VE&v=p->vt[8];((void(*)(void*))v.fn)((char*)p+v.delta);}
static __inline__ void Link(NODE*p,NODE*other){VE&v=p->vt[38];((void(*)(void*,NODE*))v.fn)((char*)p+v.delta,other);}
static __inline__ void Inactive(NODE*p){VE&v=p->vt[9];((void(*)(void*))v.fn)((char*)p+v.delta);}
/** @sly2-annot addr=0x18E540
 * Purpose: Set Sly's cane state (x18D8): exit old (remove sweep effect unless continuing, stop reach track),
 *   state 3 resolves to fore/back sweep 4/5 or 6/7, enter new (blend clip, contact deadline, sounds),
 *   continue ASEGA if transition table D_002BF448 says so
 * Info: proposed name SetJtJtcs (T1); TU cane
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus, docs/engine/player.md#10-source-files,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
extern "C" void func_0018E540(P*p,int state){
 if(p->state==state)return;
 int compatible=0;
 if(state!=-1&&p->state!=-1)compatible=state==D_002BF448[p->state];
 if(!compatible)func_001F2AD0(p,p->action);
 int exitEffect=-1;
 switch(p->state){case 1:exitEffect=p->target->exitEffect;func_001F4BF8(p);break;case 2:func_001F4BF8(p);break;case 0:func_001CCAC0(p->track,-1);break;case -1:break;}
 Emit(p,exitEffect);
 register float start __asm__("$f22")=0.0f;
 ASEG*clip=0;
 register float rate __asm__("$f21")=1.0f,stopDelay __asm__("$f20")=0.0f;
 int enterEffect=-1;register float sharedDeadline __asm__("$f0"),sharedDelay __asm__("$f1");register int sharedMode __asm__("$2");register char*clockHigh __asm__("$3");
 switch(state){
 case -1:p->surface=0;break;
 case 1:case 2:func_001F4BF8(p);break;
 case 10:func_001F4BF8(p);clip=p->alternate10;break;
 case 11:func_001F4BF8(p);clip=p->alternate11;break;
 case 0:{func_001CCAC0(p->track,0);clip=p->idle;rate=0.0f;
  register P*receiver __asm__("$4");register float half __asm__("$f1"),duration __asm__("$f0");
  __asm__("daddu %0,%3,$zero\n\t.set noat\n\tlui $1,0x3f00\n\tmtc1 $1,%1\n\t.set at\n\taddiu %2,$zero,15":"=r"(receiver),"=f"(half),"=r"(enterEffect):"r"(p):"$1");
  __asm__("lwc1 %0,%1":"=f"(duration):"m"(clip->duration));start=duration*half;
  func_001F4BF8(receiver);sharedDelay=0.1f;sharedDeadline=D_002DDED0.t;goto deadline_add;}

 case 3:{
  WORK work;
  register qword first __asm__("$2")=func_001A6F00(0,p,func_001A62E8(p->pointObject));
  register POINT*point __asm__("$3");register void*world __asm__("$4");register P*receiver __asm__("$5");
  __asm__("lw %0,%4\n\tdaddu %1,$zero,$zero\n\tsq %5,%3\n\tdaddu %2,%6,$zero":"=r"(point),"=r"(world),"=r"(receiver),"=m"(work.first.q):"m"(p->point),"r"(first),"r"(p));
  register qword second __asm__("$2")=func_001A6F00(world,receiver,point->position.q);
  register qword average __asm__("vf2");register int mode __asm__("$3");
  __asm__(".set noat\n\tlui $1,0x3f00\n\tmtc1 $1,$f0\n\t.set at\n\tqmtc2.ni %3,$vf4\n\tlqc2 $vf3,%4\n\tmfc1 $3,$f0\n\tqmtc2.ni $3,$vf1\n\tlw %1,%5\n\tvmulax.xyzw ACC,$vf3,$vf1x\n\tvmaddx.xyzw %0,$vf4,$vf1x\n\tsqc2 $vf1,%2":"=j"(average),"=r"(mode),"=m"(work.half.q):"r"(second),"m"(work.first.q),"m"(p->mode):"$1","$f0","vf1","vf3","vf4");
  work.average.q=average;
  if(!mode){
   if(D_002E1E44){state=6;goto enter6;}
   if(D_002E1E48){state=7;goto enter7;}
   if(work.average.f[1]<0.0f){state=6;goto enter6;}
   state=7;goto enter7;
  }
  if(D_002E1E44){state=4;goto enter4;}
  if(D_002E1E48){state=5;goto enter5;}
  if(work.average.f[1]<0.0f){state=4;goto enter4;}
  state=5;goto enter5;
 }
 case 4:enter4:{func_0018F560(p,&p->blend4,p->blend4.clip);enterEffect=11;
  register float deadline __asm__("$f0"),delay __asm__("$f1");
  __asm__ __volatile__("lui $4,%%hi(%4)\n\tlui $2,%%hi(%3)\n\tlwc1 %1,%%lo(%3)($2)\n\tlui $3,%%hi(%5)\n\tlwc1 %0,%%lo(%4)($4)\n\tlwc1 %2,%%lo(%5)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay):"i"(&D_002BF434),"i"(&D_002DDED0),"i"(&D_002BF430):"$2","$3","$4","memory");
  __asm__("add.s %0,%0,%2\n\tlw %1,%3":"+f"(deadline),"=r"(clip):"f"(delay),"m"(p->blend4.clip));
  p->deadline=deadline;break;}
 case 5:enter5:{func_0018F560(p,&p->blend5,p->blend5.clip);enterEffect=11;
  register float deadline __asm__("$f0"),delay __asm__("$f1");
  __asm__ __volatile__("lui $4,%%hi(%4)\n\tlui $2,%%hi(%3)\n\tlwc1 %1,%%lo(%3)($2)\n\tlui $3,%%hi(%5)\n\tlwc1 %0,%%lo(%4)($4)\n\tlwc1 %2,%%lo(%5)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay):"i"(&D_002BF438),"i"(&D_002DDED0),"i"(&D_002BF430):"$2","$3","$4","memory");
  __asm__("add.s %0,%0,%2\n\tlw %1,%3":"+f"(deadline),"=r"(clip):"f"(delay),"m"(p->blend5.clip));
  p->deadline=deadline;break;}
 case 6:enter6:{func_0018F560(p,&p->blend6,p->blend6.clip);enterEffect=11;stopDelay=D_002BF430;clip=p->blend6.clip;
  int found=func_001F0400(p->sm,0x17B);
  /* Native V1 is rebuilt after the cancellation callback. */
  clockHigh=(char*)0x002e0000;
  if(found>=0){func_001F09D8(p->component,0x17B);clockHigh=(char*)0x002e0000;}
  sharedDelay=D_002BF434;
  goto deadline_clock_read;
 }
 case 7:enter7:{func_0018F560(p,&p->blend7,p->blend7.clip);enterEffect=11;stopDelay=D_002BF430;clip=p->blend7.clip;
  int found=func_001F0400(p->sm,0x17B);
  /* Native V1 is rebuilt after the cancellation callback. */
  clockHigh=(char*)0x002e0000;
  if(found>=0){func_001F09D8(p->component,0x17B);clockHigh=(char*)0x002e0000;}
  sharedDelay=D_002BF438;
 }
 deadline_clock_read:
 __asm__ __volatile__("lwc1 %0,%%lo(%2)(%1)":"=f"(sharedDeadline):"r"(clockHigh),"i"(&D_002DDED0):"memory");
 deadline_add:p->deadline=sharedDeadline+sharedDelay;break;
 case 8:clip=p->clip8;break;
 case 9:clip=p->clip9;break;
 case 12:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$5");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$2","$3","$4","memory");
  sharedDeadline=deadline;
  __asm__("add.s %0,%0,%3\n\tlw %1,%4\n\tsw %5,%2":"+f"(sharedDeadline),"=r"(sharedMode),"=m"(p->attack):"f"(delay),"m"(p->mode),"r"(attack):"memory");
  clip=p->clip12;
  goto attack_ready;
 }
 case 13:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$5");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$2","$3","$4","memory");
  sharedDeadline=deadline;
  __asm__("add.s %0,%0,%3\n\tlw %1,%4\n\tsw %5,%2":"+f"(sharedDeadline),"=r"(sharedMode),"=m"(p->attack):"f"(delay),"m"(p->mode),"r"(attack):"memory");
  clip=p->clip13;
 }
 attack_ready:p->deadline=sharedDeadline;
 if(!sharedMode&&func_001F0400(p->sm,0x17B)>=0)func_001F09D8(p->component,0x17B);break;
 case 14:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip14):"memory");
  p->deadline=deadline;
  break;}
 case 15:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip15):"memory");
  p->deadline=deadline;
  break;}
 case 16:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip16):"memory");
  p->deadline=deadline;
  break;}
 case 17:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip17):"memory");
  p->deadline=deadline;
  break;}
 case 18:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip18):"memory");
  p->deadline=deadline;
  break;}
 case 19:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip19):"memory");
  p->deadline=deadline;
  break;}
 case 20:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip20):"memory");
  p->deadline=deadline;
  break;}
 case 21:{
  register float deadline __asm__("$f0"),delay __asm__("$f1");register int attack __asm__("$2");
  __asm__ __volatile__("lui $2,%%hi(%4)\n\tlui $4,%%hi(%5)\n\tlwc1 %1,%%lo(%4)($2)\n\tlui $3,%%hi(%6)\n\tlwc1 %0,%%lo(%5)($4)\n\taddiu %3,$zero,1\n\tlwc1 %2,%%lo(%6)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay),"=r"(attack):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$3","$4","memory");
  __asm__("add.s %0,%0,%3\n\tsw %4,%2\n\tlw %1,%5":"+f"(deadline),"=r"(clip),"=m"(p->attack):"f"(delay),"r"(attack),"m"(p->clip21):"memory");
  p->deadline=deadline;
  break;}
 case 22:case 23:{clip=state==22?p->clip22:p->clip23;
  register float deadline __asm__("$f0"),delay __asm__("$f1");
  __asm__ __volatile__("lui $2,%%hi(%3)\n\tlui $4,%%hi(%4)\n\tlwc1 %1,%%lo(%3)($2)\n\tlui $3,%%hi(%5)\n\tlwc1 %0,%%lo(%4)($4)\n\tlwc1 %2,%%lo(%5)($3)":"=f"(deadline),"=f"(delay),"=f"(stopDelay):"i"(&D_002BF440),"i"(&D_002DDED0),"i"(&D_002BF43C):"$2","$3","$4","memory");
  register int mode __asm__("$2");
  __asm__("add.s %0,%0,%3\n\tlw %1,%4\n\tsw $zero,%2":"+f"(deadline),"=r"(mode),"=m"(p->attack):"f"(delay),"m"(p->mode):"memory");
  p->deadline=deadline;
  if(!mode&&func_001F0400(p->sm,0x17B)>=0)func_001F09D8(p->component,0x17B);
  break;}
 }
 if(compatible){register ASEGA*current __asm__("$2")=p->current;enterEffect=-1;
  __asm__("lwc1 $f1,%2\n\tlwc1 $f2,%3\n\tlwc1 $f0,%4\n\tlwc1 %0,%5\n\tdiv.s %0,%0,$f2\n\tmin.s %1,$f0,$f1":"=f"(rate),"=f"(start):"m"(clip->duration),"m"(current->rate),"m"(current->localTime),"m"(current->elapsed):"$f0","$f1","$f2");
 }
 else p->finished=0;
 register int previousState __asm__("$3");
 __asm__("lw %0,%1":"=r"(previousState):"m"(p->state));
 int wasAlternate=previousState==11||previousState==2;
 int nowAlternate=state==11||state==2;
 if(wasAlternate!=nowAlternate){
  if(state==11||state==2){Active(p->alternateNode);Link(p->alternateNode,p->normalNode);Inactive(p->normalNode);}
  else{Active(p->normalNode);Link(p->normalNode,p->alternateNode);Inactive(p->alternateNode);}
 }
 p->state=state;
 register ASEG*clipArgument __asm__("$4");register float startArgument __asm__("$f12"),rateArgument __asm__("$f13"),now __asm__("$f0");
 __asm__ __volatile__("lui $2,%%hi(%4)\n\tdaddu %0,%5,$zero\n\tmov.s %1,%6\n\tlwc1 %3,%%lo(%4)($2)\n\tmov.s %2,%7":"=r"(clipArgument),"=f"(startArgument),"=f"(rateArgument),"=f"(now):"i"(&D_002DDED0),"r"(clip),"f"(start),"f"(rate):"$2","memory");
 p->reset=0;p->stateTime=now;
 func_001305B8(clipArgument,p,startArgument,rateArgument,1,&p->current);
 if(stopDelay!=0.0f){register ASEGA*current __asm__("$4");register float stopTime __asm__("$f12");
  __asm__ __volatile__("lui $3,%%hi(%2)\n\tlw %0,%3\n\tlwc1 %1,%%lo(%2)($3)":"=r"(current),"=f"(stopTime):"i"(&D_002DDED0),"m"(p->current):"$3","memory");
  func_00131A90(current,stopTime+stopDelay);
 }
 Emit(p,enterEffect);
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_VE[(sizeof(VE)==8)?1:-1];
typedef char layout_BLEND[(sizeof(BLEND)==0x1C)?1:-1];
typedef char layout_VE_delta[((unsigned long)&((VE*)0)->delta==0x0)?1:-1];
typedef char layout_VE_fn[((unsigned long)&((VE*)0)->fn==0x4)?1:-1];
typedef char layout_ASEG_duration[((unsigned long)&((ASEG*)0)->duration==0x74)?1:-1];
typedef char layout_ASEGA_localTime[((unsigned long)&((ASEGA*)0)->localTime==0x1c)?1:-1];
typedef char layout_ASEGA_elapsed[((unsigned long)&((ASEGA*)0)->elapsed==0x40)?1:-1];
typedef char layout_ASEGA_rate[((unsigned long)&((ASEGA*)0)->rate==0x48)?1:-1];
typedef char layout_NODE_vt[((unsigned long)&((NODE*)0)->vt==0xc)?1:-1];
typedef char layout_TARGET_exitEffect[((unsigned long)&((TARGET*)0)->exitEffect==0x130)?1:-1];
typedef char layout_POINT_position[((unsigned long)&((POINT*)0)->position==0x70)?1:-1];
typedef char layout_BLEND_incoming[((unsigned long)&((BLEND*)0)->incoming==0x0)?1:-1];
typedef char layout_BLEND_outgoing[((unsigned long)&((BLEND*)0)->outgoing==0xc)?1:-1];
typedef char layout_BLEND_clip[((unsigned long)&((BLEND*)0)->clip==0x18)?1:-1];
typedef char layout_P_sm[((unsigned long)&((P*)0)->sm==0x1028)?1:-1];
typedef char layout_P_component[((unsigned long)&((P*)0)->component==0x1034)?1:-1];
typedef char layout_P_alternate10[((unsigned long)&((P*)0)->alternate10==0x14d4)?1:-1];
typedef char layout_P_alternate11[((unsigned long)&((P*)0)->alternate11==0x14d8)?1:-1];
typedef char layout_P_track[((unsigned long)&((P*)0)->track==0x17d0)?1:-1];
typedef char layout_P_idle[((unsigned long)&((P*)0)->idle==0x1800)?1:-1];
typedef char layout_P_reset[((unsigned long)&((P*)0)->reset==0x1880)?1:-1];
typedef char layout_P_surface[((unsigned long)&((P*)0)->surface==0x18c0)?1:-1];
typedef char layout_P_target[((unsigned long)&((P*)0)->target==0x18c4)?1:-1];
typedef char layout_P_mode[((unsigned long)&((P*)0)->mode==0x18c8)?1:-1];
typedef char layout_P_state[((unsigned long)&((P*)0)->state==0x18d8)?1:-1];
typedef char layout_P_stateTime[((unsigned long)&((P*)0)->stateTime==0x18dc)?1:-1];
typedef char layout_P_current[((unsigned long)&((P*)0)->current==0x18e0)?1:-1];
typedef char layout_P_finished[((unsigned long)&((P*)0)->finished==0x18e4)?1:-1];
typedef char layout_P_attack[((unsigned long)&((P*)0)->attack==0x18ec)?1:-1];
typedef char layout_P_normalNode[((unsigned long)&((P*)0)->normalNode==0x19a0)?1:-1];
typedef char layout_P_pointObject[((unsigned long)&((P*)0)->pointObject==0x19a4)?1:-1];
typedef char layout_P_alternateNode[((unsigned long)&((P*)0)->alternateNode==0x19a8)?1:-1];
typedef char layout_P_deadline[((unsigned long)&((P*)0)->deadline==0x19b0)?1:-1];
typedef char layout_P_point[((unsigned long)&((P*)0)->point==0x19c0)?1:-1];
typedef char layout_P_blend4[((unsigned long)&((P*)0)->blend4==0x1a04)?1:-1];
typedef char layout_P_blend5[((unsigned long)&((P*)0)->blend5==0x1a20)?1:-1];
typedef char layout_P_blend6[((unsigned long)&((P*)0)->blend6==0x1a3c)?1:-1];
typedef char layout_P_blend7[((unsigned long)&((P*)0)->blend7==0x1a58)?1:-1];
typedef char layout_P_clip8[((unsigned long)&((P*)0)->clip8==0x1a74)?1:-1];
typedef char layout_P_clip9[((unsigned long)&((P*)0)->clip9==0x1a78)?1:-1];
typedef char layout_P_action[((unsigned long)&((P*)0)->action==0x1a88)?1:-1];
typedef char layout_P_clip22[((unsigned long)&((P*)0)->clip22==0x1d14)?1:-1];
typedef char layout_P_clip23[((unsigned long)&((P*)0)->clip23==0x1d18)?1:-1];
typedef char layout_P_clip12[((unsigned long)&((P*)0)->clip12==0x1ce0)?1:-1];
typedef char layout_P_clip13[((unsigned long)&((P*)0)->clip13==0x1ce4)?1:-1];
typedef char layout_P_clip14[((unsigned long)&((P*)0)->clip14==0x1ce8)?1:-1];
typedef char layout_P_clip15[((unsigned long)&((P*)0)->clip15==0x1cec)?1:-1];
typedef char layout_P_clip16[((unsigned long)&((P*)0)->clip16==0x1cf0)?1:-1];
typedef char layout_P_clip17[((unsigned long)&((P*)0)->clip17==0x1cf4)?1:-1];
typedef char layout_P_clip18[((unsigned long)&((P*)0)->clip18==0x1cf8)?1:-1];
typedef char layout_P_clip19[((unsigned long)&((P*)0)->clip19==0x1cfc)?1:-1];
typedef char layout_P_clip20[((unsigned long)&((P*)0)->clip20==0x1d00)?1:-1];
typedef char layout_P_clip21[((unsigned long)&((P*)0)->clip21==0x1d04)?1:-1];

typedef char layout_ASEG_prefix_size[(sizeof(ASEG)==0x78)?1:-1];
typedef char layout_ASEGA_prefix_size[(sizeof(ASEGA)==0x4C)?1:-1];
typedef char layout_NODE_prefix_size[(sizeof(NODE)==0x10)?1:-1];
typedef char layout_TARGET_prefix_size[(sizeof(TARGET)==0x134)?1:-1];
typedef char layout_POINT_prefix_size[(sizeof(POINT)==0x80)?1:-1];
typedef char layout_CLOCK_prefix_size[(sizeof(CLOCK)==8)?1:-1];
typedef char layout_P_prefix_size[(sizeof(P)==0x1D1C)?1:-1];
typedef char layout_WORK_size[(sizeof(WORK)==0x30)?1:-1];
typedef char layout_WORK_average[((unsigned long)&((WORK*)0)->average==0)?1:-1];
typedef char layout_WORK_half[((unsigned long)&((WORK*)0)->half==0x10)?1:-1];
typedef char layout_WORK_first[((unsigned long)&((WORK*)0)->first==0x20)?1:-1];
