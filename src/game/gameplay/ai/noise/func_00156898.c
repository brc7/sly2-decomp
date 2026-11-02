typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
extern float D_002BDCE0;
extern float D_002BDCF0[3];
struct Q { char pad[0x54]; float g54; char pad2[8]; float g60; };
struct S { int pad[5]; struct Q *pq; char pad2[0x4C - 0x18]; float t4C; };
static inline float GMin(float a, float b) { return a < b ? a : b; }
/** @sly2-annot addr=0x156898
 * Purpose: Current radius: template radius, grown by a quadratic over the template's ramp time
 * Info: proposed name SRadiusNoise (T2); TU noise
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/guards-ai.md#5-perception, docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
float func_00156898(struct S *p)
{
    struct Q *pq = p->pq;
    float dt, t;
    if (0.0f < pq->g60) {
        dt = GMin(D_002BDCE0, pq->g60);
        t = GMin((D_002DDED0.t - p->t4C) / dt, 1.0f);
        return pq->g54 * (D_002BDCF0[0] + t * (D_002BDCF0[1] + t * D_002BDCF0[2]));
    }
    return pq->g54;
}
