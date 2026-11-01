// EmitJtGrabSparkles = EmitJtGrabSparkles(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand)
// (name and JTGRABCAND layout from src/hand/GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi.cpp / UpdateJtGrabCandidates__FP2SWP10STEPPLAYERiP10JTGRABCAND.cpp; psw is unused).
// For every grab candidate that has a spot, emits a random number of sparkles (emitter rate * dt, the fraction
// rounded up with that probability): each one samples a position and velocity from the grab spot, scales the
// velocity, picks a birth time in the past and optionally pushes the position along the velocity. At most ten are
// collected, then handed to the emitter's particle system in one batch.
typedef int qword __attribute__((mode(TI)));

union VECTOR { qword q; float f[4]; };
struct VU_FLOAT
{
    qword q;
    VU_FLOAT() {}
    // By reference: at loop time the bound temporary adds copies (keeps the inner loop at loop.c's size), and by
    // sched1 combine has folded the call-value copy into the asm input, so the qmtc2 reads $f0 directly.
    VU_FLOAT(const float &g) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(g)); }
};
struct VU_VECTOR
{
    qword q;
    VU_VECTOR() {}
    VU_VECTOR(qword a) { q = a; }
    VU_VECTOR(const VECTOR &v) { q = v.q; }
};

struct LM { float gMin, gMax; };
struct GRABSPOT
{
    char pad0[0x20];
    virtual int FGetWrapRange(float *pasRange);
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void GetSparkle(VECTOR *ppos, VECTOR *pv, float sParam);
};
struct LO;
struct JTGRABCAND
{
    LO *plo;
    GRABSPOT *pspot;
    float sDist;
    int nPriority;          // -1: no spot to sparkle
    LO *ploUnk10;
    float tUnk14;
    float sParam;
    float dsParam;
    qword qwPos;
};
struct EMITP                // particle parameters, at EMITB + 0x50
{
    LM lmSv;                // velocity scale
    char pad08[0x3C - 0x08];
    float dtLife;
    char pad40[0x48 - 0x40];
    LM lmOffset;            // push along the velocity
    LM lmAge;               // how long ago the particle was born
};
struct EMITB { char pad0[0x50]; EMITP emitp; };
struct PRG;
struct EMITTER
{
    char pad0[0xE0];
    int fValuesChanged;
    EMITB *pemitb;
    char padE8[0xFC - 0xE8];
    float gRate;            // particles per second
    char pad100[0x150 - 0x100];
    PRG *pprg;
};
struct EMITX { PRG **ppprg; void *ppripg; EMITTER *pemitter; };
struct SW;
struct STEPPLAYER { char pad0[0x12F8]; EMITTER *pemitterGrab; };
struct CLOCK { float t, dt; };

extern "C" {
extern CLOCK D_002DDED0;    // g_clock
float GRandInRange__Fff(float gMin, float gMax);                    // GRandInRange
void *func_002333B4(void *pv, int b, unsigned int cb);          // memset
void EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(EMITB *pemitb, EMITX *pemitx, int cpart, VECTOR *apos, VECTOR *av, float *atBirth, float *atDeath,
                   void *pv1, void *pv2);
void EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand);
}
void OnEmitterValuesChanged(EMITTER *pemitter);

static inline float GRand(LM *plm) { return GRandInRange__Fff(plm->gMin, plm->gMax); }
static inline VU_VECTOR operator*(VU_FLOAT g, VU_VECTOR v)
{
    __asm__("vmulx.xyzw %0,%0,%1x" : "+j"(v.q) : "j"(g.q));
    return v;
}
// a + b * g
static inline VU_VECTOR AddScaled(VU_VECTOR a, VU_VECTOR b, VU_FLOAT g)
{
    __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "+j"(a.q) : "j"(b.q), "j"(g.q));
    return a;
}
static inline void GetSparkle(JTGRABCAND *pcand, VECTOR *ppos, VECTOR *pv)
{
    pcand->pspot->GetSparkle(ppos, pv, pcand->sParam);
}

/** @sly2-annot addr=0x195368
 * Purpose: For every candidate spawn/refresh the grab sparkle marker effect
 * Info: proposed name EmitJtGrabSparkles (T1); TU grab
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void EmitJtGrabSparkles__FP2SWP10STEPPLAYERiP10JTGRABCAND(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand)
{
    VECTOR apos[10], av[10];
    float atBirth[10], atDeath[10];
    EMITTER *pemitter = pstepplayer->pemitterGrab;
    EMITP *pemitp;
    int cpart;
    int icand;

    if (!pemitter)
        return;
    if (pemitter->fValuesChanged)
        OnEmitterValuesChanged(pemitter);
    pemitp = &pemitter->pemitb->emitp;

    cpart = 0;
    for (icand = 0; icand < ccand; icand++) {
        JTGRABCAND *pcand = &ajtgrabcand[icand];
        float g, gRand;
        int c;

        if (pcand->nPriority == -1)
            continue;
        g = pemitter->gRate * D_002DDED0.dt;
        gRand = GRandInRange__Fff(0.0f, 1.0f);
        c = (int)g;
        g -= (float)c;
        if (gRand < g)
            c++;

        while (--c >= 0) {
            VECTOR pos, v, vScaled;
            VU_FLOAT s;

            GetSparkle(pcand, &pos, &v);
            s = VU_FLOAT(GRand(&pemitp->lmSv));
            VU_VECTOR vs = s * v;
            vScaled.q = vs.q;
            float tBirth = D_002DDED0.t;
            tBirth -= GRand(&pemitp->lmAge);
            atBirth[cpart] = tBirth;
            atDeath[cpart] = tBirth + pemitp->dtLife;
            if (pemitp->lmOffset.gMin != pemitp->lmOffset.gMax) {
                VU_VECTOR posOld = pos;
                s = VU_FLOAT(GRand(&pemitp->lmOffset));
                pos.q = AddScaled(posOld, vScaled, s).q;
            }
            apos[cpart] = pos;
            av[cpart] = vScaled;
            cpart++;
            if (cpart >= sizeof(atBirth) / sizeof(atBirth[0]))
                goto LEmit;
        }
    }

LEmit:
    if (cpart > 0) {
        EMITX emitx;
        func_002333B4(&emitx, 0, sizeof(emitx));
        emitx.ppprg = &pemitter->pprg;
        emitx.pemitter = pemitter;
        EmitBlips__FP5EMITBP5EMITGiP6VECTORT3PfT5T3T3(pemitter->pemitb, &emitx, cpart, apos, av, atBirth, atDeath, 0, 0);
    }
}
