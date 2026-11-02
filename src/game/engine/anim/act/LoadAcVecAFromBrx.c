typedef struct VECTOR3 { float x, y, z; } VECTOR3;
typedef struct KEY { VECTOR3 pos; float t; VECTOR3 dpos; int pad; VECTOR3 dpos2; int pad2; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
int U16Read__18CBinaryInputStream(void *pbis);
void ReadVector__18CBinaryInputStreamP6VECTOR(void *pbis, VECTOR3 *pv);
KEY *func_001AE2A0(AREQ *preq);
/** @sly2-annot addr=0x12B5E0
 * Purpose: Reads U16 key count and VECTOR keys (with tangents) into a heap array
 * Info: proposed name LoadAcVecAFromBrx (T1); class AcVecA; TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void LoadAcVecAFromBrx(T *t, void *pbis)
{
    int i;
    int c = U16Read__18CBinaryInputStream(pbis);
    AREQ req;
    req.cb = c * sizeof(KEY);
    req.cbAlign = 0x10;
    t->c = c;
    t->a = func_001AE2A0(&req);
    for (i = 0; i < t->c; i++) {
        KEY *pkey = &t->a[i];
        pkey->t = U16Read__18CBinaryInputStream(pbis) * (1.0f / 60.0f);
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pkey->pos);
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pkey->dpos);
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pkey->dpos2);
    }
}
