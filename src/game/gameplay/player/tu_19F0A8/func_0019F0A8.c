struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct G;
extern struct G D_003DD490;
int CploFindSwObjectsByClass(struct G *, int, int, int, int, struct O **);
/** @sly2-annot addr=0x19F0A8
 * Purpose: For every STEPPLAYER (CID 0x34, up to 8) call its vt104
 * Info: proposed name NotifySwStepplayers (T2); TU tu_19F0A8
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/main-loop-startup.md#8-key-functions, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void func_0019F0A8(void) {
    struct O *ap[8];
    int c, i;
    c = CploFindSwObjectsByClass(&D_003DD490, 5, 0x34, 0, 8, ap);
    for (i = 0; i < c; i++) { struct O *o = ap[i]; struct VTE *e = &o->vt[104]; e->pfn((char *)o + e->d); }
}
