
static __inline__ float sqrtf(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }

typedef struct LM { float gMin, gMax; } LM;

typedef struct JOY {
    char pad0[0x1000];
    int nPort;       /* 0x1000 */
    int nSlot;       /* 0x1004 */
    int x1008;       /* 0x1008 */
    int x100c;
    int joyk;        /* 0x1010 */
    int x1014;
    float tRead;     /* 0x1018 */
    float tActive;   /* 0x101C */
    int x1020;       /* 0x1020 */
    int pad1024[6];
    float x;         /* 0x103C */
    float y;         /* 0x1040 */
    float uDeflect;  /* 0x1044 */
    unsigned char bX, bY; /* 0x1048 */
    short pad104a;
    int fStickMoved; /* 0x104C */
    LM alm[4];       /* 0x1050 */
    float x2;        /* 0x1070 */
    float y2;        /* 0x1074 */
    float uDeflect2; /* 0x1078 */
    unsigned char bX2, bY2; /* 0x107C */
    short pad107e;
    int fStickMoved2; /* 0x1080 */
    LM alm2[4];      /* 0x1084 */
    float dx;        /* 0x10A4 */
    float dy;        /* 0x10A8 */
    float dx2;       /* 0x10AC */
    float dy2;       /* 0x10B0 */
    unsigned int grfbtn;   /* 0x10B4 */
    unsigned int grfbtnPressed;  /* 0x10B8 */
    unsigned int grfbtnReleased; /* 0x10BC */
    unsigned char mpbtnpb[12];   /* 0x10C0 */
    int pad10cc[8];
    float gDpad;     /* 0x10EC */
} JOY;

typedef struct CLOCK {
    char pad[0x3C];
    float tReal;  /* 0x3C */
    float x40;    /* 0x40 */
} CLOCK;

typedef struct G { char pad[0x30C]; int x30c; int x310; void *x314; char pad318[0x3A0 - 0x318]; int x3a0; } G;
typedef struct SND { float t; } SND;

extern CLOCK D_002DDED0;
extern G D_002DDF40;
extern SND D_002DEB30;
extern char D_002DDEC0[];
extern char D_003DD490[];

extern void *func_002333B4(void *pv, int b, int cb);
extern int func_002432D0(int port, int slot, unsigned char *pb);
extern void func_001AE5A0(void *dst, void *src, int cb);
extern void func_00187768(JOY *p, unsigned char bX, unsigned char bY, float *px, float *py, float *pu,
                          unsigned char *pbX, unsigned char *pbY, int *pf, LM *alm);
extern void func_0014BF48(void *p, JOY *pjoy, int grfbtn);
extern int PpoHero(void *p);
extern void BroadcastGameEvent(SND *p, int n, int a);

int func_00187998(JOY *p)
{
    unsigned char ab[0x40];
    int fRet = 1;
    unsigned int old;
    unsigned int btn;
    float x, y, x2, y2;

    func_002333B4(ab, 0, 0x40);
    if (func_002432D0(p->nPort, p->nSlot, ab) > 0) {
        btn = ((ab[2] << 8) | ab[3]) ^ 0xFFFF;
        old = p->grfbtn;
        p->grfbtnPressed = btn & ~old & ~p->grfbtnReleased;
        p->grfbtnReleased = p->grfbtnReleased & btn;
        p->grfbtn = btn;
        if (p->x1008 != 0 && p->x1008 != ab[1])
            fRet = 0;
        p->x1008 = ab[1];
        if (p->joyk == 4)
            func_001AE5A0(p->mpbtnpb, &ab[8], 0xC);
        x = p->x;
        y = p->y;
        x2 = p->x2;
        y2 = p->y2;
        if (p->joyk >= 2) {
            func_00187768(p, ab[6], ab[7], &p->x, &p->y, &p->uDeflect, &p->bX, &p->bY, &p->fStickMoved, p->alm);
            func_00187768(p, ab[4], ab[5], &p->x2, &p->y2, &p->uDeflect2, &p->bX2, &p->bY2, &p->fStickMoved2, p->alm2);
        } else {


            p->x = p->y = p->uDeflect = 0.0f;
            p->y2 = p->uDeflect2 = 0.0f;
            p->x2 = 0.0f;


        }
        if (D_002DDF40.x30c == 0 || D_002DDF40.x314 != &D_002DDF40.x3a0) {
            if (p->grfbtn & 0xF000) {

                p->x = p->y = p->uDeflect = 0.0f;

                if (p->grfbtn & 0x8000)
                    p->x = -p->gDpad;
                else if (p->grfbtn & 0x2000)
                    p->x = p->gDpad;
                if (p->grfbtn & 0x1000)
                    p->y = p->gDpad;
                else if (p->grfbtn & 0x4000)
                    p->y = -p->gDpad;
                if (p->x * p->y != 0.0f) {

                    p->x = p->x * sqrtf(0.5f);
                    p->y = p->y * sqrtf(0.5f);

                }
                p->uDeflect = 1.0f;
            }
        }
        p->dx = (p->x - x) / D_002DDED0.x40;
        p->dy = (p->y - y) / D_002DDED0.x40;
        p->dx2 = (p->x2 - x2) / D_002DDED0.x40;
        p->dy2 = (p->y2 - y2) / D_002DDED0.x40;
        func_0014BF48(D_002DDEC0, p, old & 0xFFFF);
        p->tRead = D_002DDED0.tReal;
        if (p->grfbtn != 0 || p->uDeflect != 0.0f || p->uDeflect2 != 0.0f) {

            p->tActive = D_002DDED0.tReal;
            p->x1020 = 0;

            if (D_002DDED0.tReal - D_002DEB30.t > 15.0f) {
                BroadcastGameEvent(&D_002DEB30, 0xE, PpoHero(D_003DD490));
                D_002DEB30.t = D_002DDED0.tReal;
            }
        } else {
            p->x1020 = (int)(D_002DDED0.tReal - p->tActive);
        }
    } else {
        if (D_002DDED0.tReal - p->tRead >= 2.0f)
            fRet = 0;
    }
    return fRet;
}
