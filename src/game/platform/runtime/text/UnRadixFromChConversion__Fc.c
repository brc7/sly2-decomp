/** @sly2-annot addr=0x21DC18
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int UnRadixFromChConversion__Fc(unsigned char ch)
{
    switch (ch) {
    case 'd':
    case 'i':
    case 'u':
        return 10;
    case 'B':
    case 'b':
        return 2;
    case 'o':
        return 8;
    case 'x':
        return 16;
    default:
        return 10;
    }
}
