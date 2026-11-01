struct X { char pad[0x128]; int k; };
struct G { char pad[0x4774]; struct X *px; };
extern struct G D_003DD490;
void *PchzFindSwLangstr__FP2SWi(struct G *pg, int n);
void func_0021ED58(void *p, void *q);
void func_0015DA88(void *p, int f)
{
    struct X *px = D_003DD490.px;
    int k;
    if (!px)
        return;
    if (f)
        k = px->k - 1;
    else
        k = px->k;
    switch (k) {
    case 1: case 3: case 5: case 7: case 9: case 11: case 13: case 15:
        func_0021ED58(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x77));
        break;
    case 2: case 4: case 6: case 8: case 10: case 12: case 14: case 16:
        func_0021ED58(p, PchzFindSwLangstr__FP2SWi(&D_003DD490, 0x78));
        break;
    }
}
