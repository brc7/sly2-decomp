struct E { int n; int pad[3]; };
struct P { char pad[0x404]; struct E ae[8]; int c484; int i488; };
void func_00200F58(void *, int, int, int *);
/** @sly2-annot addr=0x1F1EB8
 * Purpose: Splice property getter: return the remaining loot oids (entries i488..c484) as a CRef list
 *   (RefSetArgListFromPvs kind 6)
 * Info: proposed name RefGetStepguardLoot (T2); TU pickpocket
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void *func_001F1EB8(void *r, struct P *p)
{
    int buf[8];
    int n = 0;
    int i;
    for (i = p->i488; i < p->c484; i++) buf[n++] = p->ae[i].n;
    func_00200F58(r, n, 6, buf);
    return r;
}
