typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct KEY { float a, b, c; } KEY;
typedef struct T { int pad; int c; float *at; KEY *ak[3]; int pad2[2]; int v[4]; } T;
void ReadVector__18CBinaryInputStreamP6VECTOR(void *pbis, void *pv);
int U16Read__18CBinaryInputStream(void *pbis);
int U8Read__18CBinaryInputStream(void *pbis);
float F32Read__18CBinaryInputStream(void *pbis);
void *func_001AE1D0(AREQ *preq);
/** @sly2-annot addr=0x12BF30
 * Purpose: Scalar-keyed curve: U16 count, U8 flags, F32 keys and vectors
 * Info: proposed name LoadAcFloatAFromBrx (T1); class AcFloatA; TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void LoadAcFloatAFromBrx(T *t, void *pbis)
{
    AREQ req;
    int i, j, mask, grf;
    int c;
    ReadVector__18CBinaryInputStreamP6VECTOR(pbis, t->v);
    c = U16Read__18CBinaryInputStream(pbis);
    req.cb = c * 4;
    req.cbAlign = 0x10;
    t->c = c;
    t->at = func_001AE1D0(&req);
    for (i = 0; i < t->c; i++)
        t->at[i] = U16Read__18CBinaryInputStream(pbis) * 0.016666668f;
    grf = U8Read__18CBinaryInputStream(pbis);
    for (j = 0, mask = 1; j < 3; j++, mask <<= 1) {
        if (mask & grf) {
            KEY *ak;
            req.cb = t->c * 12;
            req.cbAlign = 0x10;
            ak = func_001AE1D0(&req);
            t->ak[j] = ak;
            for (i = 0; i < t->c; i++) {
                ak[i].a = F32Read__18CBinaryInputStream(pbis);
                ak[i].b = F32Read__18CBinaryInputStream(pbis);
                ak[i].c = F32Read__18CBinaryInputStream(pbis);
            }
        }
    }
}
