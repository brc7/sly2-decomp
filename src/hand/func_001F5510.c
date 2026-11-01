typedef int qword __attribute__((mode(TI)));
typedef struct { qword row[3]; } M3;
struct OUT { qword axis; int mode; };
extern int D_002C3C20[];
extern qword D_002C3BA0[];
extern qword D_002A2DD0;
static inline qword Transform(M3 *matrix,qword value) {
    register qword x __asm__("vf2")=matrix->row[0];
    register qword y __asm__("vf1")=matrix->row[1];
    register qword z __asm__("vf3")=matrix->row[2];
    __asm__("vmulax.xyz $ACC,%1,%3x\n\tvmadday.xyz $ACC,%2,%3y\n\tvmaddz.xyz %0,%0,%3z" : "+j"(z) : "j"(x),"j"(y),"j"(value));
    return z;
}
static inline qword NormalizeOr(qword value,qword fallback) {
    register qword length __asm__("vf1");
    register qword one __asm__("vf2");
    register qword result __asm__("vf1");
    float scalar;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(length),"=&j"(one) : "j"(value));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(length) : "j"(length));
    __asm__("mtc1 %1,%0" : "=f"(scalar) : "r"(length));
    if(scalar<0.0001f) result=fallback;
    else __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(result) : "j"(length),"j"(value));
    return result;
}
/** @sly2-annot addr=0x1F5510
 * Purpose: Constraint table lookup: mode D_002C3C20[i], axis D_002C3BA0[i] transformed by matrix and
 *   normalized
 * Info: proposed name GetSoCnstrAxis (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F5510(void *unused,M3 *matrix,int index,struct OUT *out) {
    out->mode=D_002C3C20[index];
    out->axis=NormalizeOr(Transform(matrix,D_002C3BA0[index]),D_002A2DD0);
}
