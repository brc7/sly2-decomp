struct ANIMATION;
struct B {
    char pad[0x24];
    struct ANIMATION *animation24;
    char pad2[8];
    struct ANIMATION *animation30;
    int pad3;
    struct ANIMATION *animation38;
};
struct S { int n; int a; struct B *pb; };

/** @sly2-annot addr=0x209518
 * Purpose: Return the animation instance pointer for a binding kind
 * Info: proposed name PasegaFromAnimBinding (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct ANIMATION *func_00209518(struct S *p)
{
    switch (p->n) {
    case 0: return (struct ANIMATION *)p->pb;
    case 1: return p->pb->animation30;
    case 2: return p->pb->animation38;
    case 3: return p->pb->animation24;
    }
    return 0;
}
