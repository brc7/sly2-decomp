typedef struct DYNA { char *ab; int c; int cMax; int cb; } DYNA;

void func_00116D70(DYNA *pdyna, int c);
void *func_0023320C(void *pvDst, const void *pvSrc, unsigned int cb);

void CDynArray_Append(DYNA *pdyna, void *pv, int c)
{
    func_00116D70(pdyna, pdyna->c + c);
    func_0023320C(pdyna->ab + pdyna->cb * pdyna->c, pv, pdyna->cb * c);
    pdyna->c += c;
}
