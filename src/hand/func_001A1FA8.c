struct B { char pad[0x148]; unsigned long x148; };
struct S { char *x0; char pad[0xC]; int *x10; char pad2[0x1C]; struct B *x30; };
void func_001A16A8(struct S *p);
void func_00195A00(void *p, int a, int b, int c);
void func_001A0B40(struct S *p, int n);
/** @sly2-annot addr=0x1A1FA8
 * Purpose: Callback: if alive return to safe place when the hazard object has flag bit 33, else
 *   SetJtAirJump(11); dead -> state
 * Info: proposed name JtHurtFallCb (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A1FA8(struct S *p)
{
    if (*p->x10 > 0) {
        if (p->x30 && (p->x30->x148 & 0x200000000UL))
            func_001A16A8(p);
        else
            func_00195A00(p->x0 + 0x2C0, 11, 0, 0);
    } else {
        func_001A0B40(p, 8);
    }
}
