typedef struct VT { short d; short pad; void *pfn; } VT;
typedef struct A { VT *vt; } A;
typedef struct E { unsigned int key; int k; int v; struct E *next; } E;
typedef struct H { unsigned int c; E **ap; int count; A *pa; } H;
void *func_002333B4(void *, int, int);
void func_002568C0(H *, int);
unsigned int HashPchzCaseless(int);

/** @sly2-annot addr=0x2569F8
 * Purpose: insert key/value (HashPchzCaseless of key), grow when empty [game linkonce (inline/template)]
 * Info: proposed name CHash_Insert (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
int *func_002569F8(H *h, int *pk, int *pv)
{
    unsigned int hash, i;
    E *e;
    h->count++;
    if (h->c == 0)
        func_002568C0(h, 0x35);
    hash = HashPchzCaseless(*pk);
    i = hash % h->c;
    e = ((E *(*)(void *, int, int))h->pa->vt[1].pfn)((char *)h->pa + h->pa->vt[1].d, 0x10, 3);
    func_002333B4(e, 0, 0x10);
    e->key = hash;
    e->k = *pk;
    e->v = *pv;
    e->next = h->ap[i];
    h->ap[i] = e;
    return &e->v;
}
