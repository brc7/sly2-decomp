struct S { char pad[0x148]; unsigned long fl; };
void InitSo(struct S *p);
void InitMs(struct S *p)
{
    InitSo(p);
    p->fl &= ~(1UL << 35);
}
