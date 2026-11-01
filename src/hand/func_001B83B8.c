struct P { char pad[0x4C0]; char sub[0x13D0 - 0x4C0]; int n13D0; };
int func_00194640(void *p, int k);
int func_00140ED8(struct P *p, int k);
/** @sly2-annot addr=0x1B83B8
 * Purpose: MURRAY vt76(k): true for state 15, state 20 with k==0x4000, component 13 check, else base 0x140ED8
 * Info: proposed name MURRAY_vt76 (T2); class MURRAY slot76; TU murray
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int func_001B83B8(struct P *p, int k)
{
    switch (p->n13D0) {
    case 20:
        if (k == 0x4000)
            return 1;
        break;
    case 15:
        return 1;
    case 13:
        if (func_00194640(p->sub, k))
            return 1;
        break;
    }
    return func_00140ED8(p, k);
}
