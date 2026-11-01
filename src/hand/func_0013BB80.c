struct S { char pad[0x154]; float f154; char pad2[0x174 - 0x158]; int f174; char pad3[0x1e8 - 0x178]; int f1e8; };
extern float D_002A4B20;
void func_001CB518(struct S *);
void func_0013BC60(struct S *, struct S *);
void SetSoCnstrForce(struct S *, int);
void SetSoCnstrTorque(struct S *, int);
void func_001F6860(struct S *, int);
void SetLoParent_1A58D0(struct S *, int, int);
void func_0013BB80(struct S *p) {
    func_001CB518(p);
    p->f174 = 0;
    p->f154 = D_002A4B20;
    func_0013BC60(p, p);
    SetSoCnstrForce(p, 0);
    SetSoCnstrTorque(p, 0);
    func_001F6860(p, 0);
    p->f1e8 = 0;
    SetLoParent_1A58D0(p, 0, 0xf);
}
