struct VTE { short d; short pad; void (*pfn)(); };
struct S { int a, b, c; struct VTE *vt; };
void PostSoLoad(struct S *p);
/** @sly2-annot addr=0x1D7268
 * Purpose: PostSoLoad then vt79(0)
 * Info: proposed name PostSafehouseLoad (T2); class SAFEHOUSE slot15; TU tu_1D7238
 * Notes: class identity from whole_game WE names/object strings: PostCharSelectPoLoad -> PostSafehouseLoad
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150+whole_game(class
 *   identity)
 * See: docs/reference/functions/player.md#tu_1d7238-0x1d7238-0x1d7a30
 * @end-sly2-annot */
void func_001D7268(struct S *p) {
    struct VTE *e;
    PostSoLoad(p);
    e = &p->vt[79];
    ((void (*)(void *, int))e->pfn)((char *)p + e->d, 0);
}
