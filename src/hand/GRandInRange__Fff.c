int NRand(void);
float GRandInRange__Fff(float gMin, float gMax)
{
    if (gMin == gMax)
        return gMin;
    return gMin + (gMax - gMin) * (float)(NRand() % 0x16A1B) * 1.078760305972537e-05f;
}
