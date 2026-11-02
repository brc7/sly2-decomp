typedef struct Q { char pad[0x10]; struct Q *next; } Q;
typedef struct P { char pad[0x2C]; int f; int pad2; Q *pq; char pad3[8]; struct P *next; } P;
extern P *D_003E159C;
void func_001EC718(Q *q);
/** @sly2-annot addr=0x1EE7B0
 * Purpose: For every dirty SAA in D_003E159C apply its copy records and clear dirty
 * Info: proposed name PropagateSais (T1); TU shdanim
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988
 * @end-sly2-annot */
void PropagateSais(void)
{
    P *p;
    Q *q;
    int unused[4];
    for (p = D_003E159C; p; p = p->next) {
        if (p->f) {
            p->f = 0;
            for (q = p->pq; q; q = q->next) func_001EC718(q);
        }
    }
}
