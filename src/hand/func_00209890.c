typedef struct { short d; short pad; void *pfn; } VTE;
struct T { int a; int b; };
extern struct T D_002C3D98[2];
extern char D_002DEB30[];
struct S { char pad[0xC]; VTE *vt; char pad2[0x2C0 - 0x10]; long fl; };
struct L { int *pa; int *pb; };
long func_00207A20(int n);
void func_00209870(struct S *p, long fl);
int func_002099F0(long m, int a);
void BroadcastGameEvent(void *p, int b, struct S *ps);
void func_00209890(struct S *p, int n)
{
    long m = func_00207A20(n);
    struct L l;
    int b;
    unsigned int i;
    func_00209870(p, p->fl | m);
    b = -1;
    l.pa = &n;
    l.pb = &b;
    ((void (*)(void *, int, int, int, struct L *, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 7, 0, 2, &l, 0);
    for (i = 0; i < 2; i++) {
        if (func_002099F0(m, D_002C3D98[i].a))
            BroadcastGameEvent(D_002DEB30, D_002C3D98[i].b, p);
    }
}
