extern "C" double func_00245178(double);
/** @sly2-annot addr=0x21E048
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int ExpFirstFromG(double g)
{
    int exp;
    if (g > 0.0) {
        double l = func_00245178(g);
        exp = (int)l;
        if (l < 0.0 && l != (double)exp)
            exp--;
    } else {
        exp = 0;
    }
    return exp;
}
