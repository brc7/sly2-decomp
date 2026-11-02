typedef int qword __attribute__((mode(TI)));
union U { qword q; float a[4]; };
extern qword D_002A2DC0, D_002A2DD0;
float sinf(float);
float atan2f(float,float);
float tanf(float);
float cosf(float);
static inline float Sqrt(float x) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(x)); return r; }
static inline void Bc(union U *out,float f) {
    __asm__("qmtc2.ni %1, %0" : "=j"(out->q) : "r"(f));
}
static inline float Dot(const qword *a, qword b) {
    qword d, one; float r;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(*a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}
static inline float Len(qword a) {
    qword d,one; float r;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}
static inline qword Norm(qword a) {
    register qword d __asm__("vf2");
    qword one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "+j"(d) : "j"(a));
    return d;
}
static inline qword Difference(qword end,const qword *start){qword result;__asm__("vsub.xyzw %0,%1,%2":"=j"(result):"j"(end),"j"(*start));return result;}
/** @sly2-annot addr=0x1D6848
 * Purpose: Launch velocity from start to end at elevation angle g under gravity acc (sin/cos/tan/atan2)
 * Info: proposed name VecLaunchBallistic (T2); TU tu_1D6848
 * Notes: names_v2 B proposed FCalculateMuzzleVelocity__FP6VECTORT0fT0P2SO (0.891)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#6-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/physics-collision.md#tu_1d6848-0x1d6848-0x1d7238
 * @end-sly2-annot */
qword func_001D6848(float g, qword start, qword end, qword acc)
{
    union U delta, vel, scalar;
    qword d, s, n, r;
    float k=sinf(g);
    float z,len,q;
    register float numerator __asm__("$f20");
    register float angle __asm__("$f12");
    d=Difference(end,&start);
    delta.q = d;
    z = delta.a[2];
    delta.a[2] = 0.0f;
    len = Len(delta.q);
    angle=atan2f(z,len);
    if (angle<0.0f) angle=0.0f;
    q=Dot(&acc,D_002A2DC0);
    k = angle + (1.57079637051f-angle)*k;
    q=q/((z-len*tanf(k))*2.0f);
    if (q<=0.0f) return D_002A2DD0;
    q=Sqrt(q);
    numerator=len*q;
    q=numerator/cosf(k);
    vel.q=delta.q;
    vel.a[2]=len*tanf(k);
    Bc(&scalar,q);
    s=scalar.q;
    n=Norm(vel.q);
    __asm__("vmulx.xyzw %0, %1, %0x" : "+j"(s) : "j"(n));
    r = s;
    vel.q=r;
    return vel.q;
}
