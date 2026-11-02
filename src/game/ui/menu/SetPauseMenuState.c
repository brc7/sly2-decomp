struct LO;
typedef struct { short d; short pad; void (*pfn)(struct LO *, int); } VTE;
struct LO { int x0, x4, x8; VTE *vt; };
typedef struct { short d; short pad; void (*pfn)(void *); } VTS;
struct T18 { int x0, x4, x8, xC, x10, x14, x18, x1C; };
struct ENT { int k; int pad[7]; };
typedef struct E { int x0; struct ENT *x4; struct LO *x8; int xC, x10; int x14; struct T18 *x18; } E;
typedef struct S { int x0, x4, x8; VTS *vt; char pad[0x24 - 0x10]; int x24; E ae[1]; char pad2[0x1E8 - 0x28 - 0x1C]; int x1E8; } S;
struct DISPU { unsigned int dx : 12; unsigned int dy : 11; unsigned int r0 : 9; unsigned int r1; } __attribute__((aligned(8)));
struct GS { char pad[0x18]; struct DISPU d1; };
extern struct GS D_002DEB80;
extern float D_002F6468, D_002F646C;
extern float D_003DD190, D_003DD18C;
void func_00213B00(struct LO *, int);
void func_00212E40(struct LO *, int, float);
/** @sly2-annot addr=0x1AEAA8
 * Purpose: Pause menu state machine (+0x1E8, 0..11): exit actions (move selection on 5/6, close on 0) then
 *   entry actions that play widget anims of the current page (text ids 0x331..0x335, slider 0x315 for volume
 *   items)
 * Info: proposed name SetPauseMenuState (T1); TU menu
 * Notes: top50_round (0.60): Body: SP SetXxxState(p,state) shape; callers ResetPauseMenu, UpdatePauseMenu,
 *   Push/Pop/ReplacePauseMenuPage
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r5b_rest+top50_round
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void SetPauseMenuState(S *s, int st)
{
    E *pe;
    if (s->x1E8 == st)
        return;
    pe = 0;
    if (s->x24 > 0)
        pe = &s->ae[s->x24 - 1];
    switch (s->x1E8) {
    case 5:
        if (pe) {
            if (++pe->x14 >= pe->x0)
                pe->x14 = 0;
        }
        break;
    case 6:
        if (pe) {
            if (--pe->x14 < 0)
                pe->x14 = pe->x0 - 1;
        }
        break;
    case 0:
        s->vt[10].pfn((char *)s + s->vt[10].d);
        break;
    case 1:
    case 11:
        break;
    }
    s->x1E8 = st;
    switch (st) {
    case 1:
        if (pe && pe->x8) {
            struct LO *p = pe->x8;
            p->vt[49].pfn((struct LO *)((char *)p + p->vt[49].d), pe->x18->x0);
            func_00213B00(pe->x8, pe->x18->xC);
        }
        break;
    case 3:
        if (pe && pe->x8)
            func_00213B00(pe->x8, pe->x18->x14);
        break;
    case 4:
        if (pe && pe->x8)
            func_00213B00(pe->x8, pe->x18->xC);
        break;
    case 2:
    case 8:
        if (pe && pe->x8) {
            struct LO *p = pe->x8;
            p->vt[49].pfn((struct LO *)((char *)p + p->vt[49].d), pe->x18->x8);
            func_00213B00(pe->x8, pe->x18->x14);
        }
        break;
    case 5:
        if (pe && pe->x8)
            func_00213B00(pe->x8, pe->x18->x18);
        break;
    case 6:
        if (pe && pe->x8)
            func_00213B00(pe->x8, pe->x18->x1C);
        break;
    case 11:
        if (pe && pe->x8)
            func_00213B00(pe->x8, 0x331);
        break;
    case 9:
        if (pe && pe->x8)
            func_00213B00(pe->x8, 0x332);
        break;
    case 10:
        if (pe && pe->x8) {
            switch (*(int *)((char *)pe->x4 + (pe->x14 << 5))) {
            case 0x67:
                func_00213B00(pe->x8, 0x333);
                func_00213B00(pe->x8, 0x315);
                func_00212E40(pe->x8, 0x315, D_003DD190);
                break;
            case 0x68:
                func_00213B00(pe->x8, 0x334);
                func_00213B00(pe->x8, 0x315);
                func_00212E40(pe->x8, 0x315, D_003DD18C);
                break;
            case 0x3F:
                func_00213B00(pe->x8, 0x335);
                D_002F6468 = D_002DEB80.d1.dx;
                D_002F646C = D_002DEB80.d1.dy;
                break;
            }
        }
        break;
    case 7:
        if (pe && pe->x8) {
            struct LO *p = pe->x8;
            p->vt[49].pfn((struct LO *)((char *)p + p->vt[49].d), pe->x18->x4);
            func_00213B00(pe->x8, pe->x18->x10);
        }
        break;
    case 0:
        s->vt[11].pfn((char *)s + s->vt[11].d);
        break;
    }
}
