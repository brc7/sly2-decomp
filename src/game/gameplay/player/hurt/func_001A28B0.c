typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int i[4]; } U;
struct M3 { qword r[3]; };
struct VE { short d; short pad; void *pfn; };
struct X { char pad[0x80]; U v80; };
struct SMP { float a,b,c,d; };
struct P {
 char pad[0xC]; struct VE *vt;
 char pad1[0xE8-0x10]; struct X *xE8;
 char pad2[0x190-0xEC]; qword v190;
 char pad3[0x1B0-0x1A0]; struct M3 m1B0;
 char pad4[0x2DC-0x1E0]; int state2DC;
 char pad5[0xDF4-0x2E0]; int stateDF4;
 char pad6[0x10A0-0xDF8]; float height10A0, alpha10A4;
 struct SMP smp10A8;
};
struct CLOCK { float t,dt; };
extern struct CLOCK D_002DDED0;
int func_001A2788(struct P *p,void *o,qword *pos);
void func_00228C18(void *o,qword pos,qword *dir,qword *v);
void func_002297F8(struct P *p,void *o);
qword func_001A62E8(struct P *p);
float func_0011D1C0(float pos,float vel,float target,float dt,struct SMP *s,float *result);
void AddSoAcceleration(struct P *p,qword v);
static inline qword GetVel(struct P *p) { return ((qword (*)(void *))p->vt[33].pfn)((char *)p+p->vt[33].d); }
static inline qword Mul(struct M3 *m,const qword *v) { qword r; __asm__("vmulax.xyz ACC,%2,%1x\n\tvmadday.xyz ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z" : "=j"(r) : "j"(*v),"j"(m->r[0]),"j"(m->r[1]),"j"(m->r[2]));return r; }
static inline qword Add(qword a,qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b));return r; }
static inline qword Sub(qword a,qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b));return r; }
static inline qword Broadcast(int g) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(g));return r; }
static inline qword Scale(qword v,qword k) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(v),"j"(k));return r; }
/** @sly2-annot addr=0x1A28B0
 * Purpose: STEPPLAYER vt72 (shared by JT/Murray): in water/vertical states apply water current
 *   (CalculateWaterCurrent) and buoyancy (GSmoothA, AddSoVelocity); else SO slot72 0x2297F8
 * Info: proposed name ResolveStepplayerXaContact (T2); class STEPPLAYER slot72; TU hurt
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A28B0(struct P *p,void *o)
{
 U origin,vel,pos,local,dir,v50,accel,bcast;
 float z,g;
 qword v,k;
 struct CLOCK *clock;
 if(p->state2DC==5) goto vertical;
 switch(p->stateDF4) {
 case 7:
 vertical:
  origin.q=GetVel(p);
  if(origin.f[2]<0.0f) func_002297F8(p,o);
  break;
 case 3:case 10:
  if(!func_001A2788(p,o,&pos.q)) break;
  local.q=Mul(&p->m1B0,&p->v190);
  func_00228C18(o,pos.q,&dir.q,&v50.q);
  v=GetVel(p);
  vel.q=v;
  v=Add(v,local.q);
  v=Sub(v,dir.q);
  k=Broadcast(*(int *)&p->alpha10A4);
  bcast.q=k;
  v=Scale(v,k);
  accel.q=v;
  origin.q=func_001A62E8(p);
  clock=&D_002DDED0;
  func_0011D1C0(origin.f[2]+p->height10A0,vel.f[2]-dir.f[2],pos.f[2],clock->dt,&p->smp10A8,&z);
  g=(z-vel.f[2])/clock->dt-p->xE8->v80.f[2];
  accel.f[2]=accel.f[2]>g?accel.f[2]:g;
  AddSoAcceleration(p,accel.q);
  break;
 default:
  func_002297F8(p,o);
  break;
 }
}
