extern char D_0047220F;
extern void *D_002F6380, *D_002F6384, *D_002F6388, *D_002F638C;
extern int D_002F639C;
extern char D_002F6390;
void InitCritSect__FP8CRITSECT(void *p);
void StartupMemMgr(void) __asm__("StartupMemMgr__Fv");
/** @sly2-annot addr=0x1AE168
 * Purpose: Init heaps: world min/mac = align16(_end), stack min/mac = 0x2000000, stack index -1, InitCritSect
 * Info: proposed name StartupMemMgr__Fv (EXISTING); TU memory
 * Confidence: high [EXISTING 0.98] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/engine/overview.md#1-boot, docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void StartupMemMgr(void)
{
    void *end = (void *)((int)&D_0047220F & ~0x0f);
    D_002F6380 = D_002F6384 = end;
    D_002F6388 = D_002F638C = (void *)0x02000000;
    D_002F639C = -1;
    InitCritSect__FP8CRITSECT(&D_002F6390);
}
