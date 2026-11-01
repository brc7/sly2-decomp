struct S { char pad[0x88]; float a[32]; };
extern float D_002DDED0;
int GrfPressedWithin__5POCTLUif(struct S *p, unsigned int grf, float dt)
{
    int grfRet = 0;
    int i = 0;
    float *pg;
    float t = D_002DDED0;
    if (grf) {
        pg = p->a;
        do {
            if ((grf & 1) && t - *pg <= dt) grfRet |= 1 << i;
            grf >>= 1; pg++; i++;
        } while (grf);
    }
    return grfRet;
}
