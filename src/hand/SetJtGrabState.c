typedef struct { short d; short pad; void (*pfn)(void *, int); } VT;
struct O { char pad[0xC]; VT *vt; char pad2[0x1854-0x10]; int x1854; };
struct P {
    struct O *x0; float x4; int x8; int pad0c[2]; int x14; int x18; char pad[0x440-0x1C]; char x440[0x4C4-0x440];
    int x4c4; char pad2[0x500-0x4C8]; int x500;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void SetJtGrabKind(struct P *p, int n);
void SetJtJtcs__FP2JTi(struct O *o, int n);
void UpdateJtCane__FP2JT(struct O *o);
void func_001CCAC0(void *p, int n);
void func_00192A28(struct P *p);
/** @sly2-annot addr=0x1928F8
 * Purpose: Start a grab action: SetJtGrabKind(-1), store nState/tEnter/plo/pspot, owner vt88
 *   SetPlayerState(2) (=jts 7), then map nState to initial kind (6 = Sly cane reach:
 *   SetJtJtcs(0)+UpdateJtCane; 5/9 reach tracker; 8 -> kind 7; 1 -> detector reset 0x192A28; 11/12 set x500)
 * Info: proposed name SetJtGrabState (T1); TU grab
 * Notes: runtime4: JTGRAB tEnter = Murray state entry
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void SetJtGrabState(struct P *p, int n, int a, int b)
{
    SetJtGrabKind(p, -1);
    p->x8 = n;
    p->x4 = D_002DDED0.t;
    p->x18 = b;
    p->x14 = a;
    p->x0->vt[88].pfn((char *)p->x0 + p->x0->vt[88].d, 2);
    switch (n) {
    case 0: case 13:
        n = 5;
        break;
    case 6:
    {
        struct O *o = p->x0;
        n = 0;
        o->x1854 = a;
        SetJtJtcs__FP2JTi(o, 0);
        UpdateJtCane__FP2JT(o);
        break;
    }
    case 5: case 9:
        p->x4c4 = p->x14;
        func_001CCAC0(p->x440, 0);
        n = 0;
        break;
    case 8:
        n = 7;
        func_00192A28(p);
        break;
    case 1:
        n = 0;
        func_00192A28(p);
        break;
    case 11: case 12:
        n = 0;
        p->x500 = 1;
        break;
    default:
        n = 0;
        break;
    }
    SetJtGrabKind(p, n);
}
