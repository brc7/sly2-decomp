/* Apply damage and a projected knockback impulse from the message. */
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
struct P;
struct VT {short d,pad;void *fn;};
struct SPS {qword v;int kind;};
struct O;
struct Q {char pad[0xB0];struct SPS s;};
struct P {char pad[0xC];struct VT *vt;char pad10[0x58-0x10];struct O *x58;char pad5C[0xE8-0x5C];struct Q*xE8;char padEC[0x184-0xEC];int n184;char pad188[0x1A0-0x188];float g0,g1,g2,g3,gMin,gMax;};
struct MSG {char pad[0x10];int delta;char pad14[0x20-0x14];qword pos;char pad30[0x44-0x30];int flags;};
extern qword D_002A2DC0,D_002A2DA0;
qword func_001A62E8(struct P*);
qword LimitVectorLength(qword,float);
qword ApplyConstr(struct SPS*,struct O*,qword);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Reject(qword a,qword n){qword d,one;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a),"j"(n));__asm__("vmulx.xyzw %0,%1,%2x":"=j"(d):"j"(n),"j"(d));return Sub(a,d);}
static inline qword Normal(qword a,qword fallback){register qword d __asm__("vf1");qword one;float len;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"j"(d));__asm__("qmfc2.ni %0,%1":"=r"(len):"j"(d));if(len<0.0001f){d=fallback;}else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(d):"j"(d),"j"(a));}return d;}
static inline qword AddScaled(qword a,qword b,float g){qword k;U u;__asm__("qmtc2.ni %1,%0":"=j"(k):"r"(g));u.q=k;__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"=j"(a):"j"(b),"j"(k),"0"(a));return a;}
static inline float Clamp(float g,float lo,float hi){if(g<lo)return lo;if(hi<g)return hi;return g;}
int func_0017F1D0(struct P*p,struct MSG*m){
 qword dir,v;float g,t;
 if(!(m->flags&8))return 0;
 p->n184+=m->delta;
 if(p->n184<=0){struct VT*vt=p->vt;((void(*)(struct P*,int))vt[82].fn)((struct P*)((char*)p+vt[82].d),0);return 1;}
 if(!p->xE8 || p->xE8->s.kind==3)return 1;
 dir=Normal(Reject(Sub(func_001A62E8(p),m->pos),D_002A2DC0),D_002A2DA0);
 {float *a=&p->g0;t=-m->delta;g=Clamp(p->g0+t*(a[1]+t*a[2]),p->gMin,p->gMax);}
 {struct VT*vt=p->vt;v=((qword(*)(struct P*))vt[33].fn)((struct P*)((char*)p+vt[33].d));}
 v=LimitVectorLength(AddScaled(v,dir,g),p->gMax);
 v=ApplyConstr(&p->xE8->s,p->x58,v);
 {struct VT*vt=p->vt;((void(*)(struct P*,qword))vt[35].fn)((struct P*)((char*)p+vt[35].d),v);}
 return 1;
}
