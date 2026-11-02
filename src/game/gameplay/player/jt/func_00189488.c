struct S { char pad[0x18C8]; int n; };
int func_001409B8(struct S *p);
/** @sly2-annot addr=0x189488
 * Purpose: JT vt98: base 0x1409B8; if -1, return 1 for jts 1/5/6/9 else -1
 * Info: proposed name JT_vt98 (T2); class JT slot98; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_00189488(struct S *p)
{
    int r = func_001409B8(p);
    if (r != -1) return r;
    switch (p->n) {
    case 1: case 5: case 6: case 9:
        return 1;
    }
    return -1;
}
