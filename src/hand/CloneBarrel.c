struct K { int a; int b; };
struct S { char pad[0x174]; struct K k; };
void CloneSo(struct S *p);
/** @sly2-annot addr=0x183A08
 * Purpose: vt41: SO Clone keeping own field
 * Info: proposed name CloneBarrel (T1); class BARREL slot41; TU grabspot
 * Notes: v3 class identity: CloneCid88 -> CloneBarrel
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void CloneBarrel(struct S *p)
{
    struct K k = p->k;
    CloneSo(p);
    p->k = k;
}
