typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
struct E { int n; float a; float b; float c; int d; };
/** @sly2-annot addr=0x1FAF70
 * Purpose: Allocate an SFX descriptor from SW heap: sfxid=-1, sFull=300, sStart=3000, uVol=1
 * Info: proposed name NewSfx (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/physics-collision.md#6-source-files, docs/engine/sound.md#5-source-files,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
struct E *func_001FAF70(void)
{
    MSG msg;
    struct E *pe;
    msg.cb = sizeof(struct E);
    msg.nKind = 0x10;
    pe = func_001AE2A0(&msg);
    pe->n = -1;
    pe->a = 300.0f;
    pe->b = 3000.0f;
    pe->c = 1.0f;
    return pe;
}
