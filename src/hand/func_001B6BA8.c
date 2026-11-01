typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x18]; float f18; float f1C; };
struct P { char pad[0x150]; struct Q *x150; char pad0[0x190 - 0x154]; qword q190; char pad1[0x1030 - 0x1A0]; int x1030; char pad2[0x131C - 0x1034]; char x131C[0x13D8 - 0x131C]; int x13D8; float f13DC; int x13E0; int x13E4; };
extern float D_002C09A0, D_002BF730;
int func_00131968(int n);
void func_001B6078(struct P *p, int n);
void func_001B6DA8(struct P *p, int n);
int func_0019A098(struct P *p, void *q, float f);
void func_0019F590(struct P *p, int n);
qword SetVectorCylind(float a, float b, float c);
void func_001B6BA8(struct P *p)
{
    int s;
    for (;;) {
        s = p->x13D8;
        if (!p->x1030 || func_00131968(p->x1030)) func_001B6078(p, 0);
        else if (p->x13D8 == 1) {
            if (p->x13E4) func_001B6DA8(p, 2);
            else if (func_0019A098(p, p->x131C, p->f13DC)) func_0019F590(p, 0);
        }
        if (p->x13D8 == s || p->x13D8 == -1) break;
    }
    switch (p->x13D8) {
    case 0:
    case 1:
        p->q190 = SetVectorCylind(p->x150->f18 + 3.14159265f, D_002C09A0 * D_002BF730 * p->x150->f1C, 0.0f);
        break;
    }
}
