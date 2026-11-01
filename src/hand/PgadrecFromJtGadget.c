/** @sly2-annot addr=0x18D6F8
 * Purpose: Return JT gadget record for gadget id 0x1A/0x25/0x26/0x29 (x1FB0/x1FBC/x1FC8/x1FD4)
 * Info: proposed name PgadrecFromJtGadget (T1); TU jt
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void *PgadrecFromJtGadget(char *p, int n)
{
    switch (n) {
    case 0x1A: return p + 0x1FB0;
    case 0x25: return p + 0x1FBC;
    case 0x26: return p + 0x1FC8;
    case 0x29: return p + 0x1FD4;
    }
    return 0;
}
