/* Find the horizontal contact normal along a steep/shallow face boundary. */
typedef int qword __attribute__((mode(TI)));
struct JT;
union VECTOR {qword q;float f[4];int n[4];};
struct FACE {qword normal;int pad[4];};
struct GEO {int n;VECTOR *points;int k;FACE *faces;};
struct SO {char pad[0x90];GEO geom;};
struct EDGE {unsigned short a,b,faceA,faceB;};
extern qword D_002A2DA0;
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline void Normal(VECTOR*v,qword fallback){qword a=v->q;register qword d __asm__("vf1");qword one;float g;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));if(g<0.0001f){a=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%0,$Q":"=j"(a):"j"(d),"0"(a));}v->q=a;}
static inline float Dot(qword a,qword b){qword d,one;float g;__asm__("vmul.xyz %0,%3,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"0"(b),"j"(a));__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(d));return g;}
static inline qword Scale(qword a,float f,VECTOR&k){qword q,r;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));k.q=q;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(q));return r;}
/** @sly2-annot addr=0x199408
 * Purpose: (existing) For an edge between a walkable (n.z>0.7) and a wall face (n.z<0.55) compute the grab
 *   direction
 * Info: proposed name FCheckJtSingleEdge__FP2JTP2SOP4EDGEP6VECTOR (EXISTING); TU air
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
int FCheckJtSingleEdge(JT*,SO*so,EDGE*e,VECTOR*out){
 GEO*geom=&so->geom;FACE*chosen;
 VECTOR d,r,k;
 FACE*a=&geom->faces[e->faceA];FACE*b=&geom->faces[e->faceB];
 if(((float*)&a->normal)[2]>0.7f && ((float*)&b->normal)[2]<0.55f)chosen=b;
 else if(((float*)&a->normal)[2]<0.55f && ((float*)&b->normal)[2]>0.7f)chosen=a;
 else return 0;
 d.q=Sub(geom->points[e->b].q,geom->points[e->a].q);
 r.n[2]=0;
 r.f[1]=d.f[0];r.f[0]=-d.f[1];
 Normal(&r,D_002A2DA0);
 if(Dot(r.q,chosen->normal)<0.0f)r.q=Scale(r.q,-1.0f,k);
 *out=r;
 return 1;
}
