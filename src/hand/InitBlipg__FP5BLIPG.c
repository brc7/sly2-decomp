struct S { char pad[0x18]; int f18; char pad2[0x38 - 0x1c]; float f38; int pad3; int f40; char pad4[0x448 - 0x44]; int f448; };
void InitXfm(struct S *);
void InitDl__FP2DLi(void *, int);
void SetLoBusyk(struct S *, int);
void InitBlipg__FP5BLIPG(struct S *p) {
    int x;
    InitXfm(p);
    InitDl__FP2DLi(&p->f448, 0x1090);
    p->f18 = -1;
    p->f38 = 1e10f;
    SetLoBusyk(p, 1);
    x = p->f40;
    if ((x ^ 1) & 1) *(int *)x = (*(int *)x & 1) | 0xe;
    else p->f40 = (x & 1) | 0xe;
}
