typedef struct KEY { float t; float g; float dgIn; float dgOut; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
int func_00117C20(void *pbis);
float func_00117E38(void *pbis);
KEY *func_001AE2A0(AREQ *preq);
/** @sly2-annot addr=0x12CCB8
 * Purpose: U16 count + F32 keys
 * Info: proposed name LoadAcFloatKeysFromBrx (T1); class AcFloatKeys; TU act
 * Notes: names_v2 B proposed GetTnfnNose__FP4TNFNP6CPDEFIP6VECTORP2TN (0.923) | level_parser CONFIRMED-parse:
 *   payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void LoadAcFloatKeysFromBrx(T *t, void *pbis)
{
    int i;
    int c = func_00117C20(pbis);
    AREQ req;
    req.cb = c << 4;
    req.cbAlign = 0x10;
    t->c = c;
    t->a = func_001AE2A0(&req);
    for (i = 0; i < t->c; i++) {
        KEY *pkey = &t->a[i];
        pkey->t = func_00117C20(pbis) * (1.0f / 60.0f);
        pkey->g = func_00117E38(pbis);
        pkey->dgIn = func_00117E38(pbis);
        pkey->dgOut = func_00117E38(pbis);
    }
}
