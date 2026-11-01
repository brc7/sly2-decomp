struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
void func_0011CD98(void *, void *);
void UnsubscribeLoObject(struct S *p, int id) {
    struct E **ppe;
    struct E *pe;
    for (ppe = &p->pfirst; (pe = *ppe) != 0; ppe = &pe->pnext) { if (pe->id == id) { if (--pe->cref <= 0) { *ppe = pe->pnext; func_0011CD98(D_003E1144, pe); } break; } }
}
