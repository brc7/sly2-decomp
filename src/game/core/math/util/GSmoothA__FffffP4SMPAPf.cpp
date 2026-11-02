struct SMP {
    float gMaxSpeed;
    float gMinSpeed;
    float rSpeed;
};
struct SMPA : SMP {
    float gAccel;
};
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);

/** @sly2-annot addr=0x11D1C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
float GSmoothA(float gCur, float dgCur, float gTarget, float dt, SMPA *psmpa, float *pdgNext)
{
    float dgNext;
    float g = GSmooth(gCur, gTarget, dt, psmpa, &dgNext);
    if (psmpa->gAccel > 0.0f) {
        float ddg = psmpa->gAccel * dt;
        float dgMax = dgCur + ddg;
        float dgMin = dgCur - ddg;
        if (dgNext > dgMax) {
            dgNext = dgMax;
            g = gCur + dgCur * dt + 0.5f * psmpa->gAccel * dt * dt;
        } else if (dgNext < dgMin) {
            dgNext = dgMin;
            g = gCur + dgCur * dt - 0.5f * psmpa->gAccel * dt * dt;
        }
    }
    if (pdgNext)
        *pdgNext = dgNext;
    return g;
}
