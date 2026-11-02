typedef struct REC { int x0; int x4; int x8; } REC;
typedef struct OBJ { char pad[0x15C]; int x15C; char pad1[0x16C - 0x160]; void *x16C; } OBJ;
#define F(T, o) (*(T *)((char *)p + (o)))
#define A(o) ((char *)p + (o))

typedef struct SND { char pad[0xC]; int xC; } SND;
extern SND D_002DEB30;
extern char D_002DDED0[];
extern float D_002BF3EC;
extern float D_002BF3F0;
extern float D_003E1BEC;

extern void BroadcastGameEvent(void *p, int n, void *pv);
extern void ChargeGadgetMp(void *p, int n);
extern void func_0014D1D0(void *p, int n, float g);
extern void *PsndStartSound(int n);
extern void SetSndSourceLo(void *p, int n);
extern void func_00258040(void *p, void *pv);
extern void SetJtJts__FP2JT3JTS4JTBS(void *p, int a, int b);
extern void func_00195FB8(void *p, int a, int b);
extern REC *PgadrecFromJtGadget(void *p, int n);
extern void SetJtthrowState(void *p, int n);
extern void SetJtGrabState(void *p, int a, int b, int c);
extern OBJ *PloAcquireProxy(int oid, int n);
extern int func_00224610(OBJ *p);
extern void func_0018D698(void *p, REC *prec, OBJ *pobj);
extern void GrfbtnGadget(void *p, int n);
extern void func_0015C150(int x);
extern void func_00195A00(void *p, int a, int b, int c);
extern void StartStepplayerGadget(void *p, int n);

/** @sly2-annot addr=0x18DA48
 * Purpose: JT vt100 (gadget id): per-gadget action: slow-motion (clock rate via 0x14D1D0 + sound 0x67/0x66),
 *   states via SetJtJts (9,0x17,6,0x18/0xC,5/9), jump kinds 9/0xE, gadget records
 * Info: proposed name UseJtGadget (T1); class JT slot100; TU jt
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void UseJtGadget(char *p, int n)
{
    D_002DEB30.xC = n;
    BroadcastGameEvent(&D_002DEB30, 0x3C, p);
    switch (n) {
    case 0x1D: {
        void *pv;
        ChargeGadgetMp(p, n);
        func_0014D1D0(D_002DDED0, 1, D_002BF3EC);
        D_003E1BEC = -1.0f;
        pv = PsndStartSound(0x67);
        SetSndSourceLo(pv, 0);
        func_00258040(A(0x2044), pv);
        break;
    }
    case 0x2A: {
        void *pv;
        ChargeGadgetMp(p, n);
        func_0014D1D0(D_002DDED0, 1, D_002BF3F0);
        D_003E1BEC = 0.7f;
        pv = PsndStartSound(0x66);
        SetSndSourceLo(pv, 0);
        func_00258040(A(0x2044), pv);
        break;
    }
    case 0x21:
        ChargeGadgetMp(p, n);
        SetJtJts__FP2JT3JTS4JTBS(p, 9, -1);
        break;
    case 0x1B:
        ChargeGadgetMp(p, n);
        func_00195FB8(A(0x2C0), 0x13, 0);
        break;
    case 0x1A:
    case 0x25:
    case 0x26:
    case 0x29: {
        REC *prec = PgadrecFromJtGadget(p, n);
        OBJ *pobj;
        if ((unsigned int)(F(int, 0x11B4) - 4) < 3) {
            SetJtthrowState(A(0x11B0), 7);
            break;
        }
        if (prec->x8) {
            SetJtGrabState(A(0x4C0), 0xB, prec->x8, 0);
            break;
        }
        pobj = PloAcquireProxy(prec->x4, 0);
        if (!pobj)
            break;
        pobj->x15C = prec->x0;
        if (func_00224610(pobj)) {
            pobj->x16C = p;
            func_0018D698(p, prec, pobj);
        }
        F(OBJ *, 0x11C0) = pobj;
        GrfbtnGadget(p, n);
        SetJtJts__FP2JT3JTS4JTBS(p, 0x17, -1);
        SetJtthrowState(A(0x11B0), 4);
        break;
    }
    case 0x22:
    case 0x23:
        F(int, 0x9BC) = n;
        if (n == 0x23)
            F(int, 0x130C) = F(int, 0x1FE0);
        else
            F(int, 0x130C) = F(int, 0x1FE4);
        SetJtGrabState(A(0x4C0), 0xB, 0, 0);
        break;
    case 0x18:
        ChargeGadgetMp(p, n);
        SetJtJts__FP2JT3JTS4JTBS(p, 6, -1);
        break;
    case 0x19:
        ChargeGadgetMp(p, n);
        SetJtJts__FP2JT3JTS4JTBS(p, 0x18, 0xC);
        break;
    case 0x27:
        ChargeGadgetMp(p, n);
        SetJtJts__FP2JT3JTS4JTBS(p, 5, 9);
        break;
    case 0x28:
        ChargeGadgetMp(p, n);
        SetJtGrabState(A(0x4C0), 2, 0, 0);
        break;
    case 0x1F:
        if (F(int, 0x204C))
            func_0015C150(F(int, 0x204C));
        ChargeGadgetMp(p, n);
        func_00195A00(A(0x2C0), 9, 0, 0);
        break;
    case 0x1E:
        ChargeGadgetMp(p, n);
        func_00195A00(A(0x2C0), 0xE, 0, 0);
        break;
    default:
        StartStepplayerGadget(p, n);
        break;
    }
}
