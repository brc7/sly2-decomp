typedef unsigned long ulong;
struct O { char pad[0x24]; struct O *pNext; ulong grf; char pad2[0x34]; struct O *pFirst; };
void func_001A6D28(struct O *, int);
void SetSoVisMask(struct O *p, int b)
{
    struct O *q;
    *((unsigned char *)p + 0x2D) = b;
    for (q = p->pFirst; q; q = q->pNext) {
        if (!(q->grf & (1UL << 58)))
            func_001A6D28(q, b);
    }
}
