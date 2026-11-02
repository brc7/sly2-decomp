/* (Claude agent) func_001C84D8: VU nearest-of-9 selection state machine. The copy of p->index into a register
   variable (a3) reproduces the target's daddu scheduling; register pins as in other VU files. */
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct POINT {char pad[0x10];float x,y;};
struct TIMER {char pad[0xA8];float end;};
struct SGG;struct ALARM;
struct S {char pad[0x150];struct POINT*input;char pad154[0x188-0x154];int state;int pad18C,pad190;int index,step;SGG*alarms;void*pending;char pad1A4[0x1F0-0x1A4];int target;char pad1F4[0x204-0x1F4];struct TIMER*timer;};
extern float D_002C0FE8,D_002A7798;
extern qword D_002C0FF0[9];
extern struct CLOCK {float t,dt;} D_002DDED0;
extern "C" int GrfConsumePressed__5POCTLUif(void*,int,float);
extern "C" void func_001E0020(SGG*,ALARM*);
extern "C" void func_001C8918(struct S*,int);
extern "C" int FAsegaRetracted(void*);
static inline qword Pack(float x,float y,float z){register qword result __asm__("$3");register unsigned word __asm__("$2");__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(result),"=&r"(word):"f"(x),"f"(y),"f"(z));return result;}
static inline qword Sub(qword a,qword b){qword d;__asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));return d;}
static inline float Len(qword a){register qword d __asm__("vf1");register qword one __asm__("vf2");float f;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));__asm__("mtc1 %1,%0":"=f"(f):"r"(d));return f;}
static inline float ClockTime(){float t;__asm__("lwc1 %0,%1":"=f"(t):"m"(D_002DDED0.t));return t;}
static inline char*MinimumPage(){register char*page __asm__("$6");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002A7798));return page;}
static inline char*SelectionTablePage(){register char*page __asm__("$5");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(D_002C0FF0));return page;}
static inline qword*TableWithMinimum(char*minimumPage,char*tablePage,float*out){register float minimum __asm__("$f1");register qword*table __asm__("$5");__asm__ volatile("lwc1 %0,%%lo(%4)(%2)\n\taddiu %1,%3,%%lo(%5)":"=f"(minimum),"=r"(table):"r"(minimumPage),"r"(tablePage),"i"(&D_002A7798),"i"(D_002C0FF0),"m"(*(const float*)(minimumPage+0x7798)));*out=minimum;return table;}
/** @sly2-annot addr=0x1C84D8
 * Purpose: read POCTL presses (GrfConsumePressed) to dial the safe combination; EnsureSggAlarm
 * Info: proposed name UpdateVaultInput (T2); TU prize
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
extern "C" void func_001C84D8(struct S*p){
 union U point;
 int old;
 int divisor=1000;
 do{
  old=p->state;
  if(GrfConsumePressed__5POCTLUif(p->input,0x30,0)){
   func_001E0020(p->alarms,(ALARM*)0x655);
   func_001C8918(p,3);
  }
  switch(p->state){
  case 0:
   if(GrfConsumePressed__5POCTLUif(p->input,0xC0,0)){func_001C8918(p,2);break;}
   {
    char*minimumPage=MinimumPage();char*tablePage=SelectionTablePage();int current=p->index,best;register int comparison __asm__("$7")=current;
    unsigned i;qword*table;
    float bias,smallest;
    qword pt=Pack(p->input->x,p->input->y,0),vp;bias=D_002C0FE8;__asm__("qmtc2.ni %2,%0":"=j"(vp),"=m"(point.q):"r"(pt));point.q=pt;table=TableWithMinimum(minimumPage,tablePage,&smallest);best=-1;
    for(i=0;i<9;i++){
     float distance=Len(Sub(vp,table[i]));
     if(i==comparison)distance*=bias;
     if(distance<smallest){smallest=distance;best=i;}
    }
    if(best!=current){p->index=best;func_001C8918(p,1);}
   }
   break;
  case 1:
   if(p->timer->end<=ClockTime())func_001C8918(p,0);
   break;
  case 2:
   if(!p->pending||FAsegaRetracted(p->pending)){
    int next=(p->step*10)%divisor;next++;next+=p->index;
    p->step=next;
    if(next==p->target){func_001E0020(p->alarms,(ALARM*)0x655);func_001C8918(p,3);}
    else func_001C8918(p,0);
   }
   break;
  }
 }while(p->state!=old);
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char selection_layout[(sizeof(qword)==16 && sizeof(U)==16 && sizeof(float)==4 && sizeof(void*)==4 && OFF(POINT,x)==0x10 && OFF(POINT,y)==0x14 && OFF(TIMER,end)==0xA8 && OFF(S,input)==0x150 && OFF(S,state)==0x188 && OFF(S,index)==0x194 && OFF(S,step)==0x198 && OFF(S,alarms)==0x19C && OFF(S,pending)==0x1A0 && OFF(S,target)==0x1F0 && OFF(S,timer)==0x204 && OFF(CLOCK,t)==0 && OFF(CLOCK,dt)==4 && sizeof(D_002C0FF0)==0x90)?1:-1];
