// func_00185E40 laneH20v1 (Claude agent lane H20): MATCH. laneH19v2 + loop 2 through an inline StoreAddRot(&samples[i],target,&p->basis,&p->points[i].q) that rotates inside its body. A nested inline call argument is expanded before the other arguments, so in H19v2 the points address came first in the loop body; now the samples address does, the giv list (prepended) is reversed, and loop.c emits the points init `addiu v1,s1,0x1F30` before `addiu v0,sp,0x160` like the target. One register pin (direction vf2), allocation-only operands commented.
// func_00185E40 laneH19v2 (Claude agent lane H19): v1 + loop 2 stores through an inline StoreAddM(&samples[i],a,b) (operator=-style, the address is an inline param) with an allocation-only "m"(d->q) input. gcse no longer shares the loop base with the 1856C0 arg, so loop.c builds a fresh `addiu v0,sp,0x160` (no lw 0x250), the loop is still reversed, and the "m" ref keeps samples v0 / points v1. Left: the two giv inits come out samples-then-points (target points first), aligned 1 / 2 lines.
// func_00185E40 laneH19v1 (Claude agent lane H19): laneH18v3 + MulM3 through QwRotateVecR, a rotate helper with the matrix rows as operands %1..%3 and the vector last (like QwTransformPos). local-alloc ties an asm output to its operand 1 when it dies there, so c1 now reuses inv r0 (vf5) and the movement load schedules before c1 (13 -> 1). Only the loop-2 `lw 0x250` reload is left.
// func_00185E40 laneH18v3 (Claude agent lane H18): v2 + MulM3 c1 through QwRotateVecJ with allocation-only extra "j"(v),"j"(v),"j"(m->r[1].q) inputs: D1 and inv r1 get more refs, so local-alloc gives inv r0 vf5 / D1 vf4 like the target (14 -> 13; c1 output is now vf4, target vf5).
// func_00185E40 laneH17v3 (Claude agent lane H17): v2 + BuildM4L(m,pos,d) takes the frame pointer LAST, so pd=&frame is evaluated after the 11B830 call (addiu s0,sp,0x20 after the call like the target) (27 -> 25).
// func_00185E40 laneH16v5 (Claude agent lane H16): v4 + speed-block fallback as a qword value whose allocation-only "r" operand sits on the else-branch divide (QwDivideR) instead of the compare mtc1, so its lq is no longer forced before the compare (38 -> 36).
// func_00185E40 laneH16v4 (Claude agent lane H16): v3 + BuildM4 rows written through a caller-side pointer pd=&frame (a pseudo, so rows 1/2 stay s0-based like the target), the .i[3] zeros through frame and f[3]=1 through pd (41 -> 38).
// func_00185E40 laneH16v3 (Claude agent lane H16): v2 + movement rotated with QwRotateVec(&rotation,..) and a dead `M3 copy=rotation;` after the speed load (the 0x1F0 stores then follow the speed load like the target) (45 -> 41).
// func_00185E40 laneH16v2 (Claude agent lane H16): v1 + multiplier read as a float struct member through "rV"(float) (VuFloatFromMemF): the load is MEM_IN_STRUCT, so the 0xC0 temp stores no longer alias it and sched delays them like the target (50 -> 45).
// func_00185E40 laneH16v1 (Claude agent lane H16): laneH15v1 + speed written straight into a VU_FLOAT local through an inline LoadVuFloatFromMemWord(VU_FLOAT&,int) helper (no return temp pseudo, so global alloc gives len/dir vf1 and speed vf2 and the vmove is gone): 59 -> 50.
// func_00185E40 laneH15v1 (Claude agent lane H15): laneH13v3 with MulM3 in natural c0,c1,c2 order (the D_002DDFA0 address dies on the r2 load, so sched1 picks r2 first, then r0, r1 like the target): 62 -> 59.
// func_00185E40 laneH13v3 (Claude agent lane H13): v2 + one pseudo for len/dir in the movement normalize and allocation-only "r"(fallback) on its mtc1 (66 -> 62).
// func_00185E40 laneH9v1 (Claude agent lane H9): laneH7v5 + const-ref QwAdd and BuildM4 pos (78 -> 75).
/* laneH7v5 (Claude agent): fresh Lilly-style rewrite of func_00185E40, no pins, VU helper asms only. */
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
struct M3{U r[3];M3(){}M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return*this;}};
struct M4{U r[4];};
struct VTE{short delta,pad;void*fn;};
struct PARAMS{float a,b,duration;};
struct CONTROL{char pad0[0x10];float y,z;int pad18;float speed;};
struct REQUEST{char pad0[0x78];float multiplier;};
struct AXIS{float weight;int active;char pad8[8];U direction;};
struct TRIREF{int index;float u,v;};
struct LIST{int count;qword planes[8];};
struct P{char pad0[0xC];VTE*vt;char pad10[0x140];CONTROL*control;char pad154[0x3C];U acceleration;char pad1A0[0x10];M3 basis;char pad1E0[0x1C30];AXIS axes[8];REQUEST*request;int pad1F14;void*mesh;TRIREF triangle;U points[3];};
struct CLOCK{float time,dt;};
struct VU_FLOAT{qword qw;VU_FLOAT(){}VU_FLOAT(float f){__asm__("mfc1 %0,%1":"=r"(qw):"f"(f));}};
extern "C"{
extern U D_002A2DC0;extern M3 D_002DDFA0;extern CLOCK D_002DDED0;extern PARAMS D_002BE920,D_002BE930,D_002BE940;
qword func_001A62E8(P*);M4*func_001A6318(P*);
void func_00185310(void*,qword,TRIREF*,TRIREF*,qword*,qword*);
M3 BuildOrthonormalMatrixZ(qword,qword);M3 InvertMatrix3Cofactor(M3*);qword*VECTOR_opAssign(qword*,qword);
void func_0011C950(M4*,M3*,PARAMS*,M3*,qword*,float);
void func_0011F088(qword,qword,PARAMS*,qword*,float);
void func_001856B8(void*,LIST*);void func_001856C0(void*,LIST*,int,U*,PARAMS*,float);
qword func_00185A80(void*,LIST*,qword);
void SetJtJts__FP2JT3JTS4JTBS(P*,int,int);void func_001A0450(P*,AXIS*,REQUEST*);
}
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g));return v;}
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));return v;}
static inline VU_FLOAT VuFloatFromMemF(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(g));return v;}
static inline qword QwDivideR(qword a,qword g,const qword&f){qword r;__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=j"(r):"j"(a),"j"(g),"r"(f));return r;}
static inline void LoadVuFloatFromMemWord(VU_FLOAT&v,int n){__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"rV"(n));}
static inline qword QwAddP(const qword&a,const qword&b,U*s){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b),"r"(s));return r;}static inline qword QwAdd(const qword&a,const qword&b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwScaleX(qword a,VU_FLOAT g){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(g.qw));return r;}
static inline VU_FLOAT VuFloatC1(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.qw):"r"(g):"vf1");return v;} // clobber of the dead vf1: allocation only
static inline qword QwAddScaled(qword base,qword vec,VU_FLOAT k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(vec),"j"(k.qw));return r;}
static inline qword QwAddScaledJ(qword base,qword vec,VU_FLOAT k,qword f){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(vec),"j"(k.qw),"j"(f));return r;}
static inline qword QwAddScaledRef(qword base,const qword&vec,VU_FLOAT k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(base),"j"(vec),"j"(k.qw));return r;}
static inline qword QwDot(qword a,qword b){qword r,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=j"(one):"j"(a),"j"(b));return r;}
static inline qword QwSqrt(qword a){qword r;__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"j"(a));return r;}
static inline qword QwDivide(qword a,qword g){qword r;__asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"=j"(r):"j"(a),"j"(g));return r;}
static inline float GXOfQw(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw));return x;}
static inline float GXOfQwR(qword qw,const qword&f){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw),"r"(f));return x;}
static inline qword QwCross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwRotateVecAt(const M3*m,const qword*pv){qword r;__asm__("vmulax.xyz $ACC,%2,%1x\n\tvmadday.xyz $ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z":"=j"(r):"j"(*pv),"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q));return r;}
static inline qword QwRotateVecJ(const M3*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%2,%1x\n\tvmadday.xyz $ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z":"=j"(r):"j"(v),"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(v),"j"(v),"j"(m->r[1].q));return r;}static inline qword QwRotateVec(const M3*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%2,%1x\n\tvmadday.xyz $ACC,%3,%1y\n\tvmaddz.xyz %0,%4,%1z":"=j"(r):"j"(v),"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q));return r;}
static inline qword QwTransformPos(M4*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(v));return r;}
static inline qword QwCombo(qword a,qword b,VU_FLOAT ka,VU_FLOAT kb){qword r;__asm__("vmulax.xyzw $ACC,%1,%3x\n\tvmaddx.xyzw %0,%2,%4x":"=j"(r):"j"(a),"j"(b),"j"(ka.qw),"j"(kb.qw));return r;}
static inline void BuildM4L(const M3&m,const qword&pos,M4*d){VECTOR_opAssign(&d->r[3].q,pos);d->r[0]=m.r[0];d->r[1]=m.r[1];d->r[2]=m.r[2];}
static inline void BuildM4(M4*d,const M3&m,const qword&pos){VECTOR_opAssign(&d->r[3].q,pos);d->r[0]=m.r[0];d->r[1]=m.r[1];d->r[2]=m.r[2];}
static inline M3 operator*(const M3&a,const M3&b) return r{
 r.r[2].q=QwRotateVec(&a,b.r[2].q);r.r[0].q=QwRotateVec(&a,b.r[0].q);r.r[1].q=QwRotateVec(&a,b.r[1].q);
}
static inline void StoreQ(U*d,const qword&q){d->q=q;}
static inline void StoreAddM(U*d,const qword&a,const qword&b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b),"m"(d->q));d->q=r;} // "m"(d->q): allocation-only
static inline void StoreAddR(U*d,const qword&a,const qword&b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b),"r"(d));d->q=r;} // "r"(d): allocation-only
static inline void StoreAddRot(U*d,const qword&a,const M3*m,const qword*pv){StoreAddM(d,a,QwRotateVecAt(m,pv));}
static inline void ClearXyz(U*pv){pv->f[2]=0.0f;pv->f[1]=0.0f;pv->f[0]=0.0f;}
static inline qword QwRotateVecR(const M3*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(v));return r;}
static inline qword QwRotateVecRJ(const M3*m,qword v){qword r;__asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(v),"j"(v),"j"(v),"j"(m->r[1].q));return r;}
static inline void MulM3(M3*r,const M3*a,const M3*b){qword c0=QwRotateVecR(a,b->r[0].q),c1=QwRotateVecR(a,b->r[1].q),c2=QwRotateVecR(a,b->r[2].q);r->r[0].q=c0;r->r[1].q=c1;r->r[2].q=c2;}
static inline qword QwRotateVecV(qword v,M3 m){return QwRotateVec(&m,v);}
static inline void SetVec(P*p,int slot,qword q){VTE*e=&p->vt[slot];((void(*)(void*,qword))e->fn)((char*)p+e->delta,q);}
/** @sly2-annot addr=0x185E40
 * Purpose: Each frame keep Sly attached to the mesh surface: closest point, frame from normal, smoothed
 *   matrix/position, contact solve
 * Info: proposed name UpdateJtMeshAttach (T2); TU tu_185130
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
extern "C" void func_00185E40(P*p){
 U closest,normal;
 qword position=func_001A62E8(p);
 func_00185310(p->mesh,position,&p->triangle,&p->triangle,&closest.q,&normal.q);
 M4 frame;U contacts[3];
 qword difference=QwSub(closest.q,position);
 qword len=QwSqrt(QwDot(difference,difference));
 qword fallback=QwScaleX(normal.q,VuFloat(-1.0f));register qword direction asm("vf2"); // the one register pin: direction vf2
 if(GXOfQw(len)<0.0001f)direction=fallback;else direction=QwDivide(difference,len);
 qword target=QwAddScaled(closest.q,direction,VuFloat(-p->points[0].f[0]));
 M4*pd;BuildM4L(BuildOrthonormalMatrixZ(direction,D_002A2DC0.q),target,pd=&frame);frame.r[0].i[3]=frame.r[1].i[3]=frame.r[2].i[3]=0;pd->r[3].f[3]=1.0f;
 int i;
 for(i=0;i<3;i++)func_00185310(p->mesh,QwTransformPos(&frame,p->points[i].q),&p->triangle,0,&contacts[i].q,0);
 qword surface=QwCross(QwSub(contacts[2].q,contacts[0].q),QwSub(contacts[1].q,contacts[0].q));
 qword unit=QwDivide(surface,QwSqrt(QwDot(surface,surface)));
 {M3 m=BuildOrthonormalMatrixZ(unit,D_002A2DC0.q);p->basis.r[0]=m.r[0];p->basis.r[1]=m.r[1];p->basis.r[2]=m.r[2];}
 U angular,linear;
 func_0011C950(func_001A6318(p),&p->basis,&D_002BE920,0,&angular.q,D_002DDED0.dt*0.5f);
 SetVec(p,36,angular.q);
 func_0011F088(position,target,&D_002BE930,&linear.q,D_002DDED0.dt*0.5f);
 LIST planes;U samples[3];
 for(i=0;i<3;i++)StoreAddRot(&samples[i],target,&p->basis,&p->points[i].q);
 func_001856B8(p->mesh,&planes);
 func_001856C0(p->mesh,&planes,3,samples,&D_002BE940,10.0f);
 if(0.0f<p->control->speed){
  U movement;
  {float y=p->control->y,z=p->control->z;movement.f[0]=0.0f;movement.f[1]=-y;movement.f[2]=z;}
  M3 rotation;M3 inverse=InvertMatrix3Cofactor(&p->basis);
  MulM3(&rotation,&inverse,&D_002DDFA0);
  movement.q=QwRotateVec(&rotation,movement.q);movement.f[0]=0.0f;
  qword dir=QwSqrt(QwDot(movement.q,movement.q));
  VU_FLOAT speed;LoadVuFloatFromMemWord(speed,*(int*)&p->control->speed);M3 copy=rotation;
  qword fallback=D_002A2DC0.q;
  if(GXOfQw(dir)<0.0001f)dir=fallback;else dir=QwDivideR(movement.q,dir,fallback); // "r"(fallback): allocation-only
  movement.q=QwScaleX(dir,speed);
  p->acceleration.q=QwCombo(p->axes[1].direction.q,p->axes[2].direction.q,VuFloatFromMemWord(movement.i[1]),VuFloatFromMemWord(movement.i[2]));
  p->acceleration.q=QwScaleX(p->acceleration.q,VuFloatFromMemF(p->request->multiplier));
 }else ClearXyz(&p->acceleration);
 if(planes.count>0){
  qword projected=func_00185A80(p->mesh,&planes,QwRotateVec(&p->basis,QwScaleX(p->acceleration.q,VU_FLOAT(-1.0f))));
  M3 inverse=InvertMatrix3Cofactor(&p->basis);
  p->acceleration.q=QwRotateVec(&inverse,QwScaleX(projected,VU_FLOAT(-1.0f)));
 }
 int active=0;
 if(GXOfQw(QwDot(p->acceleration.q,p->acceleration.q))>1.0f)active=1;
 else if(p->control->speed!=0.0f)active=1;
 if(active){
  SetJtJts__FP2JT3JTS4JTBS(p,21,11);func_001A0450(p,p->axes,p->request);
  SetVec(p,35,QwAdd(linear.q,QwRotateVec(&p->basis,QwScaleX(p->acceleration.q,VuFloatC1(-1.0f)))));
 }else{SetJtJts__FP2JT3JTS4JTBS(p,21,10);SetVec(p,35,linear.q);}
}
