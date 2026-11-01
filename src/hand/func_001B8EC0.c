typedef struct T { char pad[0x1C]; float x1C; char pad2[0x80 - 0x20]; int x80; } T;
typedef struct P {
    char pad0[0x150];
    T *x150;
    char pad154[0x1030 - 0x154];
    void *x1030;
    char pad1034[0x1840 - 0x1034];
    int x1840;
    float x1844;
    char pad1848[0x1900 - 0x1848];
    void *x1900;
    int pad1904;
    int x1908;
    char pad190C[0x1AC0 - 0x190C];
    int x1AC0;
} P;
extern float D_002DDED0;
extern float D_002C0A30;
extern float D_002C0A34;
int func_00131968(void *p);
void SetMurrayState(P *p, int s);
void func_001B8D30(P *p, int s);
int func_0020C0D8(void *p);
void func_0020C0E8(void *p);

void func_001B8EC0(P *p)
{
    int s;
    for (;;) {
        s = p->x1840;
        switch (s) {
        case 0:
            if (!p->x1030 || func_00131968(p->x1030)) func_001B8D30(p, 1);
            break;
        case 1:
            if (!p->x1030 || func_00131968(p->x1030)) {
                SetMurrayState(p, 0);
            } else if (p->x1908) {
                func_001B8D30(p, 2);
            } else if (!p->x1AC0 && D_002DDED0 - p->x1844 > D_002C0A30) {
                func_001B8D30(p, 3);
            }
            break;
        case 2:
            if (!p->x1900 || func_0020C0D8(p->x1900)) {
                if (p->x1AC0) func_001B8D30(p, 0);
                else SetMurrayState(p, 0);
            } else {
                func_0020C0E8(p->x1900);
            }
            break;
        case 3:
            if (!p->x1030 || func_00131968(p->x1030)) {
                SetMurrayState(p, 0);
            } else if (D_002DDED0 - p->x1844 > D_002C0A34 && p->x150->x80 == 1 && 0.0f < p->x150->x1C) {
                SetMurrayState(p, 0);
            }
            break;
        }
        if (p->x1840 == s || p->x1840 == -1) break;
    }
}
