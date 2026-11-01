void func_00167E58(int n);
void func_00234CD0(int n);
/** @sly2-annot addr=0x167F78
 * Purpose: INTC VIF1/VU1 interrupt handler: reads back VU1 data memory results (colour conversion), clears
 *   the VIF1 stall (VIF1_FBRST=8), returns -1
 * Info: proposed name VU1_Interrupt (T1); TU glob
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/rendering.md#6-lighting-relight-and-outlines-the-cel-look,
 *   docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
int func_00167F78(void)
{
    func_00167E58(0x1100C320);
    func_00234CD0(0);
    *(volatile int *)0x10003C10 = 8;
    asm volatile("sync\n\tei");
    return -1;
}
