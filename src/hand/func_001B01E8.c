struct E { int a; void *pv; int pad[5]; };
struct P { int pad[3]; struct E ae[1]; };
struct PX { char pad[0x1E8]; int f; int n; };
extern char D_002BFE08[];
int func_001D9880(struct P *p);
/** @sly2-annot addr=0x1B01E8
 * Purpose: Top (or pending) page is D_002BFE08
 * Info: proposed name FPauseLoadPageB (T2); TU menu
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
int func_001B01E8(struct P *p)
{
    int n;
    struct E *pe;
    if (!func_001D9880(p)) return 0;
    n = *(int *)((char *)p + 0x24);
    if (n <= 0) return 0;
    pe = &p->ae[n];
    if (((struct PX *)p)->f == 1 && n < ((struct PX *)p)->n)
        pe = &p->ae[((struct PX *)p)->n];
    return pe->pv == D_002BFE08;
}
