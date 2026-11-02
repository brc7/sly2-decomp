struct A { char pad[0x40]; int h; char pad2[4]; float g; };
void func_00232568(int h, int a, int b, int c);
void func_00232350(int h);
/** @sly2-annot addr=0x1FBE80
 * Purpose: vt10: fade-out via snd_AutoVol(h,-4,dt*240,2) if dt>0 else snd_StopSound(h)
 * Info: proposed name StopBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void StopBanksnd(struct A *p)
{
    if (p->g > 0.0f)
        func_00232568(p->h, -4, (int)(p->g * 240.0f), 2);
    else
        func_00232350(p->h);
}
