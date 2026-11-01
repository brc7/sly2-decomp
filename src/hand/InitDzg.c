typedef struct MSG { int cb; int nKind; } MSG;
typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DZG {
    int cpsoMax;
    int cpso;
    void *apso;
    DL dlA;
    DL dlB;
    DL dlC;
    DL dlD;
    int n3C;
    void *pv40;
    void *pv44;
    void *pv48;
    void *pv4C;
} DZG;

void *func_001AE378(MSG *pmsg);
void *func_001AE318(MSG *pmsg);
void *func_002333B4(void *pv, int b, unsigned int cb);
void InitDl__FP2DLi(DL *pdl, int ibDle);

DZG *InitDzg(DZG *pdzg, int cpsoMax)
{
    MSG msg;

    pdzg->cpsoMax = cpsoMax;
    pdzg->n3C = 0;
    pdzg->cpso = 0;
    msg.cb = cpsoMax * 0x28;
    msg.nKind = 0x10;
    pdzg->apso = func_001AE378(&msg);
    func_002333B4(&pdzg->dlA, 0, sizeof(DL));
    func_002333B4(&pdzg->dlB, 0, sizeof(DL));
    func_002333B4(&pdzg->dlC, 0, sizeof(DL));
    func_002333B4(&pdzg->dlD, 0, sizeof(DL));
    InitDl__FP2DLi(&pdzg->dlA, 0x20);
    InitDl__FP2DLi(&pdzg->dlB, 0x20);
    InitDl__FP2DLi(&pdzg->dlC, 0x20);
    InitDl__FP2DLi(&pdzg->dlD, 0x20);
    msg.cb = cpsoMax * cpsoMax * 4;
    msg.nKind = 0x10;
    pdzg->pv40 = func_001AE318(&msg);
    msg.cb = cpsoMax * cpsoMax * 4;
    msg.nKind = 0x10;
    pdzg->pv44 = func_001AE318(&msg);
    msg.cb = cpsoMax * 4;
    msg.nKind = 0x10;
    pdzg->pv48 = func_001AE318(&msg);
    msg.cb = cpsoMax * 4;
    msg.nKind = 0x10;
    pdzg->pv4C = func_001AE318(&msg);
    return pdzg;
}
