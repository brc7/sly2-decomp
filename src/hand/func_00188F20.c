typedef int qword __attribute__((mode(TI)));
#define F(T, o) (*(T *)((char *)p + (o)))
#define A(o) ((char *)p + (o))

typedef struct MSG9 { void *x0; int x4; int x8; } MSG9;
typedef struct E { int x0; int x4; void *x8; } E;
typedef struct LOB { char pad[0xC]; int xC; char pad1[0x44 - 0x10]; int x44; } LOB;
typedef struct MSGB { int x0; void *x4; LOB *x8; } MSGB;
typedef struct CLR { int a, b, c; } CLR;
typedef struct O1034 { char pad[0x30]; void *x30; } O1034;
struct VTE { short d; short pad; void *pfn; };

extern char D_003DD490[];
extern char D_002BF2A0[];
extern char D_002BF2B8[];

extern void func_00140F90(void *p, int msg, void *pv);
extern void func_0018E540(void *p, int n);
extern void func_0018C250(void *p, int a, int b);
extern void func_00131B10(void *a, void *p, int c);
extern void func_00131B60(void *a, void *p, int c, void *d);
extern qword func_001A62E8(void *p);
extern void *func_001F1E20(void *a, qword q);
extern int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
extern int PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, void *c);
extern void func_001424E0(void *p, int oid, void *pv, int n);
extern void *func_00205458(void *p, int n);
extern void func_0017B1A8(void *p, int n);
extern void func_00156B98(void *p, int a, qword q, void *c);
extern void func_00195A00(void *p, int a, int b, int c);
extern void func_0018D698(void *p, E *pe, void *c);


static __inline__ void ClearX(CLR *px) { px->a = px->b = px->c = 0; }


void func_00188F20(char *p, int msg, void *pv)
{
    func_00140F90(p, msg, pv);
    switch (msg) {
    case 3:
        if (pv == F(void *, 0x18E0)) {
            if (F(int, 0x18D8) == 10)
                func_0018E540(p, 2);
            else
                func_0018E540(p, -1);
            break;
        }
        if (pv != F(void *, 0x1030))
            break;
        switch (F(int, 0x18C8)) {
        case 9:
        case 0xB:
            func_0018C250(p, 0, -1);
            break;
        case 7:
        case 0x15:
            F(int, 0x1038) = -1;
            break;
        case 5:

            if (F(int, 0x18D0) < 10) { if (F(int, 0x18D0) >= 8) func_0018C250(p, 5, 4); }

            break;
        }
        break;
    case 9: {
        MSG9 *pm = (MSG9 *)pv;
        void *x = pm->x0;
        if (x == F(void *, 0x1030) || x == F(void *, 0x18E0) ||
            (F(O1034 *, 0x1034) && x == F(O1034 *, 0x1034)->x30)) {
            switch (pm->x8) {
            case 0x16E:
                func_00131B10(F(void *, 0x18E0), p, F(int, 0x10B8));
                break;
            case 0x16F:
                func_00131B60(F(void *, 0x18E0), p, 3, D_002BF2A0);
                break;
            case 0x170:
                func_00131B60(F(void *, 0x18E0), p, 3, D_002BF2B8);
                break;
            case 0x164:
                F(int, 0x18E4) = 1;
                break;
            case 0x166:
                F(int, 0x18E8) = 1;
                break;
            case 0x165:
                if (F(int, 0x18EC)) {
                    switch (F(int, 0x18D8)) {
                    case 0xC: case 0xE: case 0x10: case 0x12: case 0x14:
                        func_0018C250(p, 5, 6);
                        break;
                    case 0xD: case 0xF: case 0x11: case 0x13: case 0x15:
                        func_0018C250(p, 5, 7);
                        break;
                    }
                }
                break;
            case 0x167:
                if (F(void *, 0x10DC) && F(int, 0x1BD4)) {
                    void *po = func_001F1E20(F(void *, 0x10DC), func_001A62E8(p));
                    if (po && !FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x5B) && !FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x5D)) {
                        int oid = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 1, 0x282, p);
                        F(void *, 0x1BCC) = po;
                        func_001424E0(p, oid, po, -1);
                    }
                }
                break;
            case 0x173: {
                void *px;

                F(int, 0x1D2C) = 1;
                ClearX((CLR *)A(0x190));

                { struct VTE *e = &F(struct VTE *, 0xC)[35]; ((void (*)(void *, qword))e->pfn)(p + e->d, F(qword, 0x190)); }
                px = func_00205458(F(void *, 0x10DC), 0xB1);
                if (px)
                    func_0017B1A8(px, 4);
                break;
            }
            case 0x174: {
                void *px = func_00205458(F(void *, 0x10DC), 0xB1);
                if (px)
                    func_0017B1A8(px, 6);
                break;
            }
            case 0x16D:
                func_00156B98(D_003DD490, 0, func_001A62E8(p), p);
                break;
            }
        }
        break;
    }
    case 1: {
        E *pe;
        int i;
        switch (F(int, 0x18C8)) {
        case 3:
        case 0x16:
            if (F(void *, 0x18C4) == pv)
                func_00195A00(A(0x2C0), 0, 0, 0);
            break;
        case 7:
            if (F(int, 0x4C8) < 5) { if (F(int, 0x4C8) >= 3 && F(void *, 0x4D4) == pv)
                func_00195A00(A(0x2C0), 0, 0, 0); }
            break;
        }
        pe = (E *)A(0x1FB0);
        for (i = 0; i < 4; i++, pe++) {
            if (pe->x8 == pv)
                func_0018D698(p, pe, 0);
        }
        break;
    }
    case 0xB: {
        MSGB *pm = (MSGB *)pv;
        int k;
        if (pm->x4 != p)
            break;
        if (!(pm->x8->x44 & 2))
            break;
        k = pm->x8->xC;
        if (k & 2) {
            switch (F(int, 0x18D8)) {
            case 4: case 6: case 0xC: case 0xE: case 0x10: case 0x12: case 0x14:
                func_0018C250(p, 0, -1);
                func_0018E540(p, 8);
                break;
            case 5: case 7: case 0xD: case 0xF: case 0x11: case 0x13: case 0x15:
                func_0018C250(p, 0, -1);
                func_0018E540(p, 9);
                break;
            }
        } else if (k & 4) {
            func_0018C250(p, 0, -1);
            func_0018E540(p, 8);
        }
        break;
    }
    }
}
