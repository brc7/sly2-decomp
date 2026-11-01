struct E { int a; int b; int c; int d; };
struct S { char pad[0x404]; struct E ae[8]; int c; int pad2[2]; int f; };
/** @sly2-annot addr=0x1F17B0
 * Purpose: If list empty or editing enabled: append loot entry {oid=x, 0, -1, count=1}
 * Info: proposed name AddStepguardLootItem (T2); TU pickpocket
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void func_001F17B0(struct S *p, int x)
{
    if (p->c <= 0 || p->f) {
        struct E *pe = &p->ae[p->c++];
        pe->a = x; //P
        pe->b = 0; //P
        pe->c = -1; //P
        pe->d = 1; //P
    }
}
