struct CRITSECT { int a[3]; };
extern CRITSECT D_002F6390;
extern int D_002F639C;
extern void *D_002F6388;
extern void *D_002F63A0[];
void EnterCritSect(CRITSECT *p);
/** @sly2-annot addr=0x1AE3D8
 * Purpose: EnterCritSect and push stack-heap mark
 * Info: proposed name InitStackImpl__Fv (EXISTING); TU memory
 * Confidence: high [EXISTING 0.97] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void InitStackImpl()
{
    EnterCritSect(&D_002F6390);
    D_002F63A0[++D_002F639C] = D_002F6388;
}
