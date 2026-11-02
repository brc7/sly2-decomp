typedef struct B { char pad[0x10]; int *base; } B;
typedef struct O { char pad[0xC]; B *pb; int (*pfn)(int *); char h[0x10]; } O;
int *func_002569F8(void *, int *, int *);

/** @sly2-annot addr=0x256AF0
 * Purpose: insert (hash(px), index of px) into owner hash [game linkonce (inline/template)]
 * Info: proposed name CHashIndex_Add (T2); TU game_linkonce
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00256AF0(O *p, int *px)
{
    int a[2];
    void *h = p->h;
    a[1] = px - p->pb->base;
    a[0] = p->pfn(px);
    func_002569F8(h, &a[0], &a[1]);
}
