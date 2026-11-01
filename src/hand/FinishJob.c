struct S { char pad[0x68]; void *f68; char pad2[0x78 - 0x6C]; int f78; };
extern char D_002DEB30[];
extern char D_002E65E0[];
void SetTaskStateCascade(void *p, int n);
void BroadcastGameEvent(void *p, int n, struct S *ps);
void EndCurrentJob(struct S *p);
void func_001ABD98(void *p, int n);
void FinishJob(struct S *p)
{
    if (!p->f78)
        SetTaskStateCascade(p->f68, 3);
    BroadcastGameEvent(D_002DEB30, 0x20, p);
    EndCurrentJob(p);
    if (!p->f78)
        func_001ABD98(D_002E65E0, 0);
}
