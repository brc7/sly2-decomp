struct S {
    char pad[0x16C]; int a, b;
    void Action() __asm__("func_001BB400");
};
struct G;
extern G D_003DD490;
typedef void (S::*PMF)();
extern "C" void EnsureSwCallback(G *, S *, PMF, int);
/** @sly2-annot addr=0x1BAED8
 * Purpose: Remember instigator and kind, EnsureSwCallback(g_sw, this, &Cid98::React)
 * Info: proposed name ScheduleCid98Reaction (T2); class CID98; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
extern "C" void func_001BAED8(S *p, int a, int b)
{
    p->b = b;
    p->a = a;
    EnsureSwCallback(&D_003DD490, p, &S::Action, 0);
}
