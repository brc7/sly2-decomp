/* func_00205910 (Lilly / Claude agent, T1593): terrain-fit of the crawl probes (our name UpdateCrawlCollide, 0x205910; see
 * re/symbols: the 3-probe floor follower of the vent crawl, called from UpdateJtGrab case 8).  MATCH, no register pins.
 * What it does: the actor's orientation is turned about the up axis (D_002A2DC0) by its heading (p+0x1A8) into a position+rotation
 * matrix matCur (copied to matOrig); the largest probe radius (length of the local probe points, start -FLT_MAX D_002A85F8) is the
 * sphere for the nearby-objects query (func_0020FC60, filter func_0020FA50 with p+0xEC as context).  Then D_002C3CE4 passes: every
 * probe point is placed with matCur and snapped to the floor by the line test func_00173610 (height->max / height->min); a probe
 * that finds no floor is dropped by D_002C3CE0 along matCur's up row.  The plane through the three probes gives a normal (cross of
 * the two edges, D_002A2DC0 if shorter than 0.0001), func_0011BA08 builds the rotation from matOrig's up row to it, func_0011BCF8
 * measures its angle, which is limited to D_002C3CE8 (func_0011B500 rebuilds the rotation about the same axis with the limit),
 * and matCur = adjustment * matOrig.  The final rotation is stored at p+0x1B0.
 * How it matches (everything came from the personal decomp's recipes, nothing from register pins):
 *  - M3 * M3 is the game's inline NRV operator* (three dead VU_VECTOR4 homes at +0x140), `pmatOrig` is a pointer declared in the loop
 *    so rows 1/2 of matOrig are read through a hoisted pointer register while row 0 is `0x40(sp)`.
 *  - the first rotation is an NRV wrapper over func_0011B500 (hidden pointer = plain `addiu a0,sp,0x80`); the loop's calls return
 *    the matrix by value (hidden pointer = a hoisted pseudo, s7).  `adjustment`/`angle` are declared after the probe loop so their
 *    frame slots come after the VU_FLOAT home of the drop scale (0xD0).
 *  - normalise-or-fallback = QwLengthUntied form (the fallback is a pointer computed before the cross product, `lq` after it);
 *    QwAddScaled adds in place ("+j" on the base) so the point/scale vf numbers match.
 *  - the frame object has its constructor/dtor called explicitly through a `frame` pointer: the RAII form loses the frame-address
 *    pseudo that the game spills at sp+0x188 (frame 0x250 instead of 0x260).
 * NOT natural: GCSE_PAD.  The spill slots 0x188/0x18C/0x190 (frame, &angle, &adjustment: gcse PRE pseudos) come out in the game's
 * order only when gcse's expression hash table has 155..173 buckets (bucket = (13259 + frame offset) mod (insns/2 | 1)); the plain
 * source has 109 (218 insns).  110 empty `asm volatile` after the last call (nothing is scheduled behind them) bring it to 165
 * without changing a single instruction.  The game's source has ~100 more insns at that point that are gone before the final
 * code; I could not find which (all-separate VU asms only add ~20).  Without the pad: 5/254 (only those slot numbers).
 */
typedef int qword __attribute__((mode(TI)));
struct MATRIX3 {float mat[3][4];} __attribute__((aligned(16)));
struct MATRIX4 {float mat[4][4];} __attribute__((aligned(16)));
struct VU_VECTOR4 {qword qw;};
union U {qword q;float f[4];int i[4];};
struct VU_FLOAT {qword qw;};
struct FRAME {FRAME(){}~FRAME(){}};
struct P {char pad0[0xEC];void *attachment;char padF0[0xB8];float heading;char pad1AC[4];MATRIX3 basis;};
struct HEIGHT {float min,max;};
struct GAME;
extern GAME D_003DD490;
extern qword D_002A2DC0;
extern const float D_002A85F8;
extern float D_002C3CE0;
extern float D_002C3CE8;
extern int D_002C3CE4;
extern "C" {
MATRIX3 func_0011B500(qword,float);
MATRIX3 *func_0011B500r(MATRIX3 *,qword,float) __asm__("func_0011B500");
MATRIX3 func_0011BA08(qword,qword);
qword func_001A62E8(P *);
qword *func_00256D18(qword *,qword);
FRAME *func_001AE478(FRAME *);
void func_001AE4A0(FRAME *,int);
void func_0020FC60(GAME *,qword,float,int (*)(void *,void *),void *,int *,void ***);
int func_0020FA50(void *,void *);
int func_00173610(qword *,float,float,int,void *);
void func_0011BCF8(MATRIX3 *,float *,qword *);
}
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwCross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"j"(a),"j"(b));return r;}
static inline float GFromVf(qword qw){float g;__asm__("qmfc2 %0, %1":"=r"(g):"j"(qw));return g;}
static inline qword QwLength(qword v){qword qw;qword vfOne;__asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z":"=j"(qw),"=j"(vfOne):"j"(v));__asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qw):"0"(qw));return qw;}
static inline float GLength(qword v){return GFromVf(QwLength(v));}
static inline qword QwLengthUntied(qword v){qword qwSq,vfOne,qwLen;__asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z":"=j"(qwSq),"=j"(vfOne):"j"(v));__asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q":"=j"(qwLen):"j"(qwSq));return qwLen;}
static inline qword QwTransformPos(MATRIX4 *pmat,qword qwPos)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w":"=j"(r):"j"(*(qword *)pmat->mat[0]),"j"(*(qword *)pmat->mat[1]),"j"(*(qword *)pmat->mat[2]),"j"(*(qword *)pmat->mat[3]),"j"(qwPos));
    return r;
}
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2 %1, %0":"=j"(v.qw):"rV"(n));return v;}
static inline qword QwAddScaled(qword base,qword vec,VU_FLOAT k){__asm__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x":"+j"(base):"j"(vec),"j"(k.qw));return base;}
static inline MATRIX3 operator*(const MATRIX3 &matL,const MATRIX3 &matR) return mat
{
    qword qwL0=*(qword *)matL.mat[0];
    qword qwL1=*(qword *)matL.mat[1];
    qword qwL2=*(qword *)matL.mat[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v0.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[0])->qw));
    *(VU_VECTOR4 *)mat.mat[0]=v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v1.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[1])->qw));
    *(VU_VECTOR4 *)mat.mat[1]=v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z":"=j"(v2.qw):"j"(qwL0),"j"(qwL1),"j"(qwL2),"j"(((VU_VECTOR4 *)matR.mat[2])->qw));
    *(VU_VECTOR4 *)mat.mat[2]=v2;
}
static inline MATRIX3 Rotation(qword axis,float angle) return r {func_0011B500r(&r,axis,angle);}
static inline void InitMat(MATRIX4 *pm,const MATRIX3 &rot,qword pos)
{
    func_00256D18((qword *)pm->mat[3],pos);
    *(MATRIX3 *)pm=rot;
    pm->mat[3][3]=1.0f;
    pm->mat[2][3]=0;pm->mat[1][3]=0;pm->mat[0][3]=0;
}
// 10 empty asm volatile (see the header: gcse hash-table size)
#define GCSE_PAD10 __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile(""); __asm__ volatile("");
/** @sly2-annot addr=0x205910
 * Purpose: Fit orientation to the floor under 3 probe points (IntersectSwBoundingSphere + line tests
 *   0x173610), tilt limited to D_002C3CE8; result in matTarget-like +0x1B0
 * Info: proposed name FitStepToTerrain (T2); TU step
 * Notes: names_v2 C proposed SetDprizeDprizes__FP6DPRIZE7DPRIZES (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#tu_14fac8-0x14fac8-0x14fca8,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
extern "C" void func_00205910(P *p,HEIGHT *height,int pointCount,const qword *localPoints)
{
    MATRIX4 matCur,matOrig;
    InitMat(&matCur,Rotation(D_002A2DC0,p->heading),func_001A62E8(p));
    matOrig=matCur;
    float radius=D_002A85F8;
    for(int i=0;i<pointCount;i++){float length=GLength(localPoints[i]);radius=(radius>length)?radius:length;}
    FRAME storage;
    FRAME *frame=&storage;
    func_001AE478(frame);
    int nearbyCount;void **nearby;
    func_0020FC60(&D_003DD490,*(qword *)matCur.mat[3],radius,func_0020FA50,p->attachment,&nearbyCount,&nearby);
    U points[3];
    U point;
    for(int pass=0;pass<D_002C3CE4;pass++){
        qword up=*(qword *)matCur.mat[2];
        for(int i=0;i<pointCount;i++){
            point.q=QwTransformPos(&matCur,localPoints[i]);
            if(func_00173610(&point.q,height->max,height->min,0,0))points[i].q=point.q;
            else points[i].q=QwAddScaled(point.q,up,VuFloatFromMemWord(*(int *)&D_002C3CE0));
        }
        MATRIX3 adjustment;
        float angle;
        const qword *pFallback=&D_002A2DC0;
        qword origin=points[0].q;
        qword first=QwSub(points[1].q,origin);
        qword second=QwSub(points[2].q,origin);
        qword qwCross=QwCross(first,second);
        qword qwFallback=*pFallback;
        qword qwLen=QwLengthUntied(qwCross);
        qword normal;
        if(GFromVf(qwLen)<0.0001f)normal=qwFallback;
        else{__asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq": :"j"(qwLen));__asm__("vmulq.xyzw %0, %1, $Q":"=j"(normal):"j"(qwCross));}
        adjustment=func_0011BA08(*(qword *)matOrig.mat[2],normal);
        func_0011BCF8(&adjustment,&angle,&point.q);
        if(D_002C3CE8<angle)adjustment=func_0011B500(point.q,D_002C3CE8);
        const MATRIX3 *pmatOrig=(MATRIX3 *)&matOrig;
        *(MATRIX3 *)&matCur=adjustment**pmatOrig;
        matCur.mat[2][3]=0;matCur.mat[1][3]=0;matCur.mat[0][3]=0;
    }
    p->basis=*(MATRIX3 *)&matCur;
    func_001AE4A0(frame,2);
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
    GCSE_PAD10
}
