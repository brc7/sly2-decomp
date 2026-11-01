extern int *D_003E1C48;
int IlevelCur__Fv(void);
int NEpisodeCur(void)
{
    int i = IlevelCur__Fv();
    if (i == -1)
        return -1;
    return D_003E1C48[i];
}
