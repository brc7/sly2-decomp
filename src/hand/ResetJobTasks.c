struct X { char pad[0x64]; int f64; char pad2[0x84 - 0x68]; struct X *pNext; };
struct S { char pad[0x7C]; int f7c; struct X *pFirst; };
void UpdateTaskSaveVars(struct X *p);
void UpdateJobSaveVars(struct S *p);
void ResetJobTasks(struct S *p)
{
    struct X *px;
    for (px = p->pFirst; px; px = px->pNext) {
        px->f64 = 0;
        UpdateTaskSaveVars(px);
    }
    p->f7c = 0;
    UpdateJobSaveVars(p);
}
