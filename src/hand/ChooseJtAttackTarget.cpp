// ChooseJtAttackTarget (Claude agent, lane G14, 2026-10-24): MATCH. laneG12v1 (aligned 2, s4/s5 swap of the hoisted D_002BF4B8 / D_003DD490 highs) plus const-ref helpers that add RTL insns which vanish after loop opt: Sub(const qword&,const qword&), AbsF(const float&), IsBonus(P*const&), and a `qword pred` copy. The node loop now has 96 real insns in loop pass 1 (> threshold 95, so only the life-2 bonus high is hoisted) and 91 in pass 2 (the D_003DD490 high is hoisted then), which gives the target's s5/s4 order. One commented register pin (heading in f21).
// history, laneG12v1: laneG11v1 with the abs.s asm replaced by __builtin_fabsf (same aligned 2). Was laneG11v1 (Claude agent, 2026-10-24): laneG10v1 (4) + an early `qword early=QwFromF(dt)` before the 11B500 call. QwFromF's mfc1 has a "memory" clobber so sched1 keeps it ahead of the call (the call depends on it), and it is defined on the SAME source line as VuFloatBits so cse treats the two asm_operands as equal (the line number is part of ASM_OPERANDS) and the later VuFloatBits reuses the early value, leaving only its dead store into the inline's v (0xD0) like the target. D_002A2DC0 is const (keeps its lq ahead of the memory clobber) and M3::operator= copies r[0],r[2],r[1]. aligned 2, ONE pin (heading f21). Left: s4/s5 swap of the hoisted D_003DD490/D_002BF4B8 high parts (target loop.c hoists the D_002BF4B8 hi first, so it lives one insn longer and is allocated second).
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int bits[4];};
struct M3{U r[3];M3(){}M3(const M3&m){r[0]=m.r[0];r[2]=m.r[2];r[1]=m.r[1];}M3&operator=(const M3&m){r[0]=m.r[0];r[2]=m.r[2];r[1]=m.r[1];return *this;}};
struct M4{U r[4];};
struct VTE{short delta,pad;void*fn;};
struct P{char pad[0xC];VTE*vt;char pad10[0xD0];P*next;int padE4;int flags;};
struct G;
extern "C" const qword D_002A2DC0;
extern "C" P*D_003E13B4;
extern "C" G D_003DD490;
extern "C" float D_002BF4B8;
extern "C" M3 func_0011B500(qword,float);
extern "C" qword func_001A62E8(P*);
extern "C" qword ConvertAloPos(P*,void*,qword);
extern "C" void LoadIdentityMatrix4(void*,void*,M4*,float,M4*);
extern "C" void FindClosestPointOnLineSegment(qword,qword,qword,qword*,float*,float*);
extern "C" int PloFindSwObjectByClass__FP2SWi3CIDP2LO(G*,int,int,P*);
extern "C" float func_002458F0(float,float);
extern "C" float RadNormalize__Ff(float);
struct VU_FLOAT{qword qw;};
static inline VU_FLOAT VuFloatBits(float g){VU_FLOAT v;__asm__("mfc1 %0,%1":"=r"(v.qw):"f"(g));return v;} static inline qword QwFromF(float g){qword q;__asm__("mfc1 %0,%1":"=r"(q):"f"(g):"memory");return q;}
static inline float AngleOf(U v){return func_002458F0(v.f[1],v.f[0]);}
static inline qword AddScaled(qword p,qword v,VU_FLOAT k){qword q;__asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %0,%3,%2x":"=j"(q):"j"(p),"j"(k.qw),"j"(v),"j"(k.qw)/*allocation-only: VU preference spills k like the target*/);return q;}
static inline qword Rotate(const M3&m,qword v){qword q;__asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(q):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"0"(v));return q;}
static inline qword Transform(const M4&m,qword v){qword q;__asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(q):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(m.r[3].q),"j"(v));return q;}
static inline qword Add(qword a,qword b){qword q;__asm__("vadd.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline qword Sub(const qword &a,const qword &b){qword q;__asm__("vsub.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline qword GetVelocity(P*p){VTE*e=&p->vt[33];qword(*f)(void*)=(qword(*)(void*))e->fn;return f((char*)p+e->delta);}
static inline void*GetFrame(P*p,M4*out){return ((void*(*)(void*,M4*))p->vt[32].fn)((char*)p+p->vt[32].delta,out);}
static inline float AbsF(const float &f){return __builtin_fabsf(f);}
static inline int IsBonus(P*const &n){return PloFindSwObjectByClass__FP2SWi3CIDP2LO(&D_003DD490,3,0x33,n);}
extern "C" void ChooseJtAttackTarget(P*p,float headingInput,int mask,float dtInput,U*offset,P**out,qword*outvector,float cone){
 register float heading __asm__("$f21")=headingInput; float dt=dtInput; // the one pin: heading in f21 (else f20/f21 swap)
 M3 rotation;
 U origin,reference,target,delta,candidate;
 M4 prediction;
 U answer;
 P*best=0;
 float nearest=300.0f;
 qword position;
 qword early=QwFromF(dt); // computed before the 11B500 call; cse merges the VuFloatBits(dt) below into it
 rotation=func_0011B500(D_002A2DC0,headingInput);
 position=func_001A62E8(p);
 VU_FLOAT scalar=VuFloatBits(dt);
 origin.q=AddScaled(position,GetVelocity(p),scalar);
 reference.q=ConvertAloPos(p,0,offset->q);
 target.q=Add(Rotate(rotation,offset->q),origin.q);
 delta.q=Sub(target.q,reference.q);
 for(P*node=D_003E13B4;node;node=node->next){
  struct{U hdr;M4 m;}fx;
  float t,distance;
  if(!(node->flags&mask))continue;
  candidate.q=func_001A62E8(node);
  void*owner=GetFrame(node,&fx.m);
  LoadIdentityMatrix4(owner,0,&prediction,dt,0);
  FindClosestPointOnLineSegment(target.q,candidate.q,Transform(prediction,fx.m.r[3].q),0,&t,&distance);
  if(IsBonus(node))distance+=D_002BF4B8;
  if(distance<nearest){
   qword other=candidate.q;
   U scratch;scratch.q=Sub(other,func_001A62E8(p));
   float angle=AbsF(RadNormalize__Ff(func_002458F0(scratch.f[1],scratch.f[0])-heading));
   if(cone<angle)continue;
   best=node;nearest=distance;
   qword pred=prediction.r[3].q; answer.q=Sub(pred,origin.q);
  }
 }
 *out=best;
 *outvector=answer.q;
}
