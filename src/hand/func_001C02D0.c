struct T { char pad[0xE8]; int ne8; char pad2[8]; int nf4; };
int func_001BFF28(void *p, int a, int b, int c, int d);
void MeasureShape(struct T *pt);
int func_001C02D0(char *p, int a, int b, struct T *pt)
{
    int n = func_001BFF28(p + 0xE0, a, b, 0x10, pt->nf4);
    if (n < 2) return 0;
    pt->ne8 = n;
    MeasureShape(pt);
    return 1;
}
