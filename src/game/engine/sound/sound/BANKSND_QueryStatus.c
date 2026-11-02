struct VT { int pad[2]; short d; short p; void (*pfn)(void *); };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x40 - 0x10]; void *p40; char pad3[0x50 - 0x44]; unsigned short fl; };
void func_001FBB30(void);
void snd_SoundIsStillPlaying_CB(void *, void (*)(void), void *);
void SetSndFlags(struct X *, int, int);
/** @sly2-annot addr=0x1FBFB0
 * Purpose: vt14: if 0x400 pending, Incref and ask IOP IsStillPlaying with callback OnBanksndStatus
 * Info: proposed name BANKSND_QueryStatus (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void BANKSND_QueryStatus(struct X *px)
{
    if (px->fl & 0x400) {
        px->pvt->pfn((char *)px + px->pvt->d);
        snd_SoundIsStillPlaying_CB(px->p40, func_001FBB30, px);
        SetSndFlags(px, 0x400, 0);
    }
}
