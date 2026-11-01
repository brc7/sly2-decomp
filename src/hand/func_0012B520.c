typedef struct VEC3 { float x, y, z; } VEC3;
typedef struct VECTOR { float x, y, z; } __attribute__((aligned(16))) VECTOR;
typedef struct S { int pad; VEC3 pos; } S;
typedef struct T {
    char pad[0x80];
    int n;          /* 0x80 */
    float g;        /* 0x84 */
    int *pn;        /* 0x88 */
    float *pg;      /* 0x8C */
    char pad2[0x9A - 0x90];
    short s;        /* 0x9A */
    char pad3[0xC0 - 0x9C];
    VECTOR v0;      /* 0xC0 */
    VECTOR v1;      /* 0xD0 */
    VECTOR v2;      /* 0xE0 */
    VECTOR v3;      /* 0xF0 */
} T;
static __inline__ void ClearVec(VECTOR *pv) { pv->x = pv->y = pv->z = 0; }
/** @sly2-annot addr=0x12B520
 * Purpose: Returns the constant value
 * Info: proposed name EvaluateAcConst (T2); class AcConst; TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012B520(S *s, T *t)
{
    t->s = 2;
    t->g = 1.0f;
    t->pn = &t->n;
    t->pg = &t->g;
    t->n = 0;
    *(VEC3 *)&t->v0 = s->pos;
    t->v2 = t->v0;
    ClearVec(&t->v1);
    ClearVec(&t->v3);
}
