struct RUMINS { char pad[4]; unsigned char fHighSpeedMotor; unsigned char bLowSpeedMotor; };
struct RUMPAT { RUMINS arumins[1]; };
enum RUMS { RUMS_Dead, RUMS_Idle, RUMS_Rumble, RUMS_Stop, RUMS_Kill };
struct RUMBLE { int nPort; int nSlot; RUMS rums; RUMPAT *prumpat; int pad; int pad2; float dtRumins; };
extern float D_002DDF10;
extern "C" int func_00243870(int, int, unsigned char *);
void StopRumbleActuators(RUMBLE *prumble);
/** @sly2-annot addr=0x1D53C0
 * Purpose: Rumble state machine transition (scePadSetActDirect)
 * Info: proposed name SetRumbleRums__FP6RUMBLE4RUMS (EXISTING); TU rumble
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r6_1D5150
 * See: docs/reference/functions/main-loop-startup.md#rumble-0x1d5150-0x1d57a8
 * @end-sly2-annot */
void SetRumbleRums(RUMBLE *prumble, RUMS rums)
{
    if (rums == prumble->rums)
        return;
    unsigned char abShockDirect[6];
    switch (rums)
    {
    case RUMS_Rumble:
        abShockDirect[0] = prumble->prumpat->arumins[0].fHighSpeedMotor;
        abShockDirect[1] = prumble->prumpat->arumins[0].bLowSpeedMotor;
        func_00243870(prumble->nPort, prumble->nSlot, abShockDirect);
        prumble->rums = rums;
        break;
    case RUMS_Stop:
    case RUMS_Kill:
        StopRumbleActuators(prumble);
        prumble->dtRumins = D_002DDF10 * 4.0f;
    case RUMS_Dead:
    case RUMS_Idle:
    default:
        prumble->rums = rums;
        break;
    }
}
