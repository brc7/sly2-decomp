typedef struct N { int x0; unsigned prev : 13; unsigned prevpg : 2; unsigned p1 : 1; unsigned next : 13; unsigned nextpg : 2; unsigned p2 : 1; char pad[0x14 - 8]; } N;
extern N *D_00452160[4];
extern unsigned int D_002DD2D4;

static inline void Handle(N *p, int *pidx, int *ppg)
{
    unsigned int i;
    for (i = 0; i < D_002DD2D4; i++) {
        if (p >= D_00452160[i] && p < D_00452160[i] + 0x100) {
            *pidx = p - D_00452160[i];
            *ppg = i;
            return;
        }
    }
    *pidx = 0;
    *ppg = 0;
}

/** @sly2-annot addr=0x255F38
 * Purpose: Splice GC pool template: push node onto a list (prev/next stored as (index,page) pairs) [game
 *   linkonce (inline/template)]
 * Info: proposed name GcPool6_PushFront (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void GcPool6_PushFront(N *p, N **phead)
{
    int idx0, pg0, idx1, pg1;
    N *head;
    Handle(p, &idx0, &pg0);
    head = *phead;
    if (head) {
        head->prev = idx0;
        head->prevpg = pg0;
    }
    Handle(*phead, &idx1, &pg1);
    p->next = idx1;
    p->nextpg = pg1;
    *phead = p;
}
