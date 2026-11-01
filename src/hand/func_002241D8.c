struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x140]; int k; };
struct Q { int a; struct O *o; };
void HandleBasicMessage(struct O *p, int k, struct Q *q);
void func_002241D8(struct O *p, int k, struct Q *q)
{
    HandleBasicMessage(p, k, q);
    if (k == 4) {
        struct O *o = q->o;
        if (o == p && (unsigned)(o->k - 1) < 2) o->vt[76].pf((char *)o + o->vt[76].d, 3);
    }
}
