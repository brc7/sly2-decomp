int func_00132B10(void *p, int n);
/** @sly2-annot addr=0x1709A8
 * Purpose: Classifies a script target: 0 if cid 0xD (CID13, rail?), 1 if cid 0xB (CID11, pipe?), else -1
 * Info: proposed name NShapeMoveKind (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
int func_001709A8(void *p) { if (func_00132B10(p, 0xD)) return 0; if (func_00132B10(p, 0xB)) return 1; return -1; }
