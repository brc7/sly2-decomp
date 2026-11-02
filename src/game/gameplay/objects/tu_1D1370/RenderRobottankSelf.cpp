// Update contact heights and rolling transforms for the linked actor nodes.
typedef int qword __attribute__((mode(TI)));
union V{qword q;float f[4];};
struct M3{V r[3];};
struct M4{M3 basis;V position;};
struct NODE{M4 matrix,local;char pad80[0x20];NODE*parent;int generation;};
struct CONTACT{char pad[0x90];float height;};
struct ENTRY{int unknown;NODE*node;CONTACT*contacts;char padC[8];float tilt,speed,angle,radius;char pad24[12];float x,y;char pad38[0x20];float height;char pad5C[4];};
struct P;
struct VE{short delta,pad;qword(*fn)(P*);};
struct P{char pad[0xC];VE*vt;char pad10[0x50];P*space;char pad64[0x12C];int count;char pad194[12];ENTRY entries[8];char pad4A0[0xB0];float drive;};
struct CLOCK{float time,dt;};
struct G{char pad[0x3D58];int generation;};
extern CLOCK D_002DDED0;
extern G D_003DD490;
extern qword D_002A2DC0,D_002A2DB0;
extern char D_002C21A0[],D_002C21B0[],D_002C21C0[];
extern float D_002C21CC;
extern "C" {
qword ConvertAloVec(P*,P*,qword);
qword func_0022FC48(P*,NODE*,qword);
CONTACT*func_001D2DD0(P*,ENTRY*);
float GSmooth__FfffP3SMPPf(float,float,float,void*,float*);
float RadNormalize__Ff(float);
M3*func_0011B500(M3*,qword,float);
qword*func_00256D18(qword*,qword);
void UpdateSoXfWorldMatrices(P*);
void RenderSoSelf(P*);
}
static inline float Abs(float f){float r;__asm__("abs.s %0,%1":"=f"(r):"f"(f));return r;}
static inline M3 Rotation(qword axis,float angle) return r{func_0011B500(&r,axis,angle);}
static inline M3 Mul(const M3&a,const M3&b) return r{
 qword x=a.r[0].q,y=a.r[1].q,z=a.r[2].q;
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(x),"j"(y),"j"(z),"j"(b.r[0].q));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(x),"j"(y),"j"(z),"j"(b.r[1].q));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(x),"j"(y),"j"(z),"j"(b.r[2].q));
}
/** @sly2-annot addr=0x1D41F8
 * Purpose: CID 77 vt19 RenderSelf: per wheel compute contact height and rolling rotation, update XF matrices,
 *   then RenderSoSelf
 * Info: proposed name RenderRobottankSelf (T1); class ROBOTTANK slot19; TU tu_1D1370
 * Notes: names_v2 v1 proposed RenderGlint__FP5GLINTP2CM (0.200) | class identity from whole_game WE
 *   names/object strings: RenderRcCarSelf -> RenderRobottankSelf
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
extern "C" void RenderRobottankSelf(P*p){
 V velocity,point;M3 basis;
 VE*method=&p->vt[33];
 velocity.q=ConvertAloVec(0,p,method->fn((P*)((char*)p+method->delta)));
 for(int i=0;i<p->count;i++){
  ENTRY*e=&p->entries[i];
  point.q=func_0022FC48(0,(NODE*)p->space,e->node->local.position.q);
  point.f[0]=e->x;point.f[1]=e->y;
  CONTACT*hit=func_001D2DD0(p,e);
  {
   float height;
   if(hit){float limit=e->height+hit->height;if(point.f[2]<limit){point.f[2]=limit;goto heightDone;}height=limit;}
   else height=e->height;
   (void)GSmooth__FfffP3SMPPf(point.f[2],height,D_002DDED0.dt,D_002C21A0,0);point.f[2]=height;
  }
  heightDone:;
  point.q=func_0022FC48(p->space,e->node->parent,point.q);
  float desired;if(i<2)desired=0.0f;else desired=p->drive/e->radius;
  if(hit){
   float current=velocity.f[0]/e->radius;
   if((0.0f<desired&&current<D_002C21CC*desired)||(desired<0.0f&&D_002C21CC*desired<current))
    e->speed=GSmooth__FfffP3SMPPf(e->speed,D_002C21CC*desired,D_002DDED0.dt,D_002C21B0,0);
   else e->speed=current;
  }else{
   float dt=D_002DDED0.dt;
   void*table;
   if(Abs(e->speed)<Abs(desired))table=D_002C21B0;else table=D_002C21C0;
   e->speed=GSmooth__FfffP3SMPPf(e->speed,desired,dt,table,0);
  }
  float dt=D_002DDED0.dt;
  e->angle=RadNormalize__Ff(e->angle+e->speed*dt);
  basis=Mul(Rotation(D_002A2DC0,e->tilt),Rotation(D_002A2DB0,e->angle));
  NODE*node=e->node;
  func_00256D18(&node->matrix.position.q,point.q);
  node->matrix.basis=basis;
  node->matrix.position.f[3]=1.0f;
  node->matrix.basis.r[1].f[3]=0.0f;
  node->matrix.basis.r[0].f[3]=0.0f;
  node->matrix.basis.r[2].f[3]=0.0f;
  register int generation=++D_003DD490.generation;
  e->node->generation=generation;
 }
 UpdateSoXfWorldMatrices(p);RenderSoSelf(p);
}
