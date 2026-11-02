/* strchr (libc-style; Sly 2 game copy). */
/** @sly2-annot addr=0x21EDF8
 * Purpose: libc-style strchr game copy
 * Info: proposed name strchr (T1); TU text
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
char *strchr(const char *p,int ch) { do { if(*(unsigned char*)p==ch) return (char*)p; } while(*p++); return 0; }
