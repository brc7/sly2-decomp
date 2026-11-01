typedef int qword __attribute__((mode(TI)));
struct O { int n; float g; int pad[2]; qword q10; qword q20; };
struct P { char pad[0x13D0]; int n13D0; char pad1[0x1830 - 0x13D4]; qword q1830; };
extern qword D_002A2DD0;
void GetStepplayerStunHold(struct P *p, struct O *po);
/** @sly2-annot addr=0x1B9B70
 * Purpose: MURRAY vt96(out): base 0x143220; for attack sets out->n=1 and target pos = predicted attack end
 *   v1830; free-move states n=1; out->g=200
 * Info: proposed name MURRAY_vt96 (T2); class MURRAY slot96; TU murray
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B9B70(struct P *p, struct O *po)
{
    GetStepplayerStunHold(p, po);
    switch (p->n13D0) {
    case 11:
        po->n = 1;
        po->q10 = p->q1830;
        po->q20 = D_002A2DD0;
        break;
    case 0:
    case 1:
    case 9:
    case 10:
        po->n = 1;
        break;
    }
    po->g = 200.0f;
}
