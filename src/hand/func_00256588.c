typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct L { int x0; struct L *next; VT *vt; } L;
typedef struct O { char pad[0x28]; L *first; } O;

/** @sly2-annot addr=0x256588
 * Purpose: for each node in list call node->vt[2](pv) [game linkonce (inline/template)]
 * Info: proposed name BroadcastToList (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00256588(O *p, void *pv)
{
    L *l;
    for (l = p->first; l; l = l->next)
        ((void (*)(void *, void *))l->vt[2].pfn)((char *)l + l->vt[2].d, pv);
}
