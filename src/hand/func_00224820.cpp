// func_00224820: trace a probe segment, record the contact in the caller's HIT list, and push the touched body.
// - Segment start..end goes through func_0014CC58; the hit object's eligible actor comes from func_001F7530.
// - A new HIT record (deduplicated by object, up to capacity) gets owner/object/filter/channel/point and
//   impulse = normal * D_002C4334 (the target speed).
// - If the body has physics: project its point velocity (GetXfVelocity) on the normal, evaluate the quadratic
//   D_002C4320 at along / D_002C4334 clamped to D_002BBB60, scale by D_002C4330, get the linear (func_001FAA00)
//   and angular (func_001FAA48) responses and, when they speed the point up along the normal, add the fraction
//   (target - along) / change (clamped to 0..1) of them to the body's velocity (virtuals 33/34 -> 35/36).
// Matching notes (T1586 lilly28 -> T1591 MATCH):
// - `bits` is ONE int shared by both float->VU loads (the record's `lw` and the impulse's `mfc1`). Used in two
//   blocks it is a global-alloc pseudo, so it takes the dying lui register $v1 (`lw $v1,0x4334($v1)`); a
//   block-local temp is placed by local-alloc, whose fake lifetimes never share a dying input register.
// - One register pin: Pin() keeps `owner` in $21 (s5); without it count/owner swap s5/s6 (global.c priority 370 vs 375).
// - MkVuk*/ScaleK/AddScaledK take their scalar as a by-value struct TEMPORARY: that temp is the shared 0x60($sp)
//   home of the original (sqc2 vf,0x60 / qmfc2+sq 0x60), so there is no separate `scalar` local.
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct O;
struct VE {short delta,pad;union {qword (*get)(void *);void (*set)(void *,qword);} f;};
struct NODE {char pad[0x70];V position;};
struct PHYS;
struct STATE {char pad[0x60];O *owner;};
struct O {char pad[0xC];VE *vt;char pad10[0x4C];NODE *rotation,*motion;char pad64[0x84];PHYS *physics;STATE *state;};
struct COLL {void *source;O *object;};
struct LSG {qword point;char remainder[0x50];};
struct HIT {O *owner;int field4;O *object;int filter,channel;char pad14[0xC];qword point,impulse;char pad40[0x20];};
struct POLY {float a,b,c;};
struct RANGE {float lo,hi;};
extern float D_002C4334;
extern float D_002C4330;
extern POLY D_002C4320;
extern RANGE D_002BBB60;
extern "C" {
COLL *func_0014CC58(int,qword,qword,int,COLL **,LSG *);
O *func_001F7530(O *,int);
void *func_002333B4(void *,int,unsigned int);
qword GetXfVelocity(NODE *,NODE *,qword);
qword func_001FAA00(O *,qword);
qword func_001FAA48(O *,qword,qword);
}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword CrossXYZ(qword a,qword b){__asm__("vopmula.xyz ACC,%1,%0\n\tvopmsub.xyz %0,%0,%1":"+j"(b):"j"(a));return b;}
static inline qword QwDot(qword a, qword b){qword qw;qword vfOne;
 __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));return qw;}
// the extra "j"(qw) is allocation-only: it makes qw a VU-class pseudo, so the compiler's qmfc2 comes from the dot result (vf1)
static inline float GXOfQw(qword qw){float x;__asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw));return x;}
static inline float Dot(qword a,qword b){return GXOfQw(QwDot(a,b));}
static inline int BitsOfFloat(float f){int n;__asm__("mfc1 %0,%1":"=r"(n):"f"(f));return n;}
struct VUK{qword q;};
static inline VUK MkVukBits(int n){VUK r;__asm__("qmtc2.ni %1,%0":"=j"(r.q):"r"(n));return r;}
static inline VUK MkVuk(float f){VUK r;__asm__("mfc1 %0,%1":"=r"(r.q):"f"(f));return r;}
static inline qword ScaleK(qword v,VUK k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(k.q));return r;}
static inline qword AddScaledK(qword old,qword v,VUK k){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%1x":"+j"(old):"j"(k.q),"j"(v));return old;}

static inline float Clamp(float f,float lo,float hi){float r;if(f<lo)r=lo;else if(hi<f)r=hi;else r=f;return r;}
static inline qword Get(O *o,int slot){VE *v=o->vt;return v[slot].f.get((char *)o+v[slot].delta);}
static inline void Set(O *o,int slot,qword q){VE *v=o->vt;v[slot].f.set((char *)o+v[slot].delta,q);}
static inline O *Pin(O *p){register O *r asm("$21");asm("daddu %0,%1,$zero":"=r"(r):"r"(p));return r;}
extern "C" void func_00224820(O *ownerIn,int filter,int channel,qword start,qword end,int queryCount,COLL **objects,int capacity,int *count,HIT *records){
 O *owner=Pin(ownerIn);LSG hit;int bits;
 COLL *collider=func_0014CC58(0,start,end,queryCount,objects,&hit);
 if(!collider)return;
 qword normal;
 O *chain=collider->object;
 {qword len,one;qword d=Sub(end,start);
 __asm__("vmul.xyz %2,%3,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%2,%2y\n\tvmaddz.x %2,%1,%2z\n\tvsqrt Q,%2x\n\tvwaitq\n\tvaddq.x %2,$vf0,Q\n\tvdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%3,Q":"=&j"(normal),"=&j"(one),"=&j"(len):"j"(d));}
 O *object=func_001F7530(chain,filter);
 if(!object)return;
 O *body=object->state->owner;
 int i;
 for(i=0;i<*count;i++)if(records[i].object==object)break;
 if(i>=*count&&*count<capacity){
  HIT *record=&records[(*count)++];
  func_002333B4(record,0,0x60);
  bits=*(int *)&D_002C4334;
  record->impulse=ScaleK(normal,MkVukBits(bits));
  record->object=object;
  record->filter=filter;
  record->channel=channel;
  record->point=hit.point;
  record->owner=owner;
 }
 if(!body->physics)return;
 float along=Dot(GetXfVelocity(body->motion,0,hit.point),normal);
 float ratio=along/D_002C4334;
 float response=Clamp(D_002C4320.a+ratio*(D_002C4320.b+ratio*D_002C4320.c),D_002BBB60.lo,D_002BBB60.hi);
 float fz=0.0f;
 if(fz<response){
 bits=BitsOfFloat(response*D_002C4330);
 qword impulse=ScaleK(normal,MkVukBits(bits));
 qword linear=func_001FAA00(body,impulse);
 qword angular=func_001FAA48(body,hit.point,impulse);
 qword delta=Sub(hit.point,body->rotation->position.q);
 float change=Dot(Add(linear,CrossXYZ(angular,delta)),normal);
 if(fz<change){
 qword velocity,spin;
  float x=(D_002C4334-along)/change;
  float one=1.0f;
 if(x<fz){}else if(one<x)fz=one;else fz=x;
 {velocity=AddScaledK(Get(body,33),linear,MkVuk(fz));}
 {spin=AddScaledK(Get(body,34),angular,MkVuk(fz));}
 Set(body,35,velocity);Set(body,36,spin);
 }
 }
}
