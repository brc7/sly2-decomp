struct O { char pad[0x520]; int x520; };
struct S { struct O *p; int pad[6]; int x1c; char pad2[0xC0-0x20]; int c0; int c4; };
/** @sly2-annot addr=0x19C870
 * Purpose: Return the state-specific record for the air kind (rail kinds 0xB-0xD use owner x520, wall run
 *   x0C4, else x0C0)
 * Info: proposed name PactadjFromJtAir (T2); TU air
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
int func_0019C870(struct S *s, int *pn) {
    int v;
    switch (s->x1c) {
    case 0xB: case 0xC: case 0xD: v = s->p->x520; break;
    case 0x11: v = s->c4; break;
    default: v = s->c0; break;
    }
    *pn = 0;
    return v;
}
