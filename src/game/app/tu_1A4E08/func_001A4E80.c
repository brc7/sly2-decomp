struct E { unsigned int x0; int x4; void *x8; int xC; int x10; int x14; int x18; };
struct S { int x0; struct E a[16]; int x1C4; };
struct P { char pad[0x108]; int x108; };
struct H { char pad[0x44]; int x44; };
extern struct P D_002F66B0;
extern struct H D_002E6550;
extern char D_003DD490[], D_002BFCA0[], D_002DEB30[];
int func_001A4E70(struct S *s);
void CMpeg_QueueMovie(struct P *p, int n);
void WipeToWorldWarp(void *g, void *a, int b, int c, int d, int e);
void func_001651D8(void *g, int a, int b);
void func_001ACC38(struct H *h, int n);
/** @sly2-annot addr=0x1A4E80
 * Purpose: Pop the next queued step and run it: movie (CMpeg::QueueMovie), world warp (WipeToWorldWarp), task
 *   (StartTaskByOid), memcard UI phase (SetMcUiPhase); recurses for instant steps
 * Info: proposed name RunNextBootStep (T2); TU tu_1A4E08
 * Notes: names_v2 C proposed SetupBulkDataFromBrx__FiP18CBinaryInputStream (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
void func_001A4E80(struct S *s)
{
    struct E *e;
    struct P *p;
    if (!func_001A4E70(s))
        return;
    e = &s->a[s->x1C4++];
    switch (e->x0) {
    case 0:
    case 1:
        p = &D_002F66B0;
        CMpeg_QueueMovie(p, e->x4);
        if (e->x0 == 1)
            p->x108 = e->x0;
        WipeToWorldWarp(D_003DD490, D_002BFCA0, 0x189, -1, 0, -1);
        break;
    case 2:
        WipeToWorldWarp(D_003DD490, e->x8, e->xC, -1, e->x10, 0x30A);
        break;
    case 3:
        func_001651D8(D_002DEB30, e->x14, e->x18);
        break;
    case 4:
        break;
    case 5:
        if (!D_002E6550.x44)
            func_001ACC38(&D_002E6550, 1);
        func_001A4E80(s);
        break;
    }
}
