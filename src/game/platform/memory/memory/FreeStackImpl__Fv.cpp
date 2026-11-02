struct CRITSECT { int a[3]; };
extern CRITSECT D_002F6390;
extern int D_002F639C;
extern void *D_002F6388;
extern void *D_002F63A0[];
void LeaveCritSect(CRITSECT *p);
/** @sly2-annot addr=0x1AE428
 * Purpose: Pop stack-heap mark and LeaveCritSect
 * Info: proposed name FreeStackImpl__Fv (EXISTING); TU memory
 * Confidence: high [EXISTING 0.97] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void FreeStackImpl()
{
    D_002F6388 = D_002F63A0[D_002F639C--];
    LeaveCritSect(&D_002F6390);
}
