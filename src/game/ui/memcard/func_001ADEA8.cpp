struct O { char pad[0x58]; int x58; int pad2[2]; int x64; };
struct S { char pad[0x24]; O *po; };
/** @sly2-annot addr=0x1ADEA8
 * Purpose: child x58>=2 or x64>=2
 * Info: proposed name FMcStatusVisible (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" int func_001ADEA8(S *p)
{
    O *o = p->po;
    if (!o) return 0;
    if (o->x58 < 2 && o->x64 < 2) return 0; return 1;
}
