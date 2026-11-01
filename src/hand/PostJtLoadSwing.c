int func_00137A30(void *p, int a, int b, void *q, int c, int d, int e, int *pn);
void PostJtLoadSwing(void *p, void *q, int *pn)
{
    int n;
    func_00137A30(p, 0xC, 3, q, 0, 0, 0, &n);
    if (pn) *pn = n;
}
