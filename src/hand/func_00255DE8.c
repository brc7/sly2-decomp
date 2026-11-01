typedef struct N { int x0; unsigned prev : 13; unsigned prevpg : 2; unsigned p1 : 1; unsigned next : 13; unsigned nextpg : 2; unsigned p2 : 1; } N;
extern char *D_00452138[4];
extern N *D_002DD2C0, *D_002DD2C8, *D_002DD2CC;

static inline N *Ptr(unsigned idx, unsigned pg)
{
    if (idx == 0 && pg == 0)
        return 0;
    return (N *)(D_00452138[pg] + idx * 12);
}

/** @sly2-annot addr=0x255DE8
 * Purpose: unlink node from its GC list (fix list heads) [game linkonce (inline/template)]
 * Info: proposed name GcSidebag_Unlink (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00255DE8(N *p)
{
    N *prev = Ptr(p->prev, p->prevpg);
    N *next = Ptr(p->next, p->nextpg);
    if (p == D_002DD2C8)
        D_002DD2C8 = next;
    if (prev) {
        prev->next = p->next;
        prev->nextpg = p->nextpg;
    } else if (p == D_002DD2C0) {
        D_002DD2C0 = next;
    } else if (p == D_002DD2CC) {
        D_002DD2CC = next;
    }
    if (next) {
        next->prev = p->prev;
        next->prevpg = p->prevpg;
    }
}
