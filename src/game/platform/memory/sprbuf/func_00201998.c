struct E { char pad[0x4C]; int n; };
extern int D_002DE5A0;
extern int D_002C3C5C;
extern struct E D_003DD2C0[];
int sceDmaSync(int ch, int a, int b);
/** @sly2-annot addr=0x201998
 * Purpose: sceDmaSync on the transfer channel, flip double-buffer index D_002C3C5C, mark new buffer state 2
 * Info: proposed name SwapSprBuf (T2); TU sprbuf
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/save-memcard.md#3-scratchpad-buffers-sprbufcpp,
 *   docs/reference/functions/core-library.md#sprbuf-0x201620-0x201d78
 * @end-sly2-annot */
void func_00201998(void) { sceDmaSync(D_002DE5A0, 0, 0); D_002C3C5C = 1 - D_002C3C5C; D_003DD2C0[D_002C3C5C].n = 2; }
