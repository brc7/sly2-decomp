typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct OBJ OBJ;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int, int, int, int, int); } a[8]; } VT;
struct OBJ {
    char pad[0xC];
    VT *vt;
    char pad10[0x40 - 0x10];
    int x40;
    char pad44[0x54 - 0x44];
    void *x54;
    int pad58;
    DL dl;
    char pad68[0x6C - 0x68];
    int x6C;
    int x70;
    int x74;
    int x78;
    int x7C;
};
void func_001E02C0(OBJ *);
void *func_001E5AE0(OBJ *, void *, int);
void func_001E1C40(void *);
int FIsDlEmpty__FP2DL(DL *);
void func_001DFEB8(OBJ *);
void func_001A6CA8(OBJ *, int);
void func_001DFD58(OBJ *p, int n)
{
    int fl;
    if (p->x74)
        return;
    p->x6C = n;
    p->x74 = 1;
    p->x78 = 0;
    p->vt->a[3].pfn((char *)p + p->vt->a[3].d, 5, 0, 0, 0, 0);
    if (!p->x74)
        return;
    if (p->x7C) {
        func_001E02C0(p);
        p->x7C = 0;
    }
    func_001E1C40(func_001E5AE0(p, p->x54, 0));
    if (!p->x74)
        return;
    if (FIsDlEmpty__FP2DL(&p->dl)) {
        func_001DFEB8(p);
        return;
    }
    fl = p->x40;
    if ((fl ^ 1) & 1)
        fl = *(int *)fl;
    if (fl & 2)
        return;
    func_001A6CA8(p, 1);
    if ((p->x40 ^ 1) & 1)
        *(int *)p->x40 |= 2;
    else
        p->x40 |= 2;
    p->x78 = 1;
}
