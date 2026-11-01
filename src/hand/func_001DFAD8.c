struct DL { void *a, *b; int ib; };
struct S { char pad[0x58]; int n58; struct DL dl; };
void InitLo(struct S *p);
void InitDl__FP2DLi(struct DL *pdl, int ib);
void func_001DFAD8(struct S *p) {
    InitLo(p);
    p->n58 = -1;
    InitDl__FP2DLi(&p->dl, 0xE4);
}
