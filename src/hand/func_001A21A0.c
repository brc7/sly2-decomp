struct S { char pad[0x4C0]; char x4C0[0x18C8 - 0x4C0]; int x18C8; };
int func_00194640(void *p, int n);
int func_00140ED8(struct S *s, int n);
/** @sly2-annot addr=0x1A21A0
 * Purpose: JT vt76 (zap kind n): hang/pipe/rope states ignore 0x4000; grab state asks FJtGrabBusy; else base
 *   0x140ED8
 * Info: proposed name JT_FIgnoreZap (T2); class JT slot76; TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
int func_001A21A0(struct S *s, int n)
{
    switch (s->x18C8) {
    case 3:
    case 8:
    case 21:
    case 22:
        if (n == 0x4000)
            return 1;
        break;
    case 7:
        if (func_00194640(s->x4C0, n))
            return 1;
        break;
    }
    return func_00140ED8(s, n);
}
