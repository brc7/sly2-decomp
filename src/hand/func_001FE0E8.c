struct E { short s0; short k; union { int n; float g; } u; };
struct P { char pad[0x22C]; void *ap[3]; void *p238; };
void func_001F80D0(void *, float);
void func_001F0660(void *, int);
/** @sly2-annot addr=0x1FE0E8
 * Purpose: Event k: role 3 -> 0x1F80D0(gesture,g); else SetSmaGoal(role SM, state)
 * Info: proposed name ApplyDialogEvent (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FE0E8(struct P *p, struct E *pe)
{
    int k = pe->k;
    if (k != -1 && p->ap[k]) {
        if (k == 3) func_001F80D0(p->p238, pe->u.g);
        else if (pe->u.n != -1) func_001F0660(p->ap[k], pe->u.n);
    }
}
