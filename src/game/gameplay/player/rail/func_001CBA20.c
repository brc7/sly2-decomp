struct K { int a, b; };
struct S { char pad[0x1B4]; struct K k; };
void CloneShape(struct S *p);
/** @sly2-annot addr=0x1CBA20
 * Purpose: RAIL::Clone (vt41): CloneShape keeping own GRABSPOT link
 * Info: proposed name CloneRail (T1); class RAIL slot41; TU rail
 * Notes: names_v2 C proposed BindHpnt__FP4HPNT (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CBA20(struct S *p)
{
    struct K k = p->k;
    CloneShape(p);
    p->k = k;
}
