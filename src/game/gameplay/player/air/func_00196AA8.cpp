typedef int qword __attribute__((mode(TI)));typedef unsigned int uint;
union U{qword q;float f[4];int i[4];};struct P;
struct CONTACT{char pad0[8];P*owner;char padC[0x80-12];U normal;char pad90[0x1C];CONTACT*next;};
struct GROUP{CONTACT*first;GROUP*next;};struct COLLISION{char pad0[0x64];GROUP*groups;CONTACT*single;};struct P{char pad0[0xEC];COLLISION*collision;};struct MOVE{P*actor;char pad4[0x18];int mode;};
struct FRAME{};struct REQ{int bytes,alignment;};union SCRATCH{U scalar;REQ request;};struct WORK{U normals[64];SCRATCH scratch;FRAME frame;U residual,original;};
extern "C"{extern U D_002A2DC0;extern float D_002A63B0;FRAME*STACKSCOPE_ctor(FRAME*);void STACKSCOPE_ctor_1AE4A0(FRAME*,int);void*func_001AE318(REQ*);void SolveInequalities(int,float*,float*,float*,float);void func_00195FB8(MOVE*,int,void*);}
static inline void Publish(U*home,qword q){__asm__ volatile("sqc2 %1,%0":"=m"(home->q):"j"(q));}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword Scale(qword v,qword k){__asm__("vmulx.xyzw %0,%1,%2x":"=j"(v):"j"(v),"j"(k));return v;}
static inline float Z(qword q){float f;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(q));__asm__("pextuw %0,$zero,%0":"+r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline qword Reject(qword value,qword up){qword q,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(q),"=&j"(one):"j"(value),"j"(up));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(q):"j"(up),"j"(q));__asm__("vsub.xyzw %0,%0,%1":"+j"(value):"j"(q));return value;}
static inline void MatrixDot(float*out,qword a,qword source){register qword q __asm__("vf2")=a,b __asm__("vf1")=source,one __asm__("vf3");register int bits __asm__("$7");__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(q),"=&j"(one):"j"(b));__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("sw %1,%0":"=m"(*out):"r"(bits));}
static inline void VectorDot(float*out,qword input,qword source){register qword a __asm__("vf3")=input,q __asm__("vf1")=source,one __asm__("vf2");register int bits __asm__("$4");__asm__("vmul.xyz %0,%2,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(q),"=&j"(one):"j"(a));__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(q));__asm__("sw %1,%0":"=m"(*out):"r"(bits));}
static inline qword AddScaled(qword input,qword source,qword scalar){register qword sum __asm__("vf3")=input,normal __asm__("vf1")=source,k __asm__("vf2")=scalar;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(sum):"j"(sum),"j"(normal),"j"(k));return sum;}
static inline qword Sub(qword input,qword other){register qword a __asm__("vf2")=input,b __asm__("vf3")=other,q __asm__("vf1");__asm__("vsub.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline float Square(qword a){qword q=a,one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(q),"=&j"(one));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(q));return f;}
static inline float*Allocate(REQ*r,int bytes){r->bytes=bytes;r->alignment=16;return(float*)func_001AE318(r);}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char CONTACT_Size[sizeof(CONTACT)==176?1:-1];
typedef char GROUP_Size[sizeof(GROUP)==8?1:-1];
typedef char COLLISION_Size[sizeof(COLLISION)==108?1:-1];
typedef char P_Size[sizeof(P)==240?1:-1];
typedef char MOVE_Size[sizeof(MOVE)==32?1:-1];
typedef char FRAME_Size[sizeof(FRAME)==1?1:-1];
typedef char REQ_Size[sizeof(REQ)==8?1:-1];
typedef char SCRATCH_Size[sizeof(SCRATCH)==16?1:-1];
typedef char WORK_Size[sizeof(WORK)==1088?1:-1];
typedef char CONTACT_owner_Offset[FIELD_OFFSET(CONTACT,owner)==8?1:-1];
typedef char CONTACT_normal_Offset[FIELD_OFFSET(CONTACT,normal)==128?1:-1];
typedef char CONTACT_next_Offset[FIELD_OFFSET(CONTACT,next)==172?1:-1];
typedef char GROUP_first_Offset[FIELD_OFFSET(GROUP,first)==0?1:-1];
typedef char GROUP_next_Offset[FIELD_OFFSET(GROUP,next)==4?1:-1];
typedef char COLLISION_groups_Offset[FIELD_OFFSET(COLLISION,groups)==100?1:-1];
typedef char COLLISION_single_Offset[FIELD_OFFSET(COLLISION,single)==104?1:-1];
typedef char P_collision_Offset[FIELD_OFFSET(P,collision)==236?1:-1];
typedef char MOVE_actor_Offset[FIELD_OFFSET(MOVE,actor)==0?1:-1];
typedef char MOVE_mode_Offset[FIELD_OFFSET(MOVE,mode)==28?1:-1];
typedef char REQ_bytes_Offset[FIELD_OFFSET(REQ,bytes)==0?1:-1];
typedef char REQ_alignment_Offset[FIELD_OFFSET(REQ,alignment)==4?1:-1];
typedef char WORK_normals_Offset[FIELD_OFFSET(WORK,normals)==0?1:-1];
typedef char WORK_scratch_Offset[FIELD_OFFSET(WORK,scratch)==1024?1:-1];
typedef char WORK_frame_Offset[FIELD_OFFSET(WORK,frame)==1040?1:-1];
typedef char WORK_residual_Offset[FIELD_OFFSET(WORK,residual)==1056?1:-1];
typedef char WORK_original_Offset[FIELD_OFFSET(WORK,original)==1072?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
typedef char uint_Size[sizeof(uint)==4?1:-1];
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char U_Align[__alignof__(U)==16?1:-1];
typedef char SCRATCH_Align[__alignof__(SCRATCH)==16?1:-1];
typedef char WORK_Align[__alignof__(WORK)==16?1:-1];
typedef char FRAME_Align[__alignof__(FRAME)==1?1:-1];
typedef char REQ_Align[__alignof__(REQ)==4?1:-1];
typedef char U_q_Offset[FIELD_OFFSET(U,q)==0?1:-1];
typedef char U_float_Offset[FIELD_OFFSET(U,f)==0?1:-1];
typedef char Normal_Count[sizeof(((WORK*)0)->normals)/sizeof(U)==64?1:-1];
typedef char Normal_Stride[(FIELD_OFFSET(WORK,normals[1])-FIELD_OFFSET(WORK,normals[0]))==16?1:-1];
/** @sly2-annot addr=0x196AA8
 * Purpose: Collect up to 64 contact normals (flipped when Sly is not the owner), SolveInequalities and return
 *   the residual direction used to slide along contacts
 * Info: proposed name ProjectJtVelocityOnContacts (T2); TU air
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" void func_00196AA8(MOVE*p,U*out){
 WORK w;unsigned base=(unsigned)w.normals-16;int count=0;CONTACT*c;GROUP*group=p->actor->collision->groups;
 for(;;){c=group?group->first:p->actor->collision->single;
  if(c){unsigned write=base+count*16;do{if((uint)count>=64)goto collected;qword normal=c->normal.q;if(p->actor!=c->owner){qword negative=Scalar(-1.0f);Publish(&w.scratch.scalar,negative);normal=Scale(normal,negative);}if(Z(normal)>0.7f)continue;write+=16;*(qword*)write=Reject(normal,D_002A2DC0.q);count++;}while((c=c->next)!=0);}
  if(!group)break;group=group->next;
 }
 collected:
 if(!count)return;
 (void)STACKSCOPE_ctor(&w.frame);
 float*matrix=Allocate(&w.scratch.request,count*count*4);float*rhs=Allocate(&w.scratch.request,count*4);float*weights=Allocate(&w.scratch.request,count*4);
 float*cursor=matrix;
 for(int i=0;i<count;i++){for(int j=0;j<count;j++){MatrixDot(cursor,((U*)((unsigned)(i*16)+(unsigned)w.normals))->q,w.normals[j].q);cursor++;}}
 {register qword original __asm__("$2");register float*write __asm__("$5");__asm__("lq %0,%2\n\tdaddu %1,%3,$zero":"=&r"(original),"=r"(write):"m"(out->q),"r"(rhs));w.residual.q=original;w.original.q=original;
 if(count>0){register qword value __asm__("vf3");register int remaining __asm__("$3");register U*normal __asm__("$2");__asm__("qmtc2.ni %3,%0\n\tdaddu %1,%4,$zero\n\tdaddu %2,%5,$zero":"=j"(value),"=&r"(remaining),"=r"(normal):"r"(original),"r"(count),"r"(w.normals));do{VectorDot(write,value,normal->q);write++;normal++;}while(--remaining!=0);}}
 SolveInequalities(count,matrix,rhs,weights,D_002A63B0);
 if(count>0){register float*read __asm__("$2")=weights;register U*normal __asm__("$4")=w.normals;for(register int remaining __asm__("$3")=count;remaining!=0;){register int bits __asm__("$5");__asm__("lw %0,%1":"=r"(bits):"m"(*read));--remaining;register qword scalar __asm__("vf2"),sum __asm__("vf3"),axis __asm__("vf1");__asm__ volatile("qmtc2.ni %5,%0\n\tlqc2 %1,%6\n\tsqc2 %0,%2\n\taddiu %3,%3,4\n\tlqc2 %4,%7":"=j"(scalar),"=j"(sum),"=m"(w.scratch.scalar.q),"+&r"(read),"=j"(axis):"r"(bits),"m"(w.residual.q),"m"(normal->q));w.residual.q=AddScaled(sum,axis,scalar);normal++;}}
 if(Square(Sub(w.original.q,w.residual.q))>1.0f){switch(p->mode){case 0:func_00195FB8(p,1,0);break;case 2:func_00195FB8(p,3,0);break;}}
 {register qword result __asm__("$4")=w.residual.q;out->q=result;}STACKSCOPE_ctor_1AE4A0(&w.frame,2);
}