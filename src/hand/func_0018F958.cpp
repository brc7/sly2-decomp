// func_0018F958: paired-response solver. For each world axis (D_002A2D00 rows) and each of the two objects (p, other;
// side 1 uses the reversed axis) it asks func_001FAA00 / func_001FAA48 for a pair of effects, then lets p's virtual
// slot 69 turn that pair into a velocity response. The difference of the two responses per axis forms a 3x3 matrix
// that CroutDecomp/CroutSolve solve against the three input floats; the out pairs are the weighted sums of the effects.
// Claude agent, T1668 (from lilly1 / laneJ9v2, aligned 1). What finished it:
//  - objects is an array of a one-pointer struct (OBJ). The top load goes through a fresh pointer (&objects[side+1]-1),
//    so cse1 does not reuse its address for call 1's objects[side].p. loop.c then combines the two loads into one giv,
//    and the later, zero-addend one becomes the representative (target init order s1 store, s3 objects, s2 arg).
//    Being COMPONENT_REFs, both loads are MEM_IN_STRUCT, so the scalar stores in the side==1 block do not invalidate
//    them and cse2 merges call 1's load into the top one (one lw, as in the target). lilly1's "m"(objects[side]) asm
//    is gone, so the s3 increment reaches call 1's delay slot.
//  - axis and scalar are plain qwords (MEM_SCALAR stores: an in-struct store in the skipped block would make cse
//    flush all memory). The (sizeof(&axis),axis) initializer marks axis addressable at its declaration, so it keeps
//    its stack slot 0x30. Call 2 reads axis through Ref(), a non-scalar MEM that waits for the .first store
//    (target lq a1 points before lq a2 axis).
// No register pins; two commented allocation-only empty asms remain (removing either one costs 1-25 aligned).
#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct DELTA{U first,second;};
struct P;
struct OBJ{P*p;};
struct VE{short delta,pad;void(*fn)(P*,DELTA*,qword,U*);};
struct P{char pad[12];VE*vt;};
extern "C"{
qword func_001A62E8(P*);
qword func_001FAA00(P*,qword);
qword func_001FAA48(P*,qword,qword);
extern U D_002A2D00[3];
}
void CroutDecomp(int,float*,float*);
void CroutSolve(int,float*,float*,float*);
struct VU_FLOAT{qword q;VU_FLOAT(){}explicit VU_FLOAT(qword a){q=a;}inline VU_FLOAT(const float&f);};
struct VU_VECTOR{qword q;VU_VECTOR(){}VU_VECTOR(qword a){q=a;}};
static inline VU_FLOAT Scalar(float value){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(value));return VU_FLOAT(q);}
static inline VU_VECTOR operator*(VU_VECTOR value,VU_FLOAT amount){qword q;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(q):"j"(value.q),"j"(amount.q));return VU_VECTOR(q);}
static inline qword Sub(VU_VECTOR first,VU_VECTOR second){qword q;__asm__("vsub.xyzw %0,%1,%2":"=j"(q):"j"(first.q),"j"(second.q));return q;}
static inline void StoreDot(float*out,const qword&a0,const qword&b){qword a=a0;qword one;float bits;__asm__("vmul.xyz %1,%1,%3\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x $ACC,%1,%1y\n\tvmaddz.x %1,%2,%1z\n\tqmfc2.ni %0,%1":"=&rV"(*out),"+j"(a),"=j"(one):"j"(b),"j"(a));}
static inline VU_FLOAT VuFloatFromMemWord(int n){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"rV"(n));return VU_FLOAT(q);}
inline VU_FLOAT::VU_FLOAT(const float&f){*this=VuFloatFromMemWord(*(int*)&f);}
static inline VU_VECTOR MulAdd3(VU_VECTOR a,VU_VECTOR b,VU_VECTOR c,VU_FLOAT x,VU_FLOAT y,VU_FLOAT z){qword out;__asm__("vmulax.xyzw $ACC,%1,%4x\n\tvmaddax.xyzw $ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x":"=j"(out):"j"(a.q),"j"(b.q),"j"(c.q),"j"(x.q),"j"(y.q),"j"(z.q));return VU_VECTOR(out);}
static inline VU_VECTOR Sum3(const U*a,const U*b,const U*c,VU_FLOAT x,VU_FLOAT y,VU_FLOAT z,qword*scalar){*scalar=x.q;VU_VECTOR va=a->q;*scalar=y.q;VU_VECTOR vb=b->q;*scalar=z.q;VU_VECTOR vc=c->q;return MulAdd3(va,vb,vc,x,y,z);}
static inline void StoreQ(U*d,VU_VECTOR v){d->q=v.q;}
static inline U*Addr(U*p){return p;}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char DELTA_Size[sizeof(DELTA)==32?1:-1];
typedef char P_Size[sizeof(P)==16?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char DELTA_first_Offset[FIELD_OFFSET(DELTA,first)==0?1:-1];
typedef char DELTA_second_Offset[FIELD_OFFSET(DELTA,second)==16?1:-1];
/* const-ref read: a non-scalar MEM, so the axis load for call 2 waits for the .first store */
static inline const qword&Ref(const qword&q){return q;}
/** @sly2-annot addr=0x18F958
 * Purpose: Solve a 3x3 system (CroutDecomp/CroutSolve) of per-axis impulse responses of Sly and the hang
 *   object (DvFromSoImpulse/DwFromSoImpulse, vt69) to get the force pair giving the requested velocity
 * Info: proposed name AnticipateJtForce (T1); TU hang
 * Notes: names_v2 v1 proposed FUN_001ceb18 (0.000)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
extern "C" void func_0018F958(P*p,P*other,qword*point,float*input,DELTA*out){
 int axisIndex=0,side;
 OBJ objects[2];U points[2];/* initializer only marks axis addressable at its declaration (keeps slot 0x30) */qword axis=(sizeof(&axis),axis);
 DELTA effects[2][3];U velocity[2];U relative[3];
 float matrix[3][3],decomposed[3][3],rhs[3],weights[3];qword scalar;
 objects[0].p=p;objects[1].p=other;
 StoreQ(&points[0],func_001A62E8(p));StoreQ(&points[1],*point);
 
 for(;axisIndex<3;axisIndex++){
  for(side=0;side<2;side++){
   /* fresh pointer form: cse1 must not reuse this address for call 1's objects[side].p (see header) */OBJ*fresh=&objects[side+1]-1;P*object=fresh->p;axis=D_002A2D00[axisIndex].q;
   if(side==1){VU_FLOAT minus=Scalar(-1.0f);axis=(VU_VECTOR(axis)*minus).q;scalar=minus.q;}
   U*second=Addr(&effects[side][axisIndex].second);DELTA*effect=&effects[side][axisIndex];
   (second-1)->q=func_001FAA00(objects[side].p,axis);
   /* empty asm, allocation-only: two extra refs make the arg and .second store givs outrank objects/points in global-alloc (target s1 store, s2 arg, s3 objects, s4 points); "=m"(scalar) keeps it a non-barrier */__asm__("" : "=m"(scalar) : "r"(effect),"r"(second));
   second->q=func_001FAA48(object,Addr(&points[side])->q,Ref(axis));
   VE*method=&p->vt[69];method->fn((P*)((char*)p+method->delta),effect,*point,&velocity[side]);
  }
  relative[axisIndex].q=Sub(velocity[0].q,velocity[1].q);
 }
 for(axisIndex=0;axisIndex<3;axisIndex++){
  float*row=matrix[axisIndex];
  int column;
  for(column=0;column<3;column++){qword value=D_002A2D00[column].q;StoreDot(row,value,relative[axisIndex].q);row++;}
  /* empty asm, allocation-only: keeps axisIndex live past the hoisted axisIndex<<2 (target addiu a3 before sll a2) */__asm__ volatile("" : : "r"(axisIndex));
  {float*in=input+axisIndex;rhs[axisIndex]=*in;}
 }
 CroutDecomp(3,&matrix[0][0],&decomposed[0][0]);
 CroutSolve(3,&decomposed[0][0],rhs,weights);
 VU_FLOAT x(weights[0]),y(weights[1]),z(weights[2]);
 for(side=0;side<2;side++){
  StoreQ(&out[side].first,Sum3(&effects[side][0].first,&effects[side][1].first,&effects[side][2].first,x,y,z,&scalar));
  StoreQ(&out[side].second,Sum3(&effects[side][0].second,&effects[side][1].second,&effects[side][2].second,x,y,z,&scalar));
 }
 
}
