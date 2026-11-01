
typedef int qword __attribute__((mode(TI)));
struct FRAME{};
struct REQ{unsigned int bytes,alignment;};
struct POLY{qword plane;char opaque10[8];int flags;};
struct ENTRY{ENTRY*child[2];POLY*poly;};
struct HIT{qword p;char pad10[0x30];float time;char pad44[0x1C];};
struct OUT{qword p,plane;int kind;};
union U{qword q;float f[4];};union SCR{REQ req;U u;};
struct WORK{FRAME frame;SCR scratch;HIT hits[16];int count;ENTRY*end;U position,endpoint;};
extern "C" FRAME*func_001AE478(FRAME*);
extern "C" void func_001AE4A0(FRAME*,int);
extern "C" void*func_001AE318(REQ*);
extern "C" int func_00118A88(ENTRY*,int,qword,float,int*,ENTRY**);
extern "C" int func_00118668(ENTRY*,int,qword,qword,int,HIT*);
extern "C" void func_00119390(qword*,qword,qword);
static inline float Max(float a,float b){float r;__asm__("max.s %0,%1,%2":"=f"(r):"f"(a),"f"(b));return r;}
static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
static inline qword Normal(qword v){register qword d __asm__("vf2"),one __asm__("vf3"),r __asm__("vf3");__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(d),"j"(v));return r;}
/** @sly2-annot addr=0x1A9108
 * Purpose: Prune a BSP to a sphere (radius >= 100) and clip its edges (PruneBsp, ClsgClipEdgeToBsp,
 *   InitPlaneFromNormalPos)
 * Info: proposed name PruneSphereBsp (T2); TU mark
 * Notes: names_v2 C proposed PxpMarkSwContactsRoot__FP2SWPP2SOi (0.167)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
extern "C" void func_001A9108(ENTRY*tree,int mask,void*geometry,qword pos,OUT*out,float radius0){
 WORK work;
 work.position.q=pos;
 float radius=Max(radius0,100.0f);
 func_001AE478(&work.frame);
 ENTRY*first;
 {REQ*r=&work.scratch.req;r->bytes=0xFFFC;r->alignment=16;first=(ENTRY*)func_001AE318(r);}
 int limit=0x1555;
 if(radius<52000.0f)do {
  work.end=first;work.count=limit;
  int status=func_00118A88(tree,mask,work.position.q,radius,&work.count,&work.end);
  if(status<0)goto more;
  if(status<2)goto more;
  if(status!=2)goto more;
  {work.count=work.end-first;
  float best=0.0f;
  int i;
  for(i=0;i<work.count;i++){
   register POLY*poly __asm__("$6")=first[i].poly;
   if(poly->flags&0x1000)continue;
   poly->flags|=0x1000;
   register qword factor __asm__("vf2"),positionVector __asm__("vf3");
   register qword argument __asm__("$7");register ENTRY*last __asm__("$4");
   register qword to __asm__("vf1");
   __asm__("qmtc2.ni %1,%0":"=j"(factor):"r"(radius+1.0f));
   __asm__("lq %0,%5\n\tsqc2 %8,%4\n\tlw %1,%6\n\tqmtc2.ni %0,%2\n\tlqc2 %3,%7\n\taddiu %1,%1,-12\n\tvadda.xyzw ACC,%2,$vf0\n\tvmaddx.xyzw %3,%3,%8x":"=&r"(argument),"=&r"(last),"=j"(positionVector),"=&j"(to),"=m"(work.scratch.u.q):"m"(work.position.q),"m"(work.end),"m"(poly->plane),"j"(factor));
   work.endpoint.q=to;
   int found=func_00118668(last,0x7FFFFFFF,to,argument,16,work.hits);
   register qword snapshot __asm__("vf1")=work.endpoint.q;
   if(found>0){
    if(work.hits[0].time>best){
     best=work.hits[0].time;
     qword normal=Normal(Sub(snapshot,work.position.q));
     out->p=work.hits[0].p;
     func_00119390(&out->plane,normal,work.hits[0].p);
     out->kind=12;
    }
   }
  }
  for(int j=0;j<work.count;j++)first[j].poly->flags&=~0x1000;
  if(best>0.0f){func_001AE4A0(&work.frame,2);return;}
 }
 more:radius+=radius;
 }while(radius<52000.0f);
 func_001AE4A0(&work.frame,2);
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char REQ_bytes_Offset[FIELD_OFFSET(REQ,bytes)==0?1:-1];
typedef char REQ_alignment_Offset[FIELD_OFFSET(REQ,alignment)==4?1:-1];
typedef char POLY_plane_Offset[FIELD_OFFSET(POLY,plane)==0?1:-1];
typedef char POLY_flags_Offset[FIELD_OFFSET(POLY,flags)==24?1:-1];
typedef char ENTRY_child_Offset[FIELD_OFFSET(ENTRY,child)==0?1:-1];
typedef char ENTRY_poly_Offset[FIELD_OFFSET(ENTRY,poly)==8?1:-1];
typedef char HIT_p_Offset[FIELD_OFFSET(HIT,p)==0?1:-1];
typedef char HIT_time_Offset[FIELD_OFFSET(HIT,time)==64?1:-1];
typedef char OUT_p_Offset[FIELD_OFFSET(OUT,p)==0?1:-1];
typedef char OUT_plane_Offset[FIELD_OFFSET(OUT,plane)==16?1:-1];
typedef char OUT_kind_Offset[FIELD_OFFSET(OUT,kind)==32?1:-1];
typedef char U_q_Offset[FIELD_OFFSET(U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(U,f)==0?1:-1];
typedef char SCR_req_Offset[FIELD_OFFSET(SCR,req)==0?1:-1];
typedef char SCR_u_Offset[FIELD_OFFSET(SCR,u)==0?1:-1];
typedef char WORK_count_Offset[FIELD_OFFSET(WORK,count)==1568?1:-1];
typedef char WORK_end_Offset[FIELD_OFFSET(WORK,end)==1572?1:-1];
typedef char WORK_position_Offset[FIELD_OFFSET(WORK,position)==1584?1:-1];
typedef char WORK_endpoint_Offset[FIELD_OFFSET(WORK,endpoint)==1600?1:-1];
typedef char FRAME_PrefixSize[sizeof(FRAME)==1?1:-1];
typedef char FRAME_Alignment[__alignof__(FRAME)==1?1:-1];
typedef char REQ_PrefixSize[sizeof(REQ)==8?1:-1];
typedef char REQ_Alignment[__alignof__(REQ)==4?1:-1];
typedef char POLY_PrefixSize[sizeof(POLY)==32?1:-1];
typedef char POLY_Alignment[__alignof__(POLY)==16?1:-1];
typedef char ENTRY_PrefixSize[sizeof(ENTRY)==12?1:-1];
typedef char ENTRY_Alignment[__alignof__(ENTRY)==4?1:-1];
typedef char HIT_PrefixSize[sizeof(HIT)==96?1:-1];
typedef char HIT_Alignment[__alignof__(HIT)==16?1:-1];
typedef char OUT_PrefixSize[sizeof(OUT)==48?1:-1];
typedef char OUT_Alignment[__alignof__(OUT)==16?1:-1];
typedef char U_PrefixSize[sizeof(U)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char SCR_PrefixSize[sizeof(SCR)==16?1:-1];
typedef char SCR_Alignment[__alignof__(SCR)==16?1:-1];
typedef char WORK_PrefixSize[sizeof(WORK)==1616?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char qword_Width[sizeof(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];

typedef char WORK_frame_Offset[FIELD_OFFSET(WORK,frame)==0?1:-1];
typedef char WORK_scratch_Offset[FIELD_OFFSET(WORK,scratch)==16?1:-1];
typedef char WORK_hits_Offset[FIELD_OFFSET(WORK,hits)==32?1:-1];
