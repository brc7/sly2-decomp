struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { struct O *po; };
extern char D_004435F0[];
void func_00227788(void *p);
/** @sly2-annot addr=0x1A3078
 * Purpose: If a letterbox object exists, speaker label/sound D_004435F0 and call its vt49 (show)
 * Info: proposed name ShowLetterbox (T1); TU letterbox
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#letterbox-0x1a3020-0x1a3260,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001A3078(struct S *p)
{
    if (p->po) {
        struct O *o;
        struct VTE *e;
        func_00227788(D_004435F0);
        o = p->po;
        e = &o->vt[49];
        e->pfn((char *)o + e->d, -1);
    }
}
