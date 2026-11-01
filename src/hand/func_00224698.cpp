// Orient an active linked object toward its target in the horizontal plane.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct S { char pad[0x150]; int k; char pad1[8]; int x15C; void *p160; };
extern qword D_002A2DC0,D_002A2DA0,D_002A2DD0;
extern char D_002C4300[],D_002C4310[];
extern "C" {
void func_001F9B20(S *);
qword func_001A62E8(void *);
M3 func_0011B830(qword,qword);
M3 *func_001A6318(S *);
void func_0011B938(M3 *,M3 *,M3 *);
void func_001F5C30(S *,M3 *,void *,qword,void *);
}
static inline qword ToVF(qword a) { qword r; __asm__ __volatile__("qmtc2.ni %1,%0" : "=j"(r) : "r"(a));return r; }
static inline qword Read(const qword &a) { qword r;__asm__("lqc2 %0,%1" : "=j"(r) : "m"(a));return r; }
static inline qword Sub(qword a,qword b) { qword r;__asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b));return r; }
static inline qword Flatten(qword d,qword up) {
    qword t,one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(t),"=&j"(one) : "j"(d),"j"(up));
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(t) : "j"(up),"j"(t));
    return Sub(d,t);
}
static inline qword Dot(qword d) { qword len,one;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(len),"=&j"(one) : "j"(d));return len; }
static inline qword Sqrt(qword len) {
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(len));return len; }
static inline float X(qword v) { float f;__asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(v));return f; }
static inline qword Scale(qword d,qword len) { qword r;
    __asm__ __volatile__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(d),"0"(len));return r; }
/** @sly2-annot addr=0x224698
 * Purpose: turns linked active object toward target in horizontal plane
 * Info: proposed name OrientCid100ToTarget (T2); class CID100 slot70; TU tu_223F70
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
extern "C" void func_00224698(S *p) {
    M3 m;
    if(p->k==0 || p->k==4)return;
    func_001F9B20(p);
    if(p->x15C!=0x1A && p->x15C!=0x29)return;
    if(p->p160) {
        qword a=ToVF(func_001A62E8(p->p160));
        qword b=func_001A62E8(p);
        qword d=Flatten(Sub(a,b),Read(D_002A2DC0));
        register qword fallback __asm__("a1")=D_002A2DA0;
        qword len=Sqrt(Dot(d));
        register qword n __asm__("vf1");
        if(X(len)<1.0e-4f)n=fallback;
        else n=Scale(d,len);
        m=func_0011B830(n,D_002A2DC0);
    } else {
        func_0011B938(func_001A6318(p),0,&m);
    }
    func_001F5C30(p,&m,D_002C4300,D_002A2DD0,D_002C4310);
}
