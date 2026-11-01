typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int i[4]; } U;
struct VE { short d; short pad; void *pfn; };
struct X { char pad[0x80]; qword v80; };
struct O { char pad[0xC]; struct VE *vt; char pad1[0xE8-0x10]; struct X *xE8; char pad2[0x198-0xEC]; float f198; void Set(qword v) __asm__("func_001FAC30"); void Negate(U &u); };
struct P { struct O *o; int state; };
extern "C" qword D_002A2DD0;
extern "C" float D_002BFA90[];
struct CLOCK { float t,dt; };
extern "C" struct CLOCK D_002DDED0;

extern "C" void func_001F5E10(struct O *p,qword a,int n,qword b,float *c);
static inline qword Broadcast(float g) { qword q; __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(g));return q; }
static inline qword BroadcastTo(float g,U *u) { qword q=Broadcast(g); u->q=q;return q; }
static inline qword Scale(qword a,qword b) { qword q; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(q) : "j"(a),"j"(b));return q; }
static inline float GetZ(qword v) { float g; __asm__("pextuw %0,$0,%1" : "=r"(g) : "r"(v));return g; }
inline void O::Negate(U &u) { qword v=xE8->v80; qword k=Broadcast(-1.0f); u.q=k; Set(Scale(v,k)); }
/** @sly2-annot addr=0x1A1148
 * Purpose: Hurt-state acceleration: in knockback states add velocity / spring toward target velocity
 *   (AccelSoTowardPosSpring)
 * Info: proposed name PresetJtAccelZap (T1); TU hurt
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
extern "C" void func_001A1148(struct P *p)
{
 U u,v,u2;
 qword k;
 float zero;
 switch(p->state) {
 case 1:case 2:
  p->o->Negate(u);
  func_001F5E10(p->o,D_002A2DD0,0,D_002A2DD0,D_002BFA90);
  break;
 case 5:case 11:
  p->o->Negate(u2);
  v.i[2]=0;v.i[1]=0;v.i[0]=0;
  zero=v.f[2];
  if(p->o->f198!=zero)
   v.f[2]=GetZ(((qword (*)(void *))p->o->vt[33].pfn)((char *)p->o+p->o->vt[33].d));
  func_001F5E10(p->o,D_002A2DD0,0,v.q,D_002BFA90);
  if(p->o->f198!=zero) {
   u.f[2]=zero;u.f[1]=zero;u.f[0]=zero;
   u.f[2]=(-p->o->f198-v.f[2])/D_002DDED0.dt;
   p->o->Set(u.q);
  }
  break;
 }
}
