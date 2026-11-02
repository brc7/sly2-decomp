typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct REC { int id; float v[2]; float g; } REC;
typedef struct T { char pad[0xE4]; int c; void *pad2; void **ap; int crec; REC *arec; } T;
typedef struct G { char pad[0x228]; void *p; } G;
extern G D_003DD040;
void PostLoLoad(T *t);
void **func_001AE2A0(AREQ *preq);
void *func_001FE990(G *pg, int id);
void func_001FC408(float g, void *p, void *q, T *t, float *pv);
/** @sly2-annot addr=0x12EC50
 * Purpose: slot 15: PostLoLoad, allocates, registers with the stream-sound group (0x1FC408) and IambFind
 *   (0x1FE990)
 * Info: proposed name PostCid43Load (T2); class CID43 slot15; TU amb
 * Notes: names_v2 C proposed ConvertAloMat__FP3ALOT0P7MATRIX3T2 (0.438)
 * Confidence: low (draft) [T2 0.85] - evidence: static naming pass; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#amb-0x12eb48-0x12f0f0
 * @end-sly2-annot */
void func_0012EC50(T *t)
{
    AREQ req;
    void *p;
    PostLoLoad(t);
    req.cb = t->c * 4;
    req.cbAlign = 0x10;
    t->ap = func_001AE2A0(&req);
    p = D_003DD040.p;
    if (p) {
        int i;
        for (i = 0; i < t->crec; i++) {
            REC *prec = &t->arec[i];
            func_001FC408(prec->g, p, func_001FE990(&D_003DD040, prec->id), t, prec->v);
        }
    }
}
