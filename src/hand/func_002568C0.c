// (Claude agent) MATCH only via register-pinned local `register E *list asm("s1")` (scheduling tie list=0 vs i=0); coordinator decide if acceptable.
typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct A { VT *vt; } A;
typedef struct E { unsigned int key; int k; int v; struct E *next; } E;
typedef struct H { int c; E **ap; int count; A *pa; } H;
void *func_002333B4(void *, int, int);

/** @sly2-annot addr=0x2568C0
 * Purpose: rehash all entries into n buckets (realloc via allocator) [game linkonce (inline/template)]
 * Info: proposed name CHash_Resize (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_002568C0(H *h, int n)
{
    register E *list asm("s1") = 0;
    E *e, *next;
    int i;
    for (i = 0; i < h->c; i++) {
        e = h->ap[i];
        if (e) {
            while (e->next)
                e = e->next;
            e->next = list;
            list = h->ap[i];
        }
    }
    h->c = n;
    h->ap = ((E **(*)(void *, void *, int, int))h->pa->vt[2].pfn)((char *)h->pa + h->pa->vt[2].d, h->ap, n * 4, 3);
    func_002333B4(h->ap, 0, h->c * 4);
    for (e = list; e; e = next) {
        unsigned int j;
        next = e->next;
        j = e->key % h->c;
        e->next = h->ap[j];
        h->ap[j] = e;
    }
}
