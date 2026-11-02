typedef int qword __attribute__((mode(TI)));
typedef struct CLOCK { float t; float dt; } CLOCK;
extern CLOCK D_002DDED0;
typedef struct Q { char pad[0x30]; qword a; char pad2[0x68]; float t; char pad3[4]; qword b; } Q;
typedef struct P { char pad[8]; Q *pq; char pad2[0x14]; qword v20; qword v30; } P;
void EvaluateBezierPos(float tMax, float t, float s, qword a, qword va, qword b, qword vb, qword *ppos, qword *pv, int n);
/** @sly2-annot addr=0x12E060
 * Purpose: vtable 0x2CA6F0 slot 5: EvaluateBezierPos
 * Info: proposed name ActUnk_2CA6F0_5 (T2); TU act
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012E060(P *p, qword *pa, qword *pb, float t)
{
    float g = D_002DDED0.t;
    Q *pq = p->pq;
    if (pq->t <= g + t) {
        if (pa) *pa = p->v20;
        if (pb) *pb = p->v30;
    } else {
        EvaluateBezierPos(pq->t - g, t, 1.0f, pq->a, pq->b, p->v20, p->v30, pa, pb, 0);
    }
}
