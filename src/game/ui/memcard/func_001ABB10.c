struct SAVE { char ab[0x5464]; };
struct M { char pad[0x24]; struct SAVE asave[4]; };
extern char D_002A6C50[], D_002A6C60[], D_002A6C78[];
int func_0021ECD8(char *buf, char *fmt, ...);
/** @sly2-annot addr=0x1ABB10
 * Purpose: sprintf('/%s/%s%d', 'BASCUS-97316YAOTWTD!', 'save', i)
 * Info: proposed name BuildSaveFilePath (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001ABB10(struct M *pm, struct SAVE *ps, char *buf)
{
    func_0021ECD8(buf, D_002A6C50, D_002A6C60, D_002A6C78, ps - pm->asave + 1);
}
