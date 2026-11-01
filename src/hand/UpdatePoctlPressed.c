struct S { char pad[0x1C]; float f1C; char p1[0x2C - 0x20]; float f2C; int grfCur; char p2[0x78 - 0x34]; int grfPrev; char p3[0x88 - 0x7C]; float a[16]; float aC8[2]; float aD0[2]; };
extern float D_002DDED0;
void UpdatePoctlPressed(struct S *p)
{
    float t = D_002DDED0;
    int grf = p->grfCur & ~p->grfPrev;
    float *pg;
    p->grfPrev = p->grfCur;
    if (grf) {
        pg = p->a;
        do { if (grf & 1) *pg = t; grf >>= 1; pg++; } while (grf);
    }
    p->aC8[p->f1C > 0.0f] = t;
    p->aD0[p->f2C > 0.0f] = t;
}
