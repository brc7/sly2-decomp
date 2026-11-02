extern void *D_002DE9F0[];
extern void *D_002DE760;
void func_0015EF30(void);
/** @sly2-annot addr=0x15EF00
 * Purpose: Font startup: loads/posts the built-in fonts (0x15EF30) and sets the default font pointer
 *   D_002DE760 = font[0]
 * Info: proposed name StartupFont (T1); TU font
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen, docs/engine/hud-ui.md#4-source-files,
 *   docs/engine/hud-ui.md#5-key-functions
 * @end-sly2-annot */
void func_0015EF00(void)
{
    func_0015EF30();
    D_002DE760 = D_002DE9F0[0];
}
