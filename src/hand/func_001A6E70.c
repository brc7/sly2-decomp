typedef unsigned long ulong;
struct S { char pad[0x28]; ulong fl; };
/** @sly2-annot addr=0x1A6E70
 * Purpose: Return bit58 of x28
 * Info: proposed name FLoNoVisInherit (T2); TU lo
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
int func_001A6E70(struct S *p) { return (int)(p->fl >> 58) & 1; }
