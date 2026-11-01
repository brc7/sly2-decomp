struct T { char pad[0x180]; int n; };
struct S { char pad[0x13EC]; int f; struct T *pt; char pad2[0x144C - 0x13F4]; int g; };
void SetMurrayState(struct S *p, int n);
void FaceStepToward(struct S *p, int n);
void StartMurrayLiftGrabspot(struct S *p, struct T *pt)
{
    if (p->f) {
        p->pt = pt;
        p->g = 0;
        SetMurrayState(p, 4);
        FaceStepToward(p, p->pt->n);
    }
}
