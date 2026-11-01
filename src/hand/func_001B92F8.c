struct O { char pad[0x28]; unsigned long f28; };
struct T { char pad[0x180]; struct O *po; };
struct P { char pad[0x1030]; int x1030; char pad1[0x13F0 - 0x1034]; struct T *x13F0; char pad2[0x1448 - 0x13F4]; int x1448; int x144C; int x1450; };
void func_001B6078(struct P *p, int n);
int func_00131968(int n);
/** @sly2-annot addr=0x1B92F8
 * Purpose: State 4: wait for grab event (x1448), abort if object gone; after attach go to carry (state 5)
 *   when anim ends
 * Info: proposed name UpdateMurrayLift (T1); TU murray
 * Notes: runtime4: state 4 lift routine (S41)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources:
 *   r5b_rest+runtime+verdicts_followups
 * See: docs/engine/player.md#6-murray-murraycpp-0x1b4728, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B92F8(struct P *p)
{
    int s;
    for (;;) {
        s = p->x1450;
        switch (s) {
        case 0: break;
        case 1:
            if (p->x1448) {
                if (!p->x13F0) func_001B6078(p, 0);
                else if (!((int)(p->x13F0->po->f28 >> 60) & 1)) {
                    func_001B6078(p, 0);
                    p->x13F0 = 0;
                }
            }
            break;
        case 2:
            if (func_00131968(p->x1030)) func_001B6078(p, 5);
            break;
        }
        if (p->x1450 == s || p->x1450 == -1) break;
    }
}
