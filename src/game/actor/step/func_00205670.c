typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} V;
struct P {char pad[0x208];int enabled;};
extern qword D_002A2DC0;
void func_002057E8(struct P*,V*);
int FIntersectRayTriangle(qword,qword,qword,qword,qword,float*,float*);
static inline qword Sub(const qword*a,const qword*b){qword av=*a,bv=*b;register qword r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=&j"(r):"j"(av),"j"(bv));return r;}
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Scale(qword a,float f,V *k){qword q,r;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));k->q=q;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(k->q));return r;}
static inline float Normal(V*v,const qword*fallback){qword a=v->q;register qword d __asm__("vf1");register qword one __asm__("vf2");float g,r;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(one):"j"(d));
__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(one));
if(g<0.0001f)v->q=*fallback;
else {register qword normalized __asm__("vf1");__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=&j"(normalized):"j"(a),"j"(one));v->q=normalized;}
__asm__("mtc1 %1,%0":"=f"(r):"r"(one));return r;}
/** @sly2-annot addr=0x205670
 * Purpose: If grounded (+0x208), intersect segment from->to with the 2 footprint triangles (0x2057E8); return
 *   hit point
 * Info: proposed name FIntersectStepFootprint (T2); TU step
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
int func_00205670(struct P*p,qword from,qword to,qword*output)
{
 V tri[6],d,k;
 float distance,length;
 register qword *out __asm__("s4")=output;
 register V *t __asm__("s1");
 register V *third __asm__("s0");
 register V *end __asm__("s2");
 register float *distanceOut __asm__("s3");
 if(!p->enabled)return 0;
 func_002057E8(p,tri);
 d.q=Sub(&to,&from);
 length=Normal(&d,&D_002A2DC0);
 distanceOut=&distance;third=&tri[2];t=tri;end=tri+6;
 do{
   int hit=FIntersectRayTriangle(d.q,from,*(qword*)((char*)third-32),*(qword*)((char*)third-16),third->q,distanceOut,0);
   if(distance<=length && (hit&1)){
      *out=Add(from,Scale(d.q,distance,&k));return 1;
   }
   t+=3;third+=3;
 }while(t<end);
 return 0;
}
