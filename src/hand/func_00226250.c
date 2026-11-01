struct VTE { short d; short pad; void (*pf)(); };
struct S { int a[3]; struct VTE *vt; char pad[0x140]; void *x; char pad2[0x6C]; void *y; void *z; void *w; };
int POCTL_FConsumePressed(void *x, int a);
void *PloAcquireProxy(void *y, int a);
void LaunchSoFromLauncher(void *z, void *v);
void func_00226250(struct S *p)
{
    if (p->x && POCTL_FConsumePressed(p->x, 0xFF)) {
        if (p->y) {
            void *v = PloAcquireProxy(p->y, 0);
            if (!v) return;
            p->w = v;
            if (p->z) LaunchSoFromLauncher(p->z, v);
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
        } else
            p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
}
