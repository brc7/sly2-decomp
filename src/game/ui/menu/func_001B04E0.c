struct E { int kind; int a[7]; };
struct G { char pad[0x20]; int x20; };
struct S { char pad[0x1C]; int x1C; };
extern struct E D_002BFEC8[11], D_002F6470[];
extern char D_003DA530[], D_002BFD38[];
extern struct G D_003D9ED0;
int FScreenShown(void *p);
void func_001AF390(struct G *g, int n, struct E *pe);
void func_001AF4D8(struct G *g, int n, struct E *pe, int a, void *b);
/** @sly2-annot addr=0x1B04E0
 * Purpose: Build 11-item options page (filtered) and push or replace
 * Info: proposed name OpenPauseOptionsPage (T2); TU menu
 * Notes: names_v2 v1 proposed FUN_001c4848 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001B04E0(struct S *p)
{
    struct E *src = D_002BFEC8;
    struct E *dst = D_002F6470;
    unsigned int i;
    for (i = 0; i < 11; i++, src++) {
        int ok = 1;
        switch (src->kind) {
        case 0x3D: break;
        case 0x3C: ok = FScreenShown(D_003DA530) != 0; break;
        case 0x3E: ok = FScreenShown(D_003DA530) == 0; break;
        }
        if (ok) *dst++ = *src;
    }
    if (p) {
        func_001AF390(&D_003D9ED0, dst - D_002F6470, D_002F6470);
        p->x1C |= 2;
    } else {
        func_001AF4D8(&D_003D9ED0, dst - D_002F6470, D_002F6470, D_003D9ED0.x20, D_002BFD38);
    }
}
