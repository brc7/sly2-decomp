struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a[3]; struct VTE *vt; char pad[0xF0]; int n100; };
struct G { char pad[0x4760]; int n; };
extern struct G D_003DD490;
void PostLoLoad(struct O *p);
int func_001C7BB8(int n);
void func_001C8030(struct O *p)
{
    PostLoLoad(p);
    p->n100 = D_003DD490.n++;
    if (func_001C7BB8(p->n100)) { struct VTE *e = &p->vt[9]; e->pfn((char *)p + e->d); }
}
