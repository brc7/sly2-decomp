typedef unsigned long ulong;
struct X;
struct N { int n0; struct N *next; int pad; struct X *px; };
struct L { char pad[0x64]; struct N *pn; };
struct E { char pad[0xA0]; int nA0; char pad2[0xC0 - 0xA4]; int nC0; };
struct X { char pad[0xE8]; struct E *pe; struct L *pl; char pad2[0x148 - 0xF0]; ulong fl; };
void BuildContactGroup(void *pv, struct X *px, int *pc, struct X **apx)
{
    struct N *pn;
    px->fl |= 0x8000000;
    apx[*pc] = px;
    (*pc)++;
    for (pn = px->pl->pn; pn; pn = pn->next) {
        struct X *q;
        struct E *pe;
        if (!pn->n0) continue;
        q = pn->px;
        if (q->fl & 0x8000000) continue;
        pe = q->pe;
        if (!pe) continue;
        if (pe->nC0 == 3 && pe->nA0 == 3) continue;
        BuildContactGroup(pv, q, pc, apx);
    }
}
