struct S { char pad[0x150]; void *p150; char pad2[0xF00 - 0x154]; int f00; int f04; char pad3[0xF20 - 0xF08]; int f20; };
int func_001443D8(struct S *p);
int func_001C46C0(void *p, int n, float g);
void func_0019CCE8(void *p);
void func_0019D330(void *p);
/** @sly2-annot addr=0x1B5038
 * Purpose: If STEPPLAYER component +0xF00 is active (0x1443D8: xF04<3): in its mode 1 any face button (0x4F0)
 *   cancels it (0x19CCE8); always ticks it (0x19D330)
 * Info: proposed name UpdateMurrayF00Input (T2); TU murray
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B5038(struct S *p)
{
    if (func_001443D8(p)) {
        if (p->f04 == 1 && !p->f20 && func_001C46C0(p->p150, 0x4F0, 0.25f)) func_0019CCE8(&p->f00);
        func_0019D330(&p->f00);
    }
}
