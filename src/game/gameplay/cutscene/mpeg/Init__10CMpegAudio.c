struct G { char pad[0x110]; int x110; };
extern struct G D_002F66B0;
extern char D_003DD040[], D_004435F0[];
struct S { int x0; int x4; char x8[0x28]; char x30[0x20]; char x50[0x50]; int xA0; int xA4; };
void snd_PauseAllSoundsInGroup(unsigned int m);
void StopDialog(void *p);
void func_00227788(void *p);
void snd_StopAllStreams(void);
int func_00231C48(void);
void snd_StreamSafeCheckCDIdle(int n);
void snd_StreamSafeCdSync(int n);
int snd_GetMasterVolume(int n);
void snd_SetMasterVolume(int n, int m);
void Init__10CByteQueueiPv(void *p, int cb, void *pv);
/** @sly2-annot addr=0x1B3040
 * Purpose: Stop game sound/music, allocate IOP voice resources for movie audio
 * Info: proposed name CMpegAudio_Init (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void Init__10CMpegAudio(struct S *p)
{
    snd_PauseAllSoundsInGroup(0xFFFFFFFF);
    StopDialog(D_003DD040);
    func_00227788(D_004435F0);
    snd_StopAllStreams();
    while (func_00231C48()) ;
    snd_StreamSafeCheckCDIdle(1);
    snd_StreamSafeCdSync(0);
    p->xA0 = snd_GetMasterVolume(2);
    p->xA4 = snd_GetMasterVolume(0x10);
    snd_SetMasterVolume(2, 0x400);
    snd_SetMasterVolume(0x10, 0x400);
    p->x0 = 1;
    p->x4 = -1;
    Init__10CByteQueueiPv(p->x30, 0x28, p->x8);
    Init__10CByteQueueiPv(p->x50, 0x18000, (void *)(D_002F66B0.x110 + 0x80000));
}
