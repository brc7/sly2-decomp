extern struct CLOCK { float a[16]; } D_002DDED0;
struct S { char p0[0x34]; float g34; };
void func_001D9870(struct S *p);
void func_001DA950(struct S *p, int a, int b);
/** @sly2-annot addr=0x1DA808
 * Purpose: Base update then close when the mode timer (+0x34) expires
 * Info: proposed name UpdateScreenTimeout (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001DA808(struct S *p) {
    func_001D9870(p);
    if (p->g34 <= D_002DDED0.a[15]) func_001DA950(p, -1, -1);
}
