typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float g[4];} U;
struct P{char pad0[0x10];void*actor;char pad14[0x5C-0x14];void*target;};
struct REQUEST{int flag;float along,up;int padC;qword point,direction;};
extern qword D_002A2DC0,D_002A2DA0;
extern float D_002BE6C4;
qword func_001A62E8(void*);
static inline qword ToVU(qword word){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(word));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Reject(qword a,qword n){qword d,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a),"j"(n));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(d):"j"(n),"j"(d));return Sub(a,d);}
static inline qword Normal(qword a,qword fallback){register qword d __asm__("vf1");qword one;float len;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(len):"j"(d));if(len<0.0001f){d=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(d):"j"(d),"j"(a));}return d;}

static inline qword NativePoint(qword point,qword direction,qword along,qword up,qword height,unsigned int*high){register unsigned int h __asm__("$2");__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddax.xyzw ACC,%2,%3x\n\tvmaddx.xyzw %0,%4,%5x\n\tlui %1,%%hi(%6)":"+j"(point),"=r"(h):"j"(direction),"j"(along),"j"(up),"j"(height),"i"(&D_002BE6C4));*high=h;return point;}
static inline qword NativeHeading(qword point,qword up,qword height){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(point):"j"(up),"j"(height));return point;}
static inline void PublishPoint(U*home,qword scalar,qword*out,qword point){__asm__ volatile("sqc2 %2,%0\n\tsqc2 %3,%1":"=m"(home->q),"=m"(*out):"j"(scalar),"j"(point));}
/** @sly2-annot addr=0x17AA78
 * Purpose: Compute hold point and direction from the holder's request (vt96) relative to the guard
 * Info: proposed name CalcBhvjuggledHoldPoint (T2); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   AdjustAloRotation (names_v2:B(unverified)) | BHV class word normalised: CalcBhvheldHoldPoint ->
 *   CalcBhvjuggledHoldPoint | runtime2/4 confirmed the BHVJUGGLED role on live guards (S02-S41)
 * Confidence: low (draft) [T2 0.45] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017AA78(struct P*p,struct REQUEST*request,qword*outPoint,qword*outDirection){

 qword first=ToVU(func_001A62E8(p->actor));
 qword direction=Normal(Reject(Sub(first,func_001A62E8(p->target)),D_002A2DC0),D_002A2DA0);
 {U scalar;
 register unsigned int addressWord __asm__("$2")=(unsigned int)&D_002A2DC0;
 register qword along __asm__("vf4"),up __asm__("vf5"),point __asm__("vf2"),height __asm__("vf6");
 __asm__("lw $3,%5\n\tlqc2 %2,0(%0)\n\tqmtc2.ni $3,%1\n\tlw %0,%6\n\tlqc2 %3,%7\n\tqmtc2.ni %0,%4":"+&r"(addressWord),"=j"(along),"=j"(up),"=j"(point),"=j"(height):"m"(request->along),"m"(request->up),"m"(request->point):"$3","memory");
 {unsigned int high;
 qword result=NativePoint(point,direction,along,up,height,&high);
 register unsigned int constantWord __asm__("$2")=high;
 register qword heading __asm__("vf1"),constant __asm__("vf3");
 __asm__("lqc2 %1,%3\n\tlw %0,%%lo(%4)(%0)\n\tqmtc2.ni %0,%2":"+r"(constantWord),"=j"(heading),"=j"(constant):"m"(request->direction),"i"(&D_002BE6C4):"memory");
 PublishPoint(&scalar,along,outPoint,result);
 heading=NativeHeading(heading,up,constant);
 PublishPoint(&scalar,height,outDirection,heading);
 scalar.q=constant;}}

}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char REQUEST_Size[(sizeof(struct REQUEST)==48)?1:-1];
typedef char P_actor_Offset[((unsigned)&((struct P*)0)->actor==0x10)?1:-1];
typedef char P_target_Offset[((unsigned)&((struct P*)0)->target==0x5C)?1:-1];
typedef char REQUEST_flag_Offset[((unsigned)&((struct REQUEST*)0)->flag==0)?1:-1];
typedef char REQUEST_along_Offset[((unsigned)&((struct REQUEST*)0)->along==4)?1:-1];
typedef char REQUEST_up_Offset[((unsigned)&((struct REQUEST*)0)->up==8)?1:-1];
typedef char REQUEST_point_Offset[((unsigned)&((struct REQUEST*)0)->point==0x10)?1:-1];
typedef char REQUEST_direction_Offset[((unsigned)&((struct REQUEST*)0)->direction==0x20)?1:-1];

typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char word_Width[sizeof(unsigned int)==4?1:-1];
typedef char integer_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==0x60?1:-1];
typedef char P_Alignment[__alignof__(struct P)==4?1:-1];
typedef char REQUEST_Alignment[__alignof__(struct REQUEST)==16?1:-1];
typedef char U_q_Offset[(unsigned)&((U*)0)->q==0?1:-1];
typedef char U_scalar_Offset[(unsigned)&((U*)0)->g==0?1:-1];
