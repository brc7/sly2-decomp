typedef int qword __attribute__((mode(TI)));
#define F(T, o) (*(T *)((char *)p + (o)))
#define A(o) ((char *)p + (o))

extern void PostLoadStepplayer(void *p);
extern void BindLoOidTable(void *p, int n, void *pv);
extern int PxfFindSo(void *p, int a, int b);
extern void func_0020B868(void *p);
extern void func_001A0300(void *p, void *a, void *b);
extern void PostJtLoadSwing(void *p, void *a, void *b);
extern int func_0012DBB0(void *p, int a, int b);
extern int func_0012DC18(void *p, int a, int b, int c, int d, qword q);
extern void func_00230100(void *p, int a, int b);
extern void func_001426E8(void *p, int a);
extern void func_0018B348(void *p);
extern void func_00137A30(void *p, int a, int b, void *c, int d, int e, int f, void *g);
extern void InitJtHands__FP7JTHANDSP2SOPP5ASEGA(void *a, void *p, void *b);
extern void SetJtJts__FP2JT3JTS4JTBS(void *p, int a, int b);
extern void SetLoBusyk(void *p, int a);
extern int func_001F0380(int a, int b, int c, int d);
extern char D_002BF1B0[];
extern qword D_002A2DD0;

void PostJtLoad(char *p)
{
    int n;
    PostLoadStepplayer(p);
    BindLoOidTable(p, 0x12, D_002BF1B0);
    F(int, 0x10B8) = PxfFindSo(p, 0x138, 1);
    F(int, 0x19E0) = PxfFindSo(p, 0x18, 6);
    F(int, 0x19DC) = PxfFindSo(p, 0x19, 6);
    F(int, 0x19D8) = PxfFindSo(p, 0x13, 1);
    F(int, 0x19D4) = PxfFindSo(p, 0x12, 1);
    F(int, 0x19D0) = PxfFindSo(p, 0x1C, 6);
    F(int, 0x19CC) = PxfFindSo(p, 0x1D, 6);
    F(int, 0x19C8) = PxfFindSo(p, 0xD, 1);
    F(int, 0x19C4) = PxfFindSo(p, 0xC, 1);
    F(int, 0x19C0) = PxfFindSo(p, 0xC, 6);
    func_0020B868(A(0x1320));
    func_0020B868(A(0x1C30));
    func_001A0300(p, A(0x1AC0), A(0x1BC0));
    PostJtLoadSwing(p, A(0x1970), A(0x1994));
    func_001A0300(p, A(0x1E10), A(0x1F10));
    F(int, 0x19E4) = func_0012DBB0(p, F(int, 0x10B8), 5);
    F(int, 0x19E8) = func_0012DBB0(p, F(int, 0x19C8), 5);
    F(int, 0x19EC) = func_0012DBB0(p, F(int, 0x19C4), 5);
    F(int, 0x19F0) = func_0012DBB0(p, F(int, 0x19D8), 7);
    F(int, 0x19F4) = func_0012DBB0(p, F(int, 0x19D4), 7);
    F(int, 0x19FC) = func_0012DBB0(F(char *, 0x19A0), *(int *)(F(char *, 0x19A0) + 0x58), 5);
    F(int, 0x19F8) = func_0012DBB0(p, F(int, 0x19C0), 5);
    n = func_0012DC18(p, F(int, 0x19C0), 5, 5, 2, D_002A2DD0);
    F(int, 0x1A00) = n;
    func_00230100(p, F(int, 0x19C0), n);
    func_001426E8(p, 0x17);
    func_0018B348(p);
    func_00137A30(p, 0xC, 2, A(0x1A04), 0, 0, 0, A(0x1A1C));
    func_00137A30(p, 0xC, 2, A(0x1A20), 0, 0, 0, A(0x1A38));
    func_00137A30(p, 0xC, 2, A(0x1A3C), 0, 0, 0, A(0x1A54));
    func_00137A30(p, 0xC, 2, A(0x1A58), 0, 0, 0, A(0x1A70));
    func_001A0300(p, A(0x13D0), A(0x14D0));
    func_00137A30(p, 0xC, 4, A(0x1F60), 0, 0, 0, A(0x1F90));
    func_00137A30(p, 0xC, 3, A(0x2014), 0, 0, 0, A(0x2038));
    F(int, 0x1804) = F(int, 0x19A4);
    InitJtHands__FP7JTHANDSP2SOPP5ASEGA(A(0x17D0), p, A(0x18E0));
    SetJtJts__FP2JT3JTS4JTBS(p, 0, -1);
    SetLoBusyk(p, 1);
    if (F(int, 0x1D24))
        F(int, 0x1D28) = func_001F0380(F(int, 0x1D24), F(int, 0x1D20), -1, 1);
}
