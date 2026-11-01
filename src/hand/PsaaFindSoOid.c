struct T { char pad[0x18]; int n; };
struct S { char pad[0xA4]; int c; struct T **apt; };
struct T *PsaaFindSoOid(struct S *p, int n)
{
    int i;
    for (i = 0; i < p->c; i++) { struct T *pt = p->apt[i]; if (pt->n == n) return pt; }
    return 0;
}
