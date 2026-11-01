struct S { char pad[0x10]; int x10; int x14; char p2[0x24]; int x3c; char p3[0x10]; int x50; };
void func_00184C60(struct S *p);
void func_001849A0(struct S *p);
void CStreamBis_Refill(struct S *p)
{
    if (p->x3c >= 0) func_00184C60(p);
    else if (p->x50 >= 0) func_001849A0(p);
    else {
        p->x10 = 0;
        p->x14 = 0;
    }
}
