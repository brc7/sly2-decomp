struct D { int a; int ib; };
extern char D_003DD490[];
int PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, void *);
void PostLoadOidField__FP2LOPv(char *p, struct D *pd) {
    *(int *)(p + pd->ib) = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pd->a, p);
}
