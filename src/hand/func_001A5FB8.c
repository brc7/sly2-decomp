typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int i[4]; } U;
struct M4 { U r[4]; };
struct CAMERA { char pad[0x90]; qword pos; };
extern struct CAMERA D_002DDF40;
qword *func_00256D18(qword *p, qword v);
void func_001A5EE8(void *p, struct M4 *m, float a, float b);
static inline qword Sub(qword a,qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b)); return r; }
static inline qword Cross(qword a,qword b) { qword r; __asm__("vopmula.xyz ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1" : "=j"(r) : "j"(a),"j"(b)); return r; }
static inline qword LenV(qword v) { qword l,t; __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(l),"=&j"(t) : "j"(v)); __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(l) : "0"(l));return l; }
static inline float GetX(qword v) { float g; __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(v));return g; }
static inline qword Normalize(qword v) { qword l=LenV(v),r; __asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q" : "=j"(r) : "j"(l),"j"(v));return r; }
static inline qword Broadcast(float g) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(g));return r; }
static inline qword Scale(qword v,qword k) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(v),"j"(k));return r; }
/** @sly2-annot addr=0x1A5FB8
 * Purpose: Build a camera-facing matrix along segment a->b (width scaling, 0.01 epsilon, uses g_cm pos
 *   D_002DDF40+0x90) and apply via 0x1A5EE8
 * Info: proposed name SetLoBeamXf (T2); TU lo
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090+verdicts_B
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A5FB8(void *p,qword a,qword b,float width,float scale)
{
 U x,y,z;
 struct M4 m; struct M4 *pm;
 U uk;
 qword d,n,t,k;
 float len;
 d=Sub(b,a);
 n=Cross(Sub(a,D_002DDF40.pos),d);
 y.q=n;
 len=GetX(LenV(n));
 if(0.01f<len) {
  len=width/len;
  pm=&m; t=Normalize(Cross(n,d));
  x.q=t;
  k=Broadcast(len);
  uk.q=k;
  n=Scale(n,k);
  y.q=n;
  k=Broadcast(0.01f);
  uk.q=k;
  z.q=Scale(d,k);
  func_00256D18(&m.r[3].q,a);
  pm->r[0].q=x.q;
  pm->r[1].q=y.q;
  pm->r[2].q=z.q;
  m.r[0].i[3]=m.r[1].i[3]=m.r[2].i[3]=0;
  pm->r[3].f[3]=1.0f;
  func_001A5EE8(p,pm,scale,scale);
 }
}
