struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
void func_0011CD98(void *, void *);
void UnsubscribeLoAll(struct S *p) {
    struct E *pe = p->pfirst;
    while (pe) { struct E *pNext = pe->pnext; func_0011CD98(D_003E1144, pe); pe = pNext; }
    p->pfirst = 0;
}
