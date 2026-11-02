typedef int qword __attribute__((mode(TI)));
typedef union U{qword q;float f[4];int i[4];}U;
typedef struct NODE{char pad0[0xE0];struct NODE*nextE0;char padE4[4];unsigned flags;char padEC[0x18];struct NODE*next104;char pad108[4];float reach;char pad110[0x64];struct NODE*next174;char pad178[0x3C];struct NODE*next1B4;}NODE;
typedef struct PAIR{U candidate,original;}PAIR;
typedef struct P{char pad0[0x10];NODE*actor;char pad14[0x28];float time;char pad40[0x10];PAIR pair[4];int index;}P;
typedef struct ROW{U point;float angle;}ROW;
typedef struct QUERY{U point;float heading;unsigned count;ROW row[64];U candidate,delta,scalar;}QUERY;
typedef struct WORK{QUERY query;NODE*result256[256];NODE*result32[32];NODE*result128[128];}WORK;
typedef struct LM{float low,high;}LM;
extern NODE*D_003E13FC,*D_003E13F0,*D_003E1408,*D_003E13E4,*D_003E13C0,*D_003E13D8,*D_003E13CC,*D_003E13B4,*D_003E1450;
extern char D_003DD490;
extern qword D_002A2DA0,D_002A2DC0;
extern const float D_002A53D0;
extern LM D_002BE5A8,D_002BE5B0;

extern "C" {
extern qword func_001A62E8(NODE*);extern float RadStepHeading(NODE*);
extern int func_00172798(QUERY*);
extern void FindShapeClosestPoint(NODE*,qword,void*,qword*,qword*,float*,float*);
extern int FIsBasicDerivedFrom__FP5BASIC3CID(NODE*,int);
extern int CploFindSwObjectsByClass(void*,int,int,NODE*,int,NODE**);
extern int func_00172888(ROW*,ROW*);
extern void func_002338C0(void*,unsigned,unsigned,int(*)(ROW*,ROW*));
extern qword SetVectorCylind(float,float,float);extern float GRandInRange__Fff(float,float);
}
struct VU_FLOAT{qword qw;};
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0, %1, %2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwSubFrom(const qword &a,qword b){qword r;__asm__("vsub.xyzw %0, %1, %2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwAdd(qword a,qword b){qword r;__asm__("vadd.xyzw %0, %1, %2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwLength(qword v){qword qw,vfOne,qwLen;
 __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z":"=j"(qw),"=j"(vfOne):"j"(v));
 __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qwLen):"j"(qw));return qwLen;}
// allocation-only "0"(vfOne): the length reuses the vfOne register, as in the target
static inline qword QwLengthOne(qword v){qword qw,vfOne,qwLen;
 __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z":"=j"(qw),"=j"(vfOne):"j"(v));
 __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qwLen):"j"(qw),"0"(vfOne));return qwLen;}
static inline qword QwLengthXY(qword v){qword qw;
 __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y":"=j"(qw):"j"(v));
 __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qw):"0"(qw));return qw;}
static inline float GFromVf(qword qw){float g;__asm__("qmfc2 %0, %1":"=r"(g):"j"(qw));return g;}
static inline float GXOfQw(qword qw){float x;__asm__("mtc1 %1, %0":"=f"(x):"r"(qw));return x;}
// allocation-only second "j"(b): the extra reference raises b's local-alloc priority, so b gets vf1 and the dot vf2 as in the target
static inline qword QwDot(qword a,qword b){qword qw,vfOne;
 __asm__("vmul.xyz %1, %2, %3\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z":"=j"(vfOne),"=j"(qw):"j"(a),"j"(b),"j"(b));return qw;}
static inline qword QwScaleX(qword v,qword g){qword qw;__asm__("vmulx.xyzw %0, %1, %2x":"=j"(qw):"j"(v),"0"(g));return qw;}
static inline qword QwDivide(qword v,qword g){qword qw;__asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q":"=j"(qw):"j"(v),"j"(g));return qw;}
static inline qword QwAddScaled(qword base,qword vec,qword g){qword qw;
 __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x":"=j"(qw):"j"(base),"j"(vec),"j"(g));return qw;}
static inline qword VuFloatFromMemWord(int n){qword v;__asm__("qmtc2 %1, %0":"=j"(v):"rV"(n));return v;}
static inline qword VuFloatVolatile(float g){qword v;
 // volatile: the target re-broadcasts the constant inside the loop (loop.c must not hoist it)
 __asm__ __volatile__("qmtc2 %1, %0":"=j"(v):"r"(g));return v;}
// vsub r = t - b. Allocation-only constraints: "=j"(t)/"1"(t) marks the copy t as consumed by the vsub, and "0"(b) puts r in b's register (as in the target)
#define QW_SUB_T(r,t,b) __asm__("vsub.xyzw %0, %1, %2":"=j"(r),"=j"(t):"0"(b),"1"(t))
/** @sly2-annot addr=0x1728B0
 * Purpose: Gathers candidate hiding places (several global object lists, cids 0x23/0x67/0x1B), sorts by
 *   heading, keeps up to 4 spread points, fills the rest randomly behind the guard
 * Info: proposed name ChooseBhv171SearchPoints (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
extern "C" void func_001728B0(P*p){
 WORK w;NODE*object;NODE**cursor;int remaining,i;unsigned accepted;float lastAngle;
 w.query.point.q=func_001A62E8(p->actor);
 w.query.heading=RadStepHeading(p->actor);w.query.count=0;
 for(object=D_003E13FC;object;object=object->next104){
  qword posObj=func_001A62E8(object);
  qword qwFallback=D_002A2DA0;
  w.query.delta.q=QwSubFrom(w.query.point.q,posObj);
  w.query.delta.i[2]=0;
  qword len=QwLengthOne(w.query.delta.q);
  qword g=VuFloatFromMemWord(*(int*)&object->reach);
  qword n;
  if(GXOfQw(len)<0.0001f){w.query.scalar.q=g;n=qwFallback;}
  else{w.query.scalar.q=g;n=QwDivide(w.query.delta.q,len);}
  w.query.candidate.q=QwAddScaled(posObj,n,g);
  func_00172798(&w.query);
 }
 for(object=D_003E13F0;object;object=object->next1B4){FindShapeClosestPoint(object,w.query.point.q,0,&w.query.candidate.q,0,0,0);func_00172798(&w.query);}
 for(object=D_003E1408;object;object=object->next104){w.query.candidate.q=func_001A62E8(object);func_00172798(&w.query);}
 for(object=D_003E13E4;object;object=object->next174){w.query.candidate.q=func_001A62E8(object);func_00172798(&w.query);}
 for(object=D_003E13C0;object;object=object->next1B4){FindShapeClosestPoint(object,w.query.point.q,0,&w.query.candidate.q,0,0,0);func_00172798(&w.query);}
 for(object=D_003E13D8;object;object=object->next104){w.query.candidate.q=func_001A62E8(object);func_00172798(&w.query);}
 for(object=D_003E13CC;object;object=object->next1B4){FindShapeClosestPoint(object,w.query.point.q,0,&w.query.candidate.q,0,0,0);func_00172798(&w.query);}
 for(object=D_003E13B4;object;object=object->nextE0){
  if((object->flags&1)&&FIsBasicDerivedFrom__FP5BASIC3CID(object,0x13)){w.query.candidate.q=func_001A62E8(object);func_00172798(&w.query);}
 }
 {int count=CploFindSwObjectsByClass(&D_003DD490,0x505,0x23,0,0x100,w.result256);
 if(count>0){cursor=w.result256;remaining=count;do{NODE*item=*cursor++;--remaining;w.query.candidate.q=func_001A62E8(item);func_00172798(&w.query);}while(remaining);}}
 {int count=CploFindSwObjectsByClass(&D_003DD490,0x505,0x67,0,0x20,w.result32);
 if(count>0){cursor=w.result32;remaining=count;do{NODE*item=*cursor++;--remaining;w.query.candidate.q=func_001A62E8(item);func_00172798(&w.query);}while(remaining);}}
 {int count=CploFindSwObjectsByClass(&D_003DD490,0x505,0x1B,0,0x80,w.result128);
 if(count>0){cursor=w.result128;remaining=count;do{NODE*item=*cursor++;--remaining;w.query.candidate.q=func_001A62E8(item);func_00172798(&w.query);}while(remaining);}}
 for(object=D_003E1450;object;object=object->next174){w.query.candidate.q=func_001A62E8(object);func_00172798(&w.query);}
 lastAngle=0.0f;
 func_002338C0(w.query.row,w.query.count,sizeof(ROW),func_00172888);
 accepted=0;
 {
  PAIR*previous=0;
  for(i=0;i<(int)w.query.count&&accepted<4;i++){
   ROW*row=&w.query.row[i];qword original=row->point.q;
   qword up=D_002A2DC0;
   qword delta=QwSub(original,w.query.point.q);
   qword qwFallback=D_002A2DA0;
   qword planar=QwSub(delta,QwScaleX(up,QwDot(delta,up)));
   qword len=QwLength(planar);
   if(GXOfQw(len)<0.0001f)planar=qwFallback;
   else planar=QwDivide(planar,len);
   qword s=VuFloatVolatile(200.0f);
   qword target=QwAddScaled(w.query.point.q,planar,s);
   w.query.delta.q=s;
   // the one register pin: the vsub consumes a copy of target in vf2, made before the previous test (target: vmove in the beqz slot)
   register qword probe __asm__("vf2")=target;
   if(previous){qword d;QW_SUB_T(d,probe,previous->candidate.q);if(GXOfQw(QwLengthXY(d))<150.0f)continue;}
   previous=&p->pair[accepted++];lastAngle=row->angle;previous->original.q=original;previous->candidate.q=target;
  }
 }
 qword origin=w.query.point.q;
 qword anchor=QwAdd(origin,SetVectorCylind(w.query.heading+3.1415927f,-350.0f,0.0f));
 while(accepted<4){
  float angle,length;qword point;
  lastAngle+=GRandInRange__Fff(D_002BE5A8.low,D_002BE5A8.high);
  angle=lastAngle+w.query.heading;
  length=GRandInRange__Fff(D_002BE5B0.low,D_002BE5B0.high);
  point=QwAdd(anchor,SetVectorCylind(angle,length,0.0f));
  {PAIR*pair=&p->pair[accepted];pair->candidate.q=point;pair->original.q=point;}accepted++;
 }
 p->index=0;p->time=D_002A53D0;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char NODE_Size[(sizeof(NODE)==0x1B8)?1:-1];
typedef char PAIR_Size[(sizeof(PAIR)==0x20)?1:-1];
typedef char P_Size[(sizeof(P)==0xE0)?1:-1];
typedef char ROW_Size[(sizeof(ROW)==0x20)?1:-1];
typedef char QUERY_Size[(sizeof(QUERY)==0x850)?1:-1];
typedef char WORK_Size[(sizeof(WORK)==0xED0)?1:-1];
typedef char LM_Size[(sizeof(LM)==8)?1:-1];
typedef char Result256_Size[(sizeof(NODE*[256])==0x400)?1:-1];
typedef char Result32_Size[(sizeof(NODE*[32])==0x80)?1:-1];
typedef char Result128_Size[(sizeof(NODE*[128])==0x200)?1:-1];
typedef char Rows_Size[(sizeof(ROW[64])==0x800)?1:-1];
typedef char Pairs_Size[(sizeof(PAIR[4])==0x80)?1:-1];
typedef char NODE_nextE0_Offset[((unsigned)&((NODE*)0)->nextE0==224)?1:-1];
typedef char NODE_flags_Offset[((unsigned)&((NODE*)0)->flags==232)?1:-1];
typedef char NODE_next104_Offset[((unsigned)&((NODE*)0)->next104==260)?1:-1];
typedef char NODE_reach_Offset[((unsigned)&((NODE*)0)->reach==268)?1:-1];
typedef char NODE_next174_Offset[((unsigned)&((NODE*)0)->next174==372)?1:-1];
typedef char NODE_next1B4_Offset[((unsigned)&((NODE*)0)->next1B4==436)?1:-1];
typedef char PAIR_candidate_Offset[((unsigned)&((PAIR*)0)->candidate==0)?1:-1];
typedef char PAIR_original_Offset[((unsigned)&((PAIR*)0)->original==16)?1:-1];
typedef char P_actor_Offset[((unsigned)&((P*)0)->actor==16)?1:-1];
typedef char P_time_Offset[((unsigned)&((P*)0)->time==60)?1:-1];
typedef char P_pair_Offset[((unsigned)&((P*)0)->pair==80)?1:-1];
typedef char P_index_Offset[((unsigned)&((P*)0)->index==208)?1:-1];
typedef char ROW_point_Offset[((unsigned)&((ROW*)0)->point==0)?1:-1];
typedef char ROW_angle_Offset[((unsigned)&((ROW*)0)->angle==16)?1:-1];
typedef char QUERY_point_Offset[((unsigned)&((QUERY*)0)->point==0)?1:-1];
typedef char QUERY_heading_Offset[((unsigned)&((QUERY*)0)->heading==16)?1:-1];
typedef char QUERY_count_Offset[((unsigned)&((QUERY*)0)->count==20)?1:-1];
typedef char QUERY_row_Offset[((unsigned)&((QUERY*)0)->row==32)?1:-1];
typedef char QUERY_candidate_Offset[((unsigned)&((QUERY*)0)->candidate==2080)?1:-1];
typedef char QUERY_delta_Offset[((unsigned)&((QUERY*)0)->delta==2096)?1:-1];
typedef char QUERY_scalar_Offset[((unsigned)&((QUERY*)0)->scalar==2112)?1:-1];
typedef char WORK_query_Offset[((unsigned)&((WORK*)0)->query==0)?1:-1];
typedef char WORK_result256_Offset[((unsigned)&((WORK*)0)->result256==2128)?1:-1];
typedef char WORK_result32_Offset[((unsigned)&((WORK*)0)->result32==3152)?1:-1];
typedef char WORK_result128_Offset[((unsigned)&((WORK*)0)->result128==3280)?1:-1];
typedef char LM_low_Offset[((unsigned)&((LM*)0)->low==0)?1:-1];
typedef char LM_high_Offset[((unsigned)&((LM*)0)->high==4)?1:-1];
