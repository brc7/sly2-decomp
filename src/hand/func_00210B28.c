struct CRef { int tagk; int tag; };
void func_001D8A38(struct CRef *r, char *s, int a);
void func_00129EC8(struct CRef *r);
extern char D_002A8A78[];
/** @sly2-annot addr=0x210B28
 * Purpose: Read save variable "episodeBest"
 * Info: proposed name NEpisodeBest (T1); TU sw
 * Notes: names_v2 v1 proposed CloneVolbtn__FP6VOLBTNT0 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_00210B28(void)
{
    struct CRef r;
    int v;
    func_001D8A38(&r, D_002A8A78, 0);
    v = r.tag;
    func_00129EC8(&r);
    return v;
}
