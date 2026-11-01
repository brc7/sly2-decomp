typedef struct { short d; short pad; void (*pfn)(void *, int); } VT;
struct C { char pad[0x10C]; int x10c; char pad2[0x124-0x110]; int x124; };
struct O { char pad[0xC]; VT *vt; char pad2[0x4C0-0x10]; char x4c0[0xF5C-0x4C0]; struct C *xf5c; float xf60;
    char pad3[0x18C4-0xF64]; struct C *x18c4; char pad4[0x1F18-0x18C8]; struct C *x1f18; };
struct P { struct O *x0; int x4; int x8; struct C *xc; int x10; float x14; int x18; int x1c; };
extern int D_002DD5BC;
void SetJtGrabState(void *p, int n, struct C *c, void *b);
void SetJtJts__FP2JT3JTS4JTBS(struct O *o, int a, int b);
void SetBentleyState(int a, int b);
void func_00195FB8(struct P *p, int a, int b);
#define B(c, off) ((c) ? (void *)((char *)(c) + (off)) : (void *)0)
/** @sly2-annot addr=0x19C688
 * Purpose: On landing on a grab target, switch per air kind (6,11,12,13,...) to SetJtGrabState with the
 *   target's GRABSPOT sub-object (+0x150/+0x190/+0xE0) or SetJtJts
 * Info: proposed name GrabJtAirTarget (T2); TU air
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
void func_0019C688(struct P *p)
{
    switch (p->x1c) {
    case 6:
        SetJtGrabState(p->x0->x4c0, 1, p->xc, B(p->xc, 0x150));
        break;
    case 11:
        SetJtGrabState(p->x0->x4c0, 0, p->xc, B(p->xc, 0x190));
        break;
    case 12:
        SetJtGrabState(p->x0->x4c0, 0, p->xc, B(p->xc, 0xE0));
        break;
    case 13:
        SetJtGrabState(p->x0->x4c0, p->xc->x10c, p->xc, B(p->xc, 0xE0));
        break;
    case 9:
        SetJtGrabState(p->x0->x4c0, 4, p->xc, B(p->xc, 0xE0));
        break;
    case 10:
        SetJtGrabState(p->x0->x4c0, 3, p->xc, B(p->xc, 0x190));
        break;
    case 14:
    {
        struct O *o = p->x0;
        o->x18c4 = p->xc;
        if (o->x18c4->x124 == 3)
            SetJtJts__FP2JT3JTS4JTBS(o, 0x16, -1);
        else
            SetJtJts__FP2JT3JTS4JTBS(o, 3, 1);
        break;
    }
    case 15:
        p->x0->xf5c = p->xc;
        p->x0->xf60 = p->x14;
        p->x0->vt[88].pfn((char *)p->x0 + p->x0->vt[88].d, 8);
        break;
    case 16:
    {
        struct O *o = p->x0;
        o->x1f18 = p->xc;
        SetJtJts__FP2JT3JTS4JTBS(o, 0x15, 0xA);
        break;
    }
        break;
    case 18:
        SetBentleyState(D_002DD5BC, 8);
        break;
    default:
        func_00195FB8(p, 4, 0);
        break;
    }
}
