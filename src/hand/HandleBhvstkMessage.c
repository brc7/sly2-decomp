struct VTE { short d; short pad; int (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct L { int x; int c; struct O *ap[1]; };
void HandleBhvstkMessage(struct L *pl, int a, int b)
{
    int i = pl->c;
    while (--i >= 0) {
        struct O *po = pl->ap[i];
        struct VTE *e = &po->vt[13];
        if (e->pfn((char *)po + e->d, a, b))
            break;
    }
}
