// func_0019AC88: speed-limited trajectory time/length helper. Builds a path with func_00185D18 (3 outputs), integrates
// the velocity with func_00117120 at t=0 and t=t, and returns min(1e6, (|v0|+|v1|) * t * 0.5).
// Trick: the halved time goes into a NEW local (h), not back into t. Overwriting t adds an anti-dependence to the
// `mov.s $f13,t` argument move, which then outranks the por a1/a2 moves in sched1 and ends up before them.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct X { char pad[0x80]; qword v80; };
struct O { char pad[0xE8]; struct X *xE8; char pad2[0x1F30-0xEC]; float f1F30; };
struct A { struct O *p; };
struct P { struct A *a; void *p4; int pad[2]; qword v10; };
qword func_001A62E8(void *p);
int func_00185D18(void *a, qword p, qword v, qword x, float f, float t, qword *r0, qword *r1, qword *r2);
void func_00117120(float tMax, float t, float s, qword a, qword va, qword b, qword vb, int m, int n, qword *r);
static inline float Len(qword v) {
 qword l, one; float r;
 __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(l), "=&j"(one) : "0"(v));
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(l) : "0"(l));
 __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(l));
 return r;
}
/** @sly2-annot addr=0x19AC88
 * Purpose: Callback: speed-limited path length/time to a target along a Bezier path (0x185D18), min(1e6,
 *   (v0+v1)*t/2)
 * Info: proposed name GMeasureJumpPath (T2); TU air
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
float func_0019AC88(struct P *p, float t)
{
 U a,b,c,d,e;
 qword pos,vel;
 struct O *o;
 void *pv;
 float g,h;
 pv=p->p4;
 o=p->a->p;
 pos=func_001A62E8(o);
 vel=p->v10;
 func_00185D18(pv, pos, vel, o->xE8->v80, o->f1F30, t, &a.q, &b.q, &c.q);
 func_00117120(t,0.0f,1.0f,pos,vel,b.q,c.q,0,0,&d.q);
 g=Len(d.q);
 func_00117120(t,t,1.0f,pos,vel,b.q,c.q,0,0,&e.q);
 g += Len(e.q);
 h = t*0.5f;
 g *= h;
 return g<1000000.0f?g:1000000.0f;
}
