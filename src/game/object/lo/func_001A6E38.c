typedef unsigned long ulong;
struct S { char pad[0x28]; ulong fl; };
/** @sly2-annot addr=0x1A6E38
 * Purpose: Set/clear bit58 of x28
 * Info: proposed name SetLoFNoVisInherit (T2); TU lo
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6E38(struct S *p, int f) { p->fl = (p->fl & ~((ulong)1 << 58)) | ((ulong)(f & 1) << 58); }
