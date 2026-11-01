extern float D_002C0C00;
int CbskFromG__Ff(float x)
{
    if (D_002C0C00 < x) return 0;
    if (x < -D_002C0C00) return 1;
    return 2;
}
