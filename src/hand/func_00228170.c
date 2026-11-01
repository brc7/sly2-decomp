struct VTE { short d; short pad; void (*pf)(); };
struct O { int a[3]; struct VTE *vt; };
extern char D_003DD490[];
struct O *func_001C4840(void *g);
/** @sly2-annot addr=0x228170
 * Purpose: Cid41 TRIGGER (optid 235): current player vt38 MatchStepXf(player, this) then splice event 5;
 *   Cid41 = in-level teleport destination (strings WARP_start/WARP_end/WARP_safehouse)
 * Info: proposed name TriggerCid41 (T2); TU exit_warp
 * Notes: names_v2 B proposed FIgnoreStepguardIntersection__FP9STEPGUARDP2SO (0.859) | descriptive only:
 *   warp-TU splice fn: hand player to object | no usable identifier; purpose only | class review: in-level
 *   teleport point; class coinage (WARPPT/TELEPORT) not adopted -> placeholder, T2 | T2: placeholder token
 *   Cid41
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources:
 *   r8_21BB88+class_review(master_names_v2_1)
 * See: docs/reference/class-tree.md#class-tree-all-199-cids,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_00228170(struct O *p)
{
    struct O *o = func_001C4840(D_003DD490);
    if (o) {
        o->vt[38].pf((char *)o + o->vt[38].d, p);
        p->vt[3].pf((char *)p + p->vt[3].d, 5, 0, 0, 0, 0);
    }
}
