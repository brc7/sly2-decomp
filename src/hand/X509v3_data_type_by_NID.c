struct X { int a; int b; };
struct X *func_00154180(int n);
/** @sly2-annot addr=0x154158
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name GetDetectorTargetStateById (EXISTING); TU detector
 * Notes: RENAME proposed -> GetDetectorTargetStateById (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.75]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/guards-ai.md#5-perception, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
int X509v3_data_type_by_NID(int n)
{
    struct X *p = func_00154180(n);
    if (p)
        return p->b;
    return -1;
}
