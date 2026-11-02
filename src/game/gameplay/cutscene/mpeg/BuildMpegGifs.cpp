typedef unsigned long ulong;
typedef unsigned int uint;
struct QW;
class GIFS {
public:
    int a[10];
    GIFS();
    void AllocStatic(int cqw, void *aqw);
    void AddDmaCnt();
    void AddPrimPack(int prim, int creg, ulong regs);
    void PackAD(long a, long d);
    void AddImage(int cqw);
    void AddDmaRefs(int cqw, void *aqw);
    void AddPrimEnd();
    void EndDmaCnt();
    void Detach(int *pcqw, QW **paqw);
};
struct P { char pad[0x110]; char *x110; int pad114; int x118; int x11C; };

/** @sly2-annot addr=0x1B3510
 * Purpose: build GIF packets to upload decoded RGB32 picture
 * Info: proposed name BuildMpegGifs (T1); TU mpeg
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
extern "C" void BuildMpegGifs(P *p)
{
    char *pb = p->x110 + 0x285A80;
    int cx = p->x118 / 16;
    int cy = p->x11C / 16;
    int i, j;
    GIFS gifs;

    gifs.AllocStatic(0x22D6, p->x110 + 0x3F8E80);
    gifs.AddDmaCnt();
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x50, 0xC180L << 36);
    gifs.PackAD(0x52, (0x8000L << 21) | 0x10);
    for (i = 0; i < cx; i++) {
        for (j = 0; j < cy; j++) {
            gifs.AddPrimPack(0, 1, 0xE);
            gifs.PackAD(0x51, ((ulong)i << 36) | ((ulong)(j << 4) << 48));
            gifs.PackAD(0x53, 0);
            gifs.AddImage(0x40);
            gifs.AddDmaRefs(0x40, (void *)((uint)pb & 0x0FFFFFFF));
            pb += 0x400;
        }
    }
    gifs.AddPrimPack(0, 1, 0xE);
    gifs.PackAD(0x3F, 0);
    gifs.PackAD(0x61, 0);
    gifs.AddPrimEnd();
    gifs.EndDmaCnt();
    gifs.Detach(0, 0);
}
