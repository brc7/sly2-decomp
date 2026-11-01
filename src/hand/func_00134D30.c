struct S { char pad[0x150]; void *f150; char pad2[0xf00 - 0x154]; int ff00; int ff04; char pad3[0x18]; int ff20; };
int func_001443D8(struct S *);
int GrfConsumePressed__5POCTLUif(void *, int, float);
void func_0019CCE8(void *);
void func_0019D330(void *);
void func_00134D30(struct S *p) {
    if (func_001443D8(p)) {
        void *pv = &p->ff00;
        if (p->ff04 == 1 && p->ff20 == 0 && GrfConsumePressed__5POCTLUif(p->f150, 0x4f0, 0.25f)) func_0019CCE8(pv);
        func_0019D330(pv);
    }
}
