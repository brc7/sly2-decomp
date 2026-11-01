struct K { int a; int b; };
struct S { char pad[0x1B4]; struct K k; };
void func_001E5DB8(struct S *p);
/** @sly2-annot addr=0x182FA0
 * Purpose: vt41: CloneShape keeping own 8-byte field
 * Info: proposed name CloneCid12 (T2); class CID12 slot41; TU grabspot
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grabspot-0x182c70-0x1848a8
 * @end-sly2-annot */
void func_00182FA0(struct S *p)
{
    struct K k = p->k;
    func_001E5DB8(p);
    p->k = k;
}
