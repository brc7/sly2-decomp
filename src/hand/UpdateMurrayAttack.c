typedef struct T { char pad[0x1C]; float x1C; } T;
typedef struct P {
    char pad0[0x150];
    T *x150;
    char pad154[0x1030 - 0x154];
    void *x1030;
    char pad1034[0x1810 - 0x1034];
    int x1810;
    char pad1814[0x181C - 0x1814];
    int x181C;
} P;
int func_00131968(void *p);
void SetMurrayState(P *p, int s);
int FMurrayCanAttack(P *p);
int GrfConsumePressed__5POCTLUif(T *t, int k, float f);
void SetMurrayAttack(P *p, int k);
int FTryMurrayTakedown(P *p);
void func_001B8938(P *p);

void UpdateMurrayAttack(P *p)
{
    int s;
    for (;;) {
        s = p->x1810;
        if (!p->x1030 || func_00131968(p->x1030)) {
            SetMurrayState(p, 0);
        } else {
            switch (p->x1810) {
            case 0:
            case 1:
                if (FMurrayCanAttack(p) && GrfConsumePressed__5POCTLUif(p->x150, 0x80, 0.25f)) {
                    SetMurrayAttack(p, p->x1810 == 0);
                } else if (!FMurrayCanAttack(p) || !FTryMurrayTakedown(p)) {
                    if (p->x181C && 0.0f < p->x150->x1C) func_001B8938(p);
                }
                break;
            case 2:
            case 3:
                if (FMurrayCanAttack(p) && GrfConsumePressed__5POCTLUif(p->x150, 0x10, 0.25f)) {
                    SetMurrayAttack(p, (p->x1810 == 2) ? 3 : 2);
                } else if (!FMurrayCanAttack(p) || !FTryMurrayTakedown(p)) {
                    if (p->x181C && 0.0f < p->x150->x1C) func_001B8938(p);
                }
                break;
            }
        }
        if (p->x1810 == s || p->x1810 == -1) break;
    }
}
