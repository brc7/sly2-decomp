typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct A { VT *vt; } A;
typedef struct E { int key; int k; int v; struct E *next; } E;
typedef struct H { int c; E **ap; int count; A *pa; } H;
void *func_002333B4(void *, int, int);

#define FREE(pa, p) ((void (*)(void *, void *))(pa)->vt[3].pfn)((char *)(pa) + (pa)->vt[3].d, p)

/** @sly2-annot addr=0x2567D0
 * Purpose: free all chained entries and bucket array through allocator vt[3] [game linkonce
 *   (inline/template)]
 * Info: proposed name CHash_Clear (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002567D0(H *h)
{
    int i;
    for (i = 0; i < h->c; i++) {
        E *e = h->ap[i];
        while (e) {
            E *next = e->next;
            FREE(h->pa, e);
            e = next;
        }
    }
    func_002333B4(h->ap, 0, h->c * 4);
    h->count = 0;
    FREE(h->pa, h->ap);
    h->c = 0;
    h->ap = 0;
}
