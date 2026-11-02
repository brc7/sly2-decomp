struct JOY {
    char pad0[0x1000];
    int nPort;      /* 0x1000 */
    int nSlot;      /* 0x1004 */
    int x1008;
    int joys;       /* 0x100C */
    int joyk;       /* 0x1010 */
    float tJoys;    /* 0x1014 */
    char pad1018[0x10CC - 0x1018];
    char rumble[0x10E8 - 0x10CC]; /* 0x10CC */
    int fRumbleEnabled; /* 0x10E8 */
};

struct CLOCK { char pad[0x3C]; float tReal; };
extern "C" CLOCK D_002DDED0;

extern "C" {
int scePadGetState_243388(int port, int slot);
int scePadInfoMode(int port, int slot, int term, int offs);
int scePadSetMainMode(int port, int slot, int offs, int lock);
int scePadInfoAct(int port, int slot, int term, int offs);
void *func_002333B4(void *pv, int b, int cb);
int scePadSetActAlign(int port, int slot, unsigned char *pb);
int scePadInfoPressMode(int port, int slot);
int scePadEnterPressMode(int port, int slot);
int scePadGetState(int port, int slot);
int func_00187998(JOY *pjoy);
void UpdateRumble__FP6RUMBLE(void *prumble);
void SetJoyJoys__FP3JOY4JOYS4JOYK(JOY *pjoy, int joys, int joyk);
}

/** @sly2-annot addr=0x1871C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/main-loop-startup.md#3-pad-input-joycpp-0x187030-0x188488,
 *   docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/guides/port-plan.md#11-known-facts-that-drive-the-design
 * @end-sly2-annot */
void UpdateJoy(JOY *pjoy)
{

    int fReady;
    int x;
    int joys, joyk;
    int state;



    fReady = 0;

    if (pjoy->joys == 0)
        return;

    state = scePadGetState_243388(pjoy->nPort, pjoy->nSlot);
    if (state == 6 || state == 2)

        fReady = 1;


    if (pjoy->joys != 3 && pjoy->joyk != 0) {
        if (D_002DDED0.tReal - pjoy->tJoys >= 2.0f)
            state = 0;
    }

    if (pjoy->joyk != 0 && state == 0)
        SetJoyJoys__FP3JOY4JOYS4JOYK(pjoy, 1, 0);


    if (!fReady)
        return;

    x = 2;

    joys = pjoy->joys;
    joyk = pjoy->joyk;

    switch (joys) {
    case 1:
        switch (joyk) {
        case 0: {

            int m;
            int n = scePadInfoMode(pjoy->nPort, pjoy->nSlot, 1, 0);
            if (n == 0)
                break;
            m = scePadInfoMode(pjoy->nPort, pjoy->nSlot, 2, 0);
            if (m > 0)
                n = m;
            if (n == 4 || n == 7) {

                joyk = 1;
                joys = m ? 1 : 3;
            }
            break;
        }
        case 1:

            if (scePadSetMainMode(pjoy->nPort, pjoy->nSlot, 1, 3) == 1)
                joys = 2;

            break;
        case 2: {
            unsigned char ab[6];
            int i;

            x = scePadInfoAct(pjoy->nPort, pjoy->nSlot, -1, 0);
            if (x != 2 ||

                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 0, 1) != 1 ||
                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 0, 2) != 2 ||
                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 0, 3) != 0 ||
                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 1, 1) != 1 ||
                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 1, 2) != 1 ||
                scePadInfoAct(pjoy->nPort, pjoy->nSlot, 1, 3) != 1) {
                joys = 3;
                break;
            }
            func_002333B4(ab, 0xFF, 6);
            for (i = 0; i < x; i++)
                ab[i] = i;
            if (scePadSetActAlign(pjoy->nPort, pjoy->nSlot, ab) == 1)
                joys = 2;
            break;
#undef x
        }
        case 3:
            if (scePadInfoPressMode(pjoy->nPort, pjoy->nSlot) != 1) {
                joys = 3;
                break;
            }
            if (scePadEnterPressMode(pjoy->nPort, pjoy->nSlot) == 1)
                joys = x;
            break;
        }
        break;
    case 2:
        switch (scePadGetState(pjoy->nPort, pjoy->nSlot)) {
        case 1:
            joys = 1;
            break;
        case 2:
            break;
        case 0:
            switch (pjoy->joyk) {
            case 1:
                joyk = 2;
                joys = 1;
                break;
            case 2:
                joyk = 3;
                joys = 1;
                break;
            case 3:
                joyk = 4;
                joys = 3;
                break;
            default:
                joyk = 0;
                break;
            }
            break;
        default:
            joyk = 0;
            break;
        }
        break;
    case 3:
        if (func_00187998(pjoy)) {
            if (pjoy->fRumbleEnabled && pjoy->joyk >= 3)
                UpdateRumble__FP6RUMBLE(pjoy->rumble);
        } else {
            joys = 1;
            joyk = 0;
        }
        break;
    }

    SetJoyJoys__FP3JOY4JOYS4JOYK(pjoy, joys, joyk);
}
