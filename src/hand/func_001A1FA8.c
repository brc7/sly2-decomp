struct B { char pad[0x148]; unsigned long x148; };
struct S { char *x0; char pad[0xC]; int *x10; char pad2[0x1C]; struct B *x30; };
void ReturnJtToSafePlaceCb(struct S *p);
void func_00195A00(void *p, int a, int b, int c);
void SetJtHurtState(struct S *p, int n);
void func_001A1FA8(struct S *p)
{
    if (*p->x10 > 0) {
        if (p->x30 && (p->x30->x148 & 0x200000000UL))
            ReturnJtToSafePlaceCb(p);
        else
            func_00195A00(p->x0 + 0x2C0, 11, 0, 0);
    } else {
        SetJtHurtState(p, 8);
    }
}
