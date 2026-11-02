extern char D_0025AFA0[], D_0025B870[], D_002DF800[];
extern char D_004421A0[], D_004424B0[], D_004424D0[], D_004424F0[], D_00442520[], D_004425A0[], D_00442560[], D_00442240[];
extern char D_000000C0[], D_00000100[];
void EnsureVu1Code__FP4VIFSPvT1(void *, void *, void *);
void func_001574B8(void *);
void func_00226D10(void *);
void func_00157598(void *, void *);
void func_00226CA8(void *, int, int);
void func_00226F28(void *, int, int, void *, int);
void func_00226CE0(void *, void *);
void func_00226D28(void *);
void func_002270C8(void *, int, void *, int);
void func_001576F0(void *);
/** @sly2-annot addr=0x211A80
 * Purpose: Pass 5 begin (kind 21): HUD microprogram + constants + DIRECT HUD GS state
 * Info: proposed name BeginRpHud (T1); TU hud
 * Notes: superseded: SetupSpriteVu1 (r7c_hud)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+rendering+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void BeginRpHud(void *p)
{
    EnsureVu1Code__FP4VIFSPvT1(p, D_0025AFA0, D_0025B870);
    func_001574B8(p);
    func_00226D10(p);
    func_00157598(p, D_002DF800);
    func_00226CA8(p, 0, 0);
    func_00226F28(p, 0xC, 7, D_004421A0, 0xD);
    func_00226F28(p, 0xC, 2, D_004424B0, 0x15);
    func_00226F28(p, 0xC, 2, D_004424D0, 0x17);
    func_00226F28(p, 0xC, 3, D_004424F0, 0x19);
    func_00226F28(p, 0xC, 4, D_00442520, 0x1C);
    func_00226F28(p, 0xC, 3, D_004425A0, 0x20);
    func_00226F28(p, 0xC, 4, D_00442560, 0x23);
    func_00226CA8(p, 0x32, 0x1E7);
    func_00226CE0(p, D_000000C0);
    func_00226D28(p);
    func_002270C8(p, 0x27, D_00442240, 0);
    func_00226D28(p);
    func_00226CE0(p, D_00000100);
    func_00226D28(p);
    func_001576F0(p);
}
