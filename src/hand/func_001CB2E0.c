struct T { int a; int *ap; };
struct S { char pad[0xE0]; struct T *pt; int x; void *pe8; };
int IcopyFromPspare__FP5PROXYP10PROXYSPARE(struct S *p, void *q);
int func_001CB2E0(struct S *p)
{
    /*B*/int i; if (!p->pe8) return 0; i = IcopyFromPspare__FP5PROXYP10PROXYSPARE(p, p->pe8); return p->pt->ap[i];/*E*/
}
