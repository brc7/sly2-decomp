struct E { int oid; int n; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int kind, int oid, void *owner);
void RequestTaskState(void *p, int n, void *owner);
void ApplyTaskStates(void *pv, int c, struct E *ae, void *owner)
{
    int i;
    for (i = 0; i < c; i++) {
        void *p = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 0x105, ae[i].oid, owner);
        if (p)
            RequestTaskState(p, ae[i].n, owner);
    }
}
