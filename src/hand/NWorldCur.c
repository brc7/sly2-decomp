extern int *D_003E1C44;
int IlevelCur__Fv(void);
int NWorldCur(void)
{
    int i = IlevelCur__Fv();
    if (i == -1)
        return -1;
    return D_003E1C44[i];
}
