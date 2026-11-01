typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
struct EMITVX { int c; float dTilt; float dPan; float tiltBase; float panBase; };
struct LM { float gMin; float gMax; };
extern qword D_002A2DA0;
extern qword D_002A2DB0;
float GRandInRange__Fff(float a, float b);
float RadNormalize__Ff(float g);
qword SetVectorSphere(float pan, float tilt, float r);
void GetNormalVectors(qword a, qword b, qword c, M3 *pm);

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
        tilt += gRand * GRandInRange__Fff(-0.5f, 0.5f) * pemitvx->dTilt;
        pan += gRand * GRandInRange__Fff(-0.5f, 0.5f) * pemitvx->dPan;
    }
    tilt = RadNormalize__Ff(tilt + 1.5707964f);
    pan = RadNormalize__Ff(pan);
    v.q = SetVectorSphere(pan, tilt, GRandInRange__Fff(plm->gMin, plm->gMax));
    v.f[2] *= sv;
    m.r[2] = *pnormal;
    GetNormalVectors(*pnormal, D_002A2DA0, D_002A2DB0, &m);
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(r) : "j"(v.q), "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]));
    *pv = r;
}
