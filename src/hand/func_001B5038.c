struct S { char pad[0x150]; void *p150; char pad2[0xF00 - 0x154]; int f00; int f04; char pad3[0xF20 - 0xF08]; int f20; };
int FStepplayerInJtmode(struct S *p);
int GrfConsumePressed__5POCTLUif(void *p, int n, float g);
void EndJtMode(void *p);
void UpdateJtMode(void *p);
void func_001B5038(struct S *p)
{
    if (FStepplayerInJtmode(p)) {
        if (p->f04 == 1 && !p->f20 && GrfConsumePressed__5POCTLUif(p->p150, 0x4F0, 0.25f)) EndJtMode(&p->f00);
        UpdateJtMode(&p->f00);
    }
}
