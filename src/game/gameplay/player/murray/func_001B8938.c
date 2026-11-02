struct S { char pad[0x1030]; void *p; char pad2[0x190C - 0x1034]; void *q; };
extern char D_002C09F8[];
void func_00131B60(void *a, struct S *p, int n, void *s);
void SetMurrayState(struct S *p, int n);
/** @sly2-annot addr=0x1B8938
 * Purpose: Fade the attack overlay (0x131B60), clear it and return to idle
 * Info: proposed name CancelMurrayAttack (T2); TU murray
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B8938(struct S *p)
{
    func_00131B60(p->p, p, 5, D_002C09F8);
    p->q = p->p;
    p->p = 0;
    SetMurrayState(p, 0);
}
