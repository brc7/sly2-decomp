struct S { char pad[0x150]; void *p150; char p1[0xDAC]; int xf00; int xf04; char p2[0x18]; int xf20; char p3[0xE1C]; int x1d40; };
int FStepplayerInJtmode(struct S *p);
int GrfConsumePressed__5POCTLUif(void *p, int n, float g);
void EndJtMode(void *p);
void UpdateJtMode(void *p);
void func_0018B2C0(struct S *p)
{
    if (FStepplayerInJtmode(p)) {
        if (p->xf04 == 1 && !p->xf20 &&
            GrfConsumePressed__5POCTLUif(p->p150, p->x1d40 == 7 ? 0x410 : 0x4F0, 0.25f))
            EndJtMode(&p->xf00);
        UpdateJtMode(&p->xf00);
    }
}
