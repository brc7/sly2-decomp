typedef struct { short d; short pad; void *pfn; } VTE;
struct T { int a; int b; };
extern struct T D_002C3D98[2];
extern char D_002DEB30[];
struct S { char pad[0xC]; VTE *vt; char pad2[0x2C0 - 0x10]; long fl; };
struct L { int *pa; int *pb; };
long GrfFromSgf(int n);
void SetStepguardGrfsgf(struct S *p, long fl);
int FGrfHasSgf(long m, int a);
void BroadcastGameEvent(void *p, int b, struct S *ps);
void AddStepguardSgf(struct S *p, int n)
{
    long m = GrfFromSgf(n);
    struct L l;
    int b;
    unsigned int i;
    SetStepguardGrfsgf(p, p->fl | m);
    b = -1;
    l.pa = &n;
    l.pb = &b;
    ((void (*)(void *, int, int, int, struct L *, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 7, 0, 2, &l, 0);
    for (i = 0; i < 2; i++) {
        if (FGrfHasSgf(m, D_002C3D98[i].a))
            BroadcastGameEvent(D_002DEB30, D_002C3D98[i].b, p);
    }
}
