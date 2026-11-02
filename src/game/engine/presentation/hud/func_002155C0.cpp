extern "C" {
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
struct X { char pad[0x14]; unsigned long *px14; };
X *func_001EC880(int);
void __4GIFS(void *);
void AllocStack__4DMASi(void *, int);
void AddPrimPack__4GIFSiiUl(void *, int, int, int);
void PackAD__4GIFSll(void *, int, unsigned long);
void AddPrimEnd__4GIFS(void *);
void DetachCopySw__4DMASPiPP2QWT2i(void *, int, int *, int, int *);
}
class FRAME { public: FRAME() { STACKSCOPE_ctor(this); } ~FRAME() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
struct BUF { int a[12]; };
struct P { char pad[0xC4]; int xC4; };
struct Q { int x0; int x4; char pad8[8]; int x10; char pad14[8]; int x1C; int x20; };
extern "C" void func_002155C0(P *p, Q *q);
/** @sly2-annot addr=0x2155C0
 * Purpose: kind-7 picture callback: GIF TEX0/TEXA packet for the texture of q->x10
 * Info: proposed name BuildSpritePictureTexPacket (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002155C0(P *p, Q *q)
{
    FRAME frame;
    if (q->x4 == 7) {
        q->x1C = 0;
        q->x20 = 0;
        X *x = func_001EC880(q->x10);
        if (x) {
            BUF b;
            __4GIFS(&b);
            AllocStack__4DMASi(&b, 0x10);
            AddPrimPack__4GIFSiiUl(&b, 0, 1, 0xE);
            PackAD__4GIFSll(&b, 6, *x->px14);
            PackAD__4GIFSll(&b, 0x14, 0x60);
            PackAD__4GIFSll(&b, 8, 5);
            AddPrimEnd__4GIFS(&b);
            DetachCopySw__4DMASPiPP2QWT2i(&b, p->xC4, &q->x1C, 0, &q->x20);
        }
    }
}
