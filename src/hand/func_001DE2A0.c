struct S { char pad[0x148]; unsigned long fl; };
void InitSo(struct S *p);
void func_001DE2A0(struct S *p) {
    InitSo(p);
    p->fl |= 0x400000000UL;
}
