/* func_00193A68 (Claude agent): SetVectorCylind's qword result assigned plainly so gcc emits the qmtc2 itself; Dot's float via an mtc1 asm fed by gcc's own qmfc2. */

typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} U;
struct T {char pad[0x18];float x18,x1C;};
struct O {char pad[0x150];struct T*x150;char pad154[0x240-0x154];qword pos;};
struct P {struct O*o;char pad4[0x260-4];float x260;char pad264[0x2A0-0x264];float x2A0;};
struct CURVE;
struct SMP {float g0,g1,g2;};
struct CLOCK {float t,dt;};
extern struct CLOCK D_002DDED0;
extern float D_002BF668;
extern qword D_002A2DC0,D_002A2DD0;
void EvaluateShapeU(struct CURVE*,float,qword*,qword*);
qword SetVectorCylind(float,float,float);
float SFromShapeU(struct CURVE*,float);
float SMaxShape(struct CURVE*);
float GSmooth__FfffP3SMPPf(float,float,float,struct SMP*,float*);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Reject(qword a,qword n){register qword d __asm__("vf2");qword one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a),"j"(n));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(d):"j"(n),"j"(d));__asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"0"(d));return d;}
static inline qword Normal(qword a,qword fallback){register qword d __asm__("vf1");register qword one __asm__("vf4");float len;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(len):"j"(d));if(len<0.0001f){d=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(d):"j"(d),"j"(a));}return d;}
static inline float Dot(qword a,qword b){qword d=b,one;float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"0"(d));__asm__("mtc1 %1,%0":"=f"(f):"r"(d));return f;}
static inline qword Scale(qword a,float g){U u;qword s,r;__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(g));u.q=s;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;}
void func_00193A68(struct P*p,struct CURVE*curve,qword*outPos,qword*outVel,float t){
 U pos,dir,dead;struct SMP smp;float low,high;qword v;float dot,current;
 struct T*shape=p->o->x150;
 EvaluateShapeU(curve,t,&pos.q,&dir.q);
 dir.q=Normal(Reject(dir.q,D_002A2DC0),D_002A2DD0);
 {register float width __asm__("$f0")=shape->x1C;register float factor __asm__("$f13")=p->x2A0;v=SetVectorCylind(p->o->x150->x18,factor*width,0.0f);}
 dot=Dot(v,dir.q);dead.q=v;
 current=SFromShapeU(curve,t);
 smp.g1=p->x260;smp.g0=p->x2A0;smp.g2=D_002BF668;
 (void)GSmooth__FfffP3SMPPf(current,50.0f,D_002DDED0.dt,&smp,&low);
 (void)GSmooth__FfffP3SMPPf(current,SMaxShape(curve)-50.0f,D_002DDED0.dt,&smp,&high);
 {float hi=high;*outPos=Sub(pos.q,p->o->pos);{float g;if(dot<low)g=low;else if(hi<dot)g=hi;else g=dot;*outVel=Scale(dir.q,g);}}
}
