// Actor update: commit completed state after every noninitial guard path.
struct VT { char pad[0x190]; short d; short pd; void (*pf)(void *); };
struct S { char pad[0xC]; struct VT *pvt; char pad10[0x48]; int x58; float x5C; char pad60[0xB0]; int x110; int x114; };
extern struct G { char pad[0x3B0C]; void *x3B0C; } D_003DD490;
extern float D_002DDF0C;
extern int D_002DD318;
extern char D_002E6550[], D_003DA4F0[], D_002DFC00[];
int func_002121C8(struct S *p);
void func_00212C50(struct S *p);
int func_001D9880(void *);
int GrfHandleJoyBtn(void *, int);
void DecrementSwHandsOff(struct G *, int);
void func_00162DF8(void *);
/** @sly2-annot addr=0x211D38
 * Purpose: CID 0x7F Update: modal box; after 0.5 s visible and button mask 0xF0 pressed ->
 *   DecrementSwHandsOff, optional resume via SW+0x3B0C, HideWidget
 * Info: proposed name UpdateWidget127 (T2); class WIDGET127 slot16; TU hud
 * Notes: names_v2 v1 proposed DrawBlot__FP4BLOT (0.016) | v2: demoted to T2 (placeholder token in name: 127)
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211D38(struct S *p)
{
    int r = func_002121C8(p);
    if (r)
        return;
    func_00212C50(p);
    if (p->x58 == 2 && 0.5f < D_002DDF0C - p->x5C && !func_001D9880(D_002E6550)
        && !func_001D9880(D_003DA4F0) && GrfHandleJoyBtn(D_002DFC00, 0xF0)) {
        DecrementSwHandsOff(&D_003DD490, 0);
        if (p->x114 == 0 && D_003DD490.x3B0C) {
            D_002DD318++;
            func_00162DF8(D_003DD490.x3B0C);
        }
        p->pvt->pf((char *)p + p->pvt->d);
    }
    p->x110 = 1;
    return;
}
