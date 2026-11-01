struct S { char pad[0xEB8]; int aEB8[8]; int cED8; int aEDC[8]; int cEFC; };
int PloNew__F3CIDP2SO3OIDi(int a, struct S *p, int b, int c);
static __inline__ void Push(struct S *p, int id, int v)
{
    p->aEDC[p->cEFC++] = v;
    p->aEB8[p->cED8++] = id;
}
int func_0020A9C0(struct S *p)
{
    int v;
    if (p->cED8 == 0) {
        v = PloNew__F3CIDP2SO3OIDi(0x78, p, 0x2CC, -1);
        Push(p, 0x2CC, v);
    } else {
        v = p->aEDC[0];
    }
    return v;
}
