typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } M4;
struct O { char pad[0x40]; M4 mat; };
void InvertMatrix4Rigid(M4 *pd, M4 *ps);
static inline qword MulPos(const M4 *m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddaz.xyz $ACC, %4, %1z\n\tvmaddw.xyz %0, %5, $vf0w"
            : "=j"(r) : "j"(v), "j"(m->r[0]), "j"(m->r[1]), "j"(m->r[2]), "j"(m->r[3]));
    return r;
}
/** @sly2-annot addr=0x22FC48
 * Purpose: Transform a position from XF space a to XF space b (NULL = world): v*=a->mat; v*=inverse(b->mat)
 * Info: proposed name ConvertXfPos (T1); TU xf
 * Notes: names_v2 C proposed ElasFromAmtlk__FP4MTLK (0.438) | top50_round (0.75): Body:
 *   MulPos(&a->mat(+0x40),v) then InvertMatrix4Rigid(b->mat) and MulPos. Same pattern as sibling ConvertXfVec
 *   0x22FCC8 (no translation) and ConvertXfMat 0x22FD38. Callers pass (xfFrom, xfTo, pos): RecalcSwXpPos (T1,
 *   ve
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; static naming pass; sources:
 *   r8_21BB88+top50_round
 * See: docs/reference/functions/animation.md#xf-0x22c818-0x231100,
 *   docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
qword func_0022FC48(struct O *a, struct O *b, qword v)
{
    if (a) v = MulPos(&a->mat, v);
    if (b) {
        M4 m;
        InvertMatrix4Rigid(&m, &b->mat);
        v = MulPos(&m, v);
    }
    return v;
}
