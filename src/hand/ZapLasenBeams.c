typedef int qword __attribute__((mode(TI)));
typedef struct LNK {
    void *x0;
    char pad4[0x10 - 4];
    qword x10;
    char pad20[0x4C - 0x20];
    void *x4C;
    char pad50[0xA0 - 0x50];
    float g;
    char padA4[0xAC - 0xA4];
    struct LNK *pNext;
} LNK;
typedef struct { char pad[0x140]; float x140; float x144; LNK *pHead; char pad14c[4]; } ENT;
typedef struct OBJ {
    char pad[0x198];
    int c;
    ENT *ae;
} OBJ;
typedef struct {
    OBJ *p;
    void *x4;
    void *x8;
    int xC;
    int x10;
    int pad14[3];
    qword x20;
    char pad30[0x30];
} MSG;
typedef struct { qword a[2]; } VPAIR;
void GetLasenBeamEnds(OBJ *, ENT *, VPAIR *, float *);
void *func_002333B4(void *, int, int);
void func_001F7598(MSG *);
void ZapLasenBeams(OBJ *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        VPAIR v;
        MSG msg;
        float g;
        ENT *e = &p->ae[i];
        GetLasenBeamEnds(p, e, &v, &g);
        if (e->pHead == 0) {
            e->x140 = e->x144 = g;
        } else {
            float gMin = g;
            LNK *pMin = 0;
            LNK *q;
            for (q = e->pHead; q; q = q->pNext) {
                if (q->g < gMin) {
                    gMin = q->g;
                    pMin = q;
                }
            }
            func_002333B4(&msg, 0, sizeof(msg));
            msg.p = p;
            msg.x4 = pMin->x0;
            msg.x8 = pMin->x4C;
            msg.xC = 0x1000;
            msg.x10 = -10;
            msg.x20 = pMin->x10;
            func_001F7598(&msg);
            e->x144 = gMin;
        }
    }
}
