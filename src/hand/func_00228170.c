struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; };
extern char D_003DD490[];
struct O *PpoHero(void *g);
void func_00228170(struct O *p)
{
    struct O *o = PpoHero(D_003DD490);
    if (o) {
        o->vt[38].pf((char *)o + o->vt[38].d, p);
        p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
}
