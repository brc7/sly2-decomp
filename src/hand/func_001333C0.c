struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
struct E *PvAllocSlotheapClear(void *);
void func_001333C0(struct S *p, int id) {
    struct E *pe;
    for (pe = p->pfirst; pe; pe = pe->pnext) {
        if (pe->id == id) { pe->cref++; return; }
    }
    {
        register struct E *peNew __asm__("$3") = PvAllocSlotheapClear(D_003E1144);
        __asm__("" : "+r"(peNew));
        peNew->id = id;
        { register int one __asm__("$2") = 1; __asm__("" : "+r"(one)); peNew->cref = one; }
        peNew->pnext = p->pfirst;
        p->pfirst = peNew;
    }
}
