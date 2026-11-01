struct MRSG { float g0; float g4; };
struct X { char pad[0x74]; float g74; };
struct E { float w; struct X *px; };
struct P { char pad[0xC0]; int cb; int c; char *ab; };
float GModPositive__Fff(float, float);
void CalculateBlendAmrsg__FiiPviiP4MRSGPf(struct P *p, int ib, int c, struct MRSG *amrsg, float *pf)
{
    float gSum = 0.0f;
    int i;
    if (c == 0) {
        if (pf) {
            for (i = 0; i < p->c; i++) {
                struct E *pe = (struct E *)(p->ab + i * p->cb);
                if (pe->w != 0.0f)
                    gSum += pe->px->g74 * pe->w;
            }
            *pf = gSum;
        }
        return;
    }
    for (i = 0; i < c; i++) {
        int j;
        amrsg[i].g4 = 0.0f;
        for (j = 0; j < p->c; j++) {
            struct E *pe = (struct E *)(p->ab + j * p->cb);
            if (pe->w != 0.0f)
                amrsg[i].g4 += ((struct MRSG *)((char *)pe + ib))[i].g4 * pe->w;
        }
        gSum += amrsg[i].g4;
    }
    {
        int fFirst = 1;
        float gAvg, gFirst;
        for (i = 0; i < p->c; i++) {
            struct E *pe = (struct E *)(p->ab + i * p->cb);
            if (pe->w != 0.0f) {
                if (fFirst) {
                    fFirst = 0;
                    gAvg = *(float *)((char *)pe + ib) / pe->px->g74;
                    gFirst = gAvg;
                } else {
                    float d = *(float *)((char *)pe + ib) / pe->px->g74 - gFirst;
                    if (d <= -0.5f)
                        d += 1.0f;
                    else if (0.5f < d)
                        d -= 1.0f;
                    gAvg += d * pe->w;
                }
            }
        }
        amrsg[0].g0 = GModPositive__Fff(gAvg, 1.0f) * gSum;
    }
    for (i = 1; i < c; i++) {
        float g = amrsg[i - 1].g0 + amrsg[i - 1].g4;
        amrsg[i].g0 = g;
        if (gSum <= g)
            amrsg[i].g0 = g - gSum;
    }
    if (pf)
        *pf = gSum;
}
