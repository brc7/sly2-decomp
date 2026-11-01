struct O { char pad[0x98]; short h98; };
struct S { char p0[0x58]; struct O *po; char p1[0x150-0x5C]; int n150; };
void UpdateSo(struct S *p);
void func_001D4FA0(struct S *p, int n);
void func_001D4F50(struct S *p) {
    UpdateSo(p);
    if (p->n150 == 0 && p->po->h98 == 3) func_001D4FA0(p, 1);
}
