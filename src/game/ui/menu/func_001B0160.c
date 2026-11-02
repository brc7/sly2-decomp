struct E { int a; void *pv; int pad[5]; };
struct P { int pad[3]; struct E ae[1]; };
struct PX { char pad[0x1E8]; int f; int n; };
extern char D_002BFDA8[];
int FScreenShown(struct P *p);
/** @sly2-annot addr=0x1B0160
 * Purpose: Top (or pending) page is D_002BFDA8
 * Info: proposed name FPauseLoadPageA (T2); TU menu
 * Notes: names_v2 v1 proposed PostStepLoad__FP4STEP (0.179)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
int func_001B0160(struct P *p)
{
    int n;
    struct E *pe;
    if (!FScreenShown(p)) return 0;
    n = *(int *)((char *)p + 0x24);
    if (n <= 0) return 0;
    pe = &p->ae[n];
    if (((struct PX *)p)->f == 1 && n < ((struct PX *)p)->n)
        pe = &p->ae[((struct PX *)p)->n];
    return pe->pv == D_002BFDA8;
}
