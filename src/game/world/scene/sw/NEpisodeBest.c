struct CRef { int tagk; int tag; };
void RefGetSaveVarV(struct CRef *r, char *s, int a);
void Decref__4CRef(struct CRef *r);
extern char D_002A8A78[];
/** @sly2-annot addr=0x210B28
 * Purpose: Read save variable "episodeBest"
 * Info: proposed name NEpisodeBest (T1); TU sw
 * Notes: names_v2 v1 proposed CloneVolbtn__FP6VOLBTNT0 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int NEpisodeBest(void)
{
    struct CRef r;
    int v;
    RefGetSaveVarV(&r, D_002A8A78, 0);
    v = r.tag;
    Decref__4CRef(&r);
    return v;
}
