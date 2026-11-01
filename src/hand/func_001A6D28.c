struct VTE { short d; short pad; void (*pfn)(); };
struct ALO {
    char pad[0x28];
    unsigned long b0 : 52;
    long n52 : 4;
    unsigned int b56 : 4;
    unsigned int f60 : 1;
};
struct SW { struct ALO *palo; };
struct O {
    char pad[0xC];
    struct VTE *vt;
    struct O *pNext;
    struct SW *psw;
    char pad2[0x15];
    unsigned char b2D;
};
extern char D_003DD490[];
void func_00147E88(void *, void *);
/** @sly2-annot addr=0x1A6D28
 * Purpose: Re-sort root, vt46(b) on plo, then OR b into grfvis(x2D) of following siblings
 * Info: proposed name SetLoVisMaskHierarchy (T2); TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001A6D28(struct O *p, int b)
{
    struct O *q;
    if (p->psw->palo->f60)
        func_00147E88(D_003DD490, p->psw);
    {
        struct VTE *e = &p->vt[46];
        ((void (*)(void *, int))e->pfn)((char *)p + e->d, b);
    }
    for (q = p->pNext; q; q = q->pNext)
        q->b2D |= b;
}
