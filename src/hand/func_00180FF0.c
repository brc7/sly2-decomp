struct E { int a; int b; int x8; char pad[0x34]; };
struct S { char pad[0x188]; int n; };
extern int D_002DFB80;
void func_0017F1A0(struct S *p, struct E *ae, int i);
void func_0017F5B0(struct S *p, int n);
/** @sly2-annot addr=0x180FF0
 * Purpose: vt55: base then start dying
 * Info: proposed name AdjustHackenemyXp (T2); class HACKENEMY slot55; TU tu_17F090
 * Notes: names_v2 v1 proposed FUN_001cac30 (0.000) | CID49->CID73 (CID 0x49=73, vtable) | class identity from
 *   whole_game WE names/object strings: AdjustCid73Xp -> AdjustHackenemyXp
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180FF0(struct S *p, struct E *ae, int i)
{
    func_0017F1A0(p, ae, i);
    if (p->n) {
        struct E *pe = &ae[1 - i];
        if (pe->x8 == D_002DFB80)
            func_0017F5B0(p, pe->x8);
    }
}
