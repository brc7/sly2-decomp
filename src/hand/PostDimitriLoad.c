struct E { int oidA; void *pA; int oidB; void *pB; int oidC; void *pC; int oidD; void *pD; int n; };
struct S { char pad[0x1010]; int c; struct E *ae; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *g, int k, int oid, struct S *p);
void PostStepguardLoad(struct S *p);
void PostDimitriLoad(struct S *p)
{
    int i;
    PostStepguardLoad(p);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->pA = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidA, p);
        pe->pB = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidB, p);
        pe->pC = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidC, p);
        pe->pD = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pe->oidD, p);
    }
}
