typedef struct LO LO;
struct LO {
    char pad[0x10];
    LO *pParent;
    char pad14[0x14];
    int x28;
    signed char ips;
    char x2D;
    long k : 4;
    unsigned long b : 8;
    unsigned int f : 1;
};
typedef struct CH { void *p; struct CH *pNext; } CH;
extern int IsrcFromIdProxy__Fi(void *);
void MatchSwObject(void *pv, LO *plo, CH *pch, unsigned int grf, void *ctx, CH *pchList, int cMax, int *pc, LO **aplo, int *pips)
{
    LO *p;
    if (!(grf & 0x400)) {
        CH *q;
        switch (plo->k) {
        case 0:
            q = pchList;
            while (1) {
                if (!q)
                    return;
                if (IsrcFromIdProxy__Fi(q->p) == IsrcFromIdProxy__Fi(pch->p))
                    break;
                if (grf & 0x800)
                    return;
                q = q->pNext;
            }
            break;
        case -1:
            while (pchList != pch) {
                if (!pchList || (grf & 0x800))
                    return;
                pchList = pchList->pNext;
            }
            break;
        case 1:
            break;
        }
    }
LMatch:
    switch (grf & 0xFF) {
    case 2:
        if (plo->pParent == ctx)
            goto LAdd;
        return;
    case 1:
        for (p = plo; p; p = p->pParent) {
            if (p->ips == 0)
                continue;
            if (p->ips == 1)
                goto LAdd;
            return;
        }
        return;
    case 3:
        if (plo->ips >= 2)
            goto LAdd;
        return;
    case 4: {
        for (p = plo; p; p = p->pParent) {
            int i = p->ips;
            if (i != 0 && i < *pips) {
                if ((grf & 0x100) && plo->f == 0)
                    return;
                *pips = i;
                *pc = 0;
                goto LAdd2;
            }
        }
        if (*pips != 0x7FFFFFFF || p)
            return;
        goto LAdd;
    }
    case 5:
    LAdd:
        if ((grf & 0x100) && plo->f == 0)
            return;
    LAdd2:
        if (*pc < cMax)
            aplo[*pc] = plo;
        (*pc)++;
        break;
    }
}
