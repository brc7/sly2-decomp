typedef struct { int cb; int cbAlign; } AREQ;
struct O { char pad[0x40]; void *x40; };
struct S {
    char pad[0x50]; int x50; int x54; int x58; char pad5c[0x14]; int x70; int x74; int x78; int x7C;
    char pad80[0x10]; float x90; int x94; int x98; char pad9c[0x20]; int xBC; struct O *xC0; void *xC4;
};
extern char D_003E1C70[], D_003E1C80[], D_003E106C[];
void LoadLoFromBrx(struct S *p, void *pbis);
int func_00117D00(void *pbis);
float func_00117E38(void *pbis);
int func_00117C80(void *pbis);
struct O *func_001AE2A0(AREQ *preq);
void CBinaryInputStream_ReadCb64(void *pbis, struct O *po);
void func_0011A070(void *, struct O *po);
void LoadSpriteTable(struct S *p, void *pbis, struct S *p2, int *pa, int *pb);
void LoadSpriteGroups(struct S *p, void *pbis, struct S *p2);
/** @sly2-annot addr=0x212298
 * Purpose: WIDGET LoadFromBrx: default anims 0x310/0x30F/0x311, LoadLoFromBrx, fStreamed, duration, width,
 *   height, fFont; immediate or deferred sprite load
 * Info: proposed name LoadWidgetFromBrx (T1); class WIDGET slot14; TU hud
 * Notes: names_v2 v1 proposed UpdateCtr__FP3CTR (0.007) | level_parser CONFIRMED-parse: payload layout
 *   followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources: r7c_hud+parse-verified
 * See: docs/engine/hud-ui.md#5-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void LoadWidgetFromBrx(struct S *p, void *pbis)
{
    int f;
    p->x70 = 0x310;
    p->x74 = 0x30F;
    p->x78 = 0x311;
    LoadLoFromBrx(p, pbis);
    f = func_00117D00(pbis);
    p->x90 = func_00117E38(pbis);
    p->x94 = func_00117C80(pbis);
    p->x98 = func_00117C80(pbis);
    p->xBC = func_00117C80(pbis);
    if (f) {
        AREQ req;
        p->x58 = 0;
        req.cb = 0x50;
        req.cbAlign = 0x10;
        p->xC4 = D_003E1C70;
        p->xC0 = func_001AE2A0(&req);
        p->xC0->x40 = p;
        CBinaryInputStream_ReadCb64(pbis, p->xC0);
        func_0011A070(D_003E106C, p->xC0);
    } else {
        p->x58 = 1;
        p->xC4 = D_003E1C80;
        LoadSpriteTable(p, pbis, p, &p->x50, &p->x54);
        LoadSpriteGroups(p, pbis, p);
    }
    p->x7C = -1;
}
