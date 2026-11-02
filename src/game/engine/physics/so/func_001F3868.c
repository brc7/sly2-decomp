typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *);
int PpathzoneFromPos(qword, int);
void func_001F37C8(void *, int);
/** @sly2-annot addr=0x1F3868
 * Purpose: Deferred callback: SetSoPhysRegion(so, region at PosLoWorld)
 * Info: proposed name UpdateSoPhysRegionCallback (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3868(void *p) { func_001F37C8(p, PpathzoneFromPos(func_001A62E8(p), 1)); }
