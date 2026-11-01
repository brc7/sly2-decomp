struct K { int a; int b; };
struct S { char pad[0x104]; struct K k; };
void CloneXfm(struct S *p);
/** @sly2-annot addr=0x1837D0
 * Purpose: vt41: CloneXfm keeping own field
 * Info: proposed name CloneCid38 (T2); class CID38 slot41; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_001837D0(struct S *p)
{
    struct K k = p->k;
    CloneXfm(p);
    p->k = k;
}
