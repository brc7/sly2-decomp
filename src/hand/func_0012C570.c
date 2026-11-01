typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct KEY { float a, b; } KEY;
typedef struct T { int pad; int c; float *at; KEY *ak[3]; int pad2[2]; int v[4]; } T;
void func_00117EB0(void *pbis, void *pv);
int func_00117C20(void *pbis);
int func_00117BD0(void *pbis);
float func_00117E38(void *pbis);
void *func_001AE1D0(AREQ *preq);
/** @sly2-annot addr=0x12C570
 * Purpose: Second scalar curve class: load
 * Info: proposed name LoadAcFloatBFromBrx (T1); class AcFloatB; TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012C570(T *t, void *pbis)
{
    AREQ req;
    int i, j, mask, grf;
    int c;
    func_00117EB0(pbis, t->v);
    c = func_00117C20(pbis);
    req.cb = c * 4;
    req.cbAlign = 0x10;
    t->c = c;
    t->at = func_001AE1D0(&req);
    for (i = 0; i < t->c; i++)
        t->at[i] = func_00117C20(pbis) * 0.016666668f;
    grf = func_00117BD0(pbis);
    for (j = 0, mask = 1; j < 3; j++, mask <<= 1) {
        if (mask & grf) {
            KEY *ak;
            req.cb = t->c * 8;
            req.cbAlign = 0x10;
            ak = func_001AE1D0(&req);
            t->ak[j] = ak;
            for (i = 0; i < t->c; i++) {
                ak[i].a = func_00117E38(pbis);
                ak[i].b = func_00117E38(pbis);
            }
        }
    }
}
