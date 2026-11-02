typedef struct { unsigned char c[4]; } RGBA;
typedef struct { int pad[4]; RGBA c; } P;
/** @sly2-annot addr=0x258458
 * Purpose: p->rgba = *c [game linkonce (inline/template)]
 * Info: proposed name CTextBox_SetTextColor (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void func_00258458(P *p, RGBA *c) { p->c = *c; }
