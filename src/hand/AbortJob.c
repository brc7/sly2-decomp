struct S { char pad[0x64]; void *f64; };
struct T { char pad[0x7C]; struct S *ps; };
extern struct T *D_003E0FA0;
extern char D_002DEB30[];
void ResetJobTasks(struct S *p);
void SetTaskStateCascade(void *p, int n);
void DeactivateTask(struct T *p);
void BroadcastGameEvent(void *p, int n, struct S *ps);
void EndCurrentJob(struct S *p);
void AbortJob(struct S *p)
{
    ResetJobTasks(p);
    SetTaskStateCascade(p->f64, 1);
    if (D_003E0FA0 && D_003E0FA0->ps == p)
        DeactivateTask(D_003E0FA0);
    BroadcastGameEvent(D_002DEB30, 0x1F, p);
    EndCurrentJob(p);
}
