typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct M4 { qword r[4]; };
extern const qword D_002BE570;
static inline qword Transform(const struct M4 *m,qword p)
{
 qword r;
 __asm__("vmulax.xyz ACC,%1,%3x\n\tvmadday.xyz ACC,%2,%3y\n\tvmaddaz.xyz ACC,%4,%3z\n\tvmaddw.xyz %0,%5,$vf0w"
 :"=j"(r):"j"(m->r[0]),"j"(m->r[1]),"j"(p),"j"(m->r[2]),"j"(m->r[3]));
 return r;
}
static inline qword Sub(qword a,qword b) { qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r; }
static inline float Length(qword v)
{
 qword d,one;float f;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
 :"=j"(d),"=&j"(one):"j"(v));
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"j"(d));
 __asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;
}
static inline qword Broadcast(float f) { qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r; }
static inline qword MAdd(qword p,qword v,qword k) {qword r;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(p),"j"(v),"j"(k));return r; }
/** @sly2-annot addr=0x167DA0
 * Purpose: Grows a sub/model cull sphere to include its projection through pshd->pmatClip (projected-volume
 *   geometry)
 * Info: proposed name ExpandSphereForProjection (T2); TU glob
 * Notes: no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
qword func_00167DA0(const struct M4 *m,qword p,float *radius)
{
 U u;
 qword d=Sub(Transform(m,D_002BE570),p);
 float len=Length(d);
 if(*radius<len) {
  float ratio=(len-*radius)*0.5f/len;
  *radius=(len+*radius)*0.5f;
  u.q=Broadcast(ratio);
  p=MAdd(p,d,u.q);
 }
 return p;
}
