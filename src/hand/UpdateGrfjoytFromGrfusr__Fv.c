extern int D_002DFBC0, D_002DFBC4, D_002E0CBC;
void UpdateGrfjoytFromGrfusr__Fv(void)
{
    int v = D_002DFBC4;
    if (v & 0x10) D_002DFBC0 = 5;
    else if (v & 0xC) D_002DFBC0 = 0;
    else if (v & 1) D_002DFBC0 = 5;
    else if (v & 2) D_002DFBC0 = 4;
    else D_002DFBC0 = 7;
    if (!(D_002DFBC0 & 2)) D_002E0CBC = 0xFFFF;
}
