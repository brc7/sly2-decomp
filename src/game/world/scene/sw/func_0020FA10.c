struct O { char pad[0x70]; long fl; };
struct P { char pad[0xEC]; struct O *po; };
struct S { int a; struct P *pp; char pad[8]; long fl; };
/** @sly2-annot addr=0x20FA10
 * Purpose: Filter: entry object is not ctx and not flag 4
 * Info: proposed name FFilterSoNotSelf (T2); TU sw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020FA10(struct O *po, struct S *p) {
    int r = 0;
    if (!(p->fl & 0x1000000000000L)) {
        struct O *o = p->pp->po;
        if (!(o->fl & 4)) r = o != po;
    }
    return r;
}
