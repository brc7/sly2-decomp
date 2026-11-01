struct S { char pad[0xB0]; int fb0; int fb4; void *fb8; void *fbc; void *fc0; int fc4; char pad2[0xCC - 0xC8]; void *fcc; };
void UpdateLo(struct S *p);
int func_002121E8(void *p, int n);
void FinishJob(struct S *p);
void func_00162FD0(struct S *p);
void SetJobCompletionPhase(struct S *p, int n);
void UpdateJob(struct S *p)
{
    UpdateLo(p);
    switch (p->fc4) {
    case 1:
        if (!func_002121E8(p->fb8, p->fb0)) {
            FinishJob(p);
            if (p->fc0 || p->fcc)
                SetJobCompletionPhase(p, 3);
            else
                SetJobCompletionPhase(p, 2);
        }
        break;
    case 2:
        if (!func_002121E8(p->fb8, p->fb4)) {
            func_00162FD0(p);
            SetJobCompletionPhase(p, 0);
        }
        break;
    case 3:
        if (!func_002121E8(p->fbc, 0x30F))
            func_00162FD0(p);
        break;
    }
}
