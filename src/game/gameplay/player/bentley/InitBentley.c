struct S10A8 { float g0; int n4; float g8; float gC; };
struct P {
    char pad0[0x3D8]; float x3D8;
    char pad1[0x1000 - 0x3DC]; int x1000;
    char pad2[0x103C - 0x1004]; float x103C; int x1040; int x1044; float x1048;
    char pad3[0x1098 - 0x104C]; float x1098; float x109C; float x10A0; float x10A4; struct S10A8 s10A8;
    char pad4[0x1320 - 0x10B8]; char a1320[0x13D0 - 0x1320]; int x13D0;
    char pad5[0x1600 - 0x13D4]; char a1600[0x16B0 - 0x1600]; int x16B0; int pad6[2]; int x16BC;
    char pad7[0x1800 - 0x16C0]; int x1800;
    char pad8[0x1824 - 0x1804]; int x1824; int pad9[2]; int x1830; int padA[2]; int x183C; int padB[2]; int x1848;
    int padC[2]; int x1854; int padD[2]; int x1860; int padE[3]; int x1870;
};
extern struct P *D_002DD5BC;
void InitStepplayer(struct P *p);
void InitGait(void *pv, struct P *p, int n);
/** @sly2-annot addr=0x134648
 * Purpose: BENTLEY vt7: InitStepplayer then Bentley defaults (camera offsets 45/125, two 0x20B840 sub-objects
 *   at +0x1320/+0x1600, gadget slot kinds 7/9/11 ...)
 * Info: proposed name InitBentley (T1); class BENTLEY slot7; TU bentley
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/player.md#10-source-files, docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void InitBentley(struct P *p)
{
    InitStepplayer(p);
//{
    p->x1040 = 0;
    p->x13D0 = -1;
    p->x16B0 = -1;
    p->x16BC = -1;
    p->x1800 = -1;
    p->x103C = 45.0f;
    p->x1048 = 125.0f;
//}
    InitGait(p->a1320, p, 0x73);
    InitGait(p->a1600, p, 0x73);
    p->x3D8 = -70.0f;
    p->x1098 = -100.0f;
    p->x109C = 100.0f;
    p->x10A0 = 50.0f;
    p->x10A4 = -10.0f;
    {
        struct S10A8 *ps = &p->s10A8;
        ps->g0 = 250.0f;
        ps->g8 = 1.0f;
        ps->n4 = 0;
        ps->gC = 4500.0f;
    }
    p->x1000 = 2;
    p->x1830 = 7;
    p->x183C = 9;
    p->x1848 = 11;
    p->x1854 = 13;
    p->x1860 = 4;
    p->x1824 = 5;
    p->x1870 = 5;
    D_002DD5BC = p;
}
