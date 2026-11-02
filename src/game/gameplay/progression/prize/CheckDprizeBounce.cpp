// CheckDprizeBounce: one commented pin (QwNormalizeInto), one allocation-only asm operand (GFromVf), VU one-op asm helpers only.
// dprize-style collision response (next to matched FCheckDprizeCollect): query the radius box with IntersectSwBoundingBox, find the deepest
// sphere / CplaneFromSphere contact that the velocity approaches, then push the position out and rebuild the velocity (floor case:
// tangent + clamped normal parts, one-shot boost; otherwise normal/tangent split with the two response factors).
// Keys (wave 4, from w3lane8.v3 at aligned 52): call results passed raw so gcc emits the qmtc2; QwNormalizeInto reads the vector
// like matched ProjectLeafTransform's NormalizeSafe; GDot's 1.0 temp is not early-clobber; "rV" on every qmtc2 of a float word read from
// memory (the lw becomes a reload: frees local-alloc and resets reload's round-robin); gSpeed read with the mtc1 form.
// Last fix (wave 5): the length test also reads its float with the mtc1 asm. A reload mtc1 has latency 2, which gave the
// gNormal copy (anti dependence on that $f0 write) sched2 priority 7 > 6 for the li.s 1e-4; the asm mtc1 costs 1, so the
// two tie and the li.s goes first as in the target.
typedef int qword __attribute__((mode(TI)));
union U { qword qw; float g[4]; };
union FLOATWORD { float g; int bits; };
struct XF { char pad[0x70]; qword pos; };
struct SOPART { XF *pxf; void *pso; float sRadius; int oid; unsigned long grf; char pad18[0x5C]; int cplane; };
struct CMCONTACT { qword pos; U plane; int kind; void *pv; };
struct P {
    char pad0[0x150]; int state; char pad154[0x1C]; qword pos; qword v;
    char pad190[0x34]; int fRaised; char pad1C8[8]; float gThreshold; FLOATWORD gBoost; FLOATWORD sRadius;
    int unk1DC; float gNormalResponse; FLOATWORD gTangentResponse;
};
class FRAME;
extern "C" {
extern char D_003DD490[];
extern qword D_002A2DA0, D_002A2DC0, D_002A2DD0, D_002A2DE0;
extern float D_002C0D74, D_002C0D78;
void func_001AE478(FRAME *);
void func_001AE4A0(FRAME *, int);
qword func_001A62E8(P *);
int FFilterDprizeObjects(void *, SOPART *);
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, SOPART *), void *, int *, SOPART ***);
int func_001A93C8(qword, float, SOPART *, void *, int, void *, int, CMCONTACT *);
float func_0011E290(qword, qword, qword *, qword *);
}
class FRAME { public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };

struct VU_FLOAT {
    qword qw;
    VU_FLOAT(float g) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "r"(g)); }
    VU_FLOAT(int bits) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(bits)); }
};
static inline qword VuFloat(float g) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(g)); return r; }
static inline qword VuFloatBits(int g) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "rV"(g)); return r; }

static inline float GFromQw(qword q) { float g; __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(q)); return g; }
/* the second "j"(q) is allocation-only: it keeps the length qword in a VU register (the "r" use alone ties the class
   costs and puts it in a GPR), so gcc reloads it with its own qmfc2 into v1 like the target. */
static inline float GFromVf(qword q) { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q), "j"(q)); return g; }
static inline qword QwAddScaled(qword base, qword dir, qword g)
{
    qword r;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(base), "j"(dir), "j"(g));
    return r;
}
static inline qword QwCombo(VU_FLOAT g1, qword v1, VU_FLOAT g2, qword v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(g1.qw), "j"(v1), "j"(g2.qw), "j"(v2));
    return r;
}
static inline qword QwSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword QwLength(qword v)
{
    qword d, one, len;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(len) : "j"(d));
    return len;
}
static inline qword QwUnit(qword v, qword len)
{
    qword r;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(r) : "j"(v), "j"(len));
    return r;
}
static inline qword QwNormalize(qword v, qword fallback)
{
    qword len = QwLength(v);
    if (GFromQw(len) < 0.0001f)
        return fallback;
    return QwUnit(v, len);
}
static inline qword QwNormalizeInto(qword *pv, const qword &fallback)
{
    register qword g asm("$2") = *(volatile qword *)pv; /* the one pin (as in ProjectLeafTransform): the target reads the vector into $v0, then qmtc2 */
    qword v = g;
    qword len = QwLength(v);
    if (GFromVf(len) < 0.0001f)
        *pv = fallback;
    else
        *pv = QwUnit(v, len);
    return len;
}
static inline float GDot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=j"(one) : "j"(a), "j"(b));
    return GFromQw(d);
}
static inline float GMin(float a, float b) { return (b < a) ? b : a; }

/** @sly2-annot addr=0x1C55E0
 * Purpose: query nearby solids (0x20FAA0) and resolve deepest contact (bounce)
 * Info: proposed name CheckDprizeBounce (T1); TU prize
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
extern "C" void CheckDprizeBounce(P *p)
{
    FRAME frame;
    U vec;
    float gDeepest = 0.0f;
    qword pos = func_001A62E8(p);
    qword posMin = (vec.qw = VuFloat(-p->sRadius.g), QwAddScaled(pos, D_002A2DE0, vec.qw));
    qword posMax = func_001A62E8(p);
    vec.qw = VuFloatBits(p->sRadius.bits);
    posMax = QwAddScaled(posMax, D_002A2DE0, vec.qw);
    int cpart;
    SOPART **appart;
    IntersectSwBoundingBox(D_003DD490, posMin, posMax, FFilterDprizeObjects, 0, &cpart, &appart);
    U normal;
    CMCONTACT acontact[8];
    for (int i = 0; i < cpart; i++) {
        SOPART *ppart = appart[i];
        if (ppart->grf & (1UL << 49)) {
            qword center = ppart->pxf->pos;
            qword posCur = func_001A62E8(p);
            float g = GFromQw(QwLength(QwSub(center, posCur))) - ppart->sRadius - p->sRadius.g;
            if (g < gDeepest) {
                vec.qw = QwNormalize(QwSub(p->pos, ppart->pxf->pos), D_002A2DA0);
                if (GDot(p->v, vec.qw) < 0.0f) {
                    gDeepest = g;
                    normal.qw = vec.qw;
                }
            }
        } else if (ppart->cplane) {
            int ccontact = func_001A93C8(func_001A62E8(p), p->sRadius.g, ppart, (void *)0x7FFFFFFF, 0, 0, 8, acontact);
            for (int j = 0; j < ccontact; j++) {
                CMCONTACT *pcontact = &acontact[j];
                float g = GDot(pcontact->pos, pcontact->plane.qw) - pcontact->plane.g[3];
                if (g < gDeepest && GDot(p->v, pcontact->plane.qw) < 0.0f) {
                    normal.qw = pcontact->plane.qw;
                    gDeepest = g;
                }
            }
        }
    }
    if (gDeepest < 0.0f) {
        U response;
        {
            U primary;
            if (normal.g[2] > 0.7f && p->state == 4) {
                float gNormal = func_0011E290(p->v, D_002A2DC0, 0, &primary.qw);
                float gSpeed = GFromVf(QwNormalizeInto(&primary.qw, D_002A2DD0));
                float gTangent = GMin(p->gTangentResponse.g * gSpeed, D_002C0D74);
                gNormal = GMin(-p->gNormalResponse * gNormal, D_002C0D78);
                response.qw = QwCombo(VU_FLOAT(gTangent), primary.qw, VU_FLOAT(gNormal), D_002A2DC0);
                if (!p->fRaised) {
                    int fTrigger = GDot(response.qw, normal.qw) < p->gThreshold;
                    if (fTrigger) {
                        response.qw = QwCombo(VU_FLOAT(gTangent), primary.qw, VU_FLOAT(p->gBoost.bits), D_002A2DC0);
                        p->fRaised = 1;
                    }
                }
            } else {
                U secondary;
                func_0011E290(p->v, normal.qw, &primary.qw, &secondary.qw);
                response.qw = QwCombo(VU_FLOAT(-p->gNormalResponse), primary.qw, VU_FLOAT(p->gTangentResponse.bits), secondary.qw);
            }
        }
        p->pos = QwAddScaled(p->pos, normal.qw, VU_FLOAT(-gDeepest).qw);
        p->v = response.qw;
    }
}
