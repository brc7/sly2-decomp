struct S { int a; int b[3]; };
void func_001A2C48(struct S *p, int n);
/** @sly2-annot addr=0x1A2C98
 * Purpose: Return number of languages available in the region
 * Info: proposed name CLanguagesRegion (T1); TU language
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/hud-ui.md#language-0x1a2bd0-0x1a3020
 * @end-sly2-annot */
int func_001A2C98(void) { struct S s; func_001A2C48(&s, 0); return s.a; }
