typedef struct { float x; float y; } P;
/** @sly2-annot addr=0x258438
 * Purpose: p->x=x; p->y=y [game linkonce (inline/template)]
 * Info: proposed name CTextBox_SetPos (T1); TU game_linkonce
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: sdk_names
 * See: docs/reference/functions/core-library.md#game_linkonce-0x254f00-0x259de4
 * @end-sly2-annot */
void CTextBox_SetPos(P *p, float x, float y) { p->x = x; p->y = y; }
