typedef struct { short d; short pad; void *pfn; } VTE;
struct Q { char pad[0xC]; VTE *vt; char pad2[0xEC - 0x10]; void *pEC; };
struct T { char pad[0xEC]; void *pEC; };
struct O { char pad[0x50]; int k; char pad2[8]; struct T *pt; };
struct S { char pad[0x26C]; char x26C[4]; };
int FIgnoreSoIntersection(struct S *p, struct Q *q);
int FStepguardHasSgf(struct S *p, int n);
struct O *PbhvFindBhvstk(void *p, int n);
int func_00134310(int a, int b);
int FIgnoreStepguardIntersection(struct S *p, struct Q *q)
{
    int r = FIgnoreSoIntersection(p, q);
    if (FStepguardHasSgf(p, 7)) {
        if (!((int (*)(void *))q->vt[62].pfn)((char *)q + q->vt[62].d))
            r = func_00134310(r, 1);
    } else if (FStepguardHasSgf(p, 0x17)) {
        struct O *o = PbhvFindBhvstk(p->x26C, 0xB1);
        if (q->pEC == o->pt->pEC) {
            switch (o->k) {
            case 0:
            case 4:
            case 5:
                r = func_00134310(r, 1);
                break;
            }
        }
    }
    return r;
}
