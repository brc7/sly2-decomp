struct E { int a; int b; int c; int d; };
struct S { char pad[0x404]; struct E ae[8]; int c; int pad2[2]; int f; };
/** @sly2-annot addr=0x1F1768
 * Purpose: If loot editing enabled (+0x490): append loot entry {oid 0x2B3 (coin), -, -1, count=x}
 * Info: proposed name AddStepguardLootCoins (T2); TU pickpocket
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/animation.md#6-source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void func_001F1768(struct S *p, int x)
{
    if (p->f) {
        struct E *pe = &p->ae[p->c++];
        pe->a = 0x2B3; pe->c = -1; pe->b = 0; pe->d = x;
    }
}
