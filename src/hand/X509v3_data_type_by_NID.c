struct X { int a; int b; };
struct X *PdteFindDetectorTarget(int n);
int X509v3_data_type_by_NID(int n)
{
    struct X *p = PdteFindDetectorTarget(n);
    if (p)
        return p->b;
    return -1;
}
