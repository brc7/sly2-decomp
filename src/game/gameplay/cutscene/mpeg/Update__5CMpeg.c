struct CLOCK { float t; float dt; char pad[0x34]; float t3c; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x10C]; float f10C; int x110; int x114; char x118[8]; int x120; char pad2[0x5C]; char x180[0xBC]; int x23C; volatile int x240; };
int CbDemuxed__5CMpegi(struct S *p, int n);
int func_0024B540(void *a, int b, int c);
void BuildMpegGifs(struct S *p);
void func_001B3700(struct S *p);
void func_00234CA0(int n);
int func_00234A40(int n);
void Update__10CMpegAudio(void *p);
/** @sly2-annot addr=0x1B4058
 * Purpose: get decoded picture, BuildMpegGifs, subtitles, signal display thread
 * Info: proposed name CMpeg_Update (T1); class CMpeg; TU mpeg
 * Notes: names_v2 v1 proposed FUN_0018f610__5CMpeg (0.000) | superseded: CMpeg_RenderPicture (arbitration) |
 *   arbitration knock-on rename: CONFIRMED static: CbDemuxed loop, sceMpegGetPicture, BuildMpegGifs
 *   (+subtitles), FlushCache, CbDemuxed loop, SignalSema, CMpegAudio::Update = Sly1 CMpeg::Update |
 *   arbitration mangling: compile a declaration in the scratch build and compare before committing
 * Confidence: high [T1 0.80] - evidence: Sly 1 / Lilly reference; arbitration; static naming pass; sources:
 *   r5_1AE130+arbitration
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void Update__5CMpeg(struct S *p)
{
    while (p->x240 == 1) CbDemuxed__5CMpegi(p, 0);
    if (func_0024B540(p->x118, p->x110 + 0x285A80, 0x5CD) < 0) return;
    if (p->x120 == 0) {
        p->f10C = D_002DDED0.t3c;
        BuildMpegGifs(p);
        func_001B3700(p);
        func_00234CA0(0);
    }
    while (p->x240) CbDemuxed__5CMpegi(p, 0);
    func_00234A40(p->x23C);
    Update__10CMpegAudio(p->x180);
}
