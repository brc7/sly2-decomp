typedef int qword __attribute__((mode(TI)));
struct M4{qword r[4];};
struct LO{char pad[0x40];M4 m;};
struct A{LO*lo;int x4;float radius;int xC;unsigned long flags;int x18,first;M4 m;char pad60[0x18];int count;qword*vertices;char pad80[0x10];int probeCount;qword*probes;};
struct B{LO*lo;char pad4[0xC];unsigned long flags;char pad18[8];M4 m;char pad60[0x48];int oldGeneration,generation;};
struct HIT{qword p;char pad10[0x30];float time;int x44,x48,tag;char pad50[0x10];};
struct W;struct C;
extern "C" M4 InvertMatrix4Rigid(const M4*);
extern "C" int ClsgClipEdgeToObject(B*,qword,qword,int,HIT*);
extern "C" void PxpAdd(W*,A*,B*,qword,qword,int,int,int,C**,float);
static inline qword TransformNew(const M4*m,qword p){qword r;__asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w":"=&j"(r):"j"(m->r[0]),"j"(m->r[1]),"j"(p),"j"(m->r[2]),"j"(m->r[3]));return r;}
static inline qword Transform(const M4*m,qword p){__asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w":"=j"(p):"j"(m->r[0]),"j"(m->r[1]),"0"(p),"j"(m->r[2]),"j"(m->r[3]));return p;}
static inline qword SubN(qword a,qword b){register qword r __asm__("vf1");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Dot(qword a,qword b){qword d,one;float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"j"(b));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
static inline qword Normal(qword v){register qword d __asm__("vf2"),one __asm__("vf3"),r __asm__("vf5");__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(d),"j"(v));return r;}
static inline qword Copy(qword q){qword r;__asm__("vmove.xyzw %0,%1":"=j"(r):"j"(q));return r;}
static inline float MinDistance(qword pos,qword dir,int count0,const qword*const*points0){
 register int count __asm__("$3")=count0;
 register float best __asm__("$f12")=0;
 if(count>0){register const qword*points __asm__("$4")=*points0; register qword p __asm__("vf4")=Copy(pos);
 register qword n __asm__("vf3")=Copy(dir);
do {float d=Dot(Sub(*points,p),n);if(d<best)best=d;points++;}while(--count);}
 return best;
}
// Sweep vertices and create contacts from the first accepted segment hit.
/** @sly2-annot addr=0x1A9D38
 * Purpose: Sweep moving vertices of one shape (old->new frame) with line tests against the other and add
 *   contacts at first hit
 * Info: proposed name MarkSoContactsSweepVertices (T2); TU mark
 * Notes: names_v2 v1 proposed PxpMarkSwContactsRoot__FP2SWPP2SOi (0.167)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
extern "C" void func_001A9D38(W*w,A*a,B*b,C**out){
 int i;
 for(i=a->first;i<a->count;i++){
  HIT hits[2];
  qword pos=TransformNew(&a->m,a->vertices[i]);
  if(b->oldGeneration!=b->generation)pos=Transform(&b->lo->m,Transform(&InvertMatrix4Rigid(&b->m),pos));
  qword dest=a->probes[i];
  int count=ClsgClipEdgeToObject(b,pos,dest,2,hits);
  if(count){
   int skip=hits[0].time==0.0f;
   if(skip<count){
    qword dir=Normal(SubN(pos,dest));
    qword hit=hits[0].p;
    float g=0.0f;
    g=MinDistance(hit,dir,a->probeCount,&a->probes);
    if(a->flags&(1UL<<49)){float d=Dot(Sub(a->lo->m.r[3],hit),dir)-a->radius;if(d<g)g=d;}
    int kind=(b->flags&(1UL<<49))?9:8;
    int tag=(b->flags&(1UL<<49))?0:hits[0].tag;
    PxpAdd(w,a,b,hit,dir,kind,tag,0,out,g);
   }
  }
 }
}
