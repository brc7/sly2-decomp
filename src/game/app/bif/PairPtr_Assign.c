// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int ref; char pad[4]; int c; char pad2[4]; void *m; } S;
extern char D_004520A8[];
void PairPtr_Release(void *, int);
void Decref__4CRef(void *); void GcPair_Unlink(S *); void func_0011CD98(void *, S *);
void PairPtr_Assign(S **pp, S *n)
{
    S *p;
    if (n) n->ref++;
    p = *pp;
    if (p) {
        if (--p->ref == 0) {
            PairPtr_Release(&p->m, 2);
            Decref__4CRef(&p->c);
            GcPair_Unlink(p);
            func_0011CD98(D_004520A8, p);
        }
    }
    *pp = n;
}
