struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { union { int oid; struct O *po; } u; int pad; };
struct L { int c; struct E ae[8]; };
extern char D_003DD490[];
struct O *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int k, int oid, void *q);
void func_00148668(struct L *l, void *q)
{
    int i;
    for (i = 0; i < l->c; i++) {
        struct O *o = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, l->ae[i].u.oid, q);
        if (o) {
            struct VTE *e;
            l->ae[i].u.po = o;
            e = &o->vt[9];
            ((void (*)(void *))e->pfn)((char *)o + e->d);
        } else {
            l->ae[i].u.po = 0;
        }
    }
}
