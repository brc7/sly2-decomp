struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002A70E0;
struct S { char pad[0x168]; float f168; int x16C; char pad1[0x1C4 - 0x170]; int x1C4; char pad2[0x220 - 0x1C8]; int x220; int x224; float f228; float f22C; };
void UpdateSo(struct S *p);
int func_001B1900(struct S *p);
int func_001B1868(struct S *p);
void FadeSoOut(struct S *p, float f);
void func_001B1328(struct S *p, int st);
void func_001B1160(struct S *p);
void func_001B0E28(struct S *p)
{
    int st;
    UpdateSo(p);
    st = p->x220;
    switch (st) {
    case 0:
        st = func_001B1900(p);
        if (st != 3 && p->f228 <= D_002DDED0.t) {
            if (p->x16C) st = 1;
        }
        break;
    case 1:
        st = func_001B1900(p);
        if (st != 3 && p->f228 <= D_002DDED0.t) st = 0;
        break;
    case 2:
        st = func_001B1900(p);
        if (st != 3 && func_001B1868(p)) st = 1;
        break;
    case 3:
        if (!p->x1C4 && func_001B1868(p)) st = 4;
        break;
    case 4:
        if (p->f22C <= D_002DDED0.t) {
            FadeSoOut(p, p->f168);
            p->f22C = D_002A70E0;
        }
        break;
    }
    func_001B1328(p, st);
    func_001B1160(p);
}
