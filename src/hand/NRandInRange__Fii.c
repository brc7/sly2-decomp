int NRand(void);

int NRandInRange__Fii(int nLow, int nHi)
{
    int nRand = NRand();
    int nBase = nLow - 1;
    return nLow + nRand % (nHi - nBase);
}
