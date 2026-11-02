int func_0021ED28(char *s);
unsigned short WSjisFromCh(unsigned char ch);
/** @sly2-annot addr=0x1EEA70
 * Purpose: Convert C string (strlen) to array of u16 SJIS codes
 * Info: proposed name ConvertPchzToSjis (EXISTING); TU sjis
 * Notes: agent/verifier proposes ConvertPchzToSjis for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> ConvertPchzToSjis (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.55] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+arbitration
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#sjis-0x1ee988-0x1eec60
 * @end-sly2-annot */
void ConvertPchzToSjis(char *s, unsigned short *pw)
{
    int i;
    int c = func_0021ED28(s);
    for (i = 0; i < c; i++) {
        unsigned short w = WSjisFromCh(s[i]);
        unsigned short lo = w & 0xFF;
        unsigned short hi = (w >> 8) & 0xFF;
        pw[i] = (lo << 8) | hi;
    }
    pw[i] = 0;
}
