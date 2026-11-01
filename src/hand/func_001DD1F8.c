typedef struct { char pad[0x58]; int x58; } W;
typedef struct { void *p; int a; int b; } ENT;
typedef struct {
    char pad[0x24];
    W *pw;
    char pad28[0x40 - 0x28];
    int kind;
    char pad44[4];
    int c;
    ENT ae[17];
    int sel;
} OBJ;
void func_00213DB0(W *, int, int *);
int FJobComplete(void *);
int FTaskComplete(void *);
void func_001DD1F8(OBJ *p)
{
    int i;
    if (p->pw->x58 != 2)
        return;
    for (i = 0; i < p->c; i++) {
        int f;
        ENT *pe = &p->ae[i];
        func_00213DB0(p->pw, i == p->sel ? i + 0x4FD : i + 0x4E9, &pe->a);
        f = 0;
        switch (p->kind) {
        case 0:
            f = FJobComplete(pe->p);
            break;
        case 1:
            f = FTaskComplete(pe->p);
            break;
        }
        func_00213DB0(p->pw, f ? i + 0x507 : -1, &pe->b);
    }
}
