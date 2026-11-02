struct S { char pad[0x184]; int n; char p1[0x38]; int x1c0; };
extern char D_002DEB30[];
int func_0017F1D0(struct S *p, void *message);
void BroadcastGameEvent(void *g, int n, struct S *p);
void func_00181268(struct S *p, int n);
/** @sly2-annot addr=0x1811B8
 * Purpose: vt75: CID47::TakeDamage + state change
 * Info: proposed name HACKENEMY_TakeDamage (T2); class HACKENEMY slot75; TU tu_17F090
 * Notes: names_v2 v1 proposed AdjustStepguardDz__FP9STEPGUARDiP2DZif (0.058) | CID49->CID73 (CID 0x49=73,
 *   vtable) | class identity from whole_game WE names/object strings: CID73_TakeDamage ->
 *   HACKENEMY_TakeDamage
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
int func_001811B8(struct S *p, void *message)
{
    register int four __asm__("$2") = 4; if (p->x1c0 == four) return 0;
    if (!func_0017F1D0(p, message)) return 0;
    if (p->n < 0) return 1;
    if (p->n <= 0) BroadcastGameEvent(D_002DEB30, 0x18, p);
    func_00181268(p, 4);
    return 1;
}
