struct DL { void *pvFirst; void *pvLast; int ibDle; };
extern "C" void func_00234700(int n, int (*pfn)(), int m);
extern "C" void func_00235178(int n);
extern "C" void InitDl__FP2DLi(DL *pdl, int ib);
extern "C" void func_002355E0(int n, int m);
extern "C" int func_00167F78();
int SemaCreate(int n, int m);
extern int D_002DEB18;
extern int D_002DEB1C;
extern DL D_002DEAF0;
extern DL D_002DEB00;
extern int D_004425D4;
extern "C" void func_001607A0()
{
    func_00234700(5, func_00167F78, 0);
    func_00235178(5);
    D_002DEB18 = SemaCreate(0, 2);
    D_002DEB1C = SemaCreate(2, 2);
    InitDl__FP2DLi(&D_002DEAF0, 0x454);
    InitDl__FP2DLi(&D_002DEB00, 0x1090);
    func_002355E0(D_004425D4, 0);
}
