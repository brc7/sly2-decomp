int GrfzpdFromSoGrfzpk(void *p, void *q);
struct N { char pad[0x10]; struct N *next; };
struct N *func_001F7530(struct N *p, void *q)
{
    for (; p; p = p->next) {
        if (GrfzpdFromSoGrfzpk(p, q))
            return p;
    }
    return 0;
}
