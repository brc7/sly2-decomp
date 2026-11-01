struct S { char pad[0x148]; unsigned long fl; };
void InitSo(struct S *p);
void InitSensor(struct S *p) {
    InitSo(p);
    p->fl |= 0x400000000UL;
}
