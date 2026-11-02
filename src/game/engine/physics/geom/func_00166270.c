typedef int qword __attribute__((mode(TI)));
struct M4 { qword r[4]; };
struct N { qword normal; char pad[0xC]; int index; };
struct Geom { int count; qword *points; int countN; struct N *normals; int x10,x14; };
struct REQ { int cb,kind; };
void *func_001AE238(struct REQ *r,void *src);
void InitPlaneFromNormalPos(struct N *n,qword v,qword p);
static inline qword Transform(const struct M4 *m,qword p) {
 qword r;
 __asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
 :"=j"(r):"j"(m->r[0]),"j"(m->r[1]),"j"(m->r[2]),"j"(m->r[3]),"j"(p));return r;
}
static inline qword Rotate(const struct M4 *m,qword p) {
 register qword a0 __asm__("vf6")=m->r[0];
 register qword a1 __asm__("vf5")=m->r[1];
 register qword a2 __asm__("vf4")=m->r[2];
 __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
 :"+j"(p):"j"(a0),"j"(a1),"j"(a2));return p;
}
static inline qword Normalize(qword v) {
 qword t,r; register qword l __asm__("vf2");
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(l),"=&j"(t):"j"(v));
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(l):"0"(l));
 __asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(l),"j"(v));return r;
}
/** @sly2-annot addr=0x166270
 * Purpose: Copies a GEOM (points + surface normals) into the SW heap and optionally transforms the points by
 *   a matrix and rotates/renormalizes the normals, recomputing plane offsets
 * Info: proposed name CloneGeom (T1); TU geom
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#7-source-files,
 *   docs/engine/physics-collision.md#4-geometry-and-queries, docs/engine/physics-collision.md#6-source-files
 * @end-sly2-annot */
void func_00166270(struct Geom *src,const struct M4 *transform,struct Geom *dest)
{
 struct REQ req;
 struct M4 m;
 int i,j;
 dest->count=src->count;
 req.cb=src->count*16;req.kind=16;
 dest->points=func_001AE238(&req,src->points);
 dest->countN=src->countN;
 req.cb=src->countN*32;req.kind=16;
 dest->normals=func_001AE238(&req,src->normals);
 dest->x10=src->x10;dest->x14=src->x14;
 if(transform) {
  m=*transform;
  for(i=0;i<dest->count;i++){ qword *point=(qword *)((int)dest->points+(i<<4)); *point=Transform(&m,*point); }
  for(j=0;j<dest->countN;j++) {
   struct N *n=&dest->normals[j];
   register const struct M4 *pm __asm__("$17")=&m;
   InitPlaneFromNormalPos(n,Normalize(Rotate(pm,n->normal)),dest->points[n->index]);
  }
 }
}
