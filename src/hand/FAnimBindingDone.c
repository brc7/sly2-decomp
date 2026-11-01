struct O { char pad[0x30]; int n30; };
struct E { int k; int h; struct O *po; };
int FAsegaRetracted(struct O *po);
int FSmaRetracted(struct O *po);
int FLeapDone(struct O *po);
int func_0020CA98(struct O *po);
/** @sly2-annot addr=0x209430
 * Purpose: True if the running instance finished/retracted (or none)
 * Info: proposed name FAnimBindingDone (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int FAnimBindingDone(struct E *pe)
{
    int f;
    switch (pe->k) {
    case 0:
        f = 0;
        if (!pe->po || FAsegaRetracted(pe->po))
            f = 1;
        break;
    case 1:
        f = 0;
        if (!pe->po || FSmaRetracted(pe->po) || !pe->po->n30)
            f = 1;
        break;
    case 2:
        f = 0;
        if (!pe->po || FLeapDone(pe->po))
            f = 1;
        break;
    case 3:
        f = 0;
        if (!pe->po || func_0020CA98(pe->po))
            f = 1;
        break;
    case -1:
    default:
        f = 1;
        break;
    }
    return f;
}
