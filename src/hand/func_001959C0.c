struct S { int a; char pad[0x17C-4]; char b[0x1B8-0x17C]; char c[4]; };
void func_00137A30(int, int, int, void *, int, int, int, void *);
/** @sly2-annot addr=0x1959C0
 * Purpose: Create the 5-entry dynamic jump ASEG blend (0x137A30 cbBl 0xC, 5 entries)
 * Info: proposed name PostLoadJtAir (T1); TU air
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_001959C0(struct S *s) { func_00137A30(s->a, 0xC, 5, s->b, 0, 0, 0, s->c); }
