typedef struct { short d; short pad; void *pfn; } VTE;
typedef struct SUB {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x58 - 0x10];
    int x58;
    char pad5c[0x70 - 0x5C];
    int x70;
    int x74;
    int x78;
} SUB;
typedef struct OBJ {
    char pad0[0xC];
    VTE *vt;
    char pad10[0x24 - 0x10];
    SUB *x24;
    char pad28[0x40 - 0x28];
    int x40;
    float x44;
    char pad48[0x25C - 0x48];
    int x25C;
    int x260;
    int x264;
} OBJ;
extern struct CLOCK { float a[16]; } D_002DDED0;
extern char D_003DD490[];
void func_0014D1D0(struct CLOCK *, float, int);
void IncrementSwHandsOff(void *, int);
void DecrementSwHandsOff(void *, int);
void func_00212A70(SUB *, int);
void func_00212A50(SUB *, int);
void *func_001FBB90(int);
void func_001FB2F8(void *, int);
void func_001DBD10(OBJ *);
void func_001DBBE0(OBJ *, int);
#define VCALL(o, n) ((void (*)(void *))(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
void func_001DC260(OBJ *p, int s)
{
    int old;
    if (!p->x24)
        s = -1;
    if (s == p->x40)
        return;
    if (p->x40 == -1) {
        func_0014D1D0(&D_002DDED0, 0.0f, 0);
        IncrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 10);
        func_00212A70(p->x24, 1);
    }
    old = p->x40;
    p->x40 = s;
    p->x44 = D_002DDED0.a[15];
    switch (s) {
    case -1:
        VCALL(p->x24, 50);
        func_0014D1D0(&D_002DDED0, 1.0f, 0);
        DecrementSwHandsOff(D_003DD490, 0);
        VCALL(p, 11);
        if (p->x24->x58 > 0)
            func_00212A50(p->x24, 1);
        p->x25C = s;
        p->x260 = s;
        p->x264 = s;
        break;
    case 0:
        p->x24->x74 = 0x4D4;
        p->x24->x70 = 0x4D5;
        p->x24->x78 = 0x4D6;
        if (old == 1) {
            func_00212A70(p->x24, 1);
            func_00212A70(p->x24, 2);
        } else {
            func_00212A50(p->x24, 2);
        }
        func_001FB2F8(func_001FBB90(0x35), 0);
        break;
    case 1:
        p->x24->x74 = 0x49B;
        p->x24->x70 = 0x49C;
        p->x24->x78 = 0x49D;
        if (old == 0) {
            func_00212A70(p->x24, 1);
            func_00212A70(p->x24, 2);
        } else {
            func_00212A50(p->x24, 2);
        }
        func_001FB2F8(func_001FBB90(0x35), 0);
        break;
    }
    func_001DBD10(p);
    func_001DBBE0(p, 0);
    ((void (*)(void *, int))p->vt[14].pfn)((char *)p + p->vt[14].d, -1);
}
