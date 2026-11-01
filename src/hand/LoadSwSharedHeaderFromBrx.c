extern int D_003E1C30;
extern int D_003E1C54;
extern int D_003E1C58;
int func_00117DB8(void *pbis);
void LoadSwSharedHeaderFromBrx(int a, void *pbis)
{
    int x = func_00117DB8(pbis);
    int y = func_00117DB8(pbis);
    D_003E1C30 = x;
    if (y != D_003E1C58) a = 1;
    D_003E1C54 = a;
    D_003E1C58 = y;
}
