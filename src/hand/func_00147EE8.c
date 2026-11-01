void RemoveDlEntry__FP2DLPv(void *p, int a);
void AppendDlEntry__FP2DLPv(void *p, int a);
struct X { char pad[0x20]; char sub[0x39A4 - 0x20]; int n; };
void func_00147EE8(struct X *p, int a)
{
    RemoveDlEntry__FP2DLPv(p->sub, a);
    if (p->n == 0) p->n = a;
    AppendDlEntry__FP2DLPv(p->sub, a);
}
