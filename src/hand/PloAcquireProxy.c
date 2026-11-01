struct T { int n; int *ap; };
struct S { char pad[0xE0]; struct T *pt; };
void *PspareAcquire(struct S *p, int n);
int IcopyFromPspare__FP5PROXYP10PROXYSPARE(struct S *p, void *q);
int PloAcquireProxy(struct S *p, int f)
{
    /*B*/void *q; if (p->pt->n != 1) return 0; q = PspareAcquire(p, f ? 1 : 2); if (!q) return 0; return p->pt->ap[IcopyFromPspare__FP5PROXYP10PROXYSPARE(p, q)];/*E*/
}
