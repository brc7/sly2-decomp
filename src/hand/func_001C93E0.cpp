struct RGBA { unsigned char r, g, b, a; };
struct TRI { int ai[3]; };
struct E { int n; struct TRI *p; struct RGBA c; };
struct V2 { float x, y; };
struct M2 { float a, b, c, d, tx, ty; };
extern struct E D_003D3A00[5];
extern struct V2 D_002C10D0[];
#include "gifs.h"
extern "C" void func_001C93E0(GIFS*,struct M2*,float);
/** @sly2-annot addr=0x1C93E0
 * Purpose: build GIF packet for the scaled/alpha quad
 * Info: proposed name DrawLoadingQuad (T2); TU prog
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/hud-ui.md#prog-0x1c8cc0-0x1c9940
 * @end-sly2-annot */
void func_001C93E0(GIFS *pgs, struct M2 *pm, float alpha)
{
    unsigned int i;
    pgs->AddPrimPack( 0, 1, 0xE);
    pgs->PackAD( 0x47, 0x31001);
    pgs->PackAD( 0x42, 0x44);
    for (i = 0; i < 5; i++) {
        struct E *pe = &D_003D3A00[i];
        struct RGBA c;
        int j;
        pgs->AddPrimPack( 0, 2, 0xE1);
        c = pe->c;
        c.a = (int)(c.a * alpha);
        pgs->PackRGBA( *(int *)&c);
        pgs->PackAD( 0, (signed char)c.a >= 0 ? 0x43 : 0x83);
        pgs->AddPrimList( 1, 4);
        for (j = 0; j < pe->n; j++) {
            int *pi = pe->p[j].ai;
            int k;
            for (k = 0; k < 3; k++) {
                struct V2 v = D_002C10D0[*pi++];
                struct V2 w;
                w.x = pm->a * v.x + pm->c * v.y + pm->tx;
                w.y = pm->b * v.x + pm->d * v.y + pm->ty;
                pgs->ListXYZF( (int)((w.x * 0.8f + 1792.0f) * 16.0f), (int)((w.y * 0.45454547f + 1936.0f) * 16.0f), 0, 0);
            }
        }
    }
}
