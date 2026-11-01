// func_0018FC60 MATCH (Claude agent lane H15), from laneH15v4 + a "memory" clobber on the dt empty asm with base assigned after it (orders the point/spin loads after the dt spill store). laneH15v4: laneH14v1 + "0V" on the dt empty asm (input stays a MEM, reload lw goes straight into the output reg), allocation-only 2nd "j" on the radius scale, QwDivideBy(g first) for the final divide, and the 2nd clamp written out in the body (g=g0; g+=...; lo/hi locals; if/else-if in place): aligned 1.
// func_0018FC60 laneH14v1 (Claude agent lane H14): laneH11v1 with `state.rotation=func_0011B570(...)` assigned directly (no M3 rotation local) and QwRotateVec(&state.rotation,...): the 11B570 return temp at 0x190 is then freed and reused by the later VU_FLOAT temps like the target (0x1C0 before). Still aligned 30 (those stores still differ in vf number: rotation rows vf9/8/7 vs target vf10/9/8).
// func_0018FC60 laneH11v1 (Claude agent lane H11): laneH10v3 + `speed*0.5f*speed`, p->other block dt as a GPR qword via an empty asm, QwAddScaled2Q(qword k) with allocation-only extra "j"(k), base declared before dt (47 -> 30).
// func_0018FC60 laneH10v3 (Claude agent lane H10): laneH10v2 + GMinOf/GMaxOf written (b<a)?b:a / (b>a)?b:a (min.s/max.s operand order): aligned 47.
// func_0018FC60 laneH9v1 (Claude agent lane H9): laneH8v1 + const-ref QwDivide, QwSub, QwProjectOut and QwCombo(a) (134 -> 75).
/* laneH8v1 (Claude agent): fresh Lilly-style rewrite of func_0018FC60, aligned 134 (old .cpp 162), frame 0x2A0 exact, no pins, VU helper asms only. */
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
struct M3{U r[3];M3(){}M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return*this;}};
struct M4{U r[4];};
struct VTE{short delta,pad;void*fn;};
struct NODE{char pad0[0xC];VTE*vt;char pad10[0x124];float property;};
struct ACCEL{char pad0[0x80];U value;};
struct LINK{int header;NODE*object;};
struct DELTA{U first,second;};
struct P{char pad0[0xC];VTE*vt;char pad10[0xD8];ACCEL*accel;char padEC[0x17D8];NODE*target;char pad18C8[8];int state;float stamp;char pad18D8[0x24];int reverse;LINK*other;char pad1904[0xC];DELTA response[2];};
struct CLQ{float g0,g1,g2;};
struct LM{float gMin,gMax;};
struct SMPA{float a,b,duration,limit;};
struct CLOCK{float t,dt;};
struct STATE{U direction,position,velocity;M3 rotation;};
struct VU_FLOAT{qword qw;};
extern "C"{
qword func_001A62E8(void*);
M4*func_001A6318(void*);
qword func_0022D348(void*,void*,qword);
float func_0011E290(qword,qword,qword*,qword*);
float func_0011D1C0(float,float,float,float,SMPA*,float*);
M3 func_0011B570(qword);
void func_0018F958(P*,NODE*,qword*,qword*,DELTA*);
extern U D_002A2DC0,D_002A2DA0,D_002A2DB0,D_002A2DD0;
extern CLQ D_002BF4F0,D_002BF520;
extern LM D_002BF500,D_002BBB60;
extern float D_002BF508,D_002BF50C,D_002BF510,D_002BF514,D_002BF540,D_002BF544;
extern SMPA D_002BF530;
extern CLOCK D_002DDED0;
}
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g));return v;}
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));return v;}
static inline qword QwAdd(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwSub(const qword&a,const qword&b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwScaleX(qword a,VU_FLOAT g){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(g.qw));return r;}
static inline qword QwAddScaled(qword base,qword vec,VU_FLOAT k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(vec),"j"(k.qw));return r;}
static inline qword QwDot(qword a,qword b){qword r,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=j"(one):"j"(a),"j"(b));return r;}
static inline qword QwSqrt(qword a){qword r;__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"j"(a));return r;}
static inline qword QwDivide(const qword&a,const qword&g){qword r;__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=j"(r):"j"(a),"j"(g));return r;}
static inline qword QwDivideBy(const qword&a,const qword&g){qword r;__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(g),"j"(a));return r;}
static inline float GXOfQw(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw));return x;}
static inline qword QwCross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwRotateVec(const M3*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%2,%1x\n\tvmadday.xyz $ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z":"=j"(r):"j"(v),"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q));return r;}
static inline qword QwCombo(const qword&a,const qword&b,VU_FLOAT ka,VU_FLOAT kb){qword r;__asm__("vmulax.xyzw $ACC,%3,%1x\n\tvmaddx.xyzw %0,%4,%2x":"=j"(r):"j"(ka.qw),"j"(kb.qw),"j"(a),"j"(b));return r;}
static inline qword QwAddScaled2(qword base,qword a,qword b,VU_FLOAT k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddax.xyzw $ACC,%2,%4x\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r):"j"(base),"j"(a),"j"(b),"j"(k.qw));return r;}
static inline qword QwScaleXK(qword a,VU_FLOAT g){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(g.qw),"j"(g.qw));return r;} /* second "j"(g) is allocation-only */
static inline qword QwProjectOut(const qword&v,const qword&axis){VU_FLOAT d;d.qw=QwDot(v,axis);return QwSub(v,QwScaleX(axis,d));}
static inline float GXOfVf(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw),"j"(qw));return x;} /* allocation-only "j": the value stays in a VF register */
static inline qword NormalizeAt(U*pv,const U*pfallback){qword len=QwSqrt(QwDot(pv->q,pv->q));if(GXOfVf(len)<0.0001f)*pv=*pfallback;else pv->q=QwDivide(pv->q,len);return len;}
static inline float GLength(qword v){return GXOfQw(QwSqrt(QwDot(v,v)));}
static inline float GEvaluateClq(CLQ*p,float x){return p->g0+x*(p->g1+x*p->g2);}
static inline float GLimitG(float g,float gMin,float gMax){if(g<gMin)return gMin;if(gMax<g)return gMax;return g;}
static inline float GMinOf(float a,float b){return(b<a)?b:a;}
static inline float GMaxOf(float a,float b){return(b>a)?b:a;}
static inline qword Velocity(void*p){VTE*e=&((NODE*)p)->vt[33];return((qword(*)(void*))e->fn)((char*)p+e->delta);}
static inline void*Space(NODE*p){VTE*e=&p->vt[32];return((void*(*)(void*,void*))e->fn)((char*)p+e->delta,0);}
static inline qword QwNormalize(qword v,qword fallback){qword len=QwSqrt(QwDot(v,v));qword r;if(GXOfQw(len)<0.0001f)r=fallback;else r=QwDivide(v,len);return r;}
static inline CLOCK*Addr(CLOCK*p){return p;}
static inline qword QwAddScaled2Q(qword base,qword a,qword b,qword k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddax.xyzw $ACC,%2,%4x\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r):"j"(base),"j"(a),"j"(b),"j"(k),"j"(k));return r;} /* second "j"(k) is allocation-only: makes k VF-preferred so it is spilled across the call (target 0x1F0) */
/** @sly2-annot addr=0x18FC60
 * Purpose: Compute swing acceleration while hanging from a hook (pendulum around the HND, GSmoothA on angle,
 *   LoadRotateMatrix) and call AnticipateJtForce
 * Info: proposed name CalculateJtHangAccel (T1); TU hang
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
extern "C" void func_0018FC60(P*p){
 U anchor,displacement,anchorVelocity,axis;
 anchor.q=func_001A62E8(p->target);
 M3 anchorBasis=*(M3*)func_001A6318(p->target);
 anchorVelocity.q=func_0022D348(Space(p->target),0,anchor.q);
 axis.q=QwSub(func_001A62E8(p),anchor.q);
 displacement=axis;
 qword length=NormalizeAt(&axis,&D_002A2DC0);
 float distance=GXOfVf(length);
 U relative,perpendicular,parallel;
 relative.q=QwSub(Velocity(p),anchorVelocity.q);
 float radial=func_0011E290(relative.q,axis.q,&parallel.q,&perpendicular.q);
 U firstAxis,angularFirst;
 firstAxis.q=QwNormalize(QwCross(axis.q,perpendicular.q),D_002A2DA0.q);
 float ratio=GLength(perpendicular.q)/distance;
 angularFirst.q=QwScaleX(firstAxis.q,VuFloat(ratio));
 U projected;
 projected.q=QwProjectOut(p->accel->value.q,axis.q);
 U secondAxis,angularSecond;
 secondAxis.q=QwNormalize(QwCross(axis.q,projected.q),D_002A2DB0.q);
 angularSecond.q=QwScaleX(secondAxis.q,VuFloat(GLength(projected.q)/distance));
 float drag=GLimitG(GEvaluateClq(&D_002BF4F0,ratio),D_002BF500.gMin,D_002BF500.gMax);
 float bias;
 if(p->state==0||p->state==2)bias=D_002BF508;else bias=D_002BF50C;
 U angular,parallelAngular,perpendicularAngular;
 angular.q=QwAddScaled(angularFirst.q,angularSecond.q,VuFloatFromMemWord(*(int*)&Addr(&D_002DDED0)->dt));
 func_0011E290(angular.q,anchorBasis.r[1].q,&parallelAngular.q,&perpendicularAngular.q);
 angular.q=QwCombo(parallelAngular.q,perpendicularAngular.q,VuFloat(drag*Addr(&D_002DDED0)->dt+1.0f),VuFloat(bias*Addr(&D_002DDED0)->dt+1.0f));
 if((p->state==0||p->state==2)&&D_002DDED0.t-p->stamp<D_002BF510){
  float amount=Addr(&D_002DDED0)->dt*D_002BF514;
  if(p->state==0)amount=-amount;
  if(p->reverse)amount=-amount;
  float down=-p->accel->value.f[2];
  float speed=GLength(perpendicular.q);
  float t=(down*displacement.f[2]+speed*0.5f*speed)/down;
  float g=D_002BF520.g0;g+=t*(D_002BF520.g1+t*D_002BF520.g2);float lo=D_002BBB60.gMin,hi=D_002BBB60.gMax;if(g<lo)g=lo;else if(hi<g)g=hi;amount*=g;
  angular.q=QwAddScaled(angular.q,anchorBasis.r[1].q,VuFloat(amount));
 }
 float speedOut;
 float next=func_0011D1C0(distance,radial,D_002BF544-p->target->property,Addr(&D_002DDED0)->dt,&D_002BF530,&speedOut);
 angular.q=QwScaleX(angular.q,VuFloat(GMinOf(D_002BF544,distance)/GMaxOf(D_002BF540,next)));
 U velocity;
 STATE state;
 if(p->other){
  NODE*object=p->other->object;
  qword base;
  qword dt;__asm__("":"=r"(dt):"0V"(*(int*)&D_002DDED0.dt):"memory"); /* empty asm, see below: the "memory" clobber keeps the point/spin/base loads after the dt store (target order) */
  base=anchorVelocity.q; /* empty asm: the dt bits as a GPR qword (target lw; sq, no qmtc2). The "V" alternative is allocation-only (never chosen: the operand is offsettable), it keeps the MEM in the asm so reload loads it straight into the output reg */
  qword point=anchor.q,spin=D_002A2DD0.q;
  state.direction=D_002A2DD0;state.position=D_002A2DD0;state.velocity.q=dt;
  qword arm=QwSub(point,func_001A62E8(object));
  velocity.q=QwAddScaled2Q(base,spin,QwCross(spin,arm),dt);
 }else velocity=anchorVelocity;
 state.rotation=func_0011B570(QwScaleX(QwAdd(angularFirst.q,angular.q),VuFloat(Addr(&D_002DDED0)->dt*0.5f)));
 state.direction.q=QwRotateVec(&state.rotation,axis.q);
 qword radius=QwScaleXK(state.direction.q,VuFloat(next));
 state.position.q=QwAdd(anchor.q,radius);
 qword final=QwAdd(QwAdd(velocity.q,QwCross(angular.q,radius)),QwScaleX(state.direction.q,VuFloatFromMemWord(*(int*)&speedOut)));
 state.velocity.q=final;
 qword current=Velocity(p);
 projected.q=QwSub(QwDivideBy(QwSub(final,current),VuFloatFromMemWord(*(int*)&Addr(&D_002DDED0)->dt).qw),p->accel->value.q);
 if(p->other)func_0018F958(p,p->other->object,&anchor.q,&projected.q,p->response);
 else p->response[0].first=projected;
}
