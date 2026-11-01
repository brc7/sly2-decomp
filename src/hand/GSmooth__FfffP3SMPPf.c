/* GSmooth (Sly 1 util.c). Code-identical except ONE toolchain nop: target has `sub.s; nop; div.s` at 0x11D0B8
   (div.s is the 2nd insn of a basic block -> original assembler pads it to position 2 with 1 nop; asfix only pads position 0). */
typedef struct SMP { float gA; float gB; float gC; } SMP;
int CSolveQuadratic__FfffPf(float a, float b, float c, float *ax);

float GSmooth__FfffP3SMPPf(SMP *psmp, float *pdgNext, float gCur, float gTarget, float dt)
{
    float dg;
    float gNew;
    float dgNew;
    float gThresh;
    float ax[2];
    int fNeg;
    int c;
    float t;

    fNeg = 0;
    if (psmp->gA == 0.0f) {
        if (pdgNext)
            *pdgNext = 0.0f;
        return gCur;
    }

    dg = gCur - gTarget;
    if (dg < 0.0f) {
        dg = -dg;
        fNeg = 1;
    }

    gThresh = (psmp->gB + psmp->gA) * psmp->gC * 0.5f;
    if (gThresh <= dg) {
        float dtT = (dg - gThresh) / psmp->gA;
        if (dt <= dtT) {
            gNew = dg - psmp->gA * dt;
            dgNew = -psmp->gA;
            goto LDone;
        }
        dt -= dtT;
        dg = gThresh;
    }

    c = CSolveQuadratic__FfffPf((psmp->gA - psmp->gB) / (psmp->gC + psmp->gC), psmp->gB, -dg, ax);
    t = ax[c - 1] - dt;
    if (0.0f < t) {
        float b = psmp->gB;
        gNew = (psmp->gA - b) * t * t / (psmp->gC + psmp->gC) + b * t;
        dgNew = -(b + (psmp->gA - b) * t / psmp->gC);
    } else {
        gNew = 0.0f;
        dgNew = -psmp->gB;
    }

LDone:
    if (fNeg) {
        gNew = gTarget - gNew;
        if (pdgNext)
            *pdgNext = -dgNew;
    } else {
        gNew = gNew + gTarget;
        if (pdgNext)
            *pdgNext = dgNew;
    }
    return gNew;
}
