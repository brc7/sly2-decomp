// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int ref; char pad[8]; int c; char pad2[4]; void *m; } S;
extern char D_004520D0[];
void func_00257250(void *, int);
void Decref__4CRef(void *); void GcBinding_Unlink(S *); void func_0011CD98(void *, S *);
void func_00257450(S **pp, S *n)
{
    S *p;
    if (n) n->ref++;
    p = *pp;
    if (p) {
        if (--p->ref == 0) {
            func_00257250(&p->m, 2);
            Decref__4CRef(&p->c);
            GcBinding_Unlink(p);
            func_0011CD98(D_004520D0, p);
        }
    }
    *pp = n;
}
