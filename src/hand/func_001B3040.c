struct G { char pad[0x110]; int x110; };
extern struct G D_002F66B0;
extern char D_003DD040[], D_004435F0[];
struct S { int x0; int x4; char x8[0x28]; char x30[0x20]; char x50[0x50]; int xA0; int xA4; };
void func_00232438(unsigned int m);
void func_001FE738(void *p);
void func_00227788(void *p);
void func_00232408(void);
int func_00231C48(void);
void func_00232D80(int n);
void func_00232DA8(int n);
int func_00232220(int n);
void func_002321E8(int n, int m);
void func_0013D558(void *p, int cb, void *pv);
/** @sly2-annot addr=0x1B3040
 * Purpose: Stop game sound/music, allocate IOP voice resources for movie audio
 * Info: proposed name CMpegAudio_Init (T1); class CMpegAudio; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void func_001B3040(struct S *p)
{
    func_00232438(0xFFFFFFFF);
    func_001FE738(D_003DD040);
    func_00227788(D_004435F0);
    func_00232408();
    while (func_00231C48()) ;
    func_00232D80(1);
    func_00232DA8(0);
    p->xA0 = func_00232220(2);
    p->xA4 = func_00232220(0x10);
    func_002321E8(2, 0x400);
    func_002321E8(0x10, 0x400);
    p->x0 = 1;
    p->x4 = -1;
    func_0013D558(p->x30, 0x28, p->x8);
    func_0013D558(p->x50, 0x18000, (void *)(D_002F66B0.x110 + 0x80000));
}
