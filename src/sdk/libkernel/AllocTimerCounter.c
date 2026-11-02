// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// MATCH with 2.95.3-114 (Sony slot layout s0@0,s1@0x10,ra@0x20) + the sq->sd/lq->ld assembler macro (Claude agent).
int func_00239450(void);
int func_0023A228(void);
int func_002394A0(void);
int AllocTimerCounter(void)
{
    int st = func_00239450();
    int r = func_0023A228();
    if (st) func_002394A0();
    return r;
}
