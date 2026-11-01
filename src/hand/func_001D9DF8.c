typedef struct { char pad0[0xC]; void *vt; } SUB;
typedef struct Q {
    char pad0[0xDFC];
    int xDFC;
    int *xE00;
    char pad[0x10C8 - 0xE04];
    int x10C8;
    char pad2[0x10EC - 0x10CC];
    int x10EC;
    char pad3[0x1300 - 0x10F0];
    int x1300;
} Q;
typedef struct OBJ {
    char pad0[0x20];
    float x20;
    float x24;
    float x28;
    float x2C;
    int x30;
    Q *x34;
    SUB *x38;
    int x3C;
    int x40;
    int x44;
    int x48;
    int x4C;
} OBJ;
extern struct G D_003DD490;
extern struct { float t; float dt; } D_002DDED0;
extern char D_002C3248[];
Q *func_001C4840(struct G *);
int func_00132B10(void *, int);
void func_001D9B60(OBJ *, int);
int func_001441A0(Q *);
int func_0011D4D0(float, float, float);
float GSmooth__FfffP3SMPPf(void *, int, float, float, float);
int func_002121E8(SUB *, int);
void func_00212E40(SUB *, int, float);
void func_001D9DF8(OBJ *p)
{
    Q *old;
    Q *q;
    if (!p->x38)
        return;
    old = p->x34;
    p->x34 = 0;
    q = func_001C4840(&D_003DD490);
    if (q && func_00132B10(q, 0x34))
        p->x34 = q;
    if (!p->x34) {
        p->x20 = p->x24 = 0;
        p->x28 = p->x2C = 0;
        if (old)
            func_001D9B60(p, 0);
        return;
    }
    if (old != p->x34) {
        if (!old)
            func_001D9B60(p, 3);
        else
            func_001D9B60(p, 5);
        if (func_00132B10(p->x34, 0x35)) {
            p->x3C = 0x33D; p->x40 = 0x33E; p->x44 = 0x33F; p->x48 = 0x340; p->x4C = 0x341;
        } else if (func_00132B10(p->x34, 0x37)) {
            p->x3C = 0x347; p->x40 = 0x348; p->x44 = 0x349; p->x48 = 0x34A; p->x4C = 0x34B;
        } else if (func_00132B10(p->x34, 0x36)) {
            p->x3C = 0x342; p->x40 = 0x343; p->x44 = 0x344; p->x48 = 0x345; p->x4C = 0x346;
        }
        p->x20 = p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x28 = p->x2C = (float)func_001441A0(p->x34) / (float)p->x34->x1300;
    }
    switch (p->x30) {
    case 1:
    case 6:
    case 7:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x2C = (float)func_001441A0(p->x34) / (float)p->x34->x1300;
        if (*p->x34->xE00 <= 0) {
            func_001D9B60(p, 0);
        } else if (p->x20 > p->x24) {
            func_001D9B60(p, 2);
        } else if (p->x20 < p->x24) {
            func_001D9B60(p, 3);
        } else if (!func_0011D4D0(p->x28, p->x2C, 0.0001f)) {
            func_001D9B60(p, 4);
        } else if (p->x34->x10EC == 4) {
            func_001D9B60(p, 7);
        } else if (p->x34->x10C8 != -1) {
            func_001D9B60(p, 6);
        } else {
            func_001D9B60(p, 1);
        }
        break;
    case 2:
        if (!func_002121E8(p->x38, p->x40))
            func_001D9B60(p, 3);
        break;
    case 5:
        if (!func_002121E8(p->x38, p->x44))
            func_001D9B60(p, 1);
        break;
    case 0:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        p->x2C = (float)func_001441A0(p->x34) / (float)p->x34->x1300;
        if (p->x20 < p->x24) {
            p->x20 = p->x24;
            func_00212E40(p->x38, 0x34F, 1.0f - p->x20);
            p->x28 = p->x2C;
            func_00212E40(p->x38, 0x350, 1.0f - p->x28);
            func_001D9B60(p, 1);
        }
        break;
    case 3:
        p->x24 = (float)*p->x34->xE00 / (float)p->x34->xDFC;
        if (p->x20 < p->x24)
            p->x20 = p->x24;
        else
            p->x20 = GSmooth__FfffP3SMPPf(D_002C3248, 0, p->x20, p->x24, D_002DDED0.dt);
        func_00212E40(p->x38, 0x34F, 1.0f - p->x20);
        if (func_0011D4D0(p->x20, p->x24, 0.0001f))
            func_001D9B60(p, 1);
        break;
    case 4:
        p->x2C = (float)func_001441A0(p->x34) / (float)p->x34->x1300;
        if (p->x28 < p->x2C)
            p->x28 = p->x2C;
        else
            p->x28 = GSmooth__FfffP3SMPPf(D_002C3248, 0, p->x28, p->x2C, D_002DDED0.dt);
        func_00212E40(p->x38, 0x350, 1.0f - p->x28);
        if (func_0011D4D0(p->x28, p->x2C, 0.0001f))
            func_001D9B60(p, 1);
        break;
    }
}
