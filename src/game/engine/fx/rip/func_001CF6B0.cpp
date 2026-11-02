// Emit an oriented particle in the supplied parent space.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {qword r[3];};
struct LO {char pad158[0x158];float height;};
struct POLY {float a,b,c;};
struct EMIT {char pad8[8];LO*owner;char padC[8];float start,duration;char pad1C[4];POLY size;};
struct O;
struct VE {short delta,pad;void(*fn)(void*,qword,int,float);};
struct O {char pad18[0x18];float duration;LO*visual;char pad20[8];float spin;char pad2C[0x14];M3 matrix;U position;char pad80[0x10];U velocity;char padA0[0x4C];VE*vt;LO*space;};
struct CLOCK {float t,dt;};
extern CLOCK D_002DDED0;
extern char D_003DD490[];
extern qword D_002A2DA0,D_002A2DB0,D_002A2DC0;
extern "C" {
O *func_001CE888(void*,void*,void*);
qword func_0022FC48(void*,LO*,qword);
float GRandInRange__Fff(float,float);
void GetNormalVectors(qword,qword,qword,qword*);
M3 LoadRotateMatrixEuler(qword);
M3 BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(qword,qword);
O *func_001CF6B0(EMIT*,LO*,LO*,qword,qword);
}
static inline float Polynomial(POLY*p,float t){return p->a+t*(p->b+t*p->c);}
static inline float Dot(qword a,qword b){qword d,one;float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(a),"j"(b));__asm__("mtc1 %1,%0":"=f"(f):"r"(d));return f;}
static inline qword Scale(qword a,float f,U&home){qword k;__asm__("qmtc2.ni %1,%0":"=j"(k):"r"(f));home.q=k;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(a):"j"(a),"j"(k));return a;}
static inline qword Transform(M3&m,qword a){__asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m.r[0]),"j"(m.r[1]),"j"(m.r[2]));return a;}
static inline M3 Mul(const M3&a,const M3&b) return r {
 qword a0=a.r[0],a1=a.r[1],a2=a.r[2];
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0]));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1]));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2]));
}
/** @sly2-annot addr=0x1CF6B0
 * Purpose: Spawn a STUCK rip (PripNewRipg) attached to the hit object at the contact point/orientation
 * Info: proposed name CreateStuck (T1); TU rip
 * Notes: names_v2 C proposed ProjectLeafTransform__FP4LEAFf (0.638)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
O *func_001CF6B0(EMIT*p,LO*visual,LO*space,qword position,qword direction){
 O*object=0;
 if(visual){
  object=func_001CE888(D_003DD490,(void*)10,0);
  if(object){
   float zero=0.0f;
   U local;local.q=func_0022FC48(0,space,position);
   float size=Polynomial(&p->size,(D_002DDED0.t-p->start)/p->duration);
   size*=GRandInRange__Fff(0.8f,1.4f);
   object->vt[1].fn((char*)object+object->vt[1].delta,local.q,0,size);
   object->visual=visual;object->duration=5.0f;object->spin=-size;object->space=space;
   M3 frame;frame.r[2]=direction;
   GetNormalVectors(direction,D_002A2DA0,D_002A2DB0,frame.r);
   U angles;angles.f[1]=zero;angles.f[0]=zero;angles.f[2]=GRandInRange__Fff(zero,6.28318548f);
   M3 rotation;
   rotation=LoadRotateMatrixEuler(angles.q);
   {M3 orient=Mul(frame,rotation);
   object->matrix=orient;}
   qword *up=&D_002A2DC0;
   if(__builtin_fabsf(Dot(direction,*up))<0.7f){
    U flat;flat.q=direction;flat.f[2]=zero;
    M3 tilt;tilt=BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(flat.q,direction);
    U home;
    object->velocity.q=Transform(tilt,Scale(*up,-50.0f,home));
   }
   object->position.f[2]+=p->owner->height;
  }
 }
 return object;
}
