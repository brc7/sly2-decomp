struct S { char pad[0xE8]; int f; };
void func_001CB518(struct S *p);
void func_001BAF18(struct S *p, int n);
void SetSoCnstrForce(struct S *p, int n);
void SetSoCnstrTorque(struct S *p, int n);
void func_001BBEA8(struct S *p)
{
    func_001CB518(p);
    func_001BAF18(p, 0x609);
    if (p->f) {
        SetSoCnstrForce(p, 1);
        SetSoCnstrTorque(p, 1);
    }
}
