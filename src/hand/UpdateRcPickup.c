struct O { char pad[0x98]; short h98; };
struct S { char p0[0x58]; struct O *po; char p1[0x150-0x5C]; int n150; };
void UpdateSo(struct S *p);
void SetRcPickupState(struct S *p, int n);
void UpdateRcPickup(struct S *p) {
    UpdateSo(p);
    if (p->n150 == 0 && p->po->h98 == 3) SetRcPickupState(p, 1);
}
