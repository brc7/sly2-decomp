struct G { char pad[0x13C]; float a[8][4]; float g1BC; float b[4]; char pad2[0x1F0 - 0x1D0]; long l1F0; long l1F8; };
extern struct G D_003DD040;
struct CLOCK;
extern struct CLOCK D_002DDED0;
void func_002319E8(int);
int func_00233048(int, int, int, int, int, int, int);
long TickNow(struct CLOCK *);
/** @sly2-annot addr=0x1FCEA0
 * Purpose: snd_StartSoundSystem, SUKR init (cmd 0), all channel/category volumes = 1, record start clock
 * Info: proposed name StartupSound (T1); TU sound
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/engine/main-loop-startup.md#open-questions, docs/engine/overview.md#1-boot,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StartupSound(void)
{
    int i, j;
    long t;
    func_002319E8(2);
    (void)func_00233048(0x524B5553, 0, 0, 0, 0, 0, 0);
    D_003DD040.g1BC = 1.0f;
    for (i = 0; i < 4; i++) {
        D_003DD040.b[i] = 1.0f;
        for (j = 0; j < 8; j++) D_003DD040.a[j][i] = 1.0f;
    }
    t = TickNow(&D_002DDED0);
    D_003DD040.l1F0 = t;
    D_003DD040.l1F8 = t;
}
