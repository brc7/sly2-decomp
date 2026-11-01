typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z; } __attribute__((aligned(16))) VECTOR;
typedef union { qword q; float f; } U;
typedef struct {
    char pad0[0x51C];
    float slip;
    char pad520[0x544 - 0x520];
    float heading;
    char pad548[0x550 - 0x548];
    float drive;
    char pad554[0x5AC - 0x554];
    void *path;
    float cursor;
} OBJ;
extern float D_002C20D0[3];
extern float D_002C20E0[2];
qword func_001A62E8(void *);
void FindShapeClosestPointFromS(void *,qword,void *,qword *,qword *,float *,float *,float);
float func_002458F0(float, float);
static inline float GetX(qword q) { float f; __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q)); return f; }
static inline float GetY(qword q) { float f; __asm__("prot3w %0, %1" : "=r"(f) : "r"(q)); return f; }
static inline float Dist2D(qword a, qword b)
{
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y" : "+j"(d));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
}
static inline float Clamp(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (hi < x) return hi;
    return x;
}
static inline qword AddScaled(qword a, qword b, float f)
{
    U u;
    register qword v __asm__("vf1");
    register qword scalar __asm__("vf2");
    __asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(f));
    u.q=scalar;
    __asm__("vadda.xyzw $ACC, %2, $vf0\n\tvmaddx.xyzw %0, %3, %1x" : "=j"(v) : "j"(u.q), "j"(a), "j"(b));
    return v;
}
static inline qword NormalizeOr(qword value,qword fallback)
{
    register qword length __asm__("vf1");
    register qword one __asm__("vf3");
    register qword result __asm__("vf2");
    qword cpu;
    float scalar;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(length),"=&j"(one):"j"(value));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(length):"0"(length));
    __asm__("mtc1 %1,%0":"=f"(scalar):"r"(length));
    cpu=fallback;
    if(scalar<0.0001f) return cpu;
    {
        __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(result):"j"(length),"j"(value));
        __asm__("qmfc2.ni %0,%1":"=r"(cpu):"j"(result));
    }
    return cpu;
}
void func_001D2490(OBJ *p)
{
    VECTOR va, vb;
    qword pos,v,dir,point;
    register qword w __asm__("vf4");
    register qword fallback __asm__("vf5");
    float f;
    pos = func_001A62E8(p);
    FindShapeClosestPointFromS(p->path,pos,0,(qword *)&va,(qword *)&vb,0,&p->cursor,p->cursor);
    {
        float dist = Dist2D(pos, *(qword *)&va);
        f = Clamp(D_002C20D0[0] + dist * (D_002C20D0[1] + dist * D_002C20D0[2]), D_002C20E0[0], D_002C20E0[1]);
    }
    point=*(qword *)&va;
    fallback=*(qword *)&vb;
    v=AddScaled(point,fallback,f);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(w) : "j"(v), "j"(pos));
    dir=NormalizeOr(w,fallback);
    p->heading = func_002458F0(GetY(dir), GetX(dir));
    p->drive = p->slip * 0.75f;
}
