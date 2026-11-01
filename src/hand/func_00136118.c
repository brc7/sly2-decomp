struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad1[0x158 - 0x10]; int x158; char pad2[0x1A0 - 0x15C]; int x1A0; };
struct Q { int x0; };
struct P { char pad[0x150]; void *p150; char pad0[0x20C - 0x154]; float g20C; char padA[0x2C0 - 0x210]; char a2C0[0x1030 - 0x2C0]; void *p1030;
    char pad1[0x13D0 - 0x1034]; int x13D0; char pad2[0x16B0 - 0x13D4]; int x16B0; struct Q *pq; char pad3[0x186C - 0x16B8]; struct O *po; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
int func_00131968(void *pv);
void func_00136040(struct P *p, int n);
void func_00224258(struct O *po, int n);
int func_00141EA8(struct P *p, int n);
int func_001C4728(void *pv, int n);
void func_00136298(struct P *p);
void func_00195A00(void *pv, int a, int b, int c);
/** @sly2-annot addr=0x136118
 * Purpose: Sub-state machine on +0x16B0
 * Info: proposed name UpdateBentleySubState16B0 (T2); TU bentley
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136118(struct P *p)
{
    int s;
    for (;;) {
        s = p->x16B0;
        switch (s) {
        case 0:
            if (func_00131968(p->p1030))
                func_00136040(p, 1);
            break;
        case 1:
        case 2: {
            struct O *po = p->po;
            if (po->x158 == 2) {
                func_00224258(po, po->x1A0);
                goto common;
            }
            if (po->x158 == 4 || !func_001C4728(p->p150, func_00141EA8(p, p->pq->x0) | 0x10)) {
                ((void (*)(void *, int))po->vt[76].pfn)((char *)po + po->vt[76].d, 1);
            common:
                func_00136298(p);
                func_00195A00(p->a2C0, 0xB, 0, 0);
            } else if (0.1f < D_002DDED0.t - p->g20C) {
                func_00136040(p, 2);
            } else {
                func_00136040(p, 1);
            }
            break;
        }
        }
        if (p->x16B0 == s || p->x13D0 != 6)
            break;
    }
}
