typedef struct LO { int pad[4]; struct LO *pnext; } LO;
typedef struct SW { int pad[0x24]; LO *pfirst; } SW;
typedef struct T { int pad[2]; SW *psw; } T;
/** @sly2-annot addr=0x12D058
 * Purpose: Helper shared by attach/detach
 * Info: proposed name ActResetLink (T2); TU act
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
int func_0012D058(T *p)
{
    LO *q = p->psw->pfirst;
    while (q) {
        if (q == (LO *)p)
            return 1;
        q = q->pnext;
    }
    return 0;
}
