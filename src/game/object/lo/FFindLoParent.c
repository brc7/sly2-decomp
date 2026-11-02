struct O { char pad[0x10]; struct O *x10; };
/** @sly2-annot addr=0x1A6AF8
 * Purpose: True if b is plo or one of its ancestors (walk psoParent)
 * Info: proposed name FFindLoParent (T1); TU lo
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
int FFindLoParent(struct O *a, struct O *b)
{
    while (a) {
        if (a == b) break;
        a = a->x10;
    }
    return a != 0;
}
