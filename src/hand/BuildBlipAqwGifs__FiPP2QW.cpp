#include "gifs.h"
extern "C" void BuildBlipAqwGifs__FiPP2QW(int n, QW *aqw)
{
    GIFS gifs;
    gifs.AllocStatic(4, aqw);
    gifs.AddPrimPack(0, 1, 0xe);
    gifs.PackAD(0x47, 0x7180b);
    gifs.PackAD(0x42, ((long)n << 2) | 0x40);
    gifs.AddPrimBulk(0x54, 0, 10, 0x424242412eUL, 0);
    gifs.EndDmaCnt();
}
