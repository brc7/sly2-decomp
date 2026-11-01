typedef int ti128 __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(); };
struct Q { char pad[0x18]; int f18; };
struct S { int pad[3]; struct VTE *vt; char pad2[0x1e8 - 0x10]; struct Q *f1e8; };
extern ti128 D_002A2DD0;
void SetLoParent_1A58D0(struct S *, struct Q *, int);
void func_001F4898(struct S *, ti128);
void func_001F4930(struct S *, ti128);
void SetSoCnstrForce(struct S *, int);
void SetSoCnstrTorque(struct S *, int);
void ApplyBombThrow(struct S *p, struct Q *q) {
    p->f1e8 = q;
    SetLoParent_1A58D0(p, q->f18 != 1 ? q : 0, 3);
    func_001F4898(p, D_002A2DD0);
    func_001F4930(p, D_002A2DD0);
    SetSoCnstrForce(p, 1);
    SetSoCnstrTorque(p, 1);
    ((void (*)(void *, int, int, int, int, int))p->vt[3].pfn)((char *)p + p->vt[3].d, 0x1b, 0, 0, 0, 0);
}
