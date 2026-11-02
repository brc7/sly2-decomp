struct MSG { int cb; int nKind; };
extern unsigned int D_002F6384;
void func_001AE1C8(int);
/** @sly2-annot addr=0x1AE1D0
 * Purpose: World (level) heap bump allocator: align up D_002F6384 to req.cbAlign, advance by align16(cb)
 * Info: proposed name PvAllocSwImpl (T1); TU memory
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#heap-0x116918-0x116ca0,
 *   docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60,
 *   docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void *func_001AE1D0(struct MSG *p)
{
    unsigned int pv;
    if (!p->cb)
        return 0;
    func_001AE1C8(p->cb);
    pv = (D_002F6384 + (p->nKind - 1)) & -p->nKind;
    D_002F6384 = pv + ((p->cb + 15) & -16);
    return (void*)pv;
}
