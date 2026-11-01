struct VTE { short d; short pad; int (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct L { int x; int c; struct O *ap[1]; };
/** @sly2-annot addr=0x1EED78
 * Purpose: Offer a step message (msgid,pv) to the behaviours, top first, until one's vt13 returns nonzero
 *   (consumed)
 * Info: proposed name HandleBhvstkMessage (T1); TU bhvstk
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_001EED78(struct L *pl, int a, int b)
{
    int i = pl->c;
    while (--i >= 0) {
        struct O *po = pl->ap[i];
        struct VTE *e = &po->vt[13];
        if (e->pfn((char *)po + e->d, a, b))
            break;
    }
}
