typedef struct T { char pad[0x1C]; float x1C; } T;
typedef struct P {
    char pad0[0x150];
    T *x150;
    char pad154[0x1030 - 0x154];
    void *x1030;
    char pad1034[0x1810 - 0x1034];
    int x1810;
    char pad1814[0x181C - 0x1814];
    int x181C;
} P;
int func_00131968(void *p);
void func_001B6078(P *p, int s);
int func_001B84E0(P *p);
int func_001C46C0(T *t, int k, float f);
void func_001B86B0(P *p, int k);
int func_001B70A0(P *p);
void func_001B8938(P *p);

/** @sly2-annot addr=0x1B8988
 * Purpose: State 11 combo: chain Square (0<->1) or 0x10 (2<->3) within combo window; takedown check; movement
 *   cancel
 * Info: proposed name UpdateMurrayAttack (T1); TU murray
 * Notes: runtime4: state 11 attack routine (S38-S40)
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources:
 *   r5b_rest+runtime+verdicts_followups
 * See: docs/engine/player.md#6-murray-murraycpp-0x1b4728, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B8988(P *p)
{
    int s;
    for (;;) {
        s = p->x1810;
        if (!p->x1030 || func_00131968(p->x1030)) {
            func_001B6078(p, 0);
        } else {
            switch (p->x1810) {
            case 0:
            case 1:
                if (func_001B84E0(p) && func_001C46C0(p->x150, 0x80, 0.25f)) {
                    func_001B86B0(p, p->x1810 == 0);
                } else if (!func_001B84E0(p) || !func_001B70A0(p)) {
                    if (p->x181C && 0.0f < p->x150->x1C) func_001B8938(p);
                }
                break;
            case 2:
            case 3:
                if (func_001B84E0(p) && func_001C46C0(p->x150, 0x10, 0.25f)) {
                    func_001B86B0(p, (p->x1810 == 2) ? 3 : 2);
                } else if (!func_001B84E0(p) || !func_001B70A0(p)) {
                    if (p->x181C && 0.0f < p->x150->x1C) func_001B8938(p);
                }
                break;
            }
        }
        if (p->x1810 == s || p->x1810 == -1) break;
    }
}
