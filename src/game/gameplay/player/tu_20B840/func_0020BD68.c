/** @sly2-annot addr=0x20BD68
 * Purpose: afFootDown[0..1] = 0
 * Info: proposed name ResetGaitFeet (T2); TU tu_20B840
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020BD68(char *p) { *(int *)(p + 0xA0) = 0; *(int *)(p + 0x9C) = 0; }
