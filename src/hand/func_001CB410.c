struct T { int n; int *ap; int c; int i; };
struct S { char pad[0xE0]; struct T *pt; };
int PspareFromIcopy(struct S *p, int i);
int func_001CB410(struct S *p, int x)
{
    /*B*/int i; int n; if (p->pt->n != 1) return 0; n = 0; for (i = p->pt->i; i < p->pt->c; i++) { if (x == p->pt->ap[i]) { n = PspareFromIcopy(p, i); break; } } return n;/*E*/
}
