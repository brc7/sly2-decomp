typedef struct LO { int tagk; int pad[6]; struct LO *pother; } LO;
typedef struct SW { int pad[0x24]; LO *pfirst; } SW;
typedef struct T { int pad[2]; SW *psw; } T;
/** @sly2-annot addr=0x12CEE0
 * Purpose: Tests whether an ACT entry is live/attached
 * Info: proposed name FActLive (T2); TU act
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
int func_0012CEE0(T *p)
{
    LO *q = p->psw->pfirst;
    if (q == (LO *)p)
        return 1;
    if (q && q->tagk == 5 && q->pother == (LO *)p)
        return 1;
    return 0;
}
