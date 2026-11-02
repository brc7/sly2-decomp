/** @sly2-annot addr=0x1A2CC0
 * Purpose: Map language index 0..13 to the 4CC 'FK$x' with x = e f i g s d p a h n w j k c (English, French,
 *   Italian, German, Spanish, Dutch, Portuguese, ..., Japanese, Korean, Chinese)
 * Info: proposed name FourccFromLanguage (T1); TU language
 * Notes: names_v2 C proposed PeaFindAsegLabel__FP4ASEG3OID (0.438)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#language-0x1a2bd0-0x1a3020
 * @end-sly2-annot */
int FourccFromLanguage(unsigned int n)
{
    switch (n) {
    case 0: return 0x65244B46;
    case 1: return 0x66244B46;
    case 2: return 0x69244B46;
    case 3: return 0x67244B46;
    case 4: return 0x73244B46;
    case 5: return 0x64244B46;
    case 6: return 0x70244B46;
    case 7: return 0x61244B46;
    case 8: return 0x68244B46;
    case 9: return 0x6E244B46;
    case 10: return 0x77244B46;
    case 11: return 0x6A244B46;
    case 12: return 0x6B244B46;
    case 13: return 0x63244B46;
    }
    return 0;
}
