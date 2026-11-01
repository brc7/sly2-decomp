typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
struct LM { float gMin; float gMax; };
extern qword D_002A2DA0;
extern qword D_002A2DB0;
float func_0011D448(float a, float b);
float func_0011CF60(float g);
qword func_0011E208(float pan, float tilt, float r);
void func_0011E2D8(qword a, qword b, qword c, M3 *pm);

/** @sly2-annot addr=0x15AF08
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ChooseEmitVelocity__FP6EMITVXffP2LMP6VECTORiT4(struct EMITVX *pemitvx, float gRand, float sv, struct LM *plm,
                                                     qword *pnormal, int i, qword *pv)
{
    int c = pemitvx->c;
    int iTilt = i / c;
    int iPan = i % c;
    float tilt = pemitvx->tiltBase + (float)iTilt * pemitvx->dTilt;
    float pan = pemitvx->panBase + (float)iPan * pemitvx->dPan;
    U v;
    M3 m;
    qword r;
    if (gRand != 0.0f) {
        tilt += gRand * func_0011D448(-0.5f, 0.5f) * pemitvx->dTilt;
        pan += gRand * func_0011D448(-0.5f, 0.5f) * pemitvx->dPan;
    }
    tilt = func_0011CF60(tilt + 1.5707964f);
    pan = func_0011CF60(pan);
    v.q = func_0011E208(pan, tilt, func_0011D448(plm->gMin, plm->gMax));
    v.f[2] *= sv;
    m.r[2] = *pnormal;
    func_0011E2D8(*pnormal, D_002A2DA0, D_002A2DB0, &m);
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v.q), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    *pv = r;
}
