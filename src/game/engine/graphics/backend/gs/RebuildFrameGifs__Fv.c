extern char D_002DF740[], D_002DF800[], D_002DF880[], D_002DF940[], D_002DF9C0[], D_002DFA80[];
extern int D_002BE598, D_002BE59C;
void BuildFrameContextGifs(int a, void *b, int c, void *d, int e);
void BuildFrameTex0Gifs__Fv(void);
/** @sly2-annot addr=0x170048
 * Purpose: Rebuilds the three per-frame context packets (different FBMSK masks) and the framebuffer-texture
 *   packets after a buffer swap
 * Info: proposed name RebuildFrameGifs (T1); TU gs
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void RebuildFrameGifs__Fv(void) {
    BuildFrameContextGifs(0xC, D_002DF740, 8, D_002DF800, 0);
    BuildFrameContextGifs(0xC, D_002DF880, 8, D_002DF940, D_002BE598);
    BuildFrameContextGifs(0xC, D_002DF9C0, 8, D_002DFA80, D_002BE59C);
    BuildFrameTex0Gifs__Fv();
}
