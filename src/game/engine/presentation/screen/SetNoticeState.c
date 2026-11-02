extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[64]; } VT;
typedef struct { char pad[0xC]; VT *vt; } LO;
typedef struct {
    char pad[0x10];
    int state;
    float t;
    int x18;
    float x1C;
    int x20;
    float x24;
    LO *x28;
} OBJ;
void StartWidgetMainAnim(LO *, int);
void ShowWidgetArgs(LO *, int, int *);
/** @sly2-annot addr=0x1DA5C8
 * Purpose: Notice state switch: 1 ShowWidgetArgs(text id), 3 hide (widget vt50), 4 swap anim 0x312, 2 main
 *   anim 0x310 after a swap
 * Info: proposed name SetNoticeState (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void SetNoticeState(OBJ *p, int n)
{
    LO *plo;
    if (n == p->state)
        return;
    switch (p->state) {
    case 3:
        if (n == 1 && p->t == D_002DDED0.t)
            n = 2;
        break;
    case 4:
        p->x18 = p->x20;
        p->x1C = p->x24;
        p->x20 = -1;
        p->x24 = -1.0f;
        break;
    }
    if (n == 0)
        p->x18 = -1;
    plo = p->x28;
    if (plo) {
        if (p->state == 4 && n == 2)
            StartWidgetMainAnim(plo, 0x310);
        switch (n) {
        case 4:
            StartWidgetMainAnim(p->x28, 0x312);
            break;
        case 3:
            p->x28->vt->a[50].pfn((char *)p->x28 + p->x28->vt->a[50].d);
            break;
        case 1:
            ShowWidgetArgs(p->x28, 1, &p->x18);
            break;
        }
    }
    p->state = n;
    p->t = D_002DDED0.t;
}
