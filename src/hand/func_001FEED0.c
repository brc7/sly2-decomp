struct M { int n0; int pad4; int n8; int nC; short h10; short pad12; int pad14[3]; short h20; short pad22; int pad24; int n28; int pad2C; };
struct P { char pad[0x64]; int n64; char pad2[0xE8 - 0x68]; int nE8; char pad3[0x138 - 0xEC]; int n138; };
int func_0011D3F8(int, int);
void *func_002333B4(void *, int, int);
void *func_001FD028(struct P *, int);
short func_001FD070(struct P *, void *);
void func_002322F0(struct M *);
/** @sly2-annot addr=0x1FEED0
 * Purpose: Special sfx handling after play: table-driven (7x6, 25) level selection; may stop voice
 * Info: proposed name PlayBanksndVariant (T2); TU sound
 * Notes: verifier: purpose partly wrong: SNDMGR helper (caller 0x1FBC58, when the sound belongs to the
 *   current player): random step from tables a[7][6]/b[25] walks +0x138 within -12..12, then sends a 4
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FEED0(struct P *p)
{
    int r;
    if (p->n64) return;
    {
    int a[7][6] = {
        { -5, -5, -5, -5, -5, -5 },
        { -5, -5, -5, -5, -5, 1 },
        { -5, -5, -5, -5, 1, 1 },
        { -5, -5, -5, 1, 1, 1 },
        { -5, -5, 1, 1, 1, 1 },
        { -5, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
    };
    int b[25] = { 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 };
    struct M m;
    r = func_0011D3F8(0, 5);
    p->n138 += a[b[p->n138 + 12]][r];
    func_002333B4(&m, 0, 0x30);
    m.n28 = 0x13;
    m.n0 = p->nE8;
    m.h10 = func_001FD070(p, func_001FD028(p, 0x28));
    m.h20 = p->n138 * 127;
    func_002322F0(&m);
    }
}
