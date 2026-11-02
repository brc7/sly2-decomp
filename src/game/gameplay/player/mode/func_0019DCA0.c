struct S { int pad[3]; int c; int d; };
/** @sly2-annot addr=0x19DCA0
 * Purpose: Set active HUD widget to arg or default (field c)
 * Info: proposed name SetJtModeHudOverride (T2); TU mode
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void func_0019DCA0(struct S *s, int a) { if (a) s->d = a; else s->d = s->c; }
