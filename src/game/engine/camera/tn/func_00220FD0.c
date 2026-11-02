typedef struct MARK { int a[4]; } MARK;
typedef struct MSG { int cb; int nKind; int k; int **pp; } MSG;
struct O { char pad[0x28]; long bits; };
struct S { char pad[0xE4]; char aE4[0x18]; char aFC[0x10]; struct O *p10C; int c; void *a; void *cur; int x11C; char pad2[0x164 - 0x120]; int flags; };
void LoadXfmXfFromBrx(struct S *p, void *pbis);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *pbis, void *pv);
void CBinaryInputStream_ReadBspc(void *pbis, void *pv, void *pv2);
MARK *func_001AE478(MARK *pm);
void func_001AE4A0(MARK *pm, int n);
void *func_001AE318(MSG *pmsg);
void *func_001AE238(MSG *pmsg, void *pv);
void func_00221818(struct S *p, int id);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(struct S *p, void *pbis);
void func_00221940(struct S *p);
void CLoadCollectorArray_ctor(MSG *pmsg);
void LoadXfmChildObjectsFromBrx(struct S *p, void *pbis, MSG *pmsg);
/** @sly2-annot addr=0x220FD0
 * Purpose: LoadXfm-style XF read, ReadGeom + ReadBspc (tunnel BSP), then tables allocated via PvAllocSlot
 * Info: proposed name LoadTnFromBrx (T1); class TN slot14; TU tn
 * Notes: names_v2 C proposed PostAloLoadCallback__FP3ALO5MSGIDPv (0.438) | level_parser CONFIRMED-parse:
 *   payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r8_21BB88+parse-verified
 * See: docs/engine/camera.md#5-key-functions,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14,
 *   docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00220FD0(struct S *p, void *pbis)
{
    MARK mark;
    MSG msg;
    void *pv;
    LoadXfmXfFromBrx(p, pbis);
    ReadGeom__18CBinaryInputStreamP4GEOM(pbis, p->aE4);
    CBinaryInputStream_ReadBspc(pbis, p->aE4, p->aFC);
    func_001AE478(&mark);
    {
        MSG *pm = &msg;
        pm->cb = 0x240;
        pm->nKind = 0x10;
        p->a = func_001AE318(pm);
    }
    func_00221818(p, 0x30);
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p, pbis);
    func_00221940(p);
    {
        MSG *pm = &msg;
        pm->cb = p->c * 0x90;
        pm->nKind = 0x10;
        pv = func_001AE238(pm, p->a);
    }
    p->x11C = 1;
    p->a = pv;
    p->cur = pv;
    CLoadCollectorArray_ctor(&msg);
    LoadXfmChildObjectsFromBrx(p, pbis, &msg);
    switch (msg.k) {
    case 0:
        break;
    case 1:
        p->p10C = (struct O *)*msg.pp;
        p->p10C->bits |= 1L << 57;
        break;
    }
    if (!p->p10C) p->flags &= ~2;
    func_001AE4A0(&mark, 2);
}
