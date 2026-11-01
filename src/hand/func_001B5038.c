struct S { char pad[0x150]; void *p150; char pad2[0xF00 - 0x154]; int f00; int f04; char pad3[0xF20 - 0xF08]; int f20; };
int func_001443D8(struct S *p);
int GrfConsumePressed__5POCTLUif(void *p, int n, float g);
void func_0019CCE8(void *p);
void func_0019D330(void *p);
void func_001B5038(struct S *p)
{
    if (func_001443D8(p)) {
        if (p->f04 == 1 && !p->f20 && GrfConsumePressed__5POCTLUif(p->p150, 0x4F0, 0.25f)) func_0019CCE8(&p->f00);
        func_0019D330(&p->f00);
    }
}
