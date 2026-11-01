// UpdateWrMatrixes (0x22C028). Circle case: the scalar-home sqc2 stores and one combine carry "memory" clobbers to fix their order (Claude agent).
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct M3 {float f[3][4];} __attribute__((aligned(16)));
struct M4 {V r[4];};
struct ONZE {float amplitude,frequency,phase,random;};
struct ONZ {float minimum,maximum,bias;int count;ONZE terms[4];};
struct CIRCLE {V uvA,vectorB,uvB;int column;float spatialFrequency,frequency,phase;};
struct BEND {ONZ bend,swivel;};
union PAYLOAD {CIRCLE circle;BEND bend;};
struct WRE {int kind,index;char pad8[8];V axis,vectorA;PAYLOAD data;};
struct WR {char pad[0x50];float time;int count,count58,index5C;WRE entries[8];int count6E0;char pad6E4[0xC];M4 matrices[4],coordinates[4];char pad8F0[0xC];void *source;};
struct CLOCK {float t,dt;};
extern CLOCK D_002DDED0;
extern "C" {
void UpdateLo(WR *);
void func_0022B7D8(WR *);
float RadNormalize__Ff(float);
void CalculateSinCos__FfPfT1(float,float *,float *);
float func_0022BF70(ONZ *);
M3 func_0011B500(qword,float);
}
static inline qword ScalarLoad(const float &f){int bits;qword q;__asm__("lw %0,%1":"=r"(bits):"m"(f));__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword ScalarHome(V &home,qword value){__asm__("sqc2 %1,%0":"=m"(home.q):"j"(value));return value;}
static inline qword Combine(qword fa,qword a,qword fb,qword b){qword r;__asm__("vmulax.xyzw ACC,%2,%1x\n\tvmaddx.xyzw %0,%4,%3x":"=j"(r):"j"(fa),"j"(a),"j"(fb),"j"(b));return r;}
static inline qword CombineM(qword fa,qword a,qword fb,qword b){qword r;__asm__("vmulax.xyzw ACC,%2,%1x\n\tvmaddx.xyzw %0,%4,%3x":"=j"(r):"j"(fa),"j"(a),"j"(fb),"j"(b):"memory");return r;}
static inline qword ScalarHomeM(V &home,qword value){__asm__("sqc2 %1,%0":"=m"(home.q):"j"(value):"memory");return value;}
static inline void CopyXYZ(V &destination,const V &v){float x=v.f[0],y=v.f[1],z=v.f[2];destination.f[0]=x;destination.f[1]=y;destination.f[2]=z;}
static inline void CopyXY(V &destination,const V &v){float x=v.f[0],y=v.f[1];destination.f[0]=x;destination.f[1]=y;}
static inline qword Unit(qword v){qword len,one,r;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(len),"=&j"(one):"j"(v));__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(r):"j"(v),"0"(len));return r;}
static inline qword UnitFresh(qword v){qword len,one,r;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(len),"=&j"(one):"j"(v));__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(len));__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=&j"(r):"j"(v),"j"(len));return r;}
static inline qword Rotate(const M3 &m,qword v){__asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(v):"j"(*(const qword *)m.f[0]),"j"(*(const qword *)m.f[1]),"j"(*(const qword *)m.f[2]));return v;}
/** @sly2-annot addr=0x22C028
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/animation.md#wr-0x22b720-0x22c818
 * @end-sly2-annot */
void UpdateWrMatrixes(WR *p){
 UpdateLo(p);
 if(D_002DDED0.t==p->time)return;
 if(p->source)func_0022B7D8(p);
 for(int i=0;i<p->count;i++){
  WRE *entry=&p->entries[i];
  M4 *matrix=&p->matrices[entry->index];
  M4 *coordinates=&p->coordinates[entry->index];
  switch(entry->kind){
  case 0:{
   V position,uv,temporary;
   float sine,cosine;
   float angle=(D_002DDED0.t*entry->data.circle.frequency+entry->data.circle.phase)*6.2831855f;
   CalculateSinCos__FfPfT1(RadNormalize__Ff(angle),&sine,&cosine);
   int column=entry->data.circle.column,next=column+1;
   V *pos0=&matrix->r[column],*pos1=&matrix->r[next],*uv0=&coordinates->r[column],*uv1=&coordinates->r[next];
   qword qn=Scalar(-sine),qc=ScalarLoad(cosine),qs=Scalar(sine);
 {position.q=Combine(qc,entry->vectorA.q,qs,entry->data.circle.vectorB.q);}
   CopyXYZ(*pos0,position);
 ScalarHomeM(temporary,qc);
 {qword t=CombineM(qn,entry->vectorA.q,qc,entry->data.circle.vectorB.q);ScalarHomeM(temporary,qs);position.q=t;ScalarHome(temporary,qc);}
   CopyXYZ(*pos1,position);
 {ScalarHomeM(temporary,qs);uv.q=Combine(qc,entry->data.circle.uvA.q,qs,entry->data.circle.uvB.q);}
   CopyXY(*uv0,uv);
 {qword a=entry->data.circle.uvA.q;ScalarHomeM(temporary,qc);uv.q=Combine(qn,a,qc,entry->data.circle.uvB.q);}
   CopyXY(*uv1,uv);
   break;
  }
  case 1:{
   M3 result,first;
   V rotated;
   float bend=func_0022BF70(&entry->data.bend.bend);
   float swivel=func_0022BF70(&entry->data.bend.swivel);
   first=func_0011B500(UnitFresh(entry->vectorA.q),swivel);
   rotated.q=Rotate(first,Unit(entry->axis.q));
   result=func_0011B500(UnitFresh(rotated.q),bend);

   matrix->r[0].f[0]=result.f[0][0]-1.0f;matrix->r[0].f[1]=result.f[0][1];matrix->r[0].f[2]=result.f[0][2];matrix->r[1].f[0]=result.f[1][0];matrix->r[1].f[1]=result.f[1][1]-1.0f;matrix->r[1].f[2]=result.f[1][2];matrix->r[2].f[0]=result.f[2][0];matrix->r[2].f[1]=result.f[2][1];matrix->r[2].f[2]=result.f[2][2]-1.0f;
   break;
  }
  }
 }
 p->time=D_002DDED0.t;
}
