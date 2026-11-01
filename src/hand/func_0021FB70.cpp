// func_0021FB70 = UpdateJtThrowArc (Lilly's Sly2Port name, 0x21FB70): while the hero holds an object (JTTHROW, jts 23)
// it predicts the arc the object would fly to the aim point. The launch point (vLaunchLocal in the hero's space) and the
// carried object's gravity give CalculateLaunchVelocity (func_001D6848) a velocity whose elevation follows lmAim; the
// apex and the end point after tPredict seconds bound a box for FindSwObjectsInBox (func_0020FAA0). 28 samples along the
// arc are line-tested (func_0014CC58); at the first hit the flight time is the closest non-negative root of the vertical
// motion (CSolveQuadratic). The flight time may change by at most 3 * dt per frame; up to 30 points of the arc are written
// back to front into the marker's ring of frames (matrix along the arc, BuildOrthonormalMatrixZ), and the four-clip / two-clip
// blends get the distance (lmAim) and heading-error (radTurnAim) weights, rate limited by 2 * dt when fUpdateBlends.
// How it matched (Lilly's agent, T1604): fresh8's stack layout plus
// - the scope is an empty STACKSCOPE with its real out-of-line ctor/dtor (a sized FRAME takes the freed 0x0 home slot);
// - the VU_FLOAT homes are by-value parameters of the arc helper (all share sp+0x0; the hit block's at 0x260 is moved
//   out of the root loop by loop.c's load_mems), and the 0.5 of the sampling loop is a literal (cse2 copies it to f22);
// - the root loop's by-value VU_VECTOR copies keep the sample loop >= 96 RTL insns, so loop.c leaves FLT_MAX's lui in it;
// - accZ / startZ are written inside the root solver's arguments: loop.c hoists them, which gives the game's schedule
//   and the f23/f24/f25 order (computed before the loop, accZ ties uFar in global.c priority);
// - one counter for both point loops, `(points + i)->q`, `pc = point++`, the basis copy through a pointer (its stores
//   use the hit address register like the game), `left` before `near`, dt read into a local before the 2.0 * dt.
typedef int qword __attribute__((mode(TI)));

union V {qword q;float f[4];int n[4];};
struct M3 {V r[3];};
struct M4 {M3 basis;V position;};
// The current stack-allocation scope (out of line: 0x1AE478 / 0x1AE4A0)
struct STACKSCOPE {STACKSCOPE();~STACKSCOPE();};
struct SW;
struct SO;
struct ASEGBL;
struct LO;
struct GRAVITY {char unk0[0x80];V vAccel;};
// The carried object: what it falls with and what FindSwObjectsInBox's filter is given
struct CARRIED {char unk0[0xE8];GRAVITY *pgravity;void *pvFilter;};
// A frame of the aim marker's arc: the time it was written, 1 when the point exists, a length (0) and the matrix
struct ARCFRAME {float t,gValid,s;M4 mat;};
struct MARKER {char unk0[0x158];int cframe;ARCFRAME *aframe;};
struct LM {float gMin,gMax;};
struct CLQ {float g0,g1,g2;};
// JTTHROW blend entry (Lilly's JTAIRBL)
struct BL {float gWeight;ASEGBL *pasegbl;int unk8;};
struct JTTHROW {
 LO *pstepplayer;char unk4[0xC];CARRIED *ploCarried;float radTurnHold,radTurnAim;LM lmAim;float tPredict;
 char unk28[0x40];BL ablB[4];ASEGBL *pasegblB;char unk9C[8];BL ablC[2];ASEGBL *pasegblC;
 V vUnkC0,vLaunchLocal,posHold,vLaunch;char unk100[0x2C];MARKER *ploMarker;float tFlight;
};
// What the line test reports (the hit point first)
struct HIT {V pos;char unk10[0x50];};
struct CLOCK {float t,dt;};
extern SW D_003DD490;
extern CLOCK D_002DDED0;
extern CLQ D_002C40F0;                         // the launch elevation over the aim distance
extern float D_002C40E4;                       // 1.35: tPredict scale with power-up 0x24
extern float D_002C4100,D_002C4104;            // 3.0 / 2.0: flight time / blend rate limits per second
extern float D_002A96A0;                       // FLT_MAX
extern V D_002A2DC0;                           // (0, 0, 1)
extern "C" {
 qword ConvertAloPos(LO *,LO *,qword);         // ConvertLoPos
 qword func_001A62E8(LO *);                    // PosLoWorld
 qword func_001D6848(qword,qword,qword,float); // CalculateLaunchVelocity
 int func_001DAEA8(int);                       // FPowerUpAcquired
 void func_0020FAA0(SW *,qword,qword,int (*)(void *,SO *),void *,int *,SO ***); // FindSwObjectsInBox
 int func_0021FB18(void *,SO *);
 SO *func_0014CC58(int,qword,qword,int,SO **,HIT *);
 int CSolveQuadratic__FfffPf(float,float,float,float *);
 M3 *BuildOrthonormalMatrixZ(M3 *,qword,qword);
 V *func_00256D18(V *,qword);
 void func_0021C5F8(MARKER *,qword);
 float func_002458F0(float,float);
 float func_00204740(LO *);                    // JtGetFacingAngle
 float RadNormalize__Ff(float);                   // RadNormalize
 void func_001384C0(ASEGBL *);
}
// A vector in a VU0 register, passed by value
struct VU_VECTOR {qword q;VU_VECTOR(){}VU_VECTOR(qword qw){q=qw;}};
// A float in x of a VU0 register; by value it has a stack home (the game's dead sqc2)
struct VU_FLOAT {qword q;};
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.q):"r"(g));return v;}
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline VU_VECTOR VuSub(VU_VECTOR a,VU_VECTOR b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a.q),"j"(b.q));return VU_VECTOR(r);}
static inline qword QwMin(qword a,qword b){qword r;__asm__("vmini.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwMax(qword a,qword b){qword r;__asm__("vmax.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float GFromVf(qword v){float g;__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(v));return g;}
static inline float GZOfQw(qword v){float g;__asm__("pextuw %0,$zero,%1":"=r"(g):"r"(v));return g;}
static inline float GLengthXY(qword v){
 __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y":"+j"(v));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(v));return GFromVf(v);
}
static inline float GLength(VU_VECTOR w){qword v=w.q;qword one;
 __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(v),"=&j"(one));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(v));return GFromVf(v);
}
// pos + v * t + a * t2
static inline qword QwBallistic(const qword &pos,const qword &a,const qword &v,VU_FLOAT t,VU_FLOAT t2){qword r;
 __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddax.xyzw ACC,%2,%3x\n\tvmaddx.xyzw %0,%4,%5x":"=j"(r):"j"(pos),"j"(v),"j"(t.q),"j"(a),"j"(t2.q));return r;
}
// Where the arc is after t seconds
static inline qword PosOnArc(const qword &pos,const qword &a,const qword &v,float t,float gHalf){return QwBallistic(pos,a,v,VuFloat(t),VuFloat(t*(t*gHalf)));}
// a * g0 + b * g1
static inline qword QwCombine(qword a,qword b,VU_FLOAT g0,VU_FLOAT g1){qword r;
 __asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r):"j"(a),"j"(g0.q),"j"(b),"j"(g1.q));return r;
}
static inline float GClamp(float g,float gMin,float gMax){if(g<gMin)return gMin;if(gMax<g)return gMax;return g;}
static inline float GMax(float g0,float g1){return g0<g1?g1:g0;}
static inline float GFromClq(const CLQ &clq,float x){return clq.g0+x*(clq.g1+x*clq.g2);}

extern "C" void func_0021FB70(JTTHROW *pjtthrow,int fUpdateBlends)
{
 qword posLaunch=ConvertAloPos(pjtthrow->pstepplayer,0,pjtthrow->vLaunchLocal.q);
 qword posHold=pjtthrow->posHold.q;
 qword vAccel=pjtthrow->ploCarried->pgravity->vAccel.q;
 qword posHero=func_001A62E8(pjtthrow->pstepplayer);
 float uFar=GClamp((GLengthXY(QwSub(posHold,posHero))-pjtthrow->lmAim.gMin)/(pjtthrow->lmAim.gMax-pjtthrow->lmAim.gMin),0.0f,1.0f);
 qword vLaunch=func_001D6848(posLaunch,pjtthrow->posHold.q,vAccel,GFromClq(D_002C40F0,uFar));
 float tApex=GMax(-GZOfQw(vLaunch)/GZOfQw(vAccel),0.0f);
 qword posApex=PosOnArc(posLaunch,vAccel,vLaunch,tApex,0.5f);
 float t;
 if(func_001DAEA8(0x24))t=D_002C40E4*pjtthrow->tPredict;
 else t=pjtthrow->tPredict;
 qword posEnd=PosOnArc(posLaunch,vAccel,vLaunch,t,0.5f);
 STACKSCOPE stackscope;
 qword posMin=QwMin(posApex,posEnd);
 qword posMax=QwMax(posApex,posEnd);
 posMax=QwMax(posLaunch,posMax);
 posMin=QwMin(posLaunch,posMin);
 int cso;SO **apso;
 func_0020FAA0(&D_003DD490,posMin,posMax,func_0021FB18,pjtthrow->ploCarried->pvFilter,&cso,&apso);
 pjtthrow->vLaunch.q=vLaunch;
 float dt=t*0.034482758f;
 V apos[30];
 qword posPrev=posLaunch;
 unsigned i;
 for(i=1;i<29;i++){
  t=(float)(int)i*dt;
  qword pos=PosOnArc(posLaunch,vAccel,vLaunch,t,0.5f);
  HIT hit;
  if(func_0014CC58(1,posPrev,pos,cso,apso,&hit)){
   V at;
   int ct=CSolveQuadratic__FfffPf(GZOfQw(vAccel)*0.5f,GZOfQw(vLaunch),GZOfQw(posLaunch)-hit.pos.f[2],at.f);
   float tBest=0.0f,sBest=D_002A96A0;
   for(int it=0;it<ct;it++){
    float tRoot=at.f[it];
    if(tRoot<0.0f)continue;
    qword posRoot=PosOnArc(posLaunch,vAccel,vLaunch,tRoot,0.5f);
    float s=GLength(VuSub(posRoot,hit.pos.q));
    if(s<sBest){sBest=s;tBest=tRoot;}
   }
   t=tBest;
   break;
  }
  posPrev=pos;
 }
 float dtMax=D_002C4100*D_002DDED0.dt;
 pjtthrow->tFlight=GClamp(t,pjtthrow->tFlight-dtMax,pjtthrow->tFlight+dtMax);
 t=pjtthrow->tFlight;
 i=0;
 for(;;){
  if(t<0.1f)break;
  apos[i].q=PosOnArc(posLaunch,vAccel,vLaunch,t,0.5f);
  if(++i>=29)break;
  t=pjtthrow->tFlight-(float)(int)i*dt;
 }
 (apos+i)->q=posLaunch;
 int cpos=i+1;
 if(pjtthrow->ploMarker){
  ARCFRAME *pframe=pjtthrow->ploMarker->aframe+pjtthrow->ploMarker->cframe;
  qword posBefore=QwCombine(apos[0].q,apos[1].q,VuFloat(2.0f),VuFloat(-1.0f));
  V *ppos=apos;
  M4 mat;M3 matRot;
  for(int iframe=0;iframe<pjtthrow->ploMarker->cframe;iframe++){
   pframe--;
   if(iframe<cpos){
    V *pposCur=ppos++;
    qword pos=pposCur->q;
    qword dpos=QwSub(pos,posBefore);
    BuildOrthonormalMatrixZ(&matRot,dpos,D_002A2DC0.q);
    M4 *pmat=&mat;
    func_00256D18(&mat.position,pos);
    pmat->basis=matRot;
    posBefore=pos;
    mat.basis.r[2].n[3]=0;
    mat.basis.r[1].n[3]=0;
    mat.basis.r[0].n[3]=0;
    pmat->position.f[3]=1.0f;
    pframe->gValid=1.0f;
   }else pframe->gValid=0.0f;
   pframe->t=D_002DDED0.t;
   pframe->s=0.0f;
   pframe->mat=mat;
  }
  func_0021C5F8(pjtthrow->ploMarker,posLaunch);
 }
 qword posAim=pjtthrow->posHold.q;
 qword posHeroNow=func_001A62E8(pjtthrow->pstepplayer);
 V dpos;
 dpos.q=QwSub(posAim,posHeroNow);
 float rad=func_002458F0(dpos.f[1],dpos.f[0]);
 float radHero=func_00204740(pjtthrow->pstepplayer);
 rad=RadNormalize__Ff(rad-radHero);
 float uTurn=GClamp(rad/pjtthrow->radTurnAim,-1.0f,1.0f)*0.5f+0.5f;
 if(fUpdateBlends){
  float uFarOld=pjtthrow->ablC[1].gWeight,uTurnOld;
  if(uFarOld>0.5f)uTurnOld=pjtthrow->ablB[3].gWeight/uFarOld;
  else uTurnOld=pjtthrow->ablB[1].gWeight/(1.0f-uFarOld);
  float dtFrame=D_002DDED0.dt;
  float du=D_002C4104*dtFrame;
  uFar=GClamp(uFar,uFarOld-du,uFarOld+du);
  uTurn=GClamp(uTurn,uTurnOld-du,uTurnOld+du);
 }
 float uTurnOpp=1.0f-uTurn;
 float uNear=1.0f-uFar;
 pjtthrow->ablB[0].gWeight=uTurnOpp*uNear;
 pjtthrow->ablB[1].gWeight=uTurn*uNear;
 pjtthrow->ablB[2].gWeight=uTurnOpp*uFar;
 pjtthrow->ablB[3].gWeight=uTurn*uFar;
 func_001384C0(pjtthrow->pasegblB);
 pjtthrow->ablC[0].gWeight=uNear;
 pjtthrow->ablC[1].gWeight=uFar;
 func_001384C0(pjtthrow->pasegblC);
}
