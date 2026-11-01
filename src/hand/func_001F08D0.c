struct O { char pad[0x40]; float g; };
struct P { char pad[0x30]; struct O *po; };
void func_00130D30(struct O *, int, float, float);
void func_00130B18(struct O *);
void func_001F0FA0(struct P *);
/** @sly2-annot addr=0x1F08D0
 * Purpose: Detach the ASEGA, seek it to the start or end depending on speed sign, retract, then
 *   EndSmaTransition
 * Info: proposed name AbortSmaTransition (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F08D0(struct P *p)
{
    struct O *po = p->po;
    if (po) {
        p->po = 0;
        if (po->g < 0.0f) func_00130D30(po, 0, 0.0f, po->g);
        else func_00130D30(po, 2, 0.0f, po->g);
        func_00130B18(po);
        func_001F0FA0(p);
    }
}
