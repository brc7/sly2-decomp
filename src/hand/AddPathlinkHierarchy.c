struct O { char pad[0x28]; int n; };
struct G { char pad[0x3D74]; int c; char pad2[0x854]; struct O *po; };
extern struct G D_003DD490;
void AddLoHierarchy(void *p);
void AddPathlinkHierarchy(void *p)
{
    struct G *pg;
    int c;
    AddLoHierarchy(p);
    pg = &D_003DD490;
    c = pg->c + 1;
    pg->c = c;
    pg->po->n = c;
}
