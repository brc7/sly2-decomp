struct P { char pad[0x268]; int x268; char pad1[0x13D0 - 0x26C]; int k; char pad2[0x13F0 - 0x13D4]; int x13F0; char pad3[0x1458 - 0x13F4]; int x1458; char pad4[0x1A58 - 0x145C]; int a1A58[0x16]; int a1AB0[3]; int x1ABC; };
int func_001944C8(void *p, int *pk);
int func_001A23F8(void *p, int *pk);
int func_0019C870(void *p, int *pk);
void func_00203D00(struct P *p, int v, int k);
/** @sly2-annot addr=0x1B8008
 * Purpose: MURRAY vt90: choose the per-state STEP parameter block (component-supplied for 2/13/14, carry
 *   table x1AB0, default x268) and apply it (0x203D00)
 * Info: proposed name ApplyMurrayStateStepParams (T2); class MURRAY slot90; TU murray
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B8008(struct P *p)
{
    int k = 0;
    int v;
    switch (p->k) {
    case 13: v = func_001944C8((char *)p + 0x4C0, &k); break;
    case 14: v = func_001A23F8((char *)p + 0xDF0, &k); break;
    case 2:
        if (p->x13F0) v = p->x1ABC;
        else v = func_0019C870((char *)p + 0x2C0, &k);
        break;
    case 5: v = p->a1AB0[p->x1458]; break;
    default: v = p->a1A58[p->k]; break;
    }
    if (!v) v = p->x268;
    func_00203D00(p, v, k);
}
