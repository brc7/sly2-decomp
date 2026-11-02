typedef struct N { int x0; unsigned prev : 13; unsigned prevpg : 2; unsigned p1 : 1; unsigned next : 13; unsigned nextpg : 2; unsigned mark : 1; } N;
extern char *D_00452110[4];
extern char D_00452120[];
extern N *D_002DD2B0, *D_002DD2B8;
extern unsigned int D_002DD598[1];
void GcProc_Unlink(N *);
void func_0011CD98(void *, void *);

static inline N *Ptr(unsigned idx, unsigned pg)
{
    if (idx == 0 && pg == 0)
        return 0;
    return (N *)(D_00452110[pg] + idx * 0x1C);
}

static inline N *Next(N *p)
{
    return Ptr(p->next, p->nextpg);
}

/** @sly2-annot addr=0x257948
 * Purpose: GC sweep of one pool (walk list, free unmarked nodes) [game linkonce (inline/template)]
 * Info: proposed name GcSweepProcs (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00257948(void)
{
    N *p;
    p = D_002DD2B0;
    while (p != D_002DD2B8) {
        N *n;
        if (p->mark != D_002DD598[0])
            p->mark = D_002DD598[0];
        n = Next(p);
        p = n;
    }
    while (p) {
        N *cur = p;
        N *next = Next(cur);
        p = next;
        if (cur->mark != D_002DD598[0]) {
            GcProc_Unlink(cur);
            func_0011CD98(D_00452120, cur);
        }
    }
}
