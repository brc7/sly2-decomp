typedef unsigned long ulong;
struct X;
struct N { int n0; struct N *next; int pad; struct X *px; };
struct L { char pad[0x64]; struct N *pn; };
struct E { char pad[0xA0]; int nA0; char pad2[0xC0 - 0xA4]; int nC0; };
struct X { char pad[0xE8]; struct E *pe; struct L *pl; char pad2[0x148 - 0xF0]; ulong fl; };
/** @sly2-annot addr=0x1F8E90
 * Purpose: Flood-fill: mark SO (fl|=0x8000000), append to group array, recurse through touching contacts
 * Info: proposed name BuildContactGroup (T1); TU solve
 * Notes: names_v2 v1 proposed FGetSoContactList__FP2SOPv (0.064)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
void func_001F8E90(void *pv, struct X *px, int *pc, struct X **apx)
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
        func_001F8E90(pv, q, pc, apx);
    }
}
